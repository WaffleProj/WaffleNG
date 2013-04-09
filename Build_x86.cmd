@echo	off
set	mingw=D:\mingw32\i686-w64-mingw32
set	libgcc=D:\mingw32\lib\gcc\i686-w64-mingw32\4.8.0\libgcc.a
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
gcc	-c -Wall -Wextra loader.c
echo	gcc	src\cmdarg.c
gcc	-c -Wall -Wextra src\cmdarg.c
echo	gcc	src\inject.c
gcc	-c -Wall -Wextra src\inject.c
echo	gcc	src\pemagic.c
gcc	-c -Wall -Wextra src\pemagic.c

echo	ld
ld	--subsystem console -L%mingw%\lib --enable-stdcall-fixup -e _Main -o ..\Release\x86\loader.exe loader.o cmdarg.o inject.o pemagic.o %libgcc% -lkernel32 -luser32 -lcomdlg32 -lmsvcrt
del	*.o 2>nul
cd..
:noloader

::goto nocore
echo	===============================================================================
echo	Build	core.dll
echo	===============================================================================
cd	core.dll

echo	gcc	core.c
gcc	-c -Wall -Wextra core.c
echo	gcc	global.c
gcc	-c -Wall -Wextra global.c
echo	gcc	init.c
gcc	-c -Wall -Wextra init.c
echo	gcc	src\membp\membp.c
gcc	-c -Wall -Wextra src\membp\membp.c
echo	gcc	src\detour\kernel32.c
gcc	-c -Wall -Wextra src\detour\kernel32.c
echo	gcc	src\detour\user32.c
gcc	-c -Wall -Wextra src\detour\user32.c
echo	gcc	src\detour\gdi32.c
gcc	-c -Wall -Wextra src\detour\\gdi32.c
echo	gcc	src\detour\psapi.c
gcc	-c -Wall -Wextra src\detour\psapi.c

echo	ld
ld	--subsystem windows --dll -L%mingw%\lib --enable-stdcall-fixup -e _DllMain -o ..\Release\x86\core.dll core.o global.o init.o membp.o kernel32.o user32.o gdi32.o psapi.o -lkernel32 -luser32 -lgdi32 -lpsapi -lshlwapi -lmingwex
del	*.o 2>nul
cd	..
:nocore