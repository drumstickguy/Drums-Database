#pragma once
#include "phosphorium.h"

VOID
SeedXorshift32(
	_In_ DWORD dwSeed
);

DWORD
Xorshift32();