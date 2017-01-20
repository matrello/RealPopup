@echo off

rem RealPopup: compila il file mak passato come input
rem versione per vs.net
rem
rem Matro :-)

call "C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"
doskey
nmake %1 /a
copy release\%2 debug\%2
