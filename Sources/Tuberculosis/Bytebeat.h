HWAVEOUT hwo;

VOID hworst(VOID) {
	// RESET BYTEBEAT (to prevent memleak) //
	waveOutReset(hwo);
}

namespace Bytebeat {
	// NAMESPACE FOR BYTEBEAT FORMULAS //
	VOID Bytebeat1(VOID) {
		BYTE tbuffer[28000 * 20];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 28000, 28000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)(((t>>10)*(t>>8)/10)+t);
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID Bytebeat2(VOID) {
		BYTE tbuffer[28000 * 32];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 28000, 28000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)((t>>2)*cos(t>>8)+t*2);
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID Bytebeat3(VOID) {
		BYTE tbuffer[32000 * 10];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)(t*t*log10(t)/8);
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID pBytebeat5(VOID) {
		BYTE tbuffer[32000 * 30];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)(t*10*(INT)((25*cos(t>>11))/8)^t);
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID Bytebeat6(VOID) {
		BYTE tbuffer[32000 * 30];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)((t/4^(t>>8|9))-t);
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID Bytebeat7(VOID) {
		BYTE tbuffer[32000 * 30];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)(t*4^(t>>4)*(INT)(sin(t>>9)*2));
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID Bytebeat8(VOID) {
		BYTE tbuffer[32000 * 30];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)((t>>3)*((t>>10|9)^(t>>6|7)));
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID Bytebeat9(VOID) {
		BYTE tbuffer[34000 * 15];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 34000, 34000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)(t+(5*t>>(t>>11&15)));
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID Bytebeat10(VOID) {
		BYTE tbuffer[34000 * 30];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)(5*t-(t>>(t>>11&15))^t);
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID Bytebeat11(VOID) {
		BYTE tbuffer[34000 * 20];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 28000, 28000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)((t>>5)*(t>>8)-t);
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
	
	VOID Bytebeat12(VOID) {
		BYTE tbuffer[34000 * 20];
		WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 25000, 25000, 1, 8, 0};
		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 1, 0, CALLBACK_NULL);
		for (ULONG t = 0; t < sizeof(tbuffer); t++) {
			tbuffer[t] = (BYTE)(t*(INT)(cos(t>>10)*5)+t^t);
		}
		WAVEHDR wh = {(PSTR)tbuffer, sizeof(tbuffer), 0, 0, 0, 0, 0, 0};
		waveOutSetVolume(hwo, 0xffffff);
		waveOutPrepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutWrite(hwo, &wh, sizeof(tbuffer));
		waveOutUnprepareHeader(hwo, &wh, sizeof(tbuffer));
		waveOutClose(hwo);
	}
}
