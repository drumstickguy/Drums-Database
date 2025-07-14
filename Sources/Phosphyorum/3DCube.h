//This cube is created by RaduMinecraft, but i modifed it.

#include "GDIConfig.h"
#define PI 3.141592653589793f
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

void Draw3DCube(HDC hdc, Point3D center, float size, float angleX, float angleY, float angleZ, float colorA, float color) {
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

DWORD WINAPI Payload_3DCube(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int wdpi = GetDeviceCaps(hdc, LOGPIXELSX);
    int hdpi = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(0, hdc);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
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
    for (;;) {
        hdc = GetDC(0);

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