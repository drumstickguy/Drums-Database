#include "GDIConfig.h"
HWND hDlg;
HANDLE glitchmsgbox1;
int MessageBoxWidth, MessageBoxHeight;
BOOL AlreadyStartGlitch = false;
HHOOK hHook = NULL;

BOOL CALLBACK EnumProc114(HWND hWnd, LPARAM lParam) {
    SetWindowTextA(hWnd, "");
    ShowWindow(hWnd, 0);
    EnableWindow(hWnd, FALSE);
    return 1;
}

DWORD WINAPI msgboxglitch(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(hDlg), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), MessageBoxWidth, MessageBoxHeight, 1, 32 };
        PRGBQUAD rgbScreen = { 0 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, MessageBoxWidth, MessageBoxHeight, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < MessageBoxWidth * MessageBoxHeight; i++) {
            rgbScreen[i].rgb = (rand() % 256) * 2106627;
        }
        for (int x = 0; x < MessageBoxWidth; x += 50) {
            for (int y = 0; y < MessageBoxHeight; y += 50) {
                StretchBlt(hcdc, x, y, 50, 50, hcdc, x, y, 28, 28, SRCCOPY);
            }
        }
        BitBlt(hdc, 0, 0, MessageBoxWidth, MessageBoxHeight, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(5);
    }
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HCBT_ACTIVATE) {
        if (AlreadyStartGlitch == true) {
            return 114514; //·ÀÖ¹ÄÚ´æÐ¹Â©
        }
        AlreadyStartGlitch = true;
        hDlg = (HWND)wParam;
        RECT rect;
        GetWindowRect(hDlg, &rect);
        MessageBoxWidth = rect.right - rect.left;
        MessageBoxHeight = rect.bottom - rect.top;
        EnumChildWindows(hDlg, EnumProc114, 0);
        glitchmsgbox1 = CreateThread(0, 0, msgboxglitch, 0, 0, 0);
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

DWORD WINAPI msgbox(LPVOID lpParam) {
    while (true) {
        hHook = SetWindowsHookEx(WH_CBT, &CBTProc, NULL, GetCurrentThreadId());
        MessageBoxA(NULL, "", "You messed up.", MB_ICONERROR | MB_ABORTRETRYIGNORE);
        if (hHook != NULL) {
            UnhookWindowsHookEx(hHook);
        }
        AlreadyStartGlitch = false;
        TerminateThread(glitchmsgbox1, 0);
        CloseHandle(glitchmsgbox1);
        Sleep(10);
    }
    return 0;
}