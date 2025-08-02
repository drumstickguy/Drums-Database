#pragma once
#include "phosphorium.h"

extern DWORD dwTickCount;

VOID
InitialTimer(
	_In_ UINT uiDelay
);

VOID
CALLBACK
TimerProc(
	_In_ HWND hWindow,
	_In_ UINT uMessage,
	_In_ UINT_PTR uptrTimerId,
	_In_ DWORD dwTime
);