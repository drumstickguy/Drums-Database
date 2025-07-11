#include "gdiconfig.h"
#include <mmsystem.h>

/*DWORD WINAPI sound2(LPVOID lpParam) { //dword version by (Soheilshahrab)
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 15] = {};
	while (1) {
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((t*(t>>(t/8000))/2)|t*((t-2296&t)>>11)) );

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
	}
}*/

DWORD WINAPI retexture(LPVOID lpParam) {
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
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, NOTSRCCOPY);
		for (INT i = 0; i < w * h; i++) {
				INT x = i % w, y = i / w;
				int code = x & y;
				rgbScreen[i].rgb -= ((code) * (code));
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, NOTSRCCOPY);
		Sleep(1);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
} 

DWORD WINAPI retexture2(LPVOID lpParam) {
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
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, NOTSRCCOPY);
		for (INT i = 0; i < w * h; i++) {
				INT x = i % w, y = i / w;
				int code = x ^ y;
				rgbScreen[i].rgb -= ((code) * (code));
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, NOTSRCCOPY);
		Sleep(1);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

void WINAPI sound2() { //non-dword
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((t*(t>>(t/8000))/2)|t*((t-2296&t)>>11)) );

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}


void WINAPI sound() { //non-dword
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 44100, 44100, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[44100 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((t*t-t/400*t/800^t/900)&192&-t/30)+t/40&t/80^t/80);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

DWORD WINAPI retexture3(LPVOID lpParam) {
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
	int issac = 0;
	for (;;) {
		hdcScreen = GetDC(0);
		issac = 1 + issac;
		if (issac >= w) issac = 0;
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, NOTSRCCOPY);
		for (INT i = 0; i < w * h; i++) {
				INT x = i % w, y = i / w;
				int code = x ^ y;
				rgbScreen[i].rgb -= (-(code >> code) | (code >> code)) | (-(code << code) | (code << code));
		}
		BitBlt(hdcScreen, 30, 0, w, h, hdcMem, 0, 0, NOTSRCCOPY);
		BitBlt(hdcScreen, 30 - w, 0, w, h, hdcScreen, 0, 0, NOTSRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
} 

void WINAPI sound3() { //non-dword
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t*(t/8000));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}


int main() {
	int msg = 0;
	while (msg <= 29) {
	MessageBoxW(NULL, L" you need to press 30 times", L"Mission 1", MB_OK | MB_ICONEXCLAMATION);
	msg = 1 + msg;
	}
	MessageBoxW(NULL, L"you're almost here, just click one more time", L"Mission 1", MB_OK | MB_ICONEXCLAMATION);
    HANDLE thread6 =  CreateThread(0, 0, retexture, 0, 0, 0);
	sound2();
    Sleep(30000);
	TerminateThread(thread6, 0);
    HANDLE thread5 =  CreateThread(0, 0, retexture2, 0, 0, 0);
    sound();
    Sleep(30000);
	TerminateThread(thread5, 0);
    HANDLE thread4 =  CreateThread(0, 0, retexture3, 0, 0, 0);
	while (1) {
    sound3();
    Sleep(30000);
	}
	//TerminateThread(thread4, 0); //the final payload goes infinite
}
