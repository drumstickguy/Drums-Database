#pragma once
#include <Windows.h>

BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam) {
	RECT rcWindow;
	GetWindowRect(hwnd, &rcWindow);
	int cxWindow = rcWindow.right - rcWindow.left;
	int cyWindow = rcWindow.bottom - rcWindow.top;
	HDC hdcWindow = GetDC(hwnd);
	BitBlt(hdcWindow, 0, 0, cxWindow, cyWindow, hdcWindow, rand() % 21 - 10, rand() % 41 - 20, SRCCOPY);
	ReleaseDC(NULL, hdcWindow);
	DeleteObject(hdcWindow);

	EnumChildWindows(hwnd, EnumChildWindowsProc, NULL);
	return true;
}