@echo off
set masm=D:\MASM32
set sdk=D:\MicrosoftSDKs\Windows\v7.1
set wininc=D:\JWasm\WinInc
set japheth=D:\JWasm

set include=%wininc%\Include;%CD%\Lib;%CD%\Src
set lib=%sdk%\Lib;%wininc%\Lib
set path=%japheth%\Bin;%masm%\Bin;%PATH%
::===========================================================
ml /Zi /Zd /nologo /c /Cp /coff loader.asm
::jwasm /Zi /Zd /nologo /c /Cp /coff loader.asm
link /DEBUG /DEBUGTYPE:CV /nologo /subsystem:sonsole /out:Bin\loader.exe loader.obj
::jwlink OPTION quiet,cvpack FORMAT window pe RUNTIME windows NAME Bin\loader.exe DEBUG codeview FILE loader.obj
if not ERRORLEVEL 1 goto _Success
@pause
:_Success	
del loader.obj