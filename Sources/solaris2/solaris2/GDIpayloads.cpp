#include "main.h";
#include "stuff.h";
#include "GDIpayloads.h";

typedef union COLOR {
    COLORREF rgb;
    COLORREF hsv;
} COLOR;

typedef union COLOR2 {
    COLORREF rgb;
    struct {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE alpha;
    };
} COLOR2;

DWORD WINAPI firstPayload(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    int blockSize = 8;

    COLOR2* data = (COLOR2*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR2), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    BLENDFUNCTION blur = { 0 };
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 50;

    while (true) {
        StretchBlt(mdc, 0, 0, x, y, hdc, 0, 0, x, y, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR2), data);

        for (int i = 0; i < y; i += blockSize) {
            for (int j = 0; j < x; j += blockSize) {
                int r = 0, g = 0, b = 0;
                for (int ii = 0; ii < blockSize && (i + ii) < y; ii++) {
                    for (int jj = 0; jj < blockSize && (j + jj) < x; jj++) {
                        COLOR2 color = data[(i + ii) * x + (j + jj)];
                        r += GetRValue(color.rgb);
                        g += GetGValue(color.rgb);
                        b += GetBValue(color.rgb);
                    }
                }
                int numPixels = blockSize * blockSize;
                r /= numPixels;
                g /= numPixels;
                b /= numPixels;

                BYTE grayValue = (BYTE)(0.299 * r + 0.587 * g + 0.114 * b);

                r = (r + grayValue) / 2;
                g = (g + grayValue) / 2;
                b = (b + grayValue) / 2;

                r = r / 32 * 32;
                g = g / 32 * 32;
                b = b / 32 * 32;

                COLOR2 blendedColor = { RGB(r, g, b) };
                for (int ii = 0; ii < blockSize && (i + ii) < y; ii++) {
                    for (int jj = 0; jj < blockSize && (j + jj) < x; jj++) {
                        data[(i + ii) * x + (j + jj)] = blendedColor;
                    }
                }
            }
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR2), data);
        GdiAlphaBlend(hdc, 0, 0, x, y, mdc, 0, 0, x, y, blur);

        Sleep(200);

        if (rand() % 9 == 0) {
            RedrawScreen(0);
        }
    }
    return 0;
}

DWORD WINAPI stretch(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int rx = 0;
    int ry = 0;

    HDC hdc = GetDC(0);

    while (true) {
        hdc = GetDC(0);

        for (int i = 0; i < 40; i++) {
            BitBlt(hdc, -5, -5, x / 2, y / 2, hdc, 0, 0, SRCAND); //0x92b246a8clu

            BitBlt(hdc, x / 2 + 5, -5, x / 2, y / 2, hdc, x / 2, 0, SRCAND);

            BitBlt(hdc, -5, y / 2 + 5, x / 2, y / 2, hdc, 0, y / 2, SRCAND);

            BitBlt(hdc, x / 2 + 5, y / 2 + 5, x / 2, y / 2, hdc, x / 2, y / 2, SRCAND);

            Sleep(20);
        }

        RedrawScreen(0);

        for (int i = 0; i < 40; i++) {
            BitBlt(hdc, -5, -5, x / 2, y / 2, hdc, 0, 0, SRCPAINT);

            BitBlt(hdc, x / 2 + 5, -5, x / 2, y / 2, hdc, x / 2, 0, SRCPAINT);

            BitBlt(hdc, -5, y / 2 + 5, x / 2, y / 2, hdc, 0, y / 2, SRCPAINT);

            BitBlt(hdc, x / 2 + 5, y / 2 + 5, x / 2, y / 2, hdc, x / 2, y / 2, SRCPAINT);

            Sleep(20);
        }

        RedrawScreen(0);

        for (int i = 0; i < 40; i++) {
            BitBlt(hdc, -5, -5, x / 2, y / 2, hdc, 0, 0, SRCCOPY);

            BitBlt(hdc, x / 2 + 5, -5, x / 2, y / 2, hdc, x / 2, 0, SRCCOPY);

            BitBlt(hdc, -5, y / 2 + 5, x / 2, y / 2, hdc, 0, y / 2, SRCCOPY);

            BitBlt(hdc, x / 2 + 5, y / 2 + 5, x / 2, y / 2, hdc, x / 2, y / 2, SRCCOPY);

            Sleep(20);
        }

        RedrawScreen(0);

        for (int i = 0; i < 40; i++) {
            BitBlt(hdc, -5, -5, x / 2, y / 2, hdc, 0, 0, 0x999999);

            BitBlt(hdc, x / 2 + 5, -5, x / 2, y / 2, hdc, x / 2, 0, 0x999999);

            BitBlt(hdc, -5, y / 2 + 5, x / 2, y / 2, hdc, 0, y / 2, 0x999999);

            BitBlt(hdc, x / 2 + 5, y / 2 + 5, x / 2, y / 2, hdc, x / 2, y / 2, 0x999999);

            Sleep(20);
        }

        RedrawScreen(0);
    }
    return 0;
}

DWORD WINAPI xWaves(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int size = 0;

    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    while (true) {
        hdc = GetDC(0);

        for (int i = 0; i < 10; i++) {
            StretchBlt(mdc, 0, 0, x, y, hdc, 0, 0, x, y, SRCCOPY);

            size = rand() % 360;

            for (int j = 0; j < x + y; j++) {
                float angle = (sin((i + j) * (3.14159 / (size * 2))) * size) + fade(size);

                BitBlt(hdc, j, angle, 1, y, mdc, j, 0, SRCCOPY);
                delay(1);
            }
            Sleep(50);
        }
    }
    return 0;
}

DWORD WINAPI hueNoShift(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int color = 0;

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    double hue, saturation, lightness;

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        color = rand() % 360;

        for (int i = 0; i < x * y; i++) {
            RGBtoHSV(data[i].hsv, hue, saturation, lightness);

            hue = fmod(color, 360);

            data[i].hsv = HSVtoRGB(hue, saturation, lightness);
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(1000);
    }
}

DWORD WINAPI SquaresRGB(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int rx = 0;
    int ry = 0;

    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    BLENDFUNCTION blur = { 0 };
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 20;

    while (true) {
        for (int i = 0; i < 4; i++) {
            rx = rand() % (x - 200);
            ry = rand() % (y - 200);

            BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);

            PatBlt(mdc, rx, ry, 200, 200, WHITENESS);

            GdiAlphaBlend(hdc, 0, 0, x, y, mdc, 0, 0, x, y, blur);

            Sleep(10);
        }
    }
}

DWORD WINAPI thirdPayload(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    COLOR* pixels = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, pixels);
    SelectObject(mdc, bmp);

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), pixels);

        for (int i = 0; i < x * y; i++) {
            BYTE red = (GetRValue(pixels[i].rgb) ^ (i % x)) + 2;
            BYTE green = (GetGValue(pixels[i].rgb) ^ (i / y)) + 2;
            BYTE blue = (GetBValue(pixels[i].rgb) ^ (i / (x * y))) + 2;
            pixels[i].rgb = RGB(red, green, blue);
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), pixels);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(10);
    }
    return 0;
}

DWORD WINAPI shapesPayload(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int speed = 50;
    int size = 0;
    int x2, y2;

    HRGN hrgn;
    XFORM xForm;
    HDC hdc = GetDC(0);

    while (true) {
        size = 500;

        for (int i = 0; i < 4; i++) {
            x2 = rand() % (x + size) - size / 2;
            y2 = rand() % (y + size) - size / 2;

            for (int j = 100; j <= size; j += 100) {
                float angle = (rand() % 360) * (3.14159f / 180.0f);
                xForm.eM11 = cos(angle);
                xForm.eM12 = sin(angle);
                xForm.eM21 = -sin(angle);
                xForm.eM22 = cos(angle);
                xForm.eDx = (float)x2;
                xForm.eDy = (float)y2;

                SetWorldTransform(hdc, &xForm);

                hrgn = CreateEllipticRgn(x2 - j / 2, y2 - j / 2, x2 + j / 2, y2 + j / 2);

                SelectClipRgn(hdc, hrgn);
                BitBlt(hdc, x2 - j / 2, y2 - j / 2, j, j, hdc, x2 - j / 2, y2 - j / 2, NOTSRCCOPY);
                DeleteObject(hrgn);

                ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
                delay(speed);
            }
        }

        for (int i = 0; i < 4; i++) {
            x2 = rand() % (x + size) - size / 2;
            y2 = rand() % (y + size) - size / 2;

            for (int j = 100; j <= size; j += 100) {
                float angle = (rand() % 360) * (3.14159f / 180.0f);
                xForm.eM11 = cos(angle);
                xForm.eM12 = sin(angle);
                xForm.eM21 = -sin(angle);
                xForm.eM22 = cos(angle);
                xForm.eDx = (float)x2;
                xForm.eDy = (float)y2;

                SetWorldTransform(hdc, &xForm);

                hrgn = CreateRectRgn(x2 - j / 2, y2 - j / 2, x2 + j / 2, y2 + j / 2);

                SelectClipRgn(hdc, hrgn);
                BitBlt(hdc, x2 - j / 2, y2 - j / 2, j, j, hdc, x2 - j / 2, y2 - j / 2, NOTSRCCOPY);
                DeleteObject(hrgn);

                ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
                delay(speed);
            }
        }

        for (int i = 0; i < 4; i++) {
            x2 = rand() % (x + size) - size / 2;
            y2 = rand() % (y + size) - size / 2;

            for (int j = 100; j <= size; j += 100) {
                POINT vertices[6];
                vertices[0].x = x2;
                vertices[0].y = y2 - j / 2;
                vertices[1].x = x2 + j / 2;
                vertices[1].y = y2 - j / 4;
                vertices[2].x = x2 + j / 2;
                vertices[2].y = y2 + j / 4;
                vertices[3].x = x2;
                vertices[3].y = y2 + j / 2;
                vertices[4].x = x2 - j / 2;
                vertices[4].y = y2 + j / 4;
                vertices[5].x = x2 - j / 2;
                vertices[5].y = y2 - j / 4;

                float angle = (rand() % 360) * (3.14159f / 180.0f);
                xForm.eM11 = cos(angle);
                xForm.eM12 = sin(angle);
                xForm.eM21 = -sin(angle);
                xForm.eM22 = cos(angle);
                xForm.eDx = (float)x2;
                xForm.eDy = (float)y2;

                SetWorldTransform(hdc, &xForm);

                hrgn = CreatePolygonRgn(vertices, 6, WINDING);

                SelectClipRgn(hdc, hrgn);
                BitBlt(hdc, x2 - j / 2, y2 - j / 2, j, j, hdc, x2 - j / 2, y2 - j / 2, NOTSRCCOPY);
                DeleteObject(hrgn);

                ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
                delay(speed);
            }
        }

        for (int i = 0; i < 4; i++) {
            x2 = rand() % (x + size) - size / 2;
            y2 = rand() % (y + size) - size / 2;

            for (int j = 100; j <= size; j += 100) {
                POINT vertices[3];
                vertices[0].x = x2;
                vertices[0].y = y2 - j / 2;
                vertices[1].x = x2 - j / 2;
                vertices[1].y = y2 + j / 2;
                vertices[2].x = x2 + j / 2;
                vertices[2].y = y2 + j / 2;

                float angle = (rand() % 360) * (3.14159f / 180.0f);
                xForm.eM11 = cos(angle);
                xForm.eM12 = sin(angle);
                xForm.eM21 = -sin(angle);
                xForm.eM22 = cos(angle);
                xForm.eDx = (float)x2;
                xForm.eDy = (float)y2;

                SetWorldTransform(hdc, &xForm);

                hrgn = CreatePolygonRgn(vertices, 3, WINDING);

                SelectClipRgn(hdc, hrgn);
                BitBlt(hdc, x2 - j / 2, y2 - j / 2, j, j, hdc, x2 - j / 2, y2 - j / 2, NOTSRCCOPY);
                DeleteObject(hrgn);

                ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
                delay(speed);
            }
        }

        if (rand() % 5 == 0) {
            RedrawScreen(0);
        }
    }

    ReleaseDC(0, hdc);
    return 0;
}

DWORD WINAPI circlePayload(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int maxSize = 0;
    int minSize = 0;

    HDC hdc = GetDC(0);
    HRGN hrgn;
    int speed = 50;

    while (true) {
        maxSize = rand() % 700;
        minSize = rand() % 100;

        int initialSize = rand() % (maxSize - minSize + 1) + minSize;
        int currentSize = initialSize;

        int xPos = rand() % (x + currentSize) - currentSize / 2;
        int yPos = rand() % (y + currentSize) - currentSize / 2;

        for (int size = currentSize; size >= minSize; size -= 50) {
            hrgn = CreateEllipticRgn(xPos - size / 2, yPos - size / 2, xPos + size / 2, yPos + size / 2);

            SelectClipRgn(hdc, hrgn);
            BitBlt(hdc, xPos - size / 2, yPos - size / 2, size, size, hdc, xPos - size / 2, yPos - size / 2, NOTSRCCOPY);

            delay(speed);
        }

        for (int size = minSize; size <= currentSize; size += 50) {
            hrgn = CreateEllipticRgn(xPos - size / 2, yPos - size / 2, xPos + size / 2, yPos + size / 2);

            SelectClipRgn(hdc, hrgn);
            BitBlt(hdc, xPos - size / 2, yPos - size / 2, size, size, hdc, xPos - size / 2, yPos - size / 2, NOTSRCCOPY);

            delay(speed);
        }
    }

    return 0;
}

DWORD WINAPI invert(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    HDC hdc = GetDC(0);

    while (true) {
        BitBlt(hdc, 0, 0, x, y, hdc, 0, 0, NOTSRCCOPY);

        Sleep(1000);
    }
    return 0;
}

DWORD WINAPI waves(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int ry = 0;

    HDC hdc = GetDC(0);

    while (true) {
        ry = rand() % y;

        if (rand() % 2 == 1) {
            BitBlt(hdc, 0, ry, x, 15, hdc, 10, ry, SRCCOPY);
        }
        else {
            BitBlt(hdc, 10, ry, x, 15, hdc, 0, ry, SRCCOPY);
        }

        Sleep(2);
    }
    return 0;
}

DWORD WINAPI highWaves(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int r = 0;

    HBRUSH hbrush = 0;
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    while (true) {
        hdc = GetDC(0);
        r = rand() % 50;

        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);

        for (int i = 0; i < x + y; i += 10) {
            int wave = sin(i / ((float)x / r) * 3.14159) * (r * 2);

            BitBlt(hdc, 0, i, x, 10, mdc, wave, i, SRCCOPY);
        }

        Sleep(50);
    }
}

struct Point3D {
    float x, y, z;
};

void DrawEllipseAt(HDC hdc, int x, int y, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    SelectObject(hdc, brush);
    Ellipse(hdc, x - 30, y - 30, x + 30, y + 30);  // Draw an ellipse with 50x50 size
    DeleteObject(brush);
}

Point3D RotatePoint(Point3D point, float angleX, float angleY, float angleZ) {
    float cosX = cos(angleX), sinX = sin(angleX);
    float cosY = cos(angleY), sinY = sin(angleY);
    float cosZ = cos(angleZ), sinZ = sin(angleZ);

    float y = point.y * cosX - point.z * sinX;
    float z = point.y * sinX + point.z * cosX;
    point.y = y;
    point.z = z;

    float x = point.x * cosY + point.z * sinY;
    z = -point.x * sinY + point.z * cosY;
    point.x = x;
    point.z = z;

    x = point.x * cosZ - point.y * sinZ;
    y = point.x * sinZ + point.y * cosZ;
    point.x = x;
    point.y = y;

    return point;
}

void Draw3DCube(HDC hdc, Point3D center, float size, float angleX, float angleY, float angleZ, float colorA) {
    Point3D vertices[8] = {
        {-size, -size, -size},
        {size, -size, -size},
        {size, size, -size},
        {-size, size, -size},
        {-size, -size, size},
        {size, -size, size},
        {size, size, size},
        {-size, size, size},
    };

    POINT screenPoints[8];

    for (int i = 0; i < 8; ++i) {
        Point3D rotated = RotatePoint(vertices[i], angleX, angleY, angleZ);
        COLORREF color = COLORHSL(colorA);

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
    int signX = 1;
    int signY = 1;
    int incrementor = 5;
    float x2 = 100.0f;
    float y2 = 100.0f;
    float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
    float angleIncrement = 0.05f;
    float colorA = 0;
    float size = 0.0f;

    while (true) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(SM_CXSCREEN);
        int y = GetSystemMetrics(SM_CYSCREEN);

        x2 += incrementor * signX;
        y2 += incrementor * signY;

        if (x2 + 75 >= x) {
            signX = -1;
            x2 = x - 76;
        }
        else if (x2 <= 75) {
            signX = 1;
            x2 = 76;
        }

        if (y2 + 75 >= y) {
            signY = -1;
            y2 = y - 76;
        }
        else if (y2 <= 75) {
            signY = 1;
            y2 = 76;
        }

        Point3D center = { x2, y2, 0.0f };
        Draw3DCube(hdc, center, size, angleX, angleY, angleZ, colorA);

        angleX += angleIncrement;
        angleY += angleIncrement;
        angleZ += angleIncrement;

        delay(50);

        ReleaseDC(0, hdc);
        colorA += 1;

        if (size >= 0 && size <= 100) {
            size += 0.5;
        }
    }

    return 0;
}

DWORD WINAPI hueCircle(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    int centerX = x / 2;
    int centerY = y / 2;
    double maxDistance = sqrt(centerX * centerX + centerY * centerY);

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        for (int i = 0; i < x * y; i++) {
            int x2 = i % x;
            int y2 = i / x;

            double distance = sqrt((x2 - centerX) * (x2 - centerX) + (y2 - centerY) * (y2 - centerY));
            double factor = pow(distance / maxDistance, 2);

            double hue, saturation, lightness;
            RGBtoHSV(data[i].rgb, hue, saturation, lightness);

            hue = fmod(hue + (factor * 360), 360);

            data[i].rgb = HSVtoRGB(hue, saturation, lightness);
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(10);
    }
    return 0;
}

DWORD WINAPI cubes(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int v = 0;
    int r = 0;

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    int centerX = x / 2;
    int centerY = y / 2;
    double maxDistance = sqrt(centerX * centerX + centerY * centerY);
    double hue, saturation, lightness;

    for (int i = 0; i < 10; i++) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        for (int i = 0; i < x * y; ++i) {
            int x2 = i % x;
            int y2 = i / x;

            double distance = sqrt((x2 - centerX) * (x2 - centerX) + (y2 - centerY) * (y2 - centerY));
            double factor = pow(distance / maxDistance, 2);

            RGBtoHSV(data[i].rgb, hue, saturation, lightness);

            hue = fmod(hue + (factor * 360), 360);

            data[i].rgb = HSVtoRGB(hue, saturation, lightness);
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(10);
    }

    while (true) {
        r = rand() % 2;
        if (r == 0) {
            for (int i = 0; i < 20; i++) {
                StretchBlt(mdc, 10, 10, x - 20, y - 20, hdc, 0, 0, x, y, SRCCOPY);
                GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

                for (int j = 0; j < x * y; ++j) {
                    if (!(sin(1000 * fade(v))))
                        v = rand() % 100;
                    data[j].rgb += v * 4;
                }

                SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
                StretchBlt(hdc, -10, -10, x + 20, y + 20, mdc, 0, 0, x, y, SRCCOPY);
                Sleep(10);
            }
        }

        if (r == 1) {
            RedrawScreen(0);
            Sleep(10);

            BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
            GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

            for (int i = 0; i < x * y; ++i) {
                int x2 = i % x;
                int y2 = i / x;

                double distance = sqrt((x2 - centerX) * (x2 - centerX) + (y2 - centerY) * (y2 - centerY));
                double factor = pow(distance / maxDistance, 2);

                RGBtoHSV(data[i].rgb, hue, saturation, lightness);

                hue = fmod(hue + (factor * 360), 360);

                data[i].rgb = HSVtoRGB(hue, saturation, lightness);
            }

            SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
            BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

            Sleep(10);
        }

        if (rand() % 3 == 0) {
            RedrawScreen(0);
            Sleep(20);
        }
    }
    return 0;
}

DWORD WINAPI colorShift(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int j = 0;

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    while (true) {
        StretchBlt(mdc, 0, 0, x, y, hdc, 0, 0, x, y, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        j = rand() % 360;

        for (int i = 0; i < x * y; ++i) {
            double hue, saturation, lightness;

            RGBtoHSV(data[i].hsv, hue, saturation, lightness);

            int red = (GetRValue(data[i].rgb) + 10) % 256;
            int green = (GetGValue(data[i].rgb) + 10) % 256;
            int blue = (GetBValue(data[i].rgb) + 10) % 256;

            hue = fmod(hue + j, 360);

            data[i].hsv = HSVtoRGB(hue, saturation, lightness);

            //data[i].rgb = RGB(red, green, blue);
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        StretchBlt(hdc, 0, 0, x, y, mdc, 0, 0, x, y, SRCCOPY);

        Sleep(10);
    }
    return 0;
}

DWORD WINAPI colorShift2(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    while (true) {
        StretchBlt(mdc, 0, 0, x, y, hdc, 0, 0, x, y, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        for (int i = 0; i < x * y; ++i) {
            ((BYTE*)(data + i))[1] += ((BYTE*)(data + i))[i % 1];
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        StretchBlt(hdc, 0, 0, x, y, mdc, 0, 0, x, y, SRCCOPY);

        Sleep(100);
    }
    return 0;
}

DWORD WINAPI brightWaves(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int rx = 0;
    int r = 0;
    double increment = 0.0;

    HBRUSH hbrush = 0;
    HDC hdc = GetDC(0);

    while (true) {
        rx = rand() % x;

        hbrush = CreateSolidBrush(COLORHSL(increment));

        SelectObject(hdc, hbrush);

        if (rand() % 2 == 0) {
            BitBlt(hdc, rx, 10, 100, y, hdc, rx, 0, 0xCE646Cu);
        }

        if (rand() % 2 == 1) {
            BitBlt(hdc, rx, -10, -100, y, hdc, rx, 0, 0xCE646Cu);
        }

        Sleep(1);

        increment += 1;

        if (rand() % 30 == 0) {
            RedrawScreen(0);
        }
    }
}

DWORD WINAPI wavesBall(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int rx = 0;
    int ry = 0;
    int xp = 0;
    int yp = 0;

    HDC hdc = GetDC(0);

    while (true) {
        rx = rand() % (x - 200);
        ry = rand() % (y - 200);

        for (int i = 300; i > 0; i--) {
            xp = rx + rand() % 11 - 5;
            yp = ry + rand() % 11 - 5;

            BitBlt(hdc, rx, ry, rand() % (1 + i), rand() % (1 + i), hdc, xp, yp, SRCCOPY);

            delay(1);
        }
    }

    ReleaseDC(0, hdc);
    return 0;
}

DWORD WINAPI Shake(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    double angle = 0.0;
    int x2;
    int y2;

    HDC hdc = GetDC(0);

    while (true) {
        x2 = (int)(sin(angle) * 10);
        y2 = (int)(cos(angle) * 10);

        BitBlt(hdc, x2, y2, x, y, hdc, 0, 0, SRCCOPY);

        Sleep(140);

        if (rand() % 5 == 0) {
            RedrawScreen(0);
        }

        angle++;
    }
}

DWORD WINAPI colorCubes(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int pixelSize = 30;
    int j = 0;

    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (x * y + x) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    BLENDFUNCTION blur = { 0 };
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 50;

    while (true) {
        hdc = GetDC(0);

        StretchBlt(mdc, 0, 0, x, y, hdc, 0, 0, x, y, SRCPAINT);
        GetBitmapBits(bmp, x * y * sizeof(RGBQUAD), data);

        for (int yBlock = 0; yBlock < y; yBlock += pixelSize) {
            for (int xBlock = 0; xBlock < x; xBlock += pixelSize) {
                int red = 0, green = 0, blue = 0, count = 0;
                for (int py = 0; py < pixelSize && (yBlock + py) < y; ++py) {
                    for (int px = 0; px < pixelSize && (xBlock + px) < x; ++px) {
                        int index = ((yBlock + py) * x + (xBlock + px));
                        blue += data[index].rgbBlue;
                        green += data[index].rgbGreen;
                        red += data[index].rgbRed;
                        ++count;
                    }
                }
                if (count > 0) {
                    blue /= count;
                    green /= count;
                    red /= count;
                }
                for (int py = 0; py < pixelSize && (yBlock + py) < y; ++py) {
                    for (int px = 0; px < pixelSize && (xBlock + px) < x; ++px) {
                        int index = ((yBlock + py) * x + (xBlock + px));
                        data[index].rgbBlue = blue;
                        data[index].rgbGreen = green;
                        data[index].rgbRed = red;
                    }
                }
            }
        }

        for (int i = 0; i < x * y; i++) {
            data[i].rgbBlue += (GetBValue(data[i].rgbBlue + rand() % 360) % RGB(255, 255, 255));
            data[i].rgbGreen += (GetGValue(data[i].rgbGreen + rand() % 360) % RGB(255, 255, 255));
            data[i].rgbRed += (GetRValue(data[i].rgbRed + rand() % 360) % RGB(255, 255, 255));
        }

        SetBitmapBits(bmp, x * y * sizeof(RGBQUAD), data);
        StretchBlt(hdc, 0, 0, x, y, mdc, 0, 0, x, y, SRCCOPY);
        Sleep(50);

        if (rand() % 3 == 0) {
            RedrawScreen(0);
            Sleep(20);
        }

        j = rand() % 255;
    }
    return 0;
}

DWORD WINAPI hueCircle2(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x2 = 0;
    int y2 = 0;
    int k = 0;

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    double hue = 0, saturation = 0, lightness = 0;

    while (true) {
        hdc = GetDC(0);

        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        for (int i = 0; i < x * y; i++) {
            int x2 = i % x;
            int y2 = i / x;

            double distance = sqrt(pow(x2 - k, 2) + pow(y2 - k, 2));

            RGBtoHSV(data[i].hsv, hue, saturation, lightness);

            hue = fmod((distance)+rand() % 60, 360);

            data[i].hsv = HSVtoRGB(hue, saturation, lightness);
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(10);

        k = rand() % x;

        if (rand() % 3 == 0) {
            //RedrawWindow(0, 0, 0, 0x85);
            //Sleep(10);
        }
    }
    return 0;
}

DWORD WINAPI dizzy1(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    while (true) {
        hdc = GetDC(0);

        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        for (int i = 0; i < x * y; i++) {
            int x2 = i % x;
            int y2 = i / x;

            data[i].rgb += (x2 + y2) ^ (x2 & y2) + (x2 - y2) & (x2 ^ y2) | y2;
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(30);
    }
    return 0;
}

DWORD WINAPI Squares(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int rx = 0;
    int ry = 0;

    HBRUSH hbrush = 0;
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    BLENDFUNCTION blur = { 0 };
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 127;

    while (true) {
        for (int i = 0; i < 4; i++) {
            rx = rand() % (x - 200);
            ry = rand() % (y - 200);

            BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);

            hbrush = CreateSolidBrush(RGB(255, 234, 9));

            SelectObject(mdc, hbrush);
            PatBlt(mdc, rx, ry, 200, 200, PATCOPY);

            GdiAlphaBlend(hdc, 0, 0, x, y, mdc, 0, 0, x, y, blur);

            Sleep(100);
        }

        hbrush = CreateSolidBrush(40);

        SelectObject(hdc, hbrush);

        BitBlt(hdc, 0, 0, x, y, hdc, 0, 0, PATINVERT);

        Sleep(10);
    }

    ReleaseDC(0, hdc);
    return 0;
}

DWORD WINAPI dizzy2(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int increment = 10;

    HDC hdc = GetDC(0);
    POINT point[3];

    while (true) {
        for (int i = 0; i < rand() % 10; i++) {
            point[0].x = increment;
            point[0].y = -increment;
            point[1].x = x + increment;
            point[1].y = increment;
            point[2].x = -increment;
            point[2].y = y - increment;

            PlgBlt(hdc, point, hdc, 0, 0, x, y, 0, 0, 0);

            Sleep(50);
        }
        for (int i = 0; i < rand() % 10; i++) {
            point[0].x = -increment;
            point[0].y = increment;
            point[1].x = x - increment;
            point[1].y = -increment;
            point[2].x = increment;
            point[2].y = y + increment;

            PlgBlt(hdc, point, hdc, 0, 0, x, y, 0, 0, 0);

            Sleep(50);
        }
    }
}

DWORD WINAPI paintRectangles(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x2 = 0;
    int y2 = 0;
    int size = 100;

    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    BLENDFUNCTION blur = { 0 };
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 127;

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);

        BitBlt(mdc, x2, y2, size, size, hdc, x2, y2, WHITENESS);

        GdiAlphaBlend(hdc, 0, 0, x, y, mdc, 0, 0, x, y, blur);

        x2 += size;

        if (x2 >= x) {
            y2 += size;
            x2 = 0;
        }
        if (y2 >= y) {
            x2 = 0;
            y2 = 0;
        }

        Sleep(50);
    }
}

DWORD WINAPI patInvert(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    HBRUSH hbrush = 0;
    HDC hdc = GetDC(0);

    while (true) {
        hbrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));

        SelectObject(hdc, hbrush);

        BitBlt(hdc, 0, 0, x, y, hdc, 0, 0, PATINVERT);

        Sleep(100);

        if (rand() % 30 == 0) {
            RedrawScreen(0);
        }
    }
}

DWORD WINAPI pixelShift(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x2 = 0;
    int y2 = 0;
    int j = 240;

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        for (int i = 0; i < x * y; i++) {
            x2 = i % x, y2 = i / y;

            data[i].rgb = (data[i].rgb + rand() % 1000) % (RGB(255, 255, 255));
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(10);
    }
}

DWORD WINAPI invertRectangles(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x2 = 0;
    int y2 = 0;
    int size = 300;

    HDC hdc = GetDC(0);

    while (true) {
        BitBlt(hdc, x2, y2, size, size, hdc, x2, y2, NOTSRCCOPY);

        x2 += size;

        if (x2 >= x) {
            y2 += size;
            x2 = 0;
        }
        if (y2 >= y) {
            x2 = 0;
            y2 = 0;
        }

        Sleep(50);
    }
}

DWORD WINAPI colorRectangles(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x2 = 0;
    int y2 = 0;
    int size = 300;
    int color = 0;

    HBRUSH hbrush = 0;
    HDC hdc = GetDC(0);

    while (true) {
        hbrush = CreateSolidBrush(COLORHSL(color));

        SelectObject(hdc, hbrush);
        BitBlt(hdc, x2, y2, size, size, hdc, x2, y2, PATINVERT);

        x2 += size;

        if (x2 >= x) {
            y2 += size;
            x2 = 0;
        }
        if (y2 >= y) {
            x2 = 0;
            y2 = 0;
        }

        Sleep(50);

        color++;
    }
}

DWORD WINAPI cubes2(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    while (true) {
        StretchBlt(mdc, 10, 10, x - 20, y - 10, hdc, 0, 0, x, y, SRCCOPY);

        StretchBlt(hdc, -10, -10, x + 20, y + 10, mdc, 0, 0, x, y, SRCCOPY);

        Sleep(10);
    }
}

DWORD WINAPI yellow(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x2 = 0;
    int y2 = 0;
    int j = 240;

    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (x * y + x) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(RGBQUAD), data);

        for (int i = 0; i < x * y; i++) {
            x2 = i % x, y2 = i / y;

            data[i].rgbBlue = (data[i].rgbBlue + rand() % 1000) % (RGB(255, 255, 255));
        }

        SetBitmapBits(bmp, x * y * sizeof(RGBQUAD), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(10);

        if (rand() % 3 == 0) {
            RedrawScreen(0);
        }
    }
}

DWORD WINAPI lastShader(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x2 = 0;
    int y2 = 0;
    int j = 240;

    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, x * y * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(RGBQUAD), data);

        for (int i = 0; i < x * y; i++) {
            x2 = i % x, y2 = i / y;

            data[i].rgbRed = (data[i].rgbRed = (i % x)) % (RGB(255, 255, 255));
        }

        SetBitmapBits(bmp, x * y * sizeof(RGBQUAD), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(2000);
    }
}

DWORD WINAPI dizzy3(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int increment = 200;

    HDC hdc = GetDC(0);
    POINT point[3];

    while (true) {
        /*
        rect.left = 0;
        rect.top = 0;
        rect.right = x;
        rect.bottom = y;
        */

        for (int i = 0; i < rand() % 10; i++) {
            point[0].x = increment;
            point[0].y = -increment;
            point[1].x = x + increment;
            point[1].y = increment;
            point[2].x = -increment;
            point[2].y = y - increment;

            PlgBlt(hdc, point, hdc, 0, 0, x, y, 0, 0, 0);

            Sleep(100);
        }
        for (int i = 0; i < rand() % 10; i++) {
            point[0].x = -increment;
            point[0].y = increment;
            point[1].x = x - increment;
            point[1].y = -increment;
            point[2].x = increment;
            point[2].y = y + increment;

            PlgBlt(hdc, point, hdc, 0, 0, x, y, 0, 0, 0);

            Sleep(1000);
        }
    }
}

DWORD WINAPI lastWaves(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int ry = 0;

    HDC hdc = GetDC(0);

    while (true) {
        ry = rand() % y;

        BitBlt(hdc, 0, ry, x, 30, hdc, 50, ry, SRCCOPY);

        Sleep(100);
    }
    return 0;
}

DWORD WINAPI lastSineWaves(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int ry = 0;
    double angle = 0.0;

    HDC hdc = GetDC(0);

    while (true) {
        for (float i = 0; i < x + y; i += 20) {
            int a = sin(angle) * 20;
            BitBlt(hdc, 0, i, x, 20, hdc, a, i, SRCCOPY);
            angle += 3.14159 / 40;

            Sleep(100);
        }
    }
    return 0;
}

DWORD WINAPI lastBrightWaves(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int rx = 0;
    int r = 0;

    HDC hdc = GetDC(0);

    while (true) {
        rx = rand() % x;

        if (rand() % 2 == 0) {
            BitBlt(hdc, rx, 10, 100, y, hdc, rx, 0, SRCPAINT);
        }

        if (rand() % 2 == 1) {
            BitBlt(hdc, rx, -10, -100, y, hdc, rx, 0, SRCPAINT);
        }

        Sleep(150);
    }
}

DWORD WINAPI goodCubes(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x2 = 0;
    int y2 = 0;

    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    while (true) {
        BitBlt(mdc, x2 + rand() % 3, y2 + rand() % 3, 100, 100, hdc, x2 + rand() % 3, y2 + rand() % 3, SRCCOPY);

        x2 = x2 + 100;

        if (x2 >= x) {
            y2 = y2 + 100;
            x2 = 0;
        }
        if (y2 >= y) {
            x2 = y2 = 0;

            BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);
        }
    }
}

DWORD WINAPI circle(LPVOID lpParam) {
    int signX = 1;
    int signY = 1;
    int incrementor = 100;
    int x2 = 10;
    int y2 = 10;
    int color = 0;

    while (true) {
        HDC hdc = GetDC(0);
        int times = 0;
        int x = GetSystemMetrics(SM_CXSCREEN);
        int y = GetSystemMetrics(SM_CYSCREEN);

        x2 += incrementor * signX;
        y2 += incrementor * signY;

        for (times = 0; times < 200; times += 1) {
            double v10 = static_cast<double>(times + 300);
            float v12 = sin(v10) * static_cast<double>(times) + 100.0;
            float v13 = cos(v10) * static_cast<double>(times) + 100.0;
            int v1 = x2 + static_cast<int>(v13);
            int v2 = y2 + static_cast<int>(v12);

            HBRUSH brush = CreateSolidBrush(COLORHSL(color));
            SelectObject(hdc, brush);
            Ellipse(hdc, v1, v2, v1 + 50, v2 + 50);

            Sleep(10);
            color++;
        }

        if (y2 + 50 >= y) {
            signY = -1;
            y2 = y - 51;
        }
        if (x2 + 50 >= x) {
            signX = -1;
            x2 = x - 51;
        }
        if (y2 <= 0) {
            signY = 1;
            y2 = 1;
        }
        if (x2 <= 0) {
            signX = 1;
            x2 = 1;
        }
    }

    return 0;
}

DWORD WINAPI coloredPixels(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int pixelSize = 5;
    int j = 0;

    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (x * y + x) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HBRUSH hbrush = 0;
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    while (true) {
        StretchBlt(mdc, 0, 0, x, y, hdc, 0, 0, x, y, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(RGBQUAD), data);

        for (int yBlock = 0; yBlock < y; yBlock += pixelSize) {
            for (int xBlock = 0; xBlock < x; xBlock += pixelSize) {
                int red = 0, green = 0, blue = 0, count = 0;
                for (int py = 0; py < pixelSize && (yBlock + py) < y; ++py) {
                    for (int px = 0; px < pixelSize && (xBlock + px) < x; ++px) {
                        int index = ((yBlock + py) * x + (xBlock + px));
                        blue += data[index].rgbBlue;
                        green += data[index].rgbGreen;
                        red += data[index].rgbRed;
                        ++count;
                    }
                }
                if (count > 0) {
                    blue /= count;
                    green /= count;
                    red /= count;
                }
                for (int py = 0; py < pixelSize && (yBlock + py) < y; ++py) {
                    for (int px = 0; px < pixelSize && (xBlock + px) < x; ++px) {
                        int index = ((yBlock + py) * x + (xBlock + px));
                        data[index].rgbBlue = blue;
                        data[index].rgbGreen = green;
                        data[index].rgbRed = red;
                    }
                }
            }
        }

        for (int i = 0; i < x * y; i++) {
            data[i].rgbBlue += j;
            data[i].rgbGreen += j;
            data[i].rgbRed += j;
        }

        SetBitmapBits(bmp, x * y * sizeof(RGBQUAD), data);
        StretchBlt(hdc, 0, 0, x, y, mdc, 0, 0, x, y, SRCCOPY);
        Sleep(500);

        if (rand() % 3 == 0) {
            RedrawScreen(0);
            Sleep(20);
        }

        j += rand() % 360;
    }
    return 0;
}

DWORD WINAPI cubesCurcle(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    int centerX = x / 2;
    int centerY = y / 2;
    double maxDistance = sqrt(centerX * centerX + centerY * centerY);
    double hue, saturation, lightness;

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        for (int i = 0; i < x * y; ++i) {
            int x2 = i % x;
            int y2 = i / x;

            double distance = sqrt((x2 - centerX) * (x2 - centerX) + (y2 - centerY) * (y2 - centerY));
            double factor = pow(distance / maxDistance, 2);

            RGBtoHSV(data[i].hsv, hue, saturation, lightness);

            hue = fmod(hue + (factor * 360), 360);

            data[i].hsv = HSVtoRGB(hue, saturation, lightness);
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        if (rand() % 3 == 0) {
            RedrawScreen(0);
            Sleep(20);
        }

        Sleep(10);
    }
    return 0;
}

DWORD WINAPI white(LPVOID lpParam) {
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x = GetSystemMetrics(SM_CXSCREEN);
    int centerX = x / 2;
    int centerY = y / 2;
    int radius = min(centerX, centerY);

    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    while (true) {
        for (int yp = 0; yp < y; ++yp) {
            for (int xp = 0; xp < x; ++xp) {
                int dx = xp - centerX;
                int dy = yp - centerY;
                double distance = sqrt(dx * dx + dy * dy);

                int intensity = 255 - (int)((distance / radius) * 255);
                if (intensity < 0) intensity = 0;

                SetPixel(mdc, xp, yp, RGB(intensity, intensity, intensity));
            }
        }

        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);

        Sleep(100);
    }
}

DWORD WINAPI colors(LPVOID lpParam) {
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x = GetSystemMetrics(SM_CXSCREEN);

    HBRUSH hbrush = 0;
    HDC hdc = GetDC(0);

    while (true) {
        hbrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));

        SelectObject(hdc, hbrush);

        BitBlt(hdc, rand() % 15 - 5, rand() % 15 - 5, x, y, hdc, 0, 0, 0x1900ac010e);

        Sleep(500);
    }
}