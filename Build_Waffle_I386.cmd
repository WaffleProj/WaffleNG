@echo	off
set	mingw=D:\mingw32\i686-w64-mingw32
set	wafflesdk=E:\Project\Mojibake\SDK
set	path=%mingw%\..\bin;%windir%\System32
set	include=%mingw%\..\include;%mingw%\include;%wafflesdk%\include
set	C_INCLUDE_PATH=%include%
set	lib=D:\SDK\v7.1\Lib;%mingw%\..\lib;%mingw%\lib;%wafflesdk%\lib\x86
set	LIBRARY_PATH=%lib%

md	Release 2>nul
md	Release\Component\Waffle 2>nul
md	Release\Component\Waffle\AMD64 2>nul
md	Release\Component\Waffle\Config 2>nul
md	Release\Component\Waffle\I386 2>nul

::goto	nowaffle
echo	===============================================================================
echo	Build	waffle.exe
echo	===============================================================================
cd	waffle.exe
taskkill /f /im waffle.exe >nul 2>nul

echo	gcc	waffle.c
gcc	-c -Wall -Wextra waffle.c
echo	gcc	src\cmdarg.c
gcc	-c -Wall -Wextra src\cmdarg.c
echo	gcc	src\pemachine.c
gcc	-c -Wall -Wextra src\pemachine.c

echo	ld
ld	--subsystem windows -L%mingw%\lib --enable-stdcall-fixup -e _Main -o ..\Release\waffle_i386.exe waffle.o cmdarg.o pemachine.o -lkernel32 -luser32 -lcomdlg32
del	*.o 2>nul
cd..
:nowaffle

::goto	noloader
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

echo	ld
ld	--subsystem windows -L%mingw%\lib --enable-stdcall-fixup -e _Main -o ..\Release\Component\Waffle\I386\loader.exe loader.o cmdarg.o inject.o -lkernel32 -luser32 -lcomdlg32 -lmsvcrt
del	*.o 2>nul
cd..
:noloader

::goto nowaffledll
echo	===============================================================================
echo	Build	waffle.dll
echo	===============================================================================
cd	waffle.dll

echo	gcc	waffle.c
gcc	-c -Wall -Wextra waffle.c
echo	gcc	detour.c
gcc	-c -Wall -Wextra detour.c
echo	gcc	init.c
gcc	-c -Wall -Wextra init.c
echo	gcc	src\membp\membp.c
gcc	-c -Wall -Wextra src\membp\membp.c

echo	ld
ld	--subsystem windows --dll -L%mingw%\lib --enable-stdcall-fixup --kill-at -e _DllMain -o ..\Release\Component\Waffle\I386\waffle.dll waffle.o detour.o init.o membp.o -lkernel32 -luser32 -lgdi32 -lpsapi -lshlwapi -lmingwex
del	*.o 2>nul
cd	..
:nowaffledll