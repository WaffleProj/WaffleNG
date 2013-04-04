@echo	off
set	toolchain=D:\JWasm
set	mingw=D:\mingw32\i686-w64-mingw32
set	sdk=D:\SDK\v7.1

set	path=%toolchain%\bin;%mingw%\..\bin;%windir%\System32
set	lib=%toolchain%\lib;%sdk%\lib;%mingw%\lib

md	Release 2>nul
cd	Release
md	x86 2>nul
md	x64 2>nul
cd..

goto	notestcase
cd	testcase.exe
taskkill /f /im testcase.exe >nul 2>nul
set	include=%CD%\Source;%toolchain%\include
jwasm	/nologo /c /Cp /coff /Fo ..\Release\testcase.obj testcase.asm
jwlink	OPTION quiet FORMAT window pe RUNTIME windows NAME ..\Release\testcase.exe FILE ..\Release\testcase.obj
del	*.err 2>nul
del	..\Release\*.obj 2>nul
cd..
:notestcase

goto	noloader
cd	loader.exe
taskkill /f /im loader.exe >nul 2>nul
set	include=%CD%\Source;%toolchain%\include;%mingw%\include
jwasm	/nologo /c /Cp /coff  /Fo ..\Release\loader.obj loader.asm
i686-w64-mingw32-gcc	-c -o ..\Release\loader.o loader.c
i686-w64-mingw32-gcc	-c -o ..\Release\CmdLine.o Source\CmdLine.c
i686-w64-mingw32-gcc	-c -o ..\Release\PECheck.o Source\PECheck.c
i686-w64-mingw32-gcc	-c -o ..\Release\DllInject.o Source\DllInject.c

jwlink	OPTION quiet FORMAT window pe RUNTIME console NAME ..\Release\x86\loader.exe FILE ..\Release\loader.obj,..\Release\loader.o,..\Release\CmdLine.o,..\Release\PECheck.o,..\Release\DllInject.o
del	*.err 2>nul
del	..\Release\*.obj 2>nul
del	..\Release\*.o 2>nul
cd..
:noloader

cd	core.dll
set	include=%CD%\Source;%toolchain%\include;%mingw%\include
jwasm	/nologo /c /Cp /coff /Fo ..\Release\core.obj core.asm
i686-w64-mingw32-gcc	-c -o ..\Release\core.o core.c
i686-w64-mingw32-gcc	-c -o ..\Release\kernel32.o Source\kernel32.c
i686-w64-mingw32-gcc	-c -o ..\Release\user32.o Source\user32.c
i686-w64-mingw32-gcc	-c -o ..\Release\gdi32.o Source\gdi32.c
i686-w64-mingw32-gcc	-c -o ..\Release\psapi.o Source\psapi.c

jwlink	OPTION quiet FORMAT window pe dll RUNTIME windows NAME ..\Release\x86\core.dll FILE ..\Release\core.obj,..\Release\core.o,..\Release\kernel32.o,..\Release\user32.o,..\Release\gdi32.o,..\Release\psapi.o
del	*.err 2>nul
del	..\Release\*.obj 2>nul
del	..\Release\*.o 2>nul
cd	..

pause