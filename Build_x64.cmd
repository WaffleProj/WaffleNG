@echo	off
set	mingw=D:\mingw64\x86_64-w64-mingw32
set	libgcc=D:\mingw64\lib\gcc\x86_64-w64-mingw32\4.8.0\libgcc.a
set	path=%mingw%\..\bin;%windir%\System32
set	include=%mingw%\..\include;%mingw%\include
set	lib=%mingw%\..\lib;%mingw%\lib

md	Release 2>nul
md	Release\x86 2>nul
md	Release\x64 2>nul

goto	noloader
echo	===============================================================================
echo	Build	loader.exe
echo	===============================================================================
cd	loader.exe
taskkill /f /im loader.exe >nul 2>nul

echo	gcc	loader.c
gcc	-c	loader.c
echo	gcc	Source\CmdLine.c
gcc	-c	Source\CmdLine.c
echo	gcc	Source\PECheck.c
gcc	-c	Source\PECheck.c
echo	gcc	Source\DllInject.c
gcc	-c	Source\DllInject.c

echo	ld
ld	--subsystem console -L%mingw%\lib -e Main -o ..\Release\x64\loader.exe loader.o CmdLine.o PECheck.o DllInject.o %libgcc% -lkernel32 -luser32 -lcomdlg32 -lmsvcrt
del	*.o 2>nul
cd..
:noloader

::goto nocore
echo	===============================================================================
echo	Build	core.dll
echo	===============================================================================
cd	core.dll

echo	gcc	core.c
gcc	-c	core.c
echo	gcc	global.c
gcc	-c	global.c
echo	gcc	Source\membp.c
gcc	-c	Source\membp.c
echo	gcc	Source\kernel32.c
gcc	-c	Source\kernel32.c
echo	gcc	Source\user32.c
gcc	-c	Source\user32.c
echo	gcc	Source\gdi32.c
gcc	-c	Source\gdi32.c
echo	gcc	Source\psapi.c
gcc	-c	Source\psapi.c

echo	ld
ld	--subsystem windows --dll -L%mingw%\lib -e DllMain -o ..\Release\x64\core.dll global.o core.o membp.o kernel32.o user32.o gdi32.o psapi.o -lkernel32 -luser32 -lgdi32 -lpsapi -lshlwapi
del	*.o 2>nul
cd	..
:nocore