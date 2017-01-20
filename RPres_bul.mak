# Microsoft Developer Studio Generated NMAKE File, Based on RPres_BUL.dsp
!IF "$(CFG)" == ""
CFG=RPres_BUL - Win32 Release
!MESSAGE No configuration specified. Defaulting to RPres_BUL - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "RPres_BUL - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RPres_BUL.mak" CFG="RPres_BUL - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RPres_BUL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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
OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\RPres_BUL.dll"


CLEAN :
	-@erase "$(INTDIR)\RPres_BUL.res"
	-@erase "$(OUTDIR)\RPres_BUL.dll"
	-@erase "$(OUTDIR)\RPres_BUL.exp"
	-@erase "$(OUTDIR)\RPres_BUL.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RPres_BUL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:no /pdb:"$(OUTDIR)\RPres_BUL.pdb" /machine:I386 /out:"$(OUTDIR)\RPres_BUL.dll" /implib:"$(OUTDIR)\RPres_BUL.lib" /NOENTRY 
LINK32_OBJS= \
	"$(INTDIR)\RPres_BUL.res"

"$(OUTDIR)\RPres_BUL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RPres_BUL_EXPORTS" /Fp"$(INTDIR)\RPres_BUL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x402 /c 1251 /fo"$(INTDIR)\RPres_BUL.res" /d "NDEBUG"

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("RPres_BUL.dep")
!INCLUDE "RPres_BUL.dep"
!ELSE 
!MESSAGE Warning: cannot find "RPres_BUL.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "RPres_BUL - Win32 Release"
SOURCE=.\RPres_BUL.rc

"$(INTDIR)\RPres_BUL.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

