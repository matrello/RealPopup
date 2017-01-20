# Microsoft Developer Studio Generated NMAKE File, Based on RPres_est.dsp
!IF "$(CFG)" == ""
CFG=RPres_est - Win32 Release
!MESSAGE No configuration specified. Defaulting to RPres_est - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "RPres_est - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RPres_est.mak" CFG="RPres_est - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RPres_est - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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

ALL : "$(OUTDIR)\RPres_est.dll"


CLEAN :
	-@erase "$(INTDIR)\RPres_est.res"
	-@erase "$(OUTDIR)\RPres_est.dll"
	-@erase "$(OUTDIR)\RPres_est.exp"
	-@erase "$(OUTDIR)\RPres_est.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RPres_est.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:no /pdb:"$(OUTDIR)\RPres_est.pdb" /machine:I386 /out:"$(OUTDIR)\RPres_est.dll" /implib:"$(OUTDIR)\RPres_est.lib" /NOENTRY 
LINK32_OBJS= \
	"$(INTDIR)\RPres_est.res"

"$(OUTDIR)\RPres_est.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RPres_est_EXPORTS" /Fp"$(INTDIR)\RPres_est.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x425 /c 1257 /fo"$(INTDIR)\RPres_est.res" /d "NDEBUG" 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("RPres_est.dep")
!INCLUDE "RPres_est.dep"
!ELSE 
!MESSAGE Warning: cannot find "RPres_est.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "RPres_est - Win32 Release"
SOURCE=.\RPres_est.rc

"$(INTDIR)\RPres_est.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

