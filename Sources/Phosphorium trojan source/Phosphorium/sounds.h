#pragma once
#include "phosphorium.h"

VOID
WINAPI
AudioSequence1(VOID);

VOID
WINAPI
AudioSequence2(VOID);

VOID
WINAPI
AudioSequence3(VOID);

VOID
WINAPI
AudioSequence4(VOID);

VOID
WINAPI
AudioSequence5(VOID);

DWORD
WINAPI
SoundThread(
	_In_ LPVOID lpVoidParam
);