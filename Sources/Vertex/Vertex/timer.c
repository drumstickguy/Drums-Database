#include "timer.h"

DWORD dwTimeElapsed;

VOID
InitializeTimer(
	_In_ UINT uDelay
) {
	SetTimer( NULL, 0, uDelay, TimerProc );

	MSG msg = { 0 };
	while ( TRUE ) {
		if ( PeekMessageW( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessageW( &msg );
		}
	}
}

VOID
CALLBACK
TimerProc(
	_In_ HWND hwndTimer,
	_In_ UINT uMsg,
	_In_ UINT_PTR ulTimerID,
	_In_ DWORD dwTime
) {

	dwTimeElapsed = dwTime;
}
