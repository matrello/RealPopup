# Microsoft Developer Studio Generated NMAKE File, Based on RPres_dan.dsp
!IF "$(CFG)" == ""
CFG=RPres_dan - Win32 Release
!MESSAGE No configuration specified. Defaulting to RPres_dan - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "RPres_dan - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RPres_dan.mak" CFG="RPres_dan - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RPres_dan - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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

ALL : "$(OUTDIR)\RPres_dan.dll"


CLEAN :
	-@erase "$(INTDIR)\RPres_dan.res"
	-@erase "$(OUTDIR)\RPres_dan.dll"
	-@erase "$(OUTDIR)\RPres_dan.exp"
	-@erase "$(OUTDIR)\RPres_dan.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RPres_dan.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:no /pdb:"$(OUTDIR)\RPres_dan.pdb" /machine:I386 /out:"$(OUTDIR)\RPres_dan.dll" /implib:"$(OUTDIR)\RPres_dan.lib" /NOENTRY 
LINK32_OBJS= \
	"$(INTDIR)\RPres_dan.res"

"$(OUTDIR)\RPres_dan.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RPRES_dan_EXPORTS" /Fp"$(INTDIR)\RPres_dan.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x406 /c 1252 /fo"$(INTDIR)\RPres_dan.res" /d "NDEBUG" 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("RPres_dan.dep")
!INCLUDE "RPres_dan.dep"
!ELSE 
!MESSAGE Warning: cannot find "RPres_dan.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "RPres_dan - Win32 Release"
SOURCE=.\RPres_dan.rc

"$(INTDIR)\RPres_dan.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

