#include "ColorFx.h"
#define TINT_SRCPAINT 0xce646cul
#define TINT_SRCAND 0x8e647dul

CONST FLOAT PI = acos(-1.0); // Screw off 3.14159265358979! (high joke)
CONST INT w = GetSystemMetrics(0), h = GetSystemMetrics(1); // Width and height variables

FLOAT WINAPI Rotate2DPoint(LPPOINT lpPoint, POINT pCenter, FLOAT angle, INT iCount) {
	for (INT i = 0; i < iCount; i++) {
		FLOAT x = lpPoint[i].x - pCenter.x;
		FLOAT y = lpPoint[i].y - pCenter.y;
		lpPoint[i].x = (INT)(cosf(angle) * x - sinf(angle) * y + pCenter.x);
		lpPoint[i].y = (INT)(sinf(angle) * x + cosf(angle) * y + pCenter.y);
	}
}

typedef union _COLORPAL32 {
	COLORREF cp32Colors;
	struct {
		BYTE cp32Blue;
		BYTE cp32Green;
		BYTE cp32Red;
		BYTE bReserved;
	};
} COLORPAL32, *LPCOLORPAL32;

DWORD GDI_MS; // Data variable reserved for preventing memory leaks
//DWORD Ticks = GetTickCount(); Why?

BITMAPINFO bmicrt(INT x, INT y) {
	BITMAPINFO bmi = {sizeof(bmi), x, y, 1, 32, BI_RGB};
	return bmi;
} // Create a bitmap info in just a few characters

typedef LPVOID *DIBSECTIONBITS; // Because yeah


/* WARNING: RECYCLED STUFF FROM DELTA.EXE */
typedef struct {
	float x;
	float y;
	float z;
} POINT3D, *PPOINT3D;




POINT3D RotatePoints(POINT3D point, float fx, float fy, float fz) {
	// (RECYCLED FROM NITRITE.EXE) //
	float cx = cos(fx), sx = sin(fx);
	float cy = cos(fy), sy = sin(fy);
	float cz = cos(fz), sz = sin(fz);
	
	float y = point.y * cx - point.z * sx;
	float z = point.y * sx + point.z * cx;
	point.y = y;
	point.z =z;
	float x = point.x * cy + point.z * sy;
	z = -point.x * sy + point.z * cy;
	point.x = x;
	point.z = z;
	x = point.x * cz - point.y * sz;
	y = point.x * sz + point.y * cz;
	point.x = x;
	point.y = y;
	
	return point;
}

COLORREF Hue2(INT clr, FLOAT cx, FLOAT cy, FLOAT cz) {
	clr = fmax(1, fmin(255, clr));
	if (clr <= 64) {
		INT gry = clr * 255 / 63.f;
		return RGB(gry, gry, gry);
	}
	FLOAT fc = (clr - 64) / 191.f;
	INT r = (INT)(255 * fc * (cx + 1.f) / 2.f);
	INT g = (INT)(255 * fc * (cy + 1.f) / 2.f);
	INT b = (INT)(255 * fc * (cz + 1.f) / 2.f);
	INT gry = 255 * (1.f - fc);
	return RGB(gry + r, gry + g, gry + b);
}



DWORD64 __stdcall Sphere2(HDC hdc, POINT3D pc, INT rad, FLOAT ax, FLOAT ay, FLOAT az) {
	for (INT i = 0; i <= 180; i += 10) {
		for (INT j = 0; j <= 360; j += 10) {
			FLOAT r = i * PI / 180.f;
			FLOAT rt = j * PI / 180.f;
			
			POINT3D pt = {rad * sin(r) * sin(rt), rad * sin(r) * cos(rt), rad * cos(rt)};
			pt = RotatePoints(pt, ax, ay, az);
			INT x = (INT)(pc.x + pt.x);
			INT y = (INT)(pc.y + pt.y);
			DrawIcon(hdc, x, y, LoadIcon(NULL, IDI_ERROR));
		}
	}
}

DWORD64 __stdcall pSphere2(HDC hdc, POINT3D pc, INT rad, FLOAT ax, FLOAT ay, FLOAT az) {
	for (INT i = 0; i <= 180; i += 10) {
		for (INT j = 0; j <= 360; j += 10) {
			FLOAT r = i * PI / 180.f;
			FLOAT rt = j * PI / 180.f;
			
			POINT3D pt = {rad * sin(r) * sin(rt), rad * sin(r) * cos(rt), rad * cos(rt)};
			pt = RotatePoints(pt, ax, ay, az);
			INT x = (INT)(pc.x + pt.x);
			INT y = (INT)(pc.y + pt.y);
			DrawIcon(hdc, x, y, LoadIcon(NULL, IDI_APPLICATION));
		}
	}
}

// RECYCLED FROM TUBERCULOSIS1.2 //
INT WINAPI SplitBlt(HDC hdc, int strength, int xSrc, int ySrc, DWORD dwSrcMode) {
	BitBlt(hdc, -strength, -strength, xSrc/2, ySrc/2, hdc, 0, 0, dwSrcMode);
	BitBlt(hdc, xSrc/2+strength, -strength, xSrc/2, ySrc/2, hdc, xSrc/2, 0, dwSrcMode);
	BitBlt(hdc, -strength, ySrc/2+strength, xSrc/2, ySrc/2, hdc, 0, ySrc/2, dwSrcMode);
	BitBlt(hdc, xSrc/2+strength, ySrc/2+strength, xSrc/2, ySrc/2, hdc, xSrc/2, ySrc/2, dwSrcMode);
}

/* End of recycled stuff. */

namespace GDI {
	DWORD WINAPI pFirstJPEGEffect(LPVOID lpProc)
	{
		// JPEG COMPRESSION //
		GDI_MS = 20 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 2;
		while (GetTickCount() < EndTime) {
			HBRUSH hbsh = CreateSolidBrush(RGB(256 - i, 256 - i, 256 - i));
			SelectObject(hdc, hbsh);
			StretchBlt(hdc, 0, 0, w, h, hdc, 0, 0, w, h, MERGECOPY);
			i++;
			if (i > 45) i = 1;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pBrightScans(LPVOID lpProc)
	{
		// BRIGHT PIXELATION //
		GDI_MS = 17 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			INT wx = w / 6, hx = h / 6;
			SelectObject(mdc, (HGDIOBJ)CreateCompatibleBitmap(hdc, w, h));
			SetStretchBltMode(mdc, STRETCH_DELETESCANS); // I think only that's needed
			StretchBlt(mdc, 0, 0, wx, hx, hdc, 0, 0, w, h, SRCCOPY);
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, wx, hx, SRCPAINT);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pDarkAlphaScans(LPVOID lpProc)
	{
		// DARK ANIMATED PIXELATION //
		GDI_MS = 14 * 1000;
		HDC hdc = GetDC(0);
		DWORD EndTime = GetTickCount() + GDI_MS;
		while (EndTime > GetTickCount()) { // r e v e r s e
			HDC mdc = CreateCompatibleDC(hdc);
			BLENDFUNCTION ftn = {AC_SRC_OVER, 0, 15, 0}; // Screw off blend!
			SelectObject(mdc, (HGDIOBJ)CreateCompatibleBitmap(hdc, w, h));
			INT wx = w / 6, hx = h / 6;
			StretchBlt(mdc, 0, 0, wx, hx, hdc, 0, 0, w, h, SRCCOPY);
			GdiAlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, wx, hx, ftn); // Most people just don't put "Gdi" in there
			if (rand() % 30 == 29) RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN); // Random redrawing without the use of a function
			Sleep(5);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pTint(LPVOID lpProc)
	{
		GDI_MS = 16 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; // HUE. SATURATION. VIBRANCE.
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); // Extra variable, cuz why not?
			StretchBlt(mdc, rand() % 8, rand() % 8, w, h, hdc, rand() % 8, rand() % 8, w, h, SRCCOPY); // Chose that number as 2 cubed
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = i;
					hsv.h = fmod((FX / 500.f) + y / h * .02f, 1); // Screw off pankoza's system, have mine instead
					hsv.s = 1.f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i += 90; // 90 isn't a number I chose randomly
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pORRotate(LPVOID lpProc)
	{
		// COMPLEX ROTATION (SRCPAINT) //
		// Uses PlgBlt instead of SetWorldTransform :/
		GDI_MS = 16 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RECT rect; // Not a pointer, so I'll just use "rect"
			GetWindowRect(GetDesktopWindow(), &rect); // What is the HWND for anyway?
			POINT point[3]; // T R I A N G L E
			INT Severity = 5, Severity2 = 10;
			if (rand() % 2 == 0) {
				i = Severity;	
				point[0].x = rect.left + i; point[0].y = rect.top - i;
				point[1].x = rect.right + i; point[1].y = rect.top + i;
				point[2].x = rect.left - i; point[2].y = rect.bottom - i;
			}
			else if (rand() % 2 == 1) {
				i = Severity2;
				point[0].x = rect.left - i; point[0].y = rect.top + i;
				point[1].x = rect.right - i; point[1].y = rect.top - i;
				point[2].x = rect.left + i; point[2].y = rect.bottom + i;
			}
			SelectObject(mdc, (HGDIOBJ)CreateCompatibleBitmap(hdc, w, h));
			PlgBlt(mdc, point, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, 0, 0);
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCPAINT);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pIconPattern(LPVOID lpProc)
	{
		GDI_MS = 16 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			INT angleX = (cos(i * 50) * sin(i * 3) * (i + 120)) + (w / 2), angleY = (sin(i * 72) * cos(i * 3) * (i + 120)) + (h / 2);
			DrawIcon(hdc, angleX, angleY, LoadIcon(NULL, IDI_ERROR));
			i++;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pMovingXOR(LPVOID lpProc)
	{
		GDI_MS = 16 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv;
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit);
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = (i + x) ^ y;
					hsv.h = fmod((FX / 500.f) + hsv.h + .02f, 1);
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i++;
			StretchBlt(hdc, 0, 0, w, h, mdc, w, 0, -w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI lTextOuts(LPVOID lpProc)
	{
		GDI_MS = 8 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			LPCSTR lpccStrings[] = {
				"Pain", "Emetophobia is killing me", "Bl00d everywhere", "Tuberculosis has infected me", // Do not mind the censor of "blood"
				"Ow", "Feeling nauseous everyday", "Disoriented", "Weak",
				"I guess my immune system won't combat this", "AAAAAAAAAAAAAAAAAAAAAA"
			};
			INT totstr = sizeof(lpccStrings) / sizeof(lpccStrings[0]);
			for (INT j = 0; j < totstr; j++) {
				SetBkColor(hdc, Hue(i) - RGB(180, 180, 180));
				SetTextColor(hdc, RGB(rand() % 0xff, rand() % 0xff, rand() % 0xff));
				SelectObject(hdc, CreateFontW(70, 0, rand() % 72 - 36, 0, FW_BOLD, FALSE, rand() % 2, FALSE, OEM_CHARSET, 0, 0, 0, 0, L"Times New Roman"));
				TextOutA(hdc, rand() % w, rand() % h, lpccStrings[j], lstrlenA(lpccStrings[j]));
				i++;
			}
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pRGBStripes(LPVOID lpProc)
	{
		GDI_MS = 4 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			INT wx = w / 8, hx = h / 8;
			BITMAPINFO lpbmi = bmicrt(wx, hx);
			SelectObject(mdc, (HGDIOBJ)CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0));
			SelectObject(hdc, CreateSolidBrush(RGB(rand() % 0xff, rand() % 0xff, rand() % 0xff)));
			StretchBlt(mdc, 0, 0, wx, hx, hdc, 0, 0, w, h, PATCOPY | SRCCOPY);
			for (INT x = 0; x < wx; x++) {
				for (INT y = 0; y < hx; y++) {
					INT INDEX = y * wx + x;
					INT FX = (x + i) ^ (y + i) * 64;
					rgb[INDEX].rgbRed += FX;
					rgb[INDEX].rgbGreen += FX;
					rgb[INDEX].rgbBlue += FX;
				}
			}
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, wx, hx, PATCOPY | SRCCOPY);
			i++;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pBrightBlendSquares(LPVOID lpProc)
	{
		GDI_MS = 16 * 1000;
		HDC hdc = GetDC(0);
		DWORD EndTime = GetTickCount() + GDI_MS;
		while (EndTime > GetTickCount()) {
			INT Size = 200;
			HDC mdc = CreateCompatibleDC(hdc);
			BLENDFUNCTION ftn = {AC_SRC_OVER, 0, 64, 0};
			SelectObject(mdc, (HGDIOBJ)CreateCompatibleBitmap(hdc, w, h));
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCPAINT);
			for (INT i = 0; i < 30; i++) {
				INT x = -Size + rand() % (w + Size), y = -Size + rand() % (h + Size);
				BitBlt(mdc, x, y, Size, Size, mdc, x + rand() % 16 - 8, y + rand() % 16 - 8, SRCPAINT);
			}
			GdiAlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, ftn);
			if (rand() % 30 == 29) RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
			Sleep(5);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pRGBMovGlitch(LPVOID lpProc)
	{
		GDI_MS = 10 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			INT wx = w / 8, hx = h / 8;
			BITMAPINFO lpbmi = bmicrt(wx, hx);
			SelectObject(mdc, CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0));
			StretchBlt(mdc, 0, 0, wx, hx, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < wx; x++) {
				for (INT y = 0; y < hx; y++) {
					INT INDEX = y * wx + x;
					INT FX = (i + x) & (i + y) ^ y;
					rgb[INDEX].rgbGreen = FX;
					rgb[INDEX].rgbRed ^= FX;
					rgb[INDEX].rgbBlue += FX;
				}
			}
			i++;
			StretchBlt(hdc, 0, 0, w + rand() % w, h, mdc, 0, 0, wx, hx, SRCINVERT);
			BitBlt(hdc, 0, 0, w, rand() % h, hdc, rand() % 256 - 128, 0, NOTSRCCOPY);
			PatBlt(hdc, 0, 0, w, rand() % h, PATINVERT);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pRGBRedFlash(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			INT wx = w / 8, hx = h / 8;
			BITMAPINFO lpbmi = bmicrt(wx, hx);
			SelectObject(mdc, CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0));
			SetStretchBltMode(mdc, STRETCH_DELETESCANS);
			StretchBlt(mdc, 0, 0, wx, hx, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < wx; x++) {
				for (INT y = 0; y < hx; y++) {
					INT INDEX = y * wx + x;
					rgb[INDEX].rgbRed += MAXCHAR;
				}
			}
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, wx, hx, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI lTouhouPayload(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		while (GetTickCount() < EndTime) {
			INT size = 500;
			HRGN rgn;
			INT x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;
			for (INT i = 4; i < size; i += 50) {
				 // Redefining. What's wrong with it?
				rgn = CreateEllipticRgn(x - i / 2, y - i / 2, i + (x - i / 2), i + (y - i / 2));
				SelectClipRgn(hdc, rgn);
				StretchBlt(hdc, x - i / 2, y - i / 2, i, i, hdc, x - i / 2, y - i / 2, w, h, PATINVERT);
				Sleep(5);
			}
			x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;
			RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
			for (INT i = 4; i < size; i += 50) {
				rgn = CreateRectRgn(x - i / 2, y - i / 2, i + (x - i / 2), i + (y - i / 2));
				SelectClipRgn(hdc, rgn);
				StretchBlt(hdc, x - i / 2, y - i / 2, i, i, hdc, x - i / 2, y - i / 2, w, h, PATINVERT);
				Sleep(5);
			}
			x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;
			RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
			for (INT i = 4; i < size; i += 50) {
				POINT pptl[3] = {
					x, y - i / 2,
					x - i / 2, y + i / 2,
					x + i / 2, y + i / 4
				};
				rgn = CreatePolygonRgn(pptl, 3, WINDING);
				SelectClipRgn(hdc, rgn);
				StretchBlt(hdc, x - i / 2, y - i / 2, i, i, hdc, x - i / 2, y - i / 2, w, h, PATINVERT);
				Sleep(5);
			}
			
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI lCustomFrequencyWaves(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			for (INT y = 0; y < h; y++) {
				INT Phase = rand() % 127 - 63;
				INT angle = 10 * sin(2 * PI * (y + i * 5) * Phase / h);
				BitBlt(hdc, 0, y, w - angle, 1, hdc, angle, y, SRCCOPY);
			}
			i++;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI lInvert(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		while (GetTickCount() < EndTime) {
			StretchBlt(hdc, 0, 0, w, h, hdc, 0, 0, w, h, NOTSRCCOPY); // NOTSRCCOPY inverts the output image
			Sleep(500);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI p3DIconAura(LPVOID lpProc)
	{
		// I called it an aura because idk what else to call it :|
		GDI_MS = 60 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT incrementX = 10, incrementY = 10;
		FLOAT spd = 0.02;
		FLOAT ax = 0, ay = 0, az = 0;
		INT x = w / 2, y = h / 2;
		INT rad = 200;
		while (GetTickCount() < EndTime) {
			x += incrementX, y += incrementY;
			POINT3D pc = {x, y, 0.f};
			Sphere2(hdc, pc, rad, ax, ay, az);
			if (x - rad < 0 || x + rad > w) incrementX = -incrementX;
			if (y - rad < 0 || y + rad > h) incrementY = -incrementY;
			ax += spd, ay += spd, az += spd;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pAnimPinkRotoRhombus(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT incrementX = 10, incrementY = 10;
		FLOAT theta = 0.f;
		INT x = w / 4, y = h / 4;
		INT rad = 75;
		while (GetTickCount() < EndTime) {
			INT cx = x + rad, cy = y + rad;
			x += incrementX, y += incrementY;
			POINT point[4];
			for (INT i = 0; i < 4; i++) {
				DOUBLE angle = 2.f * PI * i / 4;
				point[i].x = cx + (INT)(cos(angle) * rad);
				point[i].y = cy + (INT)(sin(angle) * rad);
			}
			POINT pCenter = {(x + cx) / 2, (y + cy) / 2};
			Rotate2DPoint(point, pCenter, theta, 4);
			INT rgb = rand() % 155 + 100;
			HBRUSH hbsh = CreateSolidBrush(RGB(rgb, 0, rgb)); // Make it magenta.
			SelectObject(hdc, hbsh);
			Polygon(hdc, point, 4);
			if (x - rad < 0 || x + rad > w) incrementX = -incrementX;
			if (y - rad < 0 || y + rad > h) incrementY = -incrementY;
			theta += 0.02f;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pHSVEllipses(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv;
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit);
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			INT rw = rand() % w, rh = rand() % h;
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT cx = x - (rw / 2), cy = y - (rh / 2);
					INT dx = cx * cx, dy = cy * cy;
					INT d = 128 + i;
					INT FX = d + (d * sin(sqrt(dx + dy) / d));
					hsv.h = fmod((FX / 500.f) + hsv.h + .02f, 1);
					hsv.s = 1.f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i++;
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pTintedScreenMelt(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			// Do not blame me for adding many variables
			INT rw = rand() % w, rh = rand() % h;
			INT factor = rand() % 30 - 15;
			INT meltWidth = 127;
			SelectObject(hdc, CreateSolidBrush(Hue(i)));
			BitBlt(hdc, rw, factor, meltWidth, h, hdc, rw, 0, (rand() % 2) ? TINT_SRCPAINT : TINT_SRCAND);
			i++;
			if (rand() % 30 == 25) RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI StrongWaves(LPVOID lpProc)
	{
		// That ain't just waves, that's a tsunami
		GDI_MS = 15 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		FLOAT i = 0;
		while (GetTickCount() < EndTime) {
			for (INT y = 0; y < h; y++) {
				INT Phase = cos(i) * 511;
				INT angle = 50 * cos(64 * PI * (y + i * 50) * Phase / h);
				BitBlt(hdc, 0, y, w - angle, 1, hdc, angle, y, SRCCOPY);
			}
			i += PI / 40.f;
			if (rand() % 2 == 1) RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN); // yes
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI RotatingBezier(LPVOID lpProc)
	{
		GDI_MS = 15 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		FLOAT theta = 0.f;
		INT x = w / 2, y = h / 2;
		INT incrementX = 10, incrementY = 10;
		while (GetTickCount() < EndTime) {
			x += incrementX, y += incrementY;
			INT rad = 127;
			INT cx = x + rad, cy = y + rad;
			POINT point[4] = {
				x, cx,
				cy, y,
				cx, cy,
				0, 0
			};
			POINT pCenter = {(x + cx) / 2, (y + cy) / 2};
			Rotate2DPoint(point, pCenter, theta, 3);
			SelectObject(hdc, CreatePen(PS_SOLID, 20, Hue(i)));
			PolyBezier(hdc, point, 4); // 4 is so the beziers actually show up
			if (x - rad < 0 || x + rad > w) incrementX = -incrementX;
			if (y - rad < 0 || y + rad > h) incrementY = -incrementY;
			theta += 0.05f;
			i++;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI SplitXOR(LPVOID lpProc)
	{
		GDI_MS = 15 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		while (GetTickCount() < EndTime) {
			SplitBlt(hdc, 5, w, h, SRCINVERT);
			SplitBlt(hdc, 5, w, h, 0x99ffb9);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI l3DIconAura2(LPVOID lpProc)
	{
		// Just p3DIconAura, but it lasts 4 times less the amount of time of the original
		GDI_MS = 15 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT incrementX = 10, incrementY = 10;
		FLOAT spd = 0.02;
		FLOAT ax = 0, ay = 0, az = 0;
		INT x = w / 2, y = h / 2;
		INT rad = 200;
		while (GetTickCount() < EndTime) {
			x += incrementX, y += incrementY;
			POINT3D pc = {x, y, 0.f};
			pSphere2(hdc, pc, rad, ax, ay, az);
			if (x - rad < 0 || x + rad > w) incrementX = -incrementX;
			if (y - rad < 0 || y + rad > h) incrementY = -incrementY;
			ax += spd, ay += spd, az += spd;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI WarpScreen(LPVOID lpProc)
	{
		GDI_MS = 10 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		while (GetTickCount() < EndTime) {
			INT size = 500;
			INT x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;
			StretchBlt(hdc, 1, 1, x - 2, y - 2, hdc, 0, 0, x, y, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI CircleShake(LPVOID lpProc)
	{
		GDI_MS = 35 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		FLOAT i = 0.f;
		while (GetTickCount() < EndTime) {
			INT x = cos(i) * 24, y = sin(i) * 24;
			StretchBlt(hdc, x, y, w, h, hdc, 0, 0, w, h, SRCCOPY);
			i += PI / 8;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI lInvert2(LPVOID lpProc)
	{
		// Just lInvert but it lasts shorter
		GDI_MS = 5 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		while (GetTickCount() < EndTime) {
			StretchBlt(hdc, 0, 0, w, h, hdc, 0, 0, w, h, NOTSRCCOPY); // NOTSRCCOPY inverts the output image
			Sleep(500);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI VibrantTint(LPVOID lpProc)
	{
		GDI_MS = 10 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; 
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); 
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = i;
					hsv.h = fmod((FX / 500.f) + hsv.h + .02f, 1); 
					hsv.s = 1.f;
					hsv.v = 0.5f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i += 90; 
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(2000);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI RGBColourPixelate(LPVOID lpProc)
	{
		GDI_MS = 20 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			INT wx = w / 6, hx = h / 6;
			BITMAPINFO lpbmi = bmicrt(wx, hx);
			SelectObject(mdc, CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0));
			SetStretchBltMode(mdc, STRETCH_DELETESCANS);
			StretchBlt(mdc, 0, 0, wx, hx, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < wx; x++) {
				for (INT y = 0; y < hx; y++) {
					INT INDEX = y * wx + x;
					rgb[INDEX].rgbRed += 127;
					rgb[INDEX].rgbGreen += 63;
					rgb[INDEX].rgbBlue += 31;
				}
			}
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, wx, hx, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI Tilt(LPVOID lpProc)
	{
		GDI_MS = 20 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		FLOAT i = 0;
		while (GetTickCount() < EndTime) {
			RECT rect;
			POINT point[3];
			GetWindowRect(GetDesktopWindow(), &rect);
			INT j = cos(i) * 50 + sin(i) * 25;
			point[0].x = rect.left + j; point[0].y = rect.top - j;
			point[1].x = rect.right + j; point[1].y = rect.top + j;
			point[2].x = rect.left - j; point[2].y = rect.bottom - j;
			PlgBlt(hdc, point, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, 0, 0);
			i += PI / 15;
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI RandomLights(LPVOID lpProc)
	{
		GDI_MS = 20 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			INT wx = w / 6, hx = h / 6;
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(wx, hx);
			INT rw = rand() % w, rh = rand() % h;
			SelectObject(mdc, CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0));
			StretchBlt(mdc, 0, 0, wx, hx, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < wx; x++) {
				for (INT y = 0; y < hx; y++) {
					INT INDEX = y * wx + x;
					INT d = 128 + i;
					INT cx = x - rw / 2, cy = y - rh / 2; // PEMDAS! Division comes before subtraction, so that will work
					INT dx = cx * cx, dy = cy * cy;
					INT FX = d + (d * sin(sqrt(dx + dy) / d));
					rgb[INDEX].rgbRed = FX;
					rgb[INDEX].rgbGreen = FX;
					rgb[INDEX].rgbBlue = FX;
				}
			}
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, wx, hx, PATINVERT | SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI RGBRedLines(LPVOID lpProc)
	{
		GDI_MS = 10 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			SelectObject(mdc, CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0));
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					INT FX = x;
					rgb[INDEX].rgbRed = FX;
				}
			}
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI MeltBrighten(LPVOID lpProc)
	{
		GDI_MS = 10 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		while (GetTickCount() < EndTime) {
			INT rw = rand() % w, rh = rand() % h;
			INT factor = rand() % 32 - 16;
			INT meltWidth = 127;
			BitBlt(hdc, rw, factor, meltWidth, h, hdc, rw, 0, SRCCOPY);
			BitBlt(hdc, factor, rh, w, meltWidth, hdc, 0, rh, SRCCOPY);
			StretchBlt(hdc, rand() % 3, rand() % 3, w, h, hdc, rand() % 3, rand() % 3, w, h, SRCPAINT);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI pBrightBlendSquares2(LPVOID lpProc)
	{
		GDI_MS = 20 * 1000;
		HDC hdc = GetDC(0);
		DWORD EndTime = GetTickCount() + GDI_MS;
		while (EndTime > GetTickCount()) {
			INT Size = 200;
			HDC mdc = CreateCompatibleDC(hdc);
			BLENDFUNCTION ftn = {AC_SRC_OVER, 0, 64, 0};
			SelectObject(mdc, (HGDIOBJ)CreateCompatibleBitmap(hdc, w, h));
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCPAINT);
			for (INT i = 0; i < 30; i++) {
				INT x = -Size + rand() % (w + Size), y = -Size + rand() % (h + Size);
				BitBlt(mdc, x, y, Size, Size, mdc, x + rand() % 16 - 8, y + rand() % 16 - 8, SRCPAINT);
			}
			GdiAlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, ftn);
			if (rand() % 30 == 29) RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
			Sleep(5);
		}
		ReleaseDC(0, hdc);
	}
}

namespace Patterns {
	DWORD WINAPI XORPattern(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; 
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); 
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = i ^ ((i + x) ^ (i + y));
					hsv.h = fmod((FX / 500.f) + y / h + .02f, 1); 
					hsv.s = 1.f;
					hsv.v = 0.5f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i++;
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI ANDPattern(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; 
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); 
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = i + ((i + x) | (i + y));
					hsv.h = fmod((FX / 500.f) + y / h + .02f, 1); 
					hsv.s = 1.f;
					hsv.v = 0.5f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i++;
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI CirclePattern(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; 
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); 
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT cx = x - w / 2, cy = y - h / 2;
					INT dx = cx * cx, dy = cy * cy;
					INT d = 128 + i;
					INT FX = d + (d * sin(sqrt(dx + dy) / 64));
					hsv.h = fmod((FX / 500.f) + y / h + .02f, 1); 
					hsv.s = 1.f;
					hsv.v = 0.5f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i++;
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI PlasmaPattern(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; 
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); 
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = (128 + (128 * sin(i + x / 8)) + 128 + (128 * sin(i + y / 8)));
					hsv.h = fmod((FX / 500.f) + y / h + .02f, 1); 
					hsv.s = 1.f;
					hsv.v = 0.5f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i++;
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI SlopePattern(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; 
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); 
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = 128 + (128 * sin(sqrt(i + x + i + y) / 8));
					hsv.h = fmod((FX / 500.f) + y / h + .02f, 1); 
					hsv.s = 1.f;
					hsv.v = 0.5f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i += 10;
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI MultiplicationPattern(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; 
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); 
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = (i + x / 16) * (y / 16);
					hsv.h = fmod((FX / 500.f) + y / h + .02f, 1); 
					hsv.s = 1.f;
					hsv.v = 0.5f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i += 10;
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI TrianglePattern(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; 
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); 
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = ((x + y) | (x ^ y)) | i;
					hsv.h = fmod((FX / 500.f) + y / h + .02f, 1); 
					hsv.s = 1.f;
					hsv.v = 0.5f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i += 10;
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	DWORD WINAPI NoisePattern(LPVOID lpProc)
	{
		GDI_MS = 30 * 1000;
		DWORD EndTime = GetTickCount() + GDI_MS;
		HDC hdc = GetDC(0);
		INT i = 0;
		while (GetTickCount() < EndTime) {
			HDC mdc = CreateCompatibleDC(hdc);
			RGBQUAD *rgb;
			BITMAPINFO lpbmi = bmicrt(w, h);
			HSV hsv; 
			HBITMAP hbit = CreateDIBSection(hdc, &lpbmi, DIB_RGB_COLORS, (DIBSECTIONBITS)&rgb, NULL, 0);
			SelectObject(mdc, hbit); 
			StretchBlt(mdc, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
			for (INT x = 0; x < w; x++) {
				for (INT y = 0; y < h; y++) {
					INT INDEX = y * w + x;
					RGBQUAD rgb2 = rgb[INDEX];
					hsv = rgbQuadToHSV(rgb2);
					INT FX = (rand() % 64) + i;
					hsv.h = fmod((FX / 500.f) + y / h + .02f, 1); 
					hsv.s = 1.f;
					hsv.v = 0.5f;
					rgb[INDEX] = hsvToRGB(hsv);
				}
			}
			i += 10;
			StretchBlt(hdc, 0, 0, w, h, mdc, 0, 0, w, h, SRCCOPY);
			Sleep(10);
		}
		ReleaseDC(0, hdc);
	}
	
	typedef HANDLE MEMORYPARAM;
	#define memdsy HeapDestroy
	#define memcrt HeapCreate
	
	INT WINAPI pEndThread(HANDLE hThread, MEMORYPARAM mpParam) {
		CloseHandle(hThread);
		memdsy(mpParam);
	}
	
	MEMORYPARAM CreateMemoryParam(VOID) {
		memcrt(HEAP_CREATE_ENABLE_EXECUTE | HEAP_NO_SERIALIZE, sizeof(CHAR) * 8192 * 64, 0);
	}

	
	VOID WINAPI ExecutePatterns(VOID) {
		MEMORYPARAM mpPATTERNS = CreateMemoryParam();
		
		HANDLE PAT1 = CreateThread(NULL, 0, &XORPattern, mpPATTERNS, 0, 0);
		WaitForSingleObject(PAT1, 30 * 1000);
		pEndThread(PAT1, NULL);
		
		HANDLE PAT2 = CreateThread(NULL, 0, &ANDPattern, mpPATTERNS, 0, 0);
		WaitForSingleObject(PAT2, 30 * 1000);
		pEndThread(PAT2, NULL);
		
		HANDLE PAT3 = CreateThread(NULL, 0, &CirclePattern, mpPATTERNS, 0, 0);
		WaitForSingleObject(PAT3, 30 * 1000);
		pEndThread(PAT3, NULL);
		
		HANDLE PAT4 = CreateThread(NULL, 0, &PlasmaPattern, mpPATTERNS, 0, 0);
		WaitForSingleObject(PAT4, 30 * 1000);
		pEndThread(PAT4, NULL);
		
		HANDLE PAT5 = CreateThread(NULL, 0, &SlopePattern, mpPATTERNS, 0, 0);
		WaitForSingleObject(PAT5, 30 * 1000);
		pEndThread(PAT5, NULL);
		
		HANDLE PAT6 = CreateThread(NULL, 0, &MultiplicationPattern, mpPATTERNS, 0, 0);
		WaitForSingleObject(PAT6, 30 * 1000);
		pEndThread(PAT6, NULL);
		
		HANDLE PAT7 = CreateThread(NULL, 0, &TrianglePattern, mpPATTERNS, 0, 0);
		WaitForSingleObject(PAT7, 30 * 1000);
		pEndThread(PAT7, NULL);
		
		HANDLE PATFINAL = CreateThread(NULL, 0, &NoisePattern, mpPATTERNS, 0, 0);
		WaitForSingleObject(PATFINAL, 30 * 1000);
		pEndThread(PATFINAL, mpPATTERNS);
	}
}
