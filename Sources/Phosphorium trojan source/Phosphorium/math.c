#include "phosphorium.h"

DWORD xs;

VOID
SeedXorshift32(
	_In_ DWORD dwSeed
)
{
	xs = dwSeed;
}

DWORD
Xorshift32()
{
	xs ^= xs << 13;
	xs ^= xs >> 17;
	xs ^= xs << 5;
	return xs;
}