// Hydrargyrum.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#pragma warning(disable: 4995)
#pragma comment(lib, "Dwmapi.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>
#include <cmath>
#include <stdio.h>
#include <tchar.h>
#include <ShlObj.h>
#include "Bytebeats.h"
#include "Payloads.h"
#include "GlitchMsg.h"

int StopThread(HANDLE thread) {
    TerminateThread(thread, 0);
    CloseHandle(thread);
    return 0;
}

int RefreshScreen() {
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN); InvalidateRect(0, 0, 0);
    Sleep(1);
    return 114514;
}

void RunPayload() {
    SetProcessDPIAware();
    DwmEnableComposition(0);
    Sleep(3000);
    CreateThread(0, 0, msgbox, 0, 0, 0);
    Sleep(1500);
    sound1();
    HANDLE Payload1Num1 = CreateThread(0, 0, Payload1_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload1Num1);
    RefreshScreen();

    sound2();
    HANDLE CursorBallThread = CreateThread(0, 0, CursorBall, 0, 0, 0);
    HANDLE Payload2Num1 = CreateThread(0, 0, Payload2_num1, 0, 0, 0);
    HANDLE Payload2Num2 = CreateThread(0, 0, Payload2_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload2Num1); StopThread(Payload2Num2);
    RefreshScreen();

    sound3();
    HANDLE Payload3Num1 = CreateThread(0, 0, Payload3_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload3Num1);
    RefreshScreen();

    sound4();
    HANDLE Payload4Num1 = CreateThread(0, 0, Payload4_num1, 0, 0, 0);
    HANDLE DrawIconsThread = CreateThread(0, 0, DrawIcons, 0, 0, 0);
    HANDLE DrawCursorsThread = CreateThread(0, 0, DrawCursors, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload4Num1);
    RefreshScreen();

    sound5();
    HANDLE Payload5Num1 = CreateThread(0, 0, Payload5_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload5Num1);
    RefreshScreen();

    sound6();
    HANDLE Payload6Num1 = CreateThread(0, 0, Payload6_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload6Num1);
    StopThread(DrawIconsThread); StopThread(DrawCursorsThread);
    RefreshScreen();

    sound7();
    HANDLE Payload7Num1 = CreateThread(0, 0, Payload7_num1, 0, 0, 0);
    HANDLE Payload7Num2 = CreateThread(0, 0, Payload7_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload7Num1); StopThread(Payload7Num2);
    RefreshScreen();

    sound8();
    HANDLE Payload8Num1 = CreateThread(0, 0, Payload8_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload8Num1);
    RefreshScreen();

    sound9();
    HANDLE Payload9Num1 = CreateThread(0, 0, Payload9_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload9Num1);
    RefreshScreen();

    sound10();
    HANDLE Payload10Num1 = CreateThread(0, 0, Payload10_num1, 0, 0, 0);
    HANDLE DrawTextsThread = CreateThread(0, 0, DrawTexts, 0, 0, 0);
    DrawIconsThread = CreateThread(0, 0, DrawIcons, 0, 0, 0);
    DrawCursorsThread = CreateThread(0, 0, DrawCursors, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload10Num1);
    RefreshScreen();

    sound11();
    HANDLE Payload11Num1 = CreateThread(0, 0, Payload11_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload11Num1);
    RefreshScreen();

    sound12();
    HANDLE Payload12Num1 = CreateThread(0, 0, Payload12_num1, 0, 0, 0);
    HANDLE Payload12Num2 = CreateThread(0, 0, Payload12_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload12Num1); StopThread(Payload12Num2);
    RefreshScreen();

    sound13();
    HANDLE Payload13Num1 = CreateThread(0, 0, Payload13_num1, 0, 0, 0);
    HANDLE Payload13Num2 = CreateThread(0, 0, Payload13_num2, 0, 0, 0);
    HANDLE Payload13Num3 = CreateThread(0, 0, Payload13_num3, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload13Num1); StopThread(Payload13Num2); StopThread(Payload13Num3);
    StopThread(DrawTextsThread); StopThread(DrawIconsThread); StopThread(DrawCursorsThread);
    RefreshScreen();

    sound14();
    HANDLE Payload14Num1 = CreateThread(0, 0, Payload14_num1, 0, 0, 0);
    HANDLE BeautifulShell32IconsThread = CreateThread(0, 0, BeautifulShell32Icons, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload14Num1);
    RefreshScreen();

    sound15();
    HANDLE Payload15Num1 = CreateThread(0, 0, Payload15_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload15Num1);
    RefreshScreen();

    sound16();
    HANDLE Payload16Num1 = CreateThread(0, 0, Payload16_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload16Num1);
    RefreshScreen();

    sound17();
    HANDLE Payload17Num1 = CreateThread(0, 0, Payload17_num1, 0, 0, 0);
    DrawTextsThread = CreateThread(0, 0, DrawTexts, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload17Num1);
    StopThread(CursorBallThread);
    RefreshScreen();

    sound18();
    HANDLE Payload18Num1 = CreateThread(0, 0, Payload18_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload18Num1);
    StopThread(DrawTextsThread); StopThread(BeautifulShell32IconsThread);
    RefreshScreen();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    LPCSTR FirstWarningTitle, LastWarningTitle, FirstWarningText, LastWarningText;
    FirstWarningTitle = "WARNING - Hydrargyrum.exe (GDI Only)";
    LastWarningTitle = "LAST WARNING - Hydrargyrum.exe (GDI Only)";
    FirstWarningText = "This is a GDI Only, Are you sure you want to run it?";
    LastWarningText = "Are you sure?\r\nIt won't harm your computer but it contains flashing lights and loud noises - NOT for epilepsy";

    if (MessageBoxA(NULL, FirstWarningText, FirstWarningTitle, MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
        if (MessageBoxA(NULL, LastWarningText, LastWarningTitle, MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
            RunPayload();
            ExitProcess(0);
        }
        else {
            ExitProcess(0);
        }
    }
    else {
        ExitProcess(0);
    }
}