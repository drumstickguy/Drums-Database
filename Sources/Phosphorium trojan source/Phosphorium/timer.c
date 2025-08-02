#include "phosphorium.h"

DWORD dwTickCount = 0;

VOID
InitialTimer(
	_In_ UINT uiDelay
)
{
	SetTimer(NULL, 1, uiDelay, TimerProc);
	MSG msg = { 0 };

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		Sleep(1);
	}
	KillTimer(NULL, 1);
}

VOID
CALLBACK
TimerProc(
	_In_ HWND hWindow,
	_In_ UINT uMessage,
	_In_ UINT_PTR uptrTimerId,
	_In_ DWORD dwTime
)
{
	dwTickCount++;
}