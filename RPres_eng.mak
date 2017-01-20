# Microsoft Developer Studio Generated NMAKE File, Based on RPres_eng.dsp
!IF "$(CFG)" == ""
CFG=RPres_eng - Win32 Release
!MESSAGE No configuration specified. Defaulting to RPres_eng - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "RPres_eng - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RPres_eng.mak" CFG="RPres_eng - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RPres_eng - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
OUTDIR=.\RPres_eng___Win32_Release
INTDIR=.\RPres_eng___Win32_Release

ALL : ".\Release\RPres_eng.dll"


CLEAN :
	-@erase "$(INTDIR)\RealPopup.res"
	-@erase "$(OUTDIR)\RPres_eng.exp"
	-@erase "$(OUTDIR)\RPres_eng.lib"
	-@erase ".\Release\RPres_eng.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RPres_eng.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:no /pdb:"$(OUTDIR)\RPres_eng.pdb" /machine:I386 /out:"Release/RPres_eng.dll" /implib:"$(OUTDIR)\RPres_eng.lib" /NOENTRY 
LINK32_OBJS= \
	"$(INTDIR)\RealPopup.res"

".\Release\RPres_eng.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RPRES_ENG_EXPORTS" /Fp"$(INTDIR)\RPres_eng.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x410 /c 1252 /fo"$(INTDIR)\RealPopup.res" /d "NDEBUG" 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("RPres_eng.dep")
!INCLUDE "RPres_eng.dep"
!ELSE 
!MESSAGE Warning: cannot find "RPres_eng.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "RPres_eng - Win32 Release"
SOURCE=.\RPres_ENG.rc

"$(INTDIR)\RealPopup.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

