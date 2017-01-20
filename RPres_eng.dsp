# Microsoft Developer Studio Project File - Name="RPres_eng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RPres_eng - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RPres_eng.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RPres_eng.mak" CFG="RPres_eng - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RPres_eng - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/", aaaaaaaa"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RPres_eng___Win32_Release"
# PROP BASE Intermediate_Dir "RPres_eng___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "RPres_eng___Win32_Release"
# PROP Intermediate_Dir "RPres_eng___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RPRES_ENG_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RPRES_ENG_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 /out:"Release/RPres_eng.dll" /NOENTRY
# Begin Target

# Name "RPres_eng - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\RealPopup.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_edit.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_msga.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_msgd.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_play.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_posd.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_poss.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_rich.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_srci.ico
# End Source File
# Begin Source File

SOURCE=.\res\Lgeup.cur
# End Source File
# Begin Source File

SOURCE=.\matro3.bmp
# End Source File
# Begin Source File

SOURCE=.\matro4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nettreepc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RealPopup.ico
# End Source File
# Begin Source File

SOURCE=.\res\RealPopup.rc2
# End Source File
# Begin Source File

SOURCE=.\res\rptitle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\trayicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\welcomeENG.bin
# End Source File
# End Group
# End Target
# End Project
