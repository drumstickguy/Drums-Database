#include "phosphorium.h"

BOOL
CALLBACK
MonitorEnumProc(
	_In_    HMONITOR hMonitor,
	_In_    HDC      hdcMonitor,
	_In_    PRECT    prcBounds,
	_Inout_ LPARAM   longPtr
)
{
	PRECT prcParam = (RECT*)longPtr;
	UnionRect(prcParam, prcParam, prcBounds);
	return TRUE;
}

RECT
GetVirtualScreen(VOID)
{
	RECT rcScreen = { 0 };
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&rcScreen);
	return rcScreen;
}

POINT
GetVirtualScreenPosition(VOID)
{
	RECT rcScreen = GetVirtualScreen();
	POINT ptScreen =
	{
		rcScreen.left,
		rcScreen.top
	};

	return ptScreen;
}

SIZE
GetVirtualScreenSize(VOID)
{
	RECT rcScreen = GetVirtualScreen();
	SIZE szScreen =
	{
		rcScreen.right - rcScreen.left,
		rcScreen.bottom - rcScreen.top
	};

	return szScreen;
}