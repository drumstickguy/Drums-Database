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
            CreateThread(0, 0, fake, 0, 0, 0);
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
        }
    }
}