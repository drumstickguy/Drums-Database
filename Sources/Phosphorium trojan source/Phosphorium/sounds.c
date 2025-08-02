#include "phosphorium.h"

VOID
WINAPI
AudioSequence1(VOID)
{
	HWAVEOUT hWaveout = 0;
	WAVEFORMATEX wfFormat = {
		WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0
	};

	waveOutOpen(&hWaveout, WAVE_MAPPER, &wfFormat, 0, 0, CALLBACK_NULL);

	CONST INT nDurationSeconds = 30;
	CONST INT nSampleRate = 8000;
	CONST INT nBufferSize = nSampleRate * nDurationSeconds;

	BYTE* pccWaveData = (BYTE*)LocalAlloc(0, nBufferSize);

	for (INT t = 0; t < nBufferSize; ++t)
	{
		pccWaveData[t] = (BYTE)(t | t % 255 | t % 257) - ((t >> 6 | t << 1) + (t >> 5 | t << 3 | t >> 3) | t >> 2 | t << 1);
	}

	WAVEHDR whHeader = { (LPSTR)pccWaveData, nBufferSize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutWrite(hWaveout, &whHeader, sizeof(whHeader));

	while (!(whHeader.dwFlags & WHDR_DONE)) 
	{
		Sleep(10);
	}

	waveOutUnprepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutClose(hWaveout);
	LocalFree(pccWaveData);
}

VOID
WINAPI
AudioSequence2(VOID)
{
	HWAVEOUT hWaveout = 0;
	WAVEFORMATEX wfFormat = {
		WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0
	};

	waveOutOpen(&hWaveout, WAVE_MAPPER, &wfFormat, 0, 0, CALLBACK_NULL);

	CONST INT nDurationSeconds = 30;
	CONST INT nSampleRate = 8000;
	CONST INT nBufferSize = nSampleRate * nDurationSeconds;

	BYTE* pccWaveData = (BYTE*)LocalAlloc(0, nBufferSize);

	for (INT t = 0; t < nBufferSize; ++t)
	{
		pccWaveData[t] = (BYTE)(t | t % 255 | t % 257) - (t * t >> 12) * (t >> 4 | t >> 8);
	}

	WAVEHDR whHeader = { (LPSTR)pccWaveData, nBufferSize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutWrite(hWaveout, &whHeader, sizeof(whHeader));

	while (!(whHeader.dwFlags & WHDR_DONE)) 
	{
		Sleep(10);
	}

	waveOutUnprepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutClose(hWaveout);
	LocalFree(pccWaveData);
}

VOID
WINAPI
AudioSequence3(VOID)
{
	HWAVEOUT hWaveout = 0;
	WAVEFORMATEX wfFormat = {
		WAVE_FORMAT_PCM, 1, 12000, 12000, 1, 8, 0
	};

	waveOutOpen(&hWaveout, WAVE_MAPPER, &wfFormat, 0, 0, CALLBACK_NULL);

	CONST INT nDurationSeconds = 30;
	CONST INT nSampleRate = 12000;
	CONST INT nBufferSize = nSampleRate * nDurationSeconds;

	BYTE* pccWaveData = (BYTE*)LocalAlloc(0, nBufferSize);

	for (INT t = 0; t < nBufferSize; ++t)
	{
		pccWaveData[t] = (BYTE)((t >> 1) * (15 & 0x234568a0 | (t >> 8 & 28)) * t >> 1 >> (t >> 11) * t >> 12) + (t >> 4 & t & 24);
	}

	WAVEHDR whHeader = { (LPSTR)pccWaveData, nBufferSize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutWrite(hWaveout, &whHeader, sizeof(whHeader));

	while (!(whHeader.dwFlags & WHDR_DONE)) 
	{
		Sleep(10);
	}

	waveOutUnprepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutClose(hWaveout);
	LocalFree(pccWaveData);
}

VOID
WINAPI
AudioSequence4(VOID)
{
	HWAVEOUT hWaveout = 0;
	WAVEFORMATEX wfFormat = {
		WAVE_FORMAT_PCM, 1, 12000, 12000, 1, 8, 0
	};

	waveOutOpen(&hWaveout, WAVE_MAPPER, &wfFormat, 0, 0, CALLBACK_NULL);

	CONST INT nDurationSeconds = 30;
	CONST INT nSampleRate = 12000;
	CONST INT nBufferSize = nSampleRate * nDurationSeconds;

	BYTE* pccWaveData = (BYTE*)LocalAlloc(0, nBufferSize);

	for (INT t = 0; t < nBufferSize; ++t)
	{
		pccWaveData[t] = (BYTE)(t * (t ^ t + (t >> 15 | 1) * (t - 1280 ^ t) >> 10));
	}

	WAVEHDR whHeader = { (LPSTR)pccWaveData, nBufferSize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutWrite(hWaveout, &whHeader, sizeof(whHeader));

	while (!(whHeader.dwFlags & WHDR_DONE))
	{
		Sleep(10);
	}

	waveOutUnprepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutClose(hWaveout);
	LocalFree(pccWaveData);
}

VOID
WINAPI
AudioSequence5(VOID)
{
	HWAVEOUT hWaveout = 0;
	WAVEFORMATEX wfFormat = {
		WAVE_FORMAT_PCM, 1, 12000, 12000, 1, 8, 0
	};

	waveOutOpen(&hWaveout, WAVE_MAPPER, &wfFormat, 0, 0, CALLBACK_NULL);

	CONST INT nDurationSeconds = 30;
	CONST INT nSampleRate = 12000;
	CONST INT nBufferSize = nSampleRate * nDurationSeconds;

	BYTE* pccWaveData = (BYTE*)LocalAlloc(0, nBufferSize);

	for (INT t = 0; t < nBufferSize; ++t)
	{
		pccWaveData[t] = (BYTE)(t * (2 & t >> 13 ? 7 : 5) * (3 - (3 & t >> 9) + (3 & t >> 8)) ^ (3 * -t + (t & 4096 & (t >> 11) % 32 > 28 ? 2 : 16)) | t >> 3);
	}

	WAVEHDR whHeader = { (LPSTR)pccWaveData, nBufferSize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutWrite(hWaveout, &whHeader, sizeof(whHeader));

	while (!(whHeader.dwFlags & WHDR_DONE))
	{
		Sleep(10);
	}

	waveOutUnprepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutClose(hWaveout);
	LocalFree(pccWaveData);
}

VOID
WINAPI
AudioSequence6(VOID)
{
	HWAVEOUT hWaveout = 0;
	WAVEFORMATEX wfFormat = {
		WAVE_FORMAT_PCM, 1, 12000, 12000, 1, 8, 0
	};

	waveOutOpen(&hWaveout, WAVE_MAPPER, &wfFormat, 0, 0, CALLBACK_NULL);

	CONST INT nDurationSeconds = 30;
	CONST INT nSampleRate = 12000;
	CONST INT nBufferSize = nSampleRate * nDurationSeconds;

	BYTE* pccWaveData = (BYTE*)LocalAlloc(0, nBufferSize);

	for (INT t = 0; t < nBufferSize; ++t)
	{
		pccWaveData[t] = (BYTE)(t * 2 * (4 | 7 & t >> 13) * (t >> 11 & 1) + 127) * (t * (t >> 11 & t >> 13) ^ (t >> 9 & 3) & 127);
	}

	WAVEHDR whHeader = { (LPSTR)pccWaveData, nBufferSize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutWrite(hWaveout, &whHeader, sizeof(whHeader));

	while (!(whHeader.dwFlags & WHDR_DONE))
	{
		Sleep(10);
	}

	waveOutUnprepareHeader(hWaveout, &whHeader, sizeof(whHeader));
	waveOutClose(hWaveout);
	LocalFree(pccWaveData);
}

DWORD
WINAPI
SoundThread(
	_In_ LPVOID lpVoidParam
)
{
	for (;;)
	{
		AudioSequence1();
		Sleep(PAYLOAD_TIME);

		AudioSequence2();
		Sleep(PAYLOAD_TIME);

		AudioSequence3();
		Sleep(PAYLOAD_TIME);

		AudioSequence4();
		Sleep(PAYLOAD_TIME);

		AudioSequence5();
		Sleep(PAYLOAD_TIME);

		AudioSequence6();
		Sleep(PAYLOAD_TIME);
	}
}