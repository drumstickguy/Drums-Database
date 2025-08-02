#pragma once
#include "phosphorium.h"

typedef union _RGBQUAD {
	COLORREF nRGB;
	struct {
		BYTE Red;
		BYTE Green;
		BYTE Blue;
		BYTE bUnused;
	};
} *PRGBQUAD;

typedef VOID(GDI_SHADER)(
	_In_ INT t,
	_In_ INT w,
	_In_ INT h,
	_In_ PRGBQUAD prgb
);