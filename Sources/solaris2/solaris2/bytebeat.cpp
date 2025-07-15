#include "main.h";

VOID WINAPI bytebeat_1() {
    int i = 1;

    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[8000 * 40] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 3 * (t >> 6 + t >> 4) + i / 5) << (t >> 2 - t << 8) << i);
        i++;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat_2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[8000 * 30] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(rand() % 32 + (((t >> 2) * (t >> 9) | t >> 6)) - 1);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat_3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[11025 * 20] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t << 3 | t >> 18));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat_4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[11025 * 10] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((((t >> 4) * (t >> 3) | t >> 2 & (t >> 30) ^ t >> 15) + ((t >> 2) * (t >> 9) | t >> 6)) - 1);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat_5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[8000 * 20] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((((t >> 8) * (t >> 4) | t >> 3) * 2));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat_6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[8000 * 30] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((((t >> 7) * (t >> 5) | t >> 3) ^ 4) - 1);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat_8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[11025 * 20] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t) {
        float t2 = (float)t / 11025;
        float f1 = 220.0 * sin(2 * 3.14159 * t2 * (2 + sin(t2 * 4)));
        float meow = (sin(f1 * 2 * 3.14159) + sin(f1 * 4 * 3.14159) * 0.5) * 0.5;

        buffer[t] = static_cast<char>((meow + 1.0) * 127);
    }

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat_7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[11025 * 60] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t) {
        float t2 = (float)t / 11025;
        int segment = (t / (11025 / 2)) % 8;

        float frequency;
        if (segment < 4) {
            frequency = 220.0 * (1 + segment / 3.0f);
        }
        else {
            frequency = 220.0 * (1 + (2 - (segment - 3)) / 3.0f);
        }

        float wave = sin(2 * 3.14159 * t2 * frequency);
        float meow = (sin(wave * 2 * 3.14159) + sin(wave * 4 * 3.14159) * 0.5) * 0.5;

        buffer[t] = static_cast<char>((meow + 10.0) * 127);
    }

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

/*
VOID WINAPI bytebeat_9() {
    const int hz = 11025;
    char buffer[hz * 60];
    const size_t buffsize = sizeof(buffer);

    HWAVEOUT hwo;
    WAVEHDR hdr = { buffer, buffsize, 0, 0, 0, 0, 0, 0 };
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, hz, hz, WAVE_FORMAT_PCM, 8, 0 };

    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));

    int freq1 = 0, freq2 = 0, freq3 = 0, sample1 = 0, sample2 = 0, sample3 = 0;
    for (int i = 0; i < buffsize; i++) {
        if (i % (int)(hz * 0.166) == 0) {
            freq1 = hz / (35800000 / (32 * ((random() % 510) * 100 + 200 + 1)));
            freq2 = hz / (35800000 / (32 * ((random() % 510) * 100 + 250 + 1)));
            freq3 = hz / (35800000 / (32 * ((random() % 510) * 100 + 325 + 1)));
        }

        sample1 = (i % freq1 < freq1 / 3) ? -127 : 127;
        sample2 = (i % freq2 < freq2 / 3) ? -127 : 127;
        sample3 = (i % freq3 < freq3 / 3) ? -127 : 127;

        buffer[i] = (unsigned char)(((sample1) * 0.2) + 127) << 6 | i >> 16;
    }

    waveOutWrite(hwo, &hdr, sizeof(hdr));
    waveOutUnprepareHeader(wave, &hdr, sizeof(hdr));
    waveOutClose(wave);
}


VOID WINAPI bytebeat_10() {
    const int hz = 8000;
    char buffer[hz * 60];
    const size_t buffsize = sizeof(buffer);

    HWAVEOUT hwo;
    WAVEHDR hdr = { buffer, buffsize, 0, 0, 0, 0, 0, 0 };
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, hz, hz, WAVE_FORMAT_PCM, 8, 0 };

    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));

    int freq1 = 0, freq2 = 0, freq3 = 0, sample1 = 0, sample2 = 0, sample3 = 0;
    for (int i = 0; i < buffsize; i++) {
        if (i % (int)(hz * 0.166) == 0) {
            freq1 = hz / (35800000 / (32 * ((random() % 510) * 100 + 200 + 1)));
            freq2 = hz / (35800000 / (32 * ((random() % 510) * 100 + 250 + 1)));
            freq3 = hz / (35800000 / (32 * ((random() % 510) * 100 + 325 + 1)));
        }

        sample1 = (i % freq1 < freq1 / 3) ? -127 : 127;
        sample2 = (i % freq2 < freq2 / 3) ? -127 : 127;
        sample3 = (i % freq3 < freq3 / 3) ? -127 : 127;

        buffer[i] = (unsigned char)((2 * i << 3) + (i << 1 | i >> 5) + (i >> 4) * i + (20) + (2 * i << 3 + (i << 10 | i >> 7) | i + (8)) + (((i >> 2) * (i >> 9) | i >> 6) + sqrt(i << 6 | i >> 16)) + (i >> 5 | (i >> 2) * (i >> 5))) & 255;
    }

    waveOutWrite(hwo, &hdr, sizeof(hdr));
    waveOutUnprepareHeader(wave, &hdr, sizeof(hdr));
    waveOutClose(wave);
}
*/

VOID WINAPI bytebeat_11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[8000 * 20] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((((t >> 4) * (t >> 3) | t >> 2) & ((t >> 2) * (t >> 9) | t >> 6)) - 1);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat_12() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[8000 * 30] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * rand());

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI bytebeat_13() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    char buffer[11025 * 60] = {};

    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (rand() % 64) ^ 2 * (t << 30 & t >> 18 * t << 24) + (t + (t >> 9 | t >> 13)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}