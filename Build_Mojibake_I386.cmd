@echo	off
set	mingw=D:\mingw32\i686-w64-mingw32
set	libgcc=D:\mingw32\lib\gcc\i686-w64-mingw32\4.8.1\libgcc.a
set	sdk=E:\Project\Mojibake\SDK
set	path=%mingw%\..\bin;%windir%\System32
set	include=%mingw%\..\include;%mingw%\include;%sdk%\include
set	C_INCLUDE_PATH=%include%
set	lib=%mingw%\..\lib;%mingw%\lib;%sdk%\lib\x86
set	LIBRARY_PATH=%lib%

md	Release 2>nul
md	Release\Component\Mojibake 2>nul
md	Release\Component\Mojibake\AMD64 2>nul
md	Release\Component\Mojibake\Config 2>nul
md	Release\Component\Mojibake\I386 2>nul

::goto nocore
echo	===============================================================================
echo	Build	mojibake.dll
echo	===============================================================================
cd	mojibake.dll

echo	gcc	mojibake.c
gcc	-c -Wall -Wextra mojibake.c
echo	gcc	detour.c
gcc	-c -Wall -Wextra detour.c
echo	gcc	init.c
gcc	-c -Wall -Wextra init.c
echo	gcc	src\membp\membp.c
gcc	-c -Wall -Wextra src\membp\membp.c

echo	ld
ld	--subsystem windows --dll -L%mingw%\lib --enable-stdcall-fixup -e _DllMain -o ..\Release\Component\Mojibake\I386\mojibake.dll mojibake.o detour.o init.o membp.o -lkernel32 -luser32 -lgdi32 -lpsapi -lshlwapi -lmingwex
del	*.o 2>nul
cd	..
:nocore