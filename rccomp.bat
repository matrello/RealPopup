@echo off

rem RealPopup: compila il file mak passato come input
rem
rem Matro :-)

rem
rem Root of Visual Developer Studio Common files.
set VSCommonDir=C:\VISUAL~1\COMMON

rem
rem Root of Visual Developer Studio installed files.
rem
set MSDevDir=C:\VISUAL~1\COMMON\msdev98

rem
rem Root of Visual C++ installed files.
rem
set MSVCDir=C:\VISUAL~1\VC98

rem
rem VcOsDir is used to help create either a Windows 95 or Windows NT specific path.
rem
set VcOsDir=WIN95
if "%OS%" == "Windows_NT" set VcOsDir=WINNT

rem
echo Setting environment for using Microsoft Visual C++ tools.
rem

if "%OS%" == "Windows_NT" set PATH=%MSDevDir%\BIN;%MSVCDir%\BIN;%VSCommonDir%\TOOLS\%VcOsDir%;%VSCommonDir%\TOOLS;%PATH%
if "%OS%" == "" set PATH="%MSDevDir%\BIN";"%MSVCDir%\BIN";"%VSCommonDir%\TOOLS\%VcOsDir%";"%VSCommonDir%\TOOLS";"%windir%\SYSTEM";"%PATH%"
set INCLUDE=%MSVCDir%\ATL\INCLUDE;%MSVCDir%\INCLUDE;%MSVCDir%\MFC\INCLUDE;%INCLUDE%
set LIB=%MSVCDir%\LIB;%MSVCDir%\MFC\LIB;%LIB%

set VcOsDir=
set VSCommonDir=

doskey

nmake %1 /a

copy release\%2 debug\%2

