#pragma once
#include "Vertex.h"

extern DWORD dwTimeElapsed;

extern
VOID
InitializeTimer(
	_In_ UINT uDelay
);

extern
VOID
CALLBACK
TimerProc(
	_In_ HWND hwndTimer,
	_In_ UINT uMsg,
	_In_ UINT_PTR ulTimerID,
	_In_ DWORD dwTime
);