# Microsoft Developer Studio Project File - Name="RealPopup" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RealPopup - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RealPopup.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RealPopup.mak" CFG="RealPopup - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RealPopup - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RealPopup - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "RealPopup - Win32 Release le" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RealPopup", LBAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RealPopup - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "_AFXDLL" /D "USE_RESDLL" /D "WIN32" /D "_WINDOWS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib mpr.lib netapi32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "USASETFONT" /D "_AFXDLL" /D "USE_RESDLL" /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib mpr.lib netapi32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release le"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RealPopup___Win32_Release_le"
# PROP BASE Intermediate_Dir "RealPopup___Win32_Release_le"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_le"
# PROP Intermediate_Dir "Release_le"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "_AFXDLL" /D "USE_RESDLL" /D "WIN32" /D "_WINDOWS" /Fr /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "_AFXDLL" /D "USE_RESDLL" /D "WIN32" /D "_WINDOWS" /D "VERSIONELE" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib mpr.lib netapi32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 winmm.lib mpr.lib netapi32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RealPopup - Win32 Release"
# Name "RealPopup - Win32 Debug"
# Name "RealPopup - Win32 Release le"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CmdLine.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgbars.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release le"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgCriteria.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgService.cpp
# End Source File
# Begin Source File

SOURCE=.\FntPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\getfile.cpp
# End Source File
# Begin Source File

SOURCE=.\mainres.rc
# End Source File
# Begin Source File

SOURCE=.\MaskedBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\MRUCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgRealPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\NB.cpp
# End Source File
# Begin Source File

SOURCE=.\PPEnvironment.cpp
# End Source File
# Begin Source File

SOURCE=.\PPLanguages.cpp
# End Source File
# Begin Source File

SOURCE=.\PPMessages.cpp
# End Source File
# Begin Source File

SOURCE=.\PPUsers.cpp
# End Source File
# Begin Source File

SOURCE=.\RealPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\RealPopup.rc

!IF  "$(CFG)" == "RealPopup - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release le"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RealPopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\RPMsgEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RPMsgHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\RPNetworkBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\RPSuggEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\RPUsers.cpp
# End Source File
# Begin Source File

SOURCE=.\SendDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SMB.cpp
# End Source File
# Begin Source File

SOURCE=.\SMBMSG.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

# ADD CPP /O1 /Yc"STDAFX.H"

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

# ADD CPP /Yc"STDAFX.H"

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release le"

# ADD BASE CPP /O1 /Yc"STDAFX.H"
# ADD CPP /O1 /Yc"STDAFX.H"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TbStDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CmdLine.h
# End Source File
# Begin Source File

SOURCE=.\Dictionary.h
# End Source File
# Begin Source File

SOURCE=.\dlgbars.h
# End Source File
# Begin Source File

SOURCE=.\DlgCriteria.h
# End Source File
# Begin Source File

SOURCE=.\DlgService.h
# End Source File
# Begin Source File

SOURCE=.\FntPropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\getfile.h
# End Source File
# Begin Source File

SOURCE=.\MaskedBitmap.h
# End Source File
# Begin Source File

SOURCE=.\MRUCombo.h
# End Source File
# Begin Source File

SOURCE=.\MsgPopup.h
# End Source File
# Begin Source File

SOURCE=.\MsgRealPopup.h
# End Source File
# Begin Source File

SOURCE=.\Nb.h
# End Source File
# Begin Source File

SOURCE=.\PPEnvironment.h
# End Source File
# Begin Source File

SOURCE=.\PPLanguages.h
# End Source File
# Begin Source File

SOURCE=.\PPMessages.h
# End Source File
# Begin Source File

SOURCE=.\PPUsers.h
# End Source File
# Begin Source File

SOURCE=.\RealPopup.h
# End Source File
# Begin Source File

SOURCE=.\RealPopupDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RPMsgEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RPMsgHandler.h
# End Source File
# Begin Source File

SOURCE=.\RPNetworkBrowser.h
# End Source File
# Begin Source File

SOURCE=.\RPSuggEdit.h
# End Source File
# Begin Source File

SOURCE=.\RPUsers.h
# End Source File
# Begin Source File

SOURCE=.\SendDlg.h
# End Source File
# Begin Source File

SOURCE=.\Smb.h
# End Source File
# Begin Source File

SOURCE=.\Smbmsg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TbStDialog.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Arrow_1.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
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

SOURCE=.\res\logo2.bmp
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

SOURCE=.\res\NetworkBrowserLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\playd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\playu.bmp
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

SOURCE=.\res\service.ico
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
# Begin Source File

SOURCE=.\res\welcomem.bin
# End Source File
# End Group
# End Target
# End Project
