# Microsoft Developer Studio Generated NMAKE File, Based on RealPopup.dsp
!IF "$(CFG)" == ""
CFG=RealPopup - Win32 Debug
!MESSAGE No configuration specified. Defaulting to RealPopup - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "RealPopup - Win32 Release" && "$(CFG)" !=\
 "RealPopup - Win32 Debug" && "$(CFG)" != "RealPopup - Win32 Release MFCSTATIC"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RealPopup.mak" CFG="RealPopup - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RealPopup - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RealPopup - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "RealPopup - Win32 Release MFCSTATIC" (based on\
 "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "RealPopup - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\RealPopup.exe"

!ELSE 

ALL : "$(OUTDIR)\RealPopup.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\dlgbars.obj"
	-@erase "$(INTDIR)\MsgFile.obj"
	-@erase "$(INTDIR)\MsgPopup.obj"
	-@erase "$(INTDIR)\MsgRealPopup.obj"
	-@erase "$(INTDIR)\RealPopup.obj"
	-@erase "$(INTDIR)\RealPopup.pch"
	-@erase "$(INTDIR)\RealPopup.res"
	-@erase "$(INTDIR)\RealPopupDlg.obj"
	-@erase "$(INTDIR)\RPMsgEditCtrl.obj"
	-@erase "$(INTDIR)\RPMsgHandler.obj"
	-@erase "$(INTDIR)\SendDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TbStDialog.obj"
	-@erase "$(INTDIR)\TrayIcon.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\RealPopup.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /Fp"$(INTDIR)\RealPopup.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x410 /fo"$(INTDIR)\RealPopup.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RealPopup.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=mpr.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\RealPopup.pdb" /machine:I386 /out:"$(OUTDIR)\RealPopup.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dlgbars.obj" \
	"$(INTDIR)\MsgFile.obj" \
	"$(INTDIR)\MsgPopup.obj" \
	"$(INTDIR)\MsgRealPopup.obj" \
	"$(INTDIR)\RealPopup.obj" \
	"$(INTDIR)\RealPopup.res" \
	"$(INTDIR)\RealPopupDlg.obj" \
	"$(INTDIR)\RPMsgEditCtrl.obj" \
	"$(INTDIR)\RPMsgHandler.obj" \
	"$(INTDIR)\SendDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TbStDialog.obj" \
	"$(INTDIR)\TrayIcon.obj"

"$(OUTDIR)\RealPopup.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\RealPopup.exe" "$(OUTDIR)\RealPopup.bsc"

!ELSE 

ALL : "$(OUTDIR)\RealPopup.exe" "$(OUTDIR)\RealPopup.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\dlgbars.obj"
	-@erase "$(INTDIR)\dlgbars.sbr"
	-@erase "$(INTDIR)\MsgFile.obj"
	-@erase "$(INTDIR)\MsgFile.sbr"
	-@erase "$(INTDIR)\MsgPopup.obj"
	-@erase "$(INTDIR)\MsgPopup.sbr"
	-@erase "$(INTDIR)\MsgRealPopup.obj"
	-@erase "$(INTDIR)\MsgRealPopup.sbr"
	-@erase "$(INTDIR)\RealPopup.obj"
	-@erase "$(INTDIR)\RealPopup.pch"
	-@erase "$(INTDIR)\RealPopup.res"
	-@erase "$(INTDIR)\RealPopup.sbr"
	-@erase "$(INTDIR)\RealPopupDlg.obj"
	-@erase "$(INTDIR)\RealPopupDlg.sbr"
	-@erase "$(INTDIR)\RPMsgEditCtrl.obj"
	-@erase "$(INTDIR)\RPMsgEditCtrl.sbr"
	-@erase "$(INTDIR)\RPMsgHandler.obj"
	-@erase "$(INTDIR)\RPMsgHandler.sbr"
	-@erase "$(INTDIR)\SendDlg.obj"
	-@erase "$(INTDIR)\SendDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TbStDialog.obj"
	-@erase "$(INTDIR)\TbStDialog.sbr"
	-@erase "$(INTDIR)\TrayIcon.obj"
	-@erase "$(INTDIR)\TrayIcon.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\RealPopup.bsc"
	-@erase "$(OUTDIR)\RealPopup.exe"
	-@erase "$(OUTDIR)\RealPopup.ilk"
	-@erase "$(OUTDIR)\RealPopup.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RealPopup.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x410 /fo"$(INTDIR)\RealPopup.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RealPopup.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dlgbars.sbr" \
	"$(INTDIR)\MsgFile.sbr" \
	"$(INTDIR)\MsgPopup.sbr" \
	"$(INTDIR)\MsgRealPopup.sbr" \
	"$(INTDIR)\RealPopup.sbr" \
	"$(INTDIR)\RealPopupDlg.sbr" \
	"$(INTDIR)\RPMsgEditCtrl.sbr" \
	"$(INTDIR)\RPMsgHandler.sbr" \
	"$(INTDIR)\SendDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TbStDialog.sbr" \
	"$(INTDIR)\TrayIcon.sbr"

"$(OUTDIR)\RealPopup.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=mpr.lib netapi32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\RealPopup.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\RealPopup.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\dlgbars.obj" \
	"$(INTDIR)\MsgFile.obj" \
	"$(INTDIR)\MsgPopup.obj" \
	"$(INTDIR)\MsgRealPopup.obj" \
	"$(INTDIR)\RealPopup.obj" \
	"$(INTDIR)\RealPopup.res" \
	"$(INTDIR)\RealPopupDlg.obj" \
	"$(INTDIR)\RPMsgEditCtrl.obj" \
	"$(INTDIR)\RPMsgHandler.obj" \
	"$(INTDIR)\SendDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TbStDialog.obj" \
	"$(INTDIR)\TrayIcon.obj"

"$(OUTDIR)\RealPopup.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

OUTDIR=.\ReleaseMFC
INTDIR=.\ReleaseMFC
# Begin Custom Macros
OutDir=.\ReleaseMFC
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\RealPopupMFC.exe"

!ELSE 

ALL : "$(OUTDIR)\RealPopupMFC.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\dlgbars.obj"
	-@erase "$(INTDIR)\MsgFile.obj"
	-@erase "$(INTDIR)\MsgPopup.obj"
	-@erase "$(INTDIR)\MsgRealPopup.obj"
	-@erase "$(INTDIR)\RealPopup.obj"
	-@erase "$(INTDIR)\RealPopup.pch"
	-@erase "$(INTDIR)\RealPopup.res"
	-@erase "$(INTDIR)\RealPopupDlg.obj"
	-@erase "$(INTDIR)\RPMsgEditCtrl.obj"
	-@erase "$(INTDIR)\RPMsgHandler.obj"
	-@erase "$(INTDIR)\SendDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TbStDialog.obj"
	-@erase "$(INTDIR)\TrayIcon.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\RealPopupMFC.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\RealPopup.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\ReleaseMFC/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x410 /fo"$(INTDIR)\RealPopup.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RealPopup.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=mpr.lib netapi32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\RealPopupMFC.pdb" /machine:I386\
 /out:"$(OUTDIR)\RealPopupMFC.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dlgbars.obj" \
	"$(INTDIR)\MsgFile.obj" \
	"$(INTDIR)\MsgPopup.obj" \
	"$(INTDIR)\MsgRealPopup.obj" \
	"$(INTDIR)\RealPopup.obj" \
	"$(INTDIR)\RealPopup.res" \
	"$(INTDIR)\RealPopupDlg.obj" \
	"$(INTDIR)\RPMsgEditCtrl.obj" \
	"$(INTDIR)\RPMsgHandler.obj" \
	"$(INTDIR)\SendDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TbStDialog.obj" \
	"$(INTDIR)\TrayIcon.obj"

"$(OUTDIR)\RealPopupMFC.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "RealPopup - Win32 Release" || "$(CFG)" ==\
 "RealPopup - Win32 Debug" || "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"
SOURCE=.\dlgbars.cpp
DEP_CPP_DLGBA=\
	".\dlgbars.h"\
	

!IF  "$(CFG)" == "RealPopup - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /Fp"$(INTDIR)\RealPopup.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\dlgbars.obj" : $(SOURCE) $(DEP_CPP_DLGBA) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RealPopup.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\dlgbars.obj"	"$(INTDIR)\dlgbars.sbr" : $(SOURCE) $(DEP_CPP_DLGBA)\
 "$(INTDIR)" "$(INTDIR)\RealPopup.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

CPP_SWITCHES=/nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\RealPopup.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 

"$(INTDIR)\dlgbars.obj" : $(SOURCE) $(DEP_CPP_DLGBA) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MsgFile.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

DEP_CPP_MSGFI=\
	".\MsgFile.h"\
	".\RealPopup.h"\
	

"$(INTDIR)\MsgFile.obj" : $(SOURCE) $(DEP_CPP_MSGFI) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

DEP_CPP_MSGFI=\
	".\MsgFile.h"\
	".\RealPopup.h"\
	

"$(INTDIR)\MsgFile.obj"	"$(INTDIR)\MsgFile.sbr" : $(SOURCE) $(DEP_CPP_MSGFI)\
 "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

DEP_CPP_MSGFI=\
	".\MsgFile.h"\
	".\RealPopup.h"\
	

"$(INTDIR)\MsgFile.obj" : $(SOURCE) $(DEP_CPP_MSGFI) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 

SOURCE=.\MsgPopup.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

DEP_CPP_MSGPO=\
	".\MsgPopup.h"\
	".\RealPopup.h"\
	

"$(INTDIR)\MsgPopup.obj" : $(SOURCE) $(DEP_CPP_MSGPO) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

DEP_CPP_MSGPO=\
	".\MsgPopup.h"\
	".\RealPopup.h"\
	

"$(INTDIR)\MsgPopup.obj"	"$(INTDIR)\MsgPopup.sbr" : $(SOURCE) $(DEP_CPP_MSGPO)\
 "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

DEP_CPP_MSGPO=\
	".\MsgPopup.h"\
	".\RealPopup.h"\
	

"$(INTDIR)\MsgPopup.obj" : $(SOURCE) $(DEP_CPP_MSGPO) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 

SOURCE=.\MsgRealPopup.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

DEP_CPP_MSGRE=\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	

"$(INTDIR)\MsgRealPopup.obj" : $(SOURCE) $(DEP_CPP_MSGRE) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

DEP_CPP_MSGRE=\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	

"$(INTDIR)\MsgRealPopup.obj"	"$(INTDIR)\MsgRealPopup.sbr" : $(SOURCE)\
 $(DEP_CPP_MSGRE) "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

DEP_CPP_MSGRE=\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MsgRealPopup.obj" : $(SOURCE) $(DEP_CPP_MSGRE) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 

SOURCE=.\RealPopup.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

DEP_CPP_REALP=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	".\RealPopupDlg.h"\
	".\RPMsgEditCtrl.h"\
	".\RPMsgHandler.h"\
	".\SendDlg.h"\
	".\TbStDialog.h"\
	".\TrayIcon.h"\
	

"$(INTDIR)\RealPopup.obj" : $(SOURCE) $(DEP_CPP_REALP) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

DEP_CPP_REALP=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	".\RealPopupDlg.h"\
	".\RPMsgEditCtrl.h"\
	".\RPMsgHandler.h"\
	".\SendDlg.h"\
	".\TbStDialog.h"\
	".\TrayIcon.h"\
	

"$(INTDIR)\RealPopup.obj"	"$(INTDIR)\RealPopup.sbr" : $(SOURCE)\
 $(DEP_CPP_REALP) "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

DEP_CPP_REALP=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\RealPopup.h"\
	".\RealPopupDlg.h"\
	".\RPMsgEditCtrl.h"\
	".\RPMsgHandler.h"\
	".\SendDlg.h"\
	".\TbStDialog.h"\
	".\TrayIcon.h"\
	

"$(INTDIR)\RealPopup.obj" : $(SOURCE) $(DEP_CPP_REALP) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 

SOURCE=.\RealPopup.rc
DEP_RSC_REALPO=\
	".\res\Lgeup.cur"\
	".\res\RealPopup.ico"\
	".\res\RealPopup.rc2"\
	".\res\toolbar1.bmp"\
	".\res\toolbar2.bmp"\
	".\res\trayicon.ico"\
	

"$(INTDIR)\RealPopup.res" : $(SOURCE) $(DEP_RSC_REALPO) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\RealPopupDlg.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

DEP_CPP_REALPOP=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	".\RealPopupDlg.h"\
	".\RPMsgEditCtrl.h"\
	".\RPMsgHandler.h"\
	".\SendDlg.h"\
	".\TbStDialog.h"\
	".\TrayIcon.h"\
	

"$(INTDIR)\RealPopupDlg.obj" : $(SOURCE) $(DEP_CPP_REALPOP) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

DEP_CPP_REALPOP=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	".\RealPopupDlg.h"\
	".\RPMsgEditCtrl.h"\
	".\RPMsgHandler.h"\
	".\SendDlg.h"\
	".\TbStDialog.h"\
	".\TrayIcon.h"\
	

"$(INTDIR)\RealPopupDlg.obj"	"$(INTDIR)\RealPopupDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_REALPOP) "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

DEP_CPP_REALPOP=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\RealPopup.h"\
	".\RealPopupDlg.h"\
	".\RPMsgEditCtrl.h"\
	".\RPMsgHandler.h"\
	".\SendDlg.h"\
	".\TbStDialog.h"\
	".\TrayIcon.h"\
	

"$(INTDIR)\RealPopupDlg.obj" : $(SOURCE) $(DEP_CPP_REALPOP) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 

SOURCE=.\RPMsgEditCtrl.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

DEP_CPP_RPMSG=\
	".\RealPopup.h"\
	".\RPMsgEditCtrl.h"\
	

"$(INTDIR)\RPMsgEditCtrl.obj" : $(SOURCE) $(DEP_CPP_RPMSG) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

DEP_CPP_RPMSG=\
	".\RealPopup.h"\
	".\RPMsgEditCtrl.h"\
	

"$(INTDIR)\RPMsgEditCtrl.obj"	"$(INTDIR)\RPMsgEditCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_RPMSG) "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

DEP_CPP_RPMSG=\
	".\RealPopup.h"\
	".\RPMsgEditCtrl.h"\
	

"$(INTDIR)\RPMsgEditCtrl.obj" : $(SOURCE) $(DEP_CPP_RPMSG) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 

SOURCE=.\RPMsgHandler.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

DEP_CPP_RPMSGH=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	".\RPMsgHandler.h"\
	

"$(INTDIR)\RPMsgHandler.obj" : $(SOURCE) $(DEP_CPP_RPMSGH) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

DEP_CPP_RPMSGH=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	".\RPMsgHandler.h"\
	

"$(INTDIR)\RPMsgHandler.obj"	"$(INTDIR)\RPMsgHandler.sbr" : $(SOURCE)\
 $(DEP_CPP_RPMSGH) "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

DEP_CPP_RPMSGH=\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\RealPopup.h"\
	".\RPMsgHandler.h"\
	

"$(INTDIR)\RPMsgHandler.obj" : $(SOURCE) $(DEP_CPP_RPMSGH) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 

SOURCE=.\SendDlg.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

DEP_CPP_SENDD=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	".\RPMsgEditCtrl.h"\
	".\RPMsgHandler.h"\
	".\SendDlg.h"\
	".\TbStDialog.h"\
	

"$(INTDIR)\SendDlg.obj" : $(SOURCE) $(DEP_CPP_SENDD) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

DEP_CPP_SENDD=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\MsgRealPopup.h"\
	".\RealPopup.h"\
	".\RPMsgEditCtrl.h"\
	".\RPMsgHandler.h"\
	".\SendDlg.h"\
	".\TbStDialog.h"\
	

"$(INTDIR)\SendDlg.obj"	"$(INTDIR)\SendDlg.sbr" : $(SOURCE) $(DEP_CPP_SENDD)\
 "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

DEP_CPP_SENDD=\
	".\dlgbars.h"\
	".\MsgFile.h"\
	".\MsgPopup.h"\
	".\RealPopup.h"\
	".\RPMsgEditCtrl.h"\
	".\RPMsgHandler.h"\
	".\SendDlg.h"\
	".\TbStDialog.h"\
	

"$(INTDIR)\SendDlg.obj" : $(SOURCE) $(DEP_CPP_SENDD) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "RealPopup - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /Fp"$(INTDIR)\RealPopup.pch" /Yc"STDAFX.H" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\RealPopup.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RealPopup.pch"\
 /Yc"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\RealPopup.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

CPP_SWITCHES=/nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\RealPopup.pch" /Yc"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\RealPopup.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TbStDialog.cpp

!IF  "$(CFG)" == "RealPopup - Win32 Release"

DEP_CPP_TBSTD=\
	".\dlgbars.h"\
	".\RealPopup.h"\
	".\TbStDialog.h"\
	

"$(INTDIR)\TbStDialog.obj" : $(SOURCE) $(DEP_CPP_TBSTD) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"

DEP_CPP_TBSTD=\
	".\dlgbars.h"\
	".\RealPopup.h"\
	".\TbStDialog.h"\
	

"$(INTDIR)\TbStDialog.obj"	"$(INTDIR)\TbStDialog.sbr" : $(SOURCE)\
 $(DEP_CPP_TBSTD) "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"

DEP_CPP_TBSTD=\
	".\dlgbars.h"\
	".\RealPopup.h"\
	".\TbStDialog.h"\
	

"$(INTDIR)\TbStDialog.obj" : $(SOURCE) $(DEP_CPP_TBSTD) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 

SOURCE=.\TrayIcon.cpp
DEP_CPP_TRAYI=\
	".\TrayIcon.h"\
	

!IF  "$(CFG)" == "RealPopup - Win32 Release"


"$(INTDIR)\TrayIcon.obj" : $(SOURCE) $(DEP_CPP_TRAYI) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Debug"


"$(INTDIR)\TrayIcon.obj"	"$(INTDIR)\TrayIcon.sbr" : $(SOURCE) $(DEP_CPP_TRAYI)\
 "$(INTDIR)" "$(INTDIR)\RealPopup.pch"


!ELSEIF  "$(CFG)" == "RealPopup - Win32 Release MFCSTATIC"


"$(INTDIR)\TrayIcon.obj" : $(SOURCE) $(DEP_CPP_TRAYI) "$(INTDIR)"\
 "$(INTDIR)\RealPopup.pch"


!ENDIF 


!ENDIF 

