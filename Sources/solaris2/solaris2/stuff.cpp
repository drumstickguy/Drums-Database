#include "main.h";
#include "stuff.h";

double intensity = 0.0;
bool state = false;

double fade(double maxIntensity) {
    if (state == false) {
        intensity += 1.0;
        if (intensity >= maxIntensity) {
            state = true;
        }
    }
    else {
        intensity -= 1.0;
        if (intensity <= 0) {
            state = false;
        }
    }
    return intensity;
}

int red, green, blue;

COLORREF COLORRGB(int length, int speed) {
    if (red < length) {
        red += speed;

        return RGB(red, 0, length);
    }
    else if (green < length) {
        green += speed;

        return RGB(length, green, 0);
    }
    else if (blue < length) {
        blue += speed;

        return RGB(0, length, blue);
    }
    else {
        red = 0; green = 0; blue = 0;

        return RGB(0, length, length);
    }
}

COLORREF COLORHSL(int length) {
    double h = fmod(length, 360.0);
    double s = 1.0;
    double l = 0.5;

    double c = (1.0 - fabs(2.0 * l - 1.0)) * s;
    double x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
    double m = l - c / 2.0;

    double r1, g1, b1;
    if (h < 60) {
        r1 = c;
        g1 = x;
        b1 = 0;
    }
    else if (h < 120) {
        r1 = x;
        g1 = c;
        b1 = 0;
    }
    else if (h < 180) {
        r1 = 0;
        g1 = c;
        b1 = x;
    }
    else if (h < 240) {
        r1 = 0;
        g1 = x;
        b1 = c;
    }
    else if (h < 300) {
        r1 = x;
        g1 = 0;
        b1 = c;
    }
    else {
        r1 = c;
        g1 = 0;
        b1 = x;
    }

    int red = static_cast<int>((r1 + m) * 255);
    int green = static_cast<int>((g1 + m) * 255);
    int blue = static_cast<int>((b1 + m) * 255);

    return RGB(red, green, blue);
}

void RGBtoHSV(COLORREF rgb, double& h, double& s, double& v) {
    double r = GetRValue(rgb) / 255.0;
    double g = GetGValue(rgb) / 255.0;
    double b = GetBValue(rgb) / 255.0;

    double cmax = max(r, max(g, b));
    double cmin = min(r, min(g, b));
    double delta = cmax - cmin;

    if (delta == 0) {
        h = 0;
    }
    else if (cmax == r) {
        h = 60.0 * fmod(((g - b) / delta), 6.0);
    }
    else if (cmax == g) {
        h = 60.0 * (((b - r) / delta) + 2.0);
    }
    else {
        h = 60.0 * (((r - g) / delta) + 4.0);
    }

    if (h < 0) {
        h += 360.0;
    }

    s = (cmax == 0) ? 0 : delta / cmax;
    v = cmax;
}

COLORREF HSVtoRGB(double h, double s, double v) {
    double c = v * s;
    double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = v - c;

    double r, g, b;
    if (h < 60) {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 120) {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 180) {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 240) {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 300) {
        r = x;
        g = 0;
        b = c;
    }
    else {
        r = c;
        g = 0;
        b = x;
    }

    int red = static_cast<int>((r + m) * 255);
    int green = static_cast<int>((g + m) * 255);
    int blue = static_cast<int>((b + m) * 255);

    return RGB(red, green, blue);
}

VOID WINAPI KillPayload(HANDLE h) {
    TerminateThread(h, 0);
    CloseHandle(h);
}

VOID WINAPI RedrawScreen(HWND hwnd) {
    RedrawWindow(hwnd, 0, 0, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
}

VOID WINAPI InvertColor(DWORD& color) {
    BYTE* bytes = (BYTE*)&color;
    bytes[0] = ~bytes[0];
    bytes[1] = ~bytes[1];
    bytes[2] = ~bytes[2];
}

VOID WINAPI delay(unsigned int milliseconds) {
    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

        if (elapsed >= milliseconds) {
            break;
        }

        std::this_thread::yield();
    }
}

double SineWave(double amplitude, double frequency, double time, double sampleRate) {
    return amplitude * sin(2.0 * 3.14159 * frequency * time / sampleRate);
}