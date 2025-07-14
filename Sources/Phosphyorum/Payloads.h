#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <cmath>
#include <stdio.h>
#include <tchar.h>
#include <ShlObj.h>
#include <time.h>
#include "3DCube.h"
#include "GDIConfig.h"
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "msimg32.lib")
#define RGBBRUSH (DWORD)0x1900ac010e

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
    Sleep(1);
    return 1;
}

DWORD WINAPI DrawIcons(LPVOID lpParam) {
    int size = 0;
    srand(time(NULL));
    while (true) {
        int w = GetSystemMetrics(0); int h = GetSystemMetrics(1);
        for (int i = 0; i < 5; i++) {
            size = 32 + rand() % 118;
            HICON shifang = LoadIcon(NULL, MAKEINTRESOURCE(32512 + rand() % 4));
            MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        }
        Sleep(10);
    }
}

DWORD WINAPI DrawCursors(LPVOID lpParam) {
    HICON shifang; int size = 0;
    srand(time(NULL));
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        shifang = LoadCursor(0, IDC_APPSTARTING); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_ARROW); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_CROSS); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_HELP); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_IBEAM); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_NO); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_SIZEALL); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_SIZENESW); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_SIZENS); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_SIZENWSE); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_SIZEWE); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_UPARROW); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
        shifang = LoadCursor(0, IDC_WAIT); size = 32 + rand() % 118;
        MyNewDrawIcon(rand() % w, rand() % h, size, shifang);
    }
}

DWORD WINAPI DrawTexts(LPVOID lpParam) {
    LPCSTR pszFaceName = "Courier";
    LPCSTR text[9] = { "Phosphyorum", "Element 15", "get r3kt", "Stop strugging and crying.", "You can't do anything about it!" , "Just give up, There's no hope!", "What were you thinking?", "It's all your fault!", "Still using this computer?"};
    srand(time(NULL));
    while (true) {
        int o = rand() % 9, color1 = RGB(rand() % 255, rand() % 255, rand() % 255), color2 = RGB(rand() % 255, rand() % 255, rand() % 255);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        SetTextColor(hdc, color1);
        SetBkColor(hdc, color2);
        HFONT font = CreateFontA(45, 25, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_QUALITY, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, pszFaceName);
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % w, rand() % h, text[o], strlen(text[o]));
        ReleaseDC(0, hdc);
        DeleteObject(font);
        DeleteDC(hdc);
        Sleep(20);
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

DWORD WINAPI cicici2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    while (true) {
        const int size = 5600;
        int x = w / 2, y = h / 2;
        for (int i = 0; i < size; i += 100)
        {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(50);
        }
        Sleep(500);
    }
    return 0;
}

DWORD WINAPI Payload1_num1(LPVOID lpParam) {
    srand(time(NULL));
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 50, 0 }; int count = 0, sss = rand() % 255;
    while (true) {
        int w = GetSystemMetrics(0); int h = GetSystemMetrics(1), rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 }; PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCPAINT);
        for (int x = 0; x < w; x += 35) {
            for (int y = 0; y < h; y += 35) {
                if (rand() % 8 != 1) {
                    StretchBlt(hcdc, x, y, 35, 35, hcdc, x, y, 30, 30, SRCINVERT);
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
        if (rand() % 15 == 14) { RefreshScreen(); Sleep(5); }
        Sleep(5);
    }
}

DWORD WINAPI Payload2_num1(LPVOID lpParam) {
    int i = 0; float fx;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % 40;
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
                int index = y * w + x, fx = (x * i) | (y * i); rgbquadCopy = rgbquad[index];
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
        Sleep(10); i++;
    }
    return 0;
}

DWORD WINAPI Payload3_num1(LPVOID lpParam) {
    int blocksize = 80, increment = 40; POINT point[3];
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
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
        if (rand() % 8 >= 5) { BitBlt(hcdc, -1, 0, w, h, hcdc, 0, 0, SRCAND); }
        if (rand() % 6 >= 4) {
            if (rand() % 10 >= 6) {
                point[0].x = increment;
                point[0].y = -increment;
                point[1].x = w + increment;
                point[1].y = increment;
                point[2].x = -increment;
                point[2].y = h - increment;
            }
            else {
                point[0].x = -increment;
                point[0].y = increment;
                point[1].x = w - increment;
                point[1].y = -increment;
                point[2].x = increment;
                point[2].y = h + increment;
            }
            PlgBlt(hcdc, point, hcdc, 0, 0, w, h, 0, 0, 0);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        blocksize += 5; if (blocksize >= 200) { blocksize = 80; }
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload4_num1(LPVOID lpParam) {
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
                int x = power * cos(angle * 3.1415926 / 8.0), y = power * sin(angle * 3.1415926 / 12.0);
                StretchBlt(hcdc, xxx, yyy, width - x, height - y, hcdc, xxx, yyy, width, height, rop);
            }
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        count++; angle += 3.1415926 / 2;
        if (angle > 365) { angle = 0; } if (count > 60) { count = 0; }
        Sleep(1);
    }
}

DWORD WINAPI Payload4_num2(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y += 50) {
            if (rand() % 10 == 8) {
                StretchBlt(hcdc, -5 + rand() % 10, y, w, 50, hcdc, 0, y, w, 50, SRCPAINT);
            }
            else {
                StretchBlt(hcdc, -5 + rand() % 10, y, w, 50, hcdc, 0, y, w, 50, SRCCOPY);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(50);
    }
}

DWORD WINAPI Payload4_num3(LPVOID lpParam) {
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
                int fx = (int)((i * 4) * sqrt((x | y) * i));
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
        Sleep(20); i++;
    }
}

DWORD WINAPI Payload5_num1(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndrgb = RGB(1 + rand() % 254, 1 + rand() % 254, 1 + rand() % 254);;
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
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload6_num1(LPVOID lpParam) {
    RGBQUAD rgbquadCopy; int i = 0, rop = SRCAND, rndsize = NULL, move = 30;
    srand(time(NULL));
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % 20;
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
                int index = x * h + y, fx = (int)((i ^ 2) + (i * 2) * sqrt(i));
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
            int rand_num_x = rand() % w, rand_num_y = rand() % h;
            int top_x = 0 + rand_num_x, top_y = 0 + rand_num_y;
            int bottom_x = 120 + rand_num_x, bottom_y = 120 + rand_num_y;
            HRGN circle = CreateEllipticRgn(top_x, top_y, bottom_x, bottom_y);
            InvertRgn(hcdc, circle);
            DeleteObject(circle);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); i++;
    }
}

DWORD WINAPI Payload7_num1(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
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
        Sleep(1); i++;
    }
    return 0;
}

DWORD WINAPI Payload7_num2(LPVOID lpParam) {
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        int w = GetSystemMetrics(0); int h = GetSystemMetrics(1), rndsize = 1 + rand() % 40;
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
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI Payload8_num1(LPVOID lpParam) {
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
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload9_num1(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
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
        Sleep(5); i += 2;
    }
    return 0;
}

DWORD WINAPI Payload10_num1(LPVOID lpParam) {
    srand(time(NULL));
    int xxx = 0; BLENDFUNCTION blend = { AC_SRC_OVER, 0, 80, 0 };
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
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload10_num2(LPVOID lpParam) {
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
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload11_num1(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hcdc, hBrush);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        PatBlt(hcdc, 0, 0, w, h, PATINVERT);
        StretchBlt(hcdc, -6, -6, w + 12, h + 12, hcdc, 0, 0, w, h, SRCPAINT);
        BitBlt(hcdc, rand() % 20, rand() % 20, w, h, hcdc, rand() % 20, rand() % 20, SRCCOPY);
        for (int x = 0; x < w; x += 20) {
            StretchBlt(hcdc, x, -5 + rand() % 10, 20, h, hcdc, x, 0, 20, h, SRCPAINT);
        }
        for (int y = 0; y < h; y += 20) {
            StretchBlt(hcdc, -5 + rand() % 10, y, w, 20, hcdc, 0, y, w, 20, SRCPAINT);
        }
        for (int i = 0; i < h; i++) {
            StretchBlt(hcdc, -2 + (rand() % 5), i, w, 1, hcdc, 0, i, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap); DeleteObject(hBrush);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload12_num1(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
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
        Sleep(1); i += 5;
    }
}

DWORD WINAPI Payload13_num1(LPVOID lpParam) {
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 60, 0 };
    int size = 450; int rop = SRCERASE;
    while (true) {
        int w = GetSystemMetrics(0); int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 }; PRGBQUAD rgbScreen = { 0 };
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
            BitBlt(hcdc, x, y, size, size, hcdc, x + rand() % 20 - 9, y + rand() % 20 - 9, SRCPAINT);
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload14_num1(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0); int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 }; PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w; rgbScreen[i].rgb += x ^ i * y ^ x;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload15_num1(LPVOID lpParam) {
    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 }; int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
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
                FLOAT fx = (x + (i + i * 50));
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
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload15_num2(LPVOID lpParam) {
    srand(time(NULL));
    int size_w = 450, size_h = 270;
    int signX = 1, signY = 1;
    int incrementor = 10;
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int x = rand() % (w - size_w), y = rand() % (h - size_h);
    while (true) {
        w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        x += incrementor * signX, y += incrementor * signY;
        HDC hdc = GetDC(0);
        StretchBlt(hdc, x, y, size_w, size_h, hdc, 0, 0, w, h, SRCCOPY);
        ReleaseDC(0, hdc);
        DeleteDC(hdc);
        if (y >= h - size_h) { signY = -1; }
        else if (x >= w - size_w) { signX = -1; }
        else if (y <= 0) { signY = 1; }
        else if (x <= 0) { signX = 1; }
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload16_num1(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 }; PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        BitBlt(hcdc, -1, 0, w, h, hcdc, 0, 0, SRCAND);
        BitBlt(hcdc, 0, 1, w, h, hcdc, 0, 0, SRCPAINT);
        BitBlt(hcdc, 1, 0, w, h, hcdc, 0, 0, SRCPAINT);
        BitBlt(hcdc, 0, -1, w, h, hcdc, 0, 0, SRCAND);
        for (int i = 0; i < w * h; i++) {
            rgbScreen[i].rgb -= ((i & w) + h) & ((i * 4) + (i / h));
        }
        for (int y = 0; y < h; y += 40) {
            StretchBlt(hcdc, -1 + rand() % 3, y, w, 40, hcdc, 0, y, w, 40, SRCPAINT);
        }
        for (int x = 0; x < w; x += 40) {
            StretchBlt(hcdc, x, -1 + rand() % 3, 40, h, hcdc, x, 0, 40, h, SRCPAINT);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
}

DWORD WINAPI Payload17_num1(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x, fx = ((x + (10 * i)) ^ y) + (i * 10);
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
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload18_num1(LPVOID lpParam) {
    int i = 0, xxx = 0; BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rndsize = 1 + rand() % 40;
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
            int x = rand() % w, y = rand() % h; HRGN hrgn = CreateEllipticRgn(x, y, x + 240, y + 240);
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
        Sleep(1); i++;
    }
    return 0;
}

DWORD WINAPI Payload19_num1(LPVOID lpParam) {
    HBRUSH hBrush; int size = 128;
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
            StretchBlt(hcdc, x, -20 + rand() % 40, 50, h, hcdc, x, 0, 50, h, SRCAND);
        }
        for (int y = 0; y < h; y += 50) {
            StretchBlt(hcdc, -20 + rand() % 40, y, w, 50, hcdc, 0, y, w, 50, SRCPAINT);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI Payload20_num1(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
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
        Sleep(1);
    }
    return 0;
}