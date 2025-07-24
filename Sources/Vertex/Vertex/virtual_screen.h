#pragma once
#include "Vertex.h"

extern
RECT
GetVirtualScreen(
	VOID
);

extern
POINT
GetVirtualScreenPos(
	VOID
);

extern
SIZE
GetVirtualScreenSize(
	VOID
);

extern
BOOL
CALLBACK
MonitorEnumProc(
	_In_    HMONITOR hMonitor,
	_In_    HDC      hdcMonitor,
	_In_    PRECT    prcBounds,
	_Inout_ LPARAM   lParam
);