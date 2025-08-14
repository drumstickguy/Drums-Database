//End Gateway.exe source code - Modifed by camellia-y7x (I removed some functions.)
#include <iostream>
#include <memory>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <windows.h>
#include <windef.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <cmath>
#pragma comment(lib, "Winmm.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
using namespace std;
double intensity = 0.0;
bool state = false;
HWND hDlg;
HANDLE glitchmsgbox1;
int MessageBoxWidth;
int MessageBoxHeight;

typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
typedef struct
{
	float x;
	float y;
	float z;
} VERTEX;
typedef struct
{
	int vtx0;
	int vtx1;
} EDGE;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;
struct Point3D {
	float x, y, z;
};
namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
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

		if (fDelta != 0.f)
		{
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

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
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

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}

void DrawEllipseAt(HDC hdc, int x, int y, COLORREF color){ 
	HBRUSH brush = CreateSolidBrush(color); SelectObject(hdc, brush);
	DrawIcon(hdc, x - 20, y - 20, LoadIcon(NULL,MAKEINTRESOURCE(32511 + (rand() % 8))));
	DeleteObject(brush);
}

Point3D RotatePoint(Point3D point, float angleX, float angleY, float angleZ) {
	float cosX = cos(angleX), sinX = sin(angleX);
	float cosY = cos(angleY), sinY = sin(angleY);
	float cosZ = cos(angleZ), sinZ = sin(angleZ);

	float y = point.y * cosX - point.z * sinX;
	float z = point.y * sinX + point.z * cosX;
	point.y = y;
	point.z = z;

	float x = point.x * cosY + point.z * sinY;
	z = -point.x * sinY + point.z * cosY;
	point.x = x;
	point.z = z;

	x = point.x * cosZ - point.y * sinZ;
	y = point.x * sinZ + point.y * cosZ;
	point.x = x;
	point.y = y;

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
		COLORREF color = NULL;

		int screenX = static_cast<int>(center.x + rotated.x);
		int screenY = static_cast<int>(center.y + rotated.y);

		screenPoints[i].x = screenX;
		screenPoints[i].y = screenY;

		DrawEllipseAt(hdc, screenX, screenY, color);
	}

	/*POINT polyline1[5] = {screenPoints[0], screenPoints[1], screenPoints[2], screenPoints[3], screenPoints[0]};
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
	Polyline(hdc, &connectingLines[6], 2);*/
}

void InitDPI() {
	HMODULE hModule = LoadLibraryA("user32.dll");
	BOOL(WINAPI * SetProcessDPIAware)(VOID) = (BOOL(WINAPI*)(VOID))GetProcAddress(hModule, "SetProcessDPIAware");
	if (SetProcessDPIAware) {
		SetProcessDPIAware();
	}
	FreeLibrary(hModule);
}

VOID WINAPI MsgBoxCorruptionThread(HWND hwndMsgBox) {
	HDC hdc = GetDC(hwndMsgBox);
	RECT rect;
	GetWindowRect(hwndMsgBox, &rect);
	int w = rect.right - rect.left, h = rect.bottom - rect.top;
	InvalidateRect(0, 0, 0);
	for (;;) {
		SelectObject(hdc, CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255)));
		PatBlt(hdc, 0, 0, w, h, PATINVERT);
		Sleep(100);
	}
}

LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		HWND hwndMsgBox = (HWND)wParam;
		Sleep(100);
		//		ShowWindow(hwndMsgBox, 5);
		HANDLE handle = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)MsgBoxCorruptionThread, hwndMsgBox, 0, NULL);
		return 0;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
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
			rgbScreen[i].rgb += (x + y) & i;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI cubeicon(LPVOID lpParam)
{
	HDC hdc = GetDC(0);
	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	int wdpi = GetDeviceCaps(hdc, LOGPIXELSX);
	int hdpi = GetDeviceCaps(hdc, LOGPIXELSY);
	int x = sw * wdpi / 96;
	int y = sh * hdpi / 96;
	int signX = 2;
	int signY = 2;
	int incrementor = 10;
	float x2 = 100.0/*f*/;
	float y2 = 100.0/*f*/;
	float angleX = 0.0/*f*/, angleY = 0.0/*f*/, angleZ = 0.0/*f*/;
	float angleIncrement = 0.04/*f*/;
	float colorA = 0;
	float size = 100/*f*/;
	InvalidateRect(0, 0, 0);
	BitBlt(hdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
	for (;;) {
		HDC hdc = GetDC(0);
		int sw = GetSystemMetrics(SM_CXSCREEN);
		int sh = GetSystemMetrics(SM_CYSCREEN);
		int wdpi = GetDeviceCaps(hdc, LOGPIXELSX);
		int hdpi = GetDeviceCaps(hdc, LOGPIXELSY);
		int x = sw * wdpi / 96;
		int y = sh * hdpi / 96;

		x2 += incrementor * signX;
		y2 += incrementor * signY;

		if (x2 + 75 >= x) {
			signX = -1;
			x2 = x - 76;
		}
		else if (x2 <= 75) {
			signX = 1;
			x2 = 76;
		}

		if (y2 + 75 >= y) {
			signY = -1;
			y2 = y - 76;
		}
		else if (y2 <= 75) {
			signY = 1;
			y2 = 76;
		}


		Point3D center = { x2, y2, 0.0f };
		SelectObject(hdc, CreatePen(0, 1, RGB(255, 255, 255)));
		Draw3DCube(hdc, center, size, angleX, angleY, angleZ, colorA);

		angleX += angleIncrement;
		angleY += angleIncrement;
		angleZ += angleIncrement;

		Sleep(20);
		ReleaseDC(0, hdc);
		colorA += 1;

		if (size >= 0 && size <= 100) {
			size += 0.5;
		}
	}

	return 0;
}

DWORD WINAPI Faults(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	for (;;)
	{
		BitBlt(hdc, -2, -2, w/2, h/2, hdc, 0, 0, SRCINVERT);
		BitBlt(hdc, (w/2)+2, -2, w / 2, h/2, hdc, w/2, 0, SRCINVERT);
		BitBlt(hdc, -2, (h/2)+2, w / 2, h / 2, hdc,0, h/2, SRCINVERT);
		BitBlt(hdc, (w/2)+2, (h / 2) + 2, w / 2, h / 2, hdc, w/2, h / 2, SRCINVERT);
		Sleep(50);
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
		for (int i = 0; i < w * h; i++) {
			rgbScreen[i].rgb = (rgbScreen[i].rgb * 2) % (RGB(255, 255, 255));
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen);
		DeleteDC(hdcScreen);
		Sleep(50);
	}
}

DWORD WINAPI Color(LPVOID lpParam) {
	HDC desk = GetDC(0);
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	for (;;)
	{
		desk = GetDC(0);
		SelectObject(desk, CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255)));
		PatBlt(desk, 0, 0, sw, sh, PATINVERT);
		Sleep(100);
	}
}

DWORD WINAPI text(LPVOID lpParam) {
	int dwStartTime = time(NULL);
	for (;;) {
		int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
		int a = rand() % 250;
		HDC hdc = GetDC(NULL);
		HDC hcdc = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(hcdc, hBitmap);
		BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		SetBkMode(hcdc, 1);
		LPCSTR text[5] = { "End Gateway.exe", "XUGE" };
		int tmp = rand() % 2;
		SetTextColor(hcdc, RGB(255-(rand() % 255), 255-(rand() % 255), 255-(rand() % 255)));
		HFONT font = CreateFont(250-a, (250-a)/2, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"114514");
		SelectObject(hcdc, font);
		TextOutA(hcdc, w-(rand() % w), h-(rand() % h), text[tmp], strlen(text[tmp]));
		BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdc);
		ReleaseDC(NULL, hcdc);
		DeleteObject(font);
		DeleteObject(hdc);
		DeleteObject(hcdc);
		DeleteObject(hBitmap);
		Sleep(5);
	}
	RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
	return 0;
}

DWORD WINAPI fasces(LPVOID lpParam) {
    //Due to the presence of Nazi symbol, So I removed it.
    //:)
}

DWORD WINAPI Faults2(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	for (;;)
	{
		BitBlt(hdc, 2, 2, w / 2, h / 2, hdc, 0, 0, SRCPAINT);
		BitBlt(hdc, (w / 2) - 2, 2, w / 2, h / 2, hdc, w / 2, 0, SRCPAINT);
		BitBlt(hdc, 2, (h / 2) - 2, w / 2, h / 2, hdc, 0, h / 2, SRCPAINT);
		BitBlt(hdc, (w / 2) - 2, (h / 2) - 2, w / 2, h / 2, hdc, w / 2, h / 2, SRCPAINT);
		Sleep(50);
	}
}

DWORD WINAPI icons(LPVOID lpParam)
{
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	int wdpi = GetDeviceCaps(hdc, LOGPIXELSX);
	int hdpi = GetDeviceCaps(hdc, LOGPIXELSY);
	HINSTANCE HSHELL32 = LoadLibrary(_T("Shell32.dll"));
	for (;;) {
		int a = rand() % 1000;
		DrawIconEx(hdc,w-(rand() % w), h-(rand() % h), LoadIconW(HSHELL32, MAKEINTRESOURCE(rand() % 336)), 1000-a, 1000-a, NULL, NULL, DI_NORMAL);
		Sleep(10);
	}
}

DWORD WINAPI invert(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	for (;;) {
		PatBlt(hdc,rand()%w,rand()%h,rand()%w,rand()%h, DSTINVERT);
		Sleep(10);
	}
}

DWORD WINAPI squ(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int wdpi = GetDeviceCaps(hdc, LOGPIXELSX);
	int hdpi = GetDeviceCaps(hdc, LOGPIXELSY);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	for (;;) {
		for (int x = 0; x <= w; x += 64) {
			SelectObject(hdc, CreateSolidBrush(RGB(255-(rand() % 255), rand() % 255, (rand()%129)*2)));
			PatBlt(hdc, x, 0, 64, h, PATINVERT);
			Sleep(100);
		}
		for (int y = 0; y <= h; y += 64) {
			SelectObject(hdc, CreateSolidBrush(RGB((rand() % 65)*4, rand() % 255, (rand() % 33)*8)));
			SelectObject(hdc,CreatePen(PS_NULL,0,NULL));
			Rectangle(hdc, 0, y, w, y+65);
			Sleep(100);
		}
	}
}

DWORD WINAPI shader3(LPVOID lpvd)
{
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
	for (;;)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;
				int fx =4 * i+(x & y);
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
	return 0;
}

DWORD WINAPI gatewey(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	/*HBRUSH brush1 = CreateSolidBrush(RGB(109, 218, 208));
	HBRUSH brush2 = CreateSolidBrush(RGB(93, 162, 158));
	HBRUSH brush3 = CreateSolidBrush(RGB(21, 103, 114));
	HBRUSH brush4 = CreateSolidBrush(RGB(96, 101, 131));*/
	HBRUSH brush5 = CreateSolidBrush(RGB(21, 33, 45));
	for (;;) {
		SelectObject(hdc, brush5);
		Rectangle(hdc, 0, 0, w, h);
		DeleteObject;
		Sleep(1000);
	}
}

DWORD WINAPI star(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	for (;;) {
		int x1 = rand() % w, y1 = rand() % h;
		HBRUSH brush = (CreateSolidBrush(RGB(109, 218, 208)));
		SelectObject(hdc, brush);
		Rectangle(hdc, x1,y1, x1+64, y1+64);
		DeleteObject;
		Sleep(100);
	}
}

DWORD WINAPI startext(LPVOID lpParam) {
	HDC hdc;
	int sx = 0, sy = 0;
	LPCWSTR lpText = L"This is the gateway.";
	for (;;)
	{
		hdc = GetWindowDC(GetDesktopWindow());
		sx = GetSystemMetrics(0);
		sy = GetSystemMetrics(1);
		SetTextColor(hdc, RGB(0, 255, 255));
		SetBkColor(hdc, RGB(21, 103, 114));
		TextOutW(hdc, rand() % sx, rand() % sy, lpText, wcslen(lpText));
		Sleep(50);
	}
	return 0;
}

DWORD WINAPI starball(LPVOID lpParam) {
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	int signX = 1;
	int signY = 1;
	int signX1 = 1;
	int signY1 = 1;
	int incrementor = 10;
	int x = 10;
	int y = 10;
	for (;;) {
		HDC hdc = GetDC(0);
		int top_x = 0 + x;
		int top_y = 0 + y;
		int bottom_x = 100 + x;
		int bottom_y = 100 + y;
		x += incrementor * signX;
		y += incrementor * signY;
		HBRUSH brush = CreateSolidBrush(RGB(93, 162, 158));
		SelectObject(hdc, brush);
		Ellipse(hdc, top_x, top_y, bottom_x, bottom_y);
		if (y >= GetSystemMetrics(SM_CYSCREEN))
		{
			signY = -1;
		}

		if (x >= GetSystemMetrics(SM_CXSCREEN))
		{
			signX = -1;
		}

		if (y == 0)
		{
			signY = 1;
		}

		if (x == 0)
		{
			signX = 1;
		}
		Sleep(10);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI stararc(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	for (;;) {
		HPEN pen = (CreatePen(PS_DASH,5,RGB(96, 101, 131)));
		SelectObject(hdc, pen);
		Arc(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
		Sleep(100);
	}
}

DWORD WINAPI strangeblt(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	POINT wPt[4];
	wPt[0].x = rand()%w;
	wPt[0].y = rand() % h;
	wPt[1].x = rand() % w;
	wPt[1].y = rand() % h;
	wPt[2].x = rand() % w;
	wPt[2].y = rand() % h;
	wPt[3].x = rand() % w;
	wPt[3].y = rand() % h;
	for (;;) {
		PlgBlt(hdc, wPt, hdc, rand() % w, rand() % h, rand() % w, rand() % h, NULL, rand() % w, rand() % h);
		Sleep(100);
	}
}


DWORD WINAPI curs(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	for (;;) {
		DrawIconEx(hdc, rand() % w, rand() % h, LoadCursor(NULL, MAKEINTRESOURCE(32511 + (rand() % 152))),rand()%w,rand()%h,NULL,NULL,DI_NORMAL);
		Sleep(10);
	}
}

DWORD WINAPI mohu(LPVOID lpParam)
{
	for (;;)
	{
		HDC hdc = GetDC(NULL);
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		HBITMAP hbm = CreateCompatibleBitmap(hdc, w, h);
		HDC hdcTemp = CreateCompatibleDC(hdc);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcTemp, hbm);
		BitBlt(hdcTemp, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		int numShifts = 600;
		for (int i = 0; i < numShifts; i++)
		{
			int x = rand() % w;
			int y = rand() % h;
			int dx = (rand() % 3) - 1;
			int dy = (rand() % 3) - 1;
			BitBlt(hdcTemp, x + dx, y + dy, w - x, h - y, hdcTemp, x, y, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, w, h, hdcTemp, 0, 0, SRCCOPY);
		SelectObject(hdcTemp, hbmOld);
		DeleteDC(hdcTemp);
		DeleteObject(hbm);
		ReleaseDC(NULL, hdc);
	}
}

DWORD WINAPI doudong(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int wdpi = GetDeviceCaps(hdc, LOGPIXELSX);
	int hdpi = GetDeviceCaps(hdc, LOGPIXELSY);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	for (;;) {
		StretchBlt(hdc, (rand() % 16) * wdpi / 96, (rand() % 16) * hdpi / 96, w, h, hdc, 0, 0, w, h, SRCCOPY);
		Sleep(125);
		StretchBlt(hdc, -((rand() % 16) * wdpi / 96), (rand() % 16) * hdpi / 96, w, h, hdc, 0, 0, w, h, SRCCOPY);
		Sleep(125);
		StretchBlt(hdc, -((rand() % 16) * wdpi / 96), -((rand() % 16) * hdpi / 96), w, h, hdc, 0, 0, w, h, SRCCOPY);
		Sleep(125);
		StretchBlt(hdc, (rand() % 16) * wdpi / 96, -((rand() % 16) * hdpi / 96), w, h, hdc, 0, 0, w, h, SRCCOPY);
		Sleep(125);
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t >> 5 & (t << 4) + 6 * t * (t >> 9 & (t >> 3 | t >> 4 | t >> 5) & t >> 9));
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
		buffer[t] = static_cast<char>(t * (t >> 8 | t >> 8) & 1000 & t >> 8 ^ (t & t >> 8 | t >> 8));
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
		buffer[t] = static_cast<char>(10 * (t >> 7 | t | t >> 6) + 4 * (t & t >> 13 | t >> 6));
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
		buffer[t] = static_cast<char>(t * (t >> 8 & 6) >> (t ^ (t >> 8)) * t >> 9 & t ^ t % 255 & t >> (t % 257 | 4) & 100 & t);
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
		buffer[t] = static_cast<char>(t * (t >> 8 | t >> 8) & 1000 & t >> 8 ^ (t | t >> 8 | t >> 8));
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
		buffer[t] = static_cast<char>(t * (t >> 8 & t & t >> 9 & t >> 13));
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 12000, 12000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[12000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (0x114514 >> (t >> 9 & 44) & 15) | t >> (t & 2048 ? 8 : 6) | 40000 / (t | 4000));
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
		buffer[t] = static_cast<char>((t * (0xCA98CA98 >> (t >> 11 & 45) & 14) | 40000 / (t | 4091)) & -t >> 8);
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
		buffer[t] = static_cast<char>((t * (84 & t >> 8)) | (t * (21 & t >> 12)));
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
		buffer[t] = static_cast<char>((t * (t & 16384 ? 6 : 5) * (1 + (1 & t >> 1)) >> (1 & t >> 1) | t >> (t & 2048 ? t & 128 ? 8 : 4 : 2)) & (t * (t & 16384 ? 6 : 5) * (4 - (3 & t >> 8)) >> (3 & -t >> (t & 4096 ? 2 : 15))));
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

int main() {
	InitDPI();
	CREATE_NO_WINDOW;
HHOOK hMsgHookA = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
	if (MessageBoxW(NULL, L"Warning! This program is a computer virus. It may make your computer cannot work normally. Whether to run or not?\r\n\r\n\
Please don't maliciously open this program on other people's or public computers! If you accidentally opened it, please click the 'No' button to cancel the run. If you want to run it, please make sure you are running it on your own computer, or ensure that the virus on this computer is in a secure environment (such as a virtual machine, sandbox, etc.) and turn off all antivirus software. If you are running this program on other people's or public computers, please make sure you are running the harmless edition of this program, and then click the 'Yes' button to continue.", L"End Gateway.exe", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDNO)
	{
		UnhookWindowsHookEx(hMsgHookA);
		ExitProcess(0);
	}
	else
	{
		UnhookWindowsHookEx(hMsgHookA);
		HHOOK hMsgHookB = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
		if (MessageBoxW(NULL, L"This is the last warning!!!\r\n\r\n\
Do you want to really run? After running, your computer may not work normally! If you run the harmful edition of this program on other people's or public computers, you will be responsible for any losses and legal liabilities caused by running this program! The writer of this computer virus isn't responsible!!!", L"End Gateway.exe", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDNO)
		{
		UnhookWindowsHookEx(hMsgHookB);
			ExitProcess(0);
		}
		else
		{
			UnhookWindowsHookEx(hMsgHookB);
			sound1();
			//HANDLE thread0 = CreateThread(0, 0, msgbox, 0, 0, 0);
			HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
			//HANDLE thread2 = CreateThread(0, 0, cubeicon, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread1, 0);
			InvalidateRect(0, 0, 0);
			sound2();
			HANDLE thread3 = CreateThread(0, 0, Faults, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread3, 0);
			InvalidateRect(0, 0, 0);
			sound3();
			HANDLE thread4 = CreateThread(0, 0, text, 0, 0, 0);
			HANDLE thread5 = CreateThread(0, 0, Color, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread4, 0);
			TerminateThread(thread5, 0);
			InvalidateRect(0, 0, 0);
			sound4();
			HANDLE thread6 = CreateThread(0, 0, fasces, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread6, 0);
			InvalidateRect(0, 0, 0);
			sound5();
			HANDLE thread7 = CreateThread(0, 0, Faults2, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread7, 0);
			InvalidateRect(0, 0, 0);
			sound6();
			HANDLE thread8 = CreateThread(0, 0, icons, 0, 0, 0);
			HANDLE thread9 = CreateThread(0, 0, invert, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread8, 0);
			TerminateThread(thread9, 0);
			InvalidateRect(0, 0, 0);
			sound7();
			HANDLE thread10 = CreateThread(0, 0, squ, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread10, 0);
			InvalidateRect(0, 0, 0);
			sound8();
			HANDLE thread11 = CreateThread(0, 0, shader3, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread11, 0);
			InvalidateRect(0, 0, 0);
			sound9();
			HANDLE thread12 = CreateThread(0, 0, gatewey, 0, 0, 0);
			HANDLE thread13 = CreateThread(0, 0, star, 0, 0, 0);
			HANDLE thread14 = CreateThread(0, 0, starball, 0, 0, 0);
			HANDLE thread15 = CreateThread(0, 0, startext, 0, 0, 0);
			HANDLE thread16 = CreateThread(0, 0, stararc, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread12, 0);
			TerminateThread(thread13, 0);
			TerminateThread(thread14, 0);
			TerminateThread(thread15, 0);
			TerminateThread(thread16, 0);
			InvalidateRect(0, 0, 0);
			sound10();
			HANDLE thread17 = CreateThread(0, 0, strangeblt, 0, 0, 0);
			HANDLE thread18 = CreateThread(0, 0, curs, 0, 0, 0);
			HANDLE thread19 = CreateThread(0, 0, mohu, 0, 0, 0);
			HANDLE thread20 = CreateThread(0, 0, doudong, 0, 0, 0);
			HANDLE thread21 = CreateThread(0, 0, shader2, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread17, 0);
			TerminateThread(thread18, 0);
			TerminateThread(thread19, 0);
			TerminateThread(thread20, 0);
			TerminateThread(thread21, 0);
			//TerminateThread(thread0, 0);
			//TerminateThread(thread2, 0);
			InvalidateRect(0, 0, 0);
			return 0;
		}
	}
}