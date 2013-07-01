@echo	off
set	Project=Waffle
call	Common	StartUp	%1
call	Common	CreateDirectory

::goto nowaffle
call	Common	ChangeDirectory	waffle.exe
call	Common	Compile	waffle.c
call	Common	Compile	src\cmdarg.c
call	Common	Compile	src\pemachine.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows -L%MinGW%\lib --enable-stdcall-fixup -e _Main -o ..\Release\%Project%_%Machine%.exe waffle.o cmdarg.o pemachine.o -lkernel32 -luser32 -lcomdlg32
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows -L%MinGW%\lib --enable-stdcall-fixup -e  Main -o ..\Release\%Project%_%Machine%.exe waffle.o cmdarg.o pemachine.o -lkernel32 -luser32 -lcomdlg32
	)
call	Common	CleanUp
:nowaffleexe

::goto	noloader
call	Common	ChangeDirectory	loader.exe
call	Common	Compile	loader.c
call	Common	Compile	src\cmdarg.c
call	Common	Compile	src\inject.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows -L%MinGW%\lib --enable-stdcall-fixup -e _Main -o ..\Release\Component\%Project%\%Machine%\loader.exe loader.o cmdarg.o inject.o -lkernel32 -luser32 -lcomdlg32 -lmsvcrt
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows -L%MinGW%\lib --enable-stdcall-fixup -e  Main -o ..\Release\Component\%Project%\%Machine%\loader.exe loader.o cmdarg.o inject.o -lkernel32 -luser32 -lcomdlg32 -lmsvcrt
	)
call	Common	CleanUp
:noloaderexe

::goto	nowaffledll
call	Common	ChangeDirectory	waffle.dll
call	Common	Compile	waffle.c
call	Common	Compile	detour.c
call	Common	Compile	init.c
call	Common	Compile	src\membp\membp.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e _DllMain -o ..\Release\Component\%Project%\%Machine%\waffle.dll waffle.o detour.o init.o membp.o -lkernel32 -luser32 -lgdi32 -lpsapi -lshlwapi -lmingwex
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e DllMain -o ..\Release\Component\%Project%\%Machine%\waffle.dll waffle.o detour.o init.o membp.o -lkernel32 -luser32 -lgdi32 -lpsapi -lshlwapi -lmingwex
	)
call	Common	CleanUp
:nowaffledll

::goto	noloaderdll
call	Common	ChangeDirectory	loader.dll
call	Common	Compile	loader.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e _DllMain -o ..\Release\Component\%Project%\%Machine%\loader.dll loader.o -lkernel32 -luser32 -lshlwapi
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e DllMain -o ..\Release\Component\%Project%\%Machine%\loader.dll loader.o -lkernel32 -luser32 -lshlwapi
	)
call	Common	CleanUp
:noloaderdll