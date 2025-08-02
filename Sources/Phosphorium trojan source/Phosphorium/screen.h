#pragma once
#include "phosphorium.h"

BOOL
CALLBACK
MonitorEnumProc(
	_In_    HMONITOR hMonitor,
	_In_    HDC      hdcMonitor,
	_In_    PRECT    prcBounds,
	_Inout_ LPARAM   longPtr
);

RECT
GetVirtualScreen(VOID);

POINT
GetVirtualScreenPosition(VOID);

SIZE
GetVirtualScreenSize(VOID);