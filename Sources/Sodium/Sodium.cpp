#pragma warning(disable: 4995)
#pragma comment(lib, "Dwmapi.lib")
#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>
#include <cmath>
#include <stdio.h>
#include <tchar.h>
#include <ShlObj.h>
#include "Sodium_Bytebeats.h"
#include "Sodium_Payloads.h"
#include "Sodium_3DCube.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

int StopThread(HANDLE thread) {
    TerminateThread(thread, 0);
    CloseHandle(thread);
    return 0;
}

BOOL RunPayload() {
    Sleep(10);
    DwmEnableComposition(0);
    sound1();
    HANDLE Payload1_thread1 = CreateThread(0, 0, Payload1_num1, 0, 0, 0);
    HANDLE Payload1_thread2 = CreateThread(0, 0, Payload1_num2, 0, 0, 0);
    HANDLE Payload1_thread3 = CreateThread(0, 0, Payload1_num3, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload1_thread1);
    StopThread(Payload1_thread2);
    StopThread(Payload1_thread3);
    RefreshScreen();

    sound2();
    HANDLE Payload2_thread1 = CreateThread(0, 0, Payload2_num1, 0, 0, 0);
    HANDLE Payload2_thread2 = CreateThread(0, 0, Payload2_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload2_thread1);
    StopThread(Payload2_thread2);
    RefreshScreen();

    sound3();
    HANDLE Payload3_thread1 = CreateThread(0, 0, Payload3_num1, 0, 0, 0);
    HANDLE drawiconsthread = CreateThread(0, 0, DrawIcons, 0, 0, 0);
    HANDLE drawcursorsthread = CreateThread(0, 0, DrawCursors, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload3_thread1);
    RefreshScreen();

    sound4();
    HANDLE Payload4_thread1 = CreateThread(0, 0, Payload4_num1, 0, 0, 0);
    HANDLE Payload4_thread2 = CreateThread(0, 0, Payload4_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload4_thread1);
    StopThread(Payload4_thread2);
    StopThread(drawiconsthread);
    StopThread(drawcursorsthread);
    RefreshScreen();

    sound5();
    HANDLE Payload5_thread1 = CreateThread(0, 0, Payload5_num1, 0, 0, 0);
    HANDLE Payload5_thread2 = CreateThread(0, 0, Payload5_num2, 0, 0, 0);
    HANDLE Payload5_thread3 = CreateThread(0, 0, Payload5_num3, 0, 0, 0);
    HANDLE cubethread = CreateThread(0, 0, Payload_3DCube, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload5_thread1);
    StopThread(Payload5_thread2);
    StopThread(Payload5_thread3);
    RefreshScreen();

    sound6();
    HANDLE Payload6_thread1 = CreateThread(0, 0, Payload6_num1, 0, 0, 0);
    HANDLE Payload6_thread2 = CreateThread(0, 0, Payload6_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload6_thread1);
    StopThread(Payload6_thread2);
    RefreshScreen();

    sound7();
    HANDLE Payload7_thread1 = CreateThread(0, 0, Payload7_num1, 0, 0, 0);
    HANDLE Payload7_thread2 = CreateThread(0, 0, Payload7_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload7_thread1);
    StopThread(Payload7_thread2);
    RefreshScreen();

    sound8();
    HANDLE Payload8_thread1 = CreateThread(0, 0, Payload8_num1, 0, 0, 0);
    HANDLE Payload8_thread2 = CreateThread(0, 0, Payload8_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload8_thread1);
    StopThread(Payload8_thread2);
    RefreshScreen();

    sound9();
    HANDLE Payload9_thread1 = CreateThread(0, 0, Payload9_num1, 0, 0, 0);
    HANDLE Payload9_thread2 = CreateThread(0, 0, Payload9_num2, 0, 0, 0);
    HANDLE Payload9_thread3 = CreateThread(0, 0, Payload9_num3, 0, 0, 0);
    drawiconsthread = CreateThread(0, 0, DrawIcons, 0, 0, 0);
    drawcursorsthread = CreateThread(0, 0, DrawCursors, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload9_thread1);
    StopThread(Payload9_thread2);
    StopThread(Payload9_thread3);
    RefreshScreen();

    sound10();
    HANDLE Payload10_thread1 = CreateThread(0, 0, Payload10_num1, 0, 0, 0);
    HANDLE Payload10_thread2 = CreateThread(0, 0, Payload10_num2, 0, 0, 0);
    HANDLE Payload10_thread3 = CreateThread(0, 0, Payload10_num3, 0, 0, 0);
    HANDLE drawtextsthread = CreateThread(0, 0, DrawTexts, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload10_thread1);
    StopThread(Payload10_thread2);
    StopThread(Payload10_thread3);
    RefreshScreen();

    sound11();
    HANDLE Payload11_thread1 = CreateThread(0, 0, Payload11_num1, 0, 0, 0);
    HANDLE Payload11_thread2 = CreateThread(0, 0, Payload11_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload11_thread1);
    StopThread(Payload11_thread2);
    StopThread(drawtextsthread);
    RefreshScreen();

    sound12();
    HANDLE Payload12_thread1 = CreateThread(0, 0, Payload12_num1, 0, 0, 0);
    HANDLE Payload12_thread2 = CreateThread(0, 0, Payload12_num2, 0, 0, 0);
    HANDLE Payload12_thread3 = CreateThread(0, 0, Payload12_num3, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload12_thread1);
    StopThread(Payload12_thread2);
    StopThread(Payload12_thread3);
    RefreshScreen();

    sound13();
    HANDLE Payload13_thread1 = CreateThread(0, 0, Payload13_num1, 0, 0, 0);
    HANDLE Payload13_thread2 = CreateThread(0, 0, Payload13_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload13_thread1);
    StopThread(Payload13_thread2);
    RefreshScreen();

    sound14();
    HANDLE Payload14_thread1 = CreateThread(0, 0, Payload14_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload14_thread1);
    RefreshScreen();

    sound15();
    HANDLE Payload15_thread1 = CreateThread(0, 0, Payload15_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload15_thread1);
    RefreshScreen();

    sound16();
    HANDLE Payload16_thread1 = CreateThread(0, 0, Payload16_num1, 0, 0, 0);
    HANDLE Payload16_thread2 = CreateThread(0, 0, Payload16_num2, 0, 0, 0);
    HANDLE Payload16_thread3 = CreateThread(0, 0, Payload16_num3, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload16_thread1);
    StopThread(Payload16_thread2);
    StopThread(Payload16_thread3);
    RefreshScreen();

    sound17();
    HANDLE Payload17_thread1 = CreateThread(0, 0, Payload17_num1, 0, 0, 0);
    HANDLE Payload17_thread2 = CreateThread(0, 0, Payload17_num2, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload17_thread1);
    StopThread(Payload17_thread2);
    RefreshScreen();

    sound18();
    HANDLE Payload18_thread1 = CreateThread(0, 0, Payload18_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload18_thread1);
    RefreshScreen();

    sound19();
    HANDLE Payload19_thread1 = CreateThread(0, 0, Payload19_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload19_thread1);
    RefreshScreen();

    sound20();
    HANDLE Payload20_thread1 = CreateThread(0, 0, Payload20_num1, 0, 0, 0);
    HANDLE Payload20_thread2 = CreateThread(0, 0, Payload20_num2, 0, 0, 0);
    HANDLE Payload20_thread3 = CreateThread(0, 0, Payload20_num3, 0, 0, 0);
    HANDLE Payload20_thread4 = CreateThread(0, 0, Payload20_num4, 0, 0, 0);
    HANDLE Payload20_thread5 = CreateThread(0, 0, Payload20_num5, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload20_thread1);
    StopThread(Payload20_thread2);
    StopThread(Payload20_thread3);
    StopThread(Payload20_thread4);
    StopThread(Payload20_thread5);
    RefreshScreen();

    sound21();
    HANDLE Payload21_thread1 = CreateThread(0, 0, Payload21_num1, 0, 0, 0);
    HANDLE Payload21_thread2 = CreateThread(0, 0, Payload21_num2, 0, 0, 0);
    HANDLE Payload21_thread3 = CreateThread(0, 0, Payload21_num3, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload21_thread1);
    StopThread(Payload21_thread2);
    StopThread(Payload21_thread3);
    RefreshScreen();

    sound22();
    HANDLE Payload22_thread1 = CreateThread(0, 0, Payload22_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload22_thread1);
    RefreshScreen();

    sound23();
    HANDLE Payload23_thread1 = CreateThread(0, 0, Payload23_num1, 0, 0, 0);
    HANDLE Payload23_thread2 = CreateThread(0, 0, Payload23_num2, 0, 0, 0);
    drawtextsthread = CreateThread(0, 0, DrawTexts, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload23_thread1);
    StopThread(Payload23_thread2);
    RefreshScreen();

    sound24();
    HANDLE Payload24_thread1 = CreateThread(0, 0, Payload24_num1, 0, 0, 0);
    Sleep(30000);
    StopThread(Payload24_thread1);
    StopThread(drawiconsthread);
    StopThread(drawcursorsthread);
    StopThread(drawtextsthread);
    StopThread(cubethread);
    RefreshScreen();
    return 0;
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //---= Sodium.exe =----
    SetProcessDPIAware();
    LPCSTR FirstWarning_Title = "Sodium.exe - Malware alert";
    LPCSTR LastWarning_Title = "Sodium.exe - LAST WARNING";
    LPCSTR FirstWarning_Text = "WARNING!\r\n\r\nYou are running a safety GDI malware called Sodium.exe by Coder-Linjian and WaterdragonWX-Zoey.\r\nThis malware will not destroy your computer. But it will show some cool effects on your screen and play some loud noises.\r\nIf you are running this program without the knowledge of what will this program do, Just click \"No\" to quit this malware.\r\n\r\nDo you want to execute this malware?";
    LPCSTR LastWarning_Text = "LAST WARNING!\r\n\r\nBefore you decide to continue execute Sodium.exe, Please make sure you are running in a safe environment!\r\nIf you have photosensitive epilepsy, Please don't execute this malware!\r\n\r\nDo you really want to run this malware? Coder-Linjian and WaterdragonWX-Zoey are not responsible for any damages!";

    if (MessageBoxA(NULL, FirstWarning_Text, FirstWarning_Title, MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
        if (MessageBoxA(NULL, LastWarning_Text, LastWarning_Title, MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
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