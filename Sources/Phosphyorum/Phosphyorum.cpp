// Phosphyorum.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4995)
#pragma comment(lib, "Dwmapi.lib")
#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>
#include <cmath>
#include <stdio.h>
#include <tchar.h>
#include <ShlObj.h>
#include "Bytebeats.h"
#include "Payloads.h"

int StopThread(HANDLE thread) {
    TerminateThread(thread, 0);
    CloseHandle(thread);
    return 0;
}

DWORD WINAPI fakeerror(LPVOID lpParam) {
    MessageBoxA(NULL, "C:\\ is not accessible, Access denied.", "Local Disk (C:\\)", MB_ICONERROR);
    return 0;
}

HANDLE drawcubethread = NULL, drawiconsthread = NULL, drawcursorsthread = NULL, drawtextsthread = NULL, cicicithread = NULL;
void RunPayload() {
    sound1();
    HANDLE Payload1Num1 = CreateThread(0, 0, Payload1_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload1Num1);
    RefreshScreen();

    sound2();
    drawiconsthread = CreateThread(0, 0, DrawIcons, 0, 0, 0);
    drawcursorsthread = CreateThread(0, 0, DrawCursors, 0, 0, 0);
    HANDLE Payload2Num1 = CreateThread(0, 0, Payload2_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload2Num1);
    RefreshScreen();

    sound3();
    HANDLE Payload3Num1 = CreateThread(0, 0, Payload3_num1, 0, 0, 0);
    cicicithread = CreateThread(0, 0, cicici, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload3Num1);
    RefreshScreen();

    sound4();
    HANDLE Payload4Num1 = CreateThread(0, 0, Payload4_num1, 0, 0, 0);
    HANDLE Payload4Num2 = CreateThread(0, 0, Payload4_num2, 0, 0, 0);
    HANDLE Payload4Num3 = CreateThread(0, 0, Payload4_num3, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload4Num1); StopThread(Payload4Num2); StopThread(Payload4Num3); StopThread(cicicithread);
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
    RefreshScreen();

    sound7();
    drawcubethread = CreateThread(0, 0, Payload_3DCube, 0, 0, 0);
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
    HANDLE Payload10Num2 = CreateThread(0, 0, Payload10_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload10Num1); StopThread(Payload10Num2);
    RefreshScreen();

    sound11();
    HANDLE Payload11Num1 = CreateThread(0, 0, Payload11_num1, 0, 0, 0);
    drawtextsthread = CreateThread(0, 0, DrawTexts, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload11Num1);
    RefreshScreen();

    sound12();
    HANDLE Payload12Num1 = CreateThread(0, 0, Payload12_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload12Num1);
    RefreshScreen();

    sound13();
    HANDLE Payload13Num1 = CreateThread(0, 0, Payload13_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload13Num1);
    RefreshScreen();

    sound14();
    HANDLE Payload14Num1 = CreateThread(0, 0, Payload14_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload14Num1); StopThread(drawcubethread);
    RefreshScreen();

    sound15();
    HANDLE Payload15Num1 = CreateThread(0, 0, Payload15_num1, 0, 0, 0);
    HANDLE Payload15Num2 = CreateThread(0, 0, Payload15_num2, 0, 0, 0);
    cicicithread = CreateThread(0, 0, cicici, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload15Num1); StopThread(Payload15Num2); StopThread(cicicithread);
    RefreshScreen();

    sound16();
    HANDLE Payload16Num1 = CreateThread(0, 0, Payload16_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload16Num1);
    RefreshScreen();

    sound17();
    HANDLE Payload17Num1 = CreateThread(0, 0, Payload17_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload17Num1);
    RefreshScreen();

    sound18();
    HANDLE Payload18Num1 = CreateThread(0, 0, Payload18_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload18Num1);
    RefreshScreen();

    sound19();
    HANDLE Payload19Num1 = CreateThread(0, 0, Payload19_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload19Num1); StopThread(drawiconsthread); StopThread(drawcursorsthread); StopThread(drawtextsthread);
    RefreshScreen();

    sound20();
    HANDLE Payload20Num1 = CreateThread(0, 0, Payload20_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload20Num1);
    RefreshScreen();
    return;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SetProcessDPIAware();
    if (MessageBoxA(NULL, "This is a GDI Only, Are you sure you want to run it?", "WARNING - Phosphyorum.exe (GDI Only)", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
        if (MessageBoxA(NULL, "Are you sure?\r\nIt won't harm your computer but it contains flashing lights and loud noises - NOT for epilepsy", "LAST WARNING - Phosphyorum.exe (GDI Only)", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
            DwmEnableComposition(0);
            CreateThread(0, 0, fakeerror, 0, 0, 0);
            Sleep(5000);
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