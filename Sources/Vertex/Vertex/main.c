#include "Vertex.h"

INT
WINAPI 
wWinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     PWSTR     szCmdLine,
    _In_     INT       nShowCmd
) {


    CreateThread( NULL, 0, ( PTHREAD_START_ROUTINE )InitializeTimer, ( PVOID )1000, 0, NULL );
    ExecutePayload( Payload1, INFINITE ); 
}