class RegEditor
{
public:
	bool WriteReg(HKEY MainKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwType, CONST BYTE *lpData, DWORD cbData);
	bool DeleteReg(HKEY MainKey, LPCTSTR lpSubKey, LPCTSTR lpValueName);
};



bool RegEditor::WriteReg(HKEY MainKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwType, CONST BYTE *lpData, DWORD cbData)
{
    HKEY hkey=NULL;
	if(RegCreateKey(MainKey, lpSubKey, &hkey) == ERROR_SUCCESS)
	{
		if(RegSetValueEx(hkey, lpValueName, NULL, dwType, (LPBYTE)lpData, cbData) == ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			return true;
		}
		else
		{
			RegCloseKey(hkey);
			return false;
		}

	}
	else
	{
		RegCloseKey(hkey);
		return false;
	}

	RegCloseKey(hkey);
	return false;
}



bool RegEditor::DeleteReg(HKEY MainKey, LPCTSTR lpSubKey, LPCTSTR lpValueName)
{
	HKEY hkey=NULL;
	if(RegOpenKeyEx(MainKey, lpSubKey, NULL, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		if(RegDeleteValue(hkey, lpValueName) == ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			return true;
		}
		else
		{
			RegCloseKey(hkey);
			return false;
		}
	}
	else
	{
		RegCloseKey(hkey);
		return false;
	}

	RegCloseKey(hkey);
	return false;

}