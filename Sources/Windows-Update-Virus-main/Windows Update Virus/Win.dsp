# Microsoft Developer Studio Project File - Name="Win" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Win - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Win.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Win.mak" CFG="Win - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Win - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Win - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Win - Win32 Release"
# Name "Win - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\PictureEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Win.cpp
# End Source File
# Begin Source File

SOURCE=.\Win.rc
# End Source File
# Begin Source File

SOURCE=.\WinDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DiskTools.h
# End Source File
# Begin Source File

SOURCE=.\FileTool.h
# End Source File
# Begin Source File

SOURCE=.\HookClass.h
# End Source File
# Begin Source File

SOURCE=.\ModifyRegistry.h
# End Source File
# Begin Source File

SOURCE=.\PictureEx.h
# End Source File
# Begin Source File

SOURCE=.\RegEditor.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Win.h
# End Source File
# Begin Source File

SOURCE=.\WinDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\kb\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ks\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pic\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\skb\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\kb\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ks\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pic\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\skb\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\kb\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ks\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pic\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\skb\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\kb\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ks\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\skb\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\kb\5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on\5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\skb\5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\kb\6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on\6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\skb\6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\kb\7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\kb\8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Black.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ks\blue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Button01.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Button1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Button2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Button2_NO.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Eyes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gif.gif
# End Source File
# Begin Source File

SOURCE=.\res\gif2.gif
# End Source File
# Begin Source File

SOURCE=.\res\load.gif
# End Source File
# Begin Source File

SOURCE=.\res\load2.gif
# End Source File
# Begin Source File

SOURCE=.\res\load3.gif
# End Source File
# Begin Source File

SOURCE=.\res\LoadBackground1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Null.bmp
# End Source File
# Begin Source File

SOURCE=".\res\src=http___5b0988e595225.cdn.sohucs.com_images_20170925_7ff7ee5bb0f04c94a3c908ca2ed01851.jpeg&refer=http___5b0988e595225.cdn.sohucs.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\taskbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\taskbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Win.ico
# End Source File
# Begin Source File

SOURCE=.\res\Win.rc2
# End Source File
# Begin Source File

SOURCE=.\res\zm\z1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zm\z2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zm\z3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zm\z4.bmp
# End Source File
# Begin Source File

SOURCE=".\res\此电脑 - 无效.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\此电脑.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\回收站 - 清空.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\回收站 - 无效.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\回收站.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\开始菜单.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\开始菜单2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\恐怖.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\任务栏此电脑.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\任务栏回收站.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\任务栏日记.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\任务栏图标2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\任务栏图标世界再见.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\任务栏文件夹.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\日记.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\文本文档.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\文本文档1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\文本文档2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\文件夹 - 无效.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\文件夹.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\文件夹1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\文件夹2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\无标题.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\无标题2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\再见，世界.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\桌面图标 - 无效.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\桌面图标2 - 无效.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\桌面图标2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\桌面主背景.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\Music\1.wav
# End Source File
# Begin Source File

SOURCE=.\res\Music\2.wav
# End Source File
# Begin Source File

SOURCE=.\res\Music\3.wav
# End Source File
# Begin Source File

SOURCE=.\res\Music\4.wav
# End Source File
# Begin Source File

SOURCE=.\res\Music\5.wav
# End Source File
# Begin Source File

SOURCE=.\res\Music\6.wav
# End Source File
# Begin Source File

SOURCE=.\res\BlurredScreen.wav
# End Source File
# Begin Source File

SOURCE=.\res\CREEPY.wav
# End Source File
# Begin Source File

SOURCE=.\res\doog.wav
# End Source File
# Begin Source File

SOURCE=.\res\doog2.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\SHUTDOWN.wav
# End Source File
# Begin Source File

SOURCE=".\res\Windows Recycle.wav"
# End Source File
# Begin Source File

SOURCE=.\res\winload.wav
# End Source File
# Begin Source File

SOURCE=".\res\背景音乐.wav"
# End Source File
# End Target
# End Project
