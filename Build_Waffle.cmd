@echo	off
set	Project=Waffle
call	Common	StartUp	%1

::goto nowaffle
call	Common	ChangeDirectory	waffle.exe
call	Common	Compile	waffle.c
call	Common	Compile	src\cmdarg.c
call	Common	Compile	src\pemachine.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows -L%MinGW%\lib --enable-stdcall-fixup -e _Main -o %OUTPUT_PATH%\%Project%_%Machine%.exe waffle.o cmdarg.o pemachine.o -lkernel32 -luser32 -lcomdlg32
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows -L%MinGW%\lib --enable-stdcall-fixup -e  Main -o %OUTPUT_PATH%\%Project%_%Machine%.exe waffle.o cmdarg.o pemachine.o -lkernel32 -luser32 -lcomdlg32
	)
call	Common	CleanUp
:nowaffleexe

::goto	noloader
call	Common	ChangeDirectory	loader.exe
call	Common	Compile	loader.c
call	Common	Compile	src\cmdarg.c
call	Common	Compile	src\inject.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows -L%MinGW%\lib --enable-stdcall-fixup -e _Main -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_loader.exe loader.o cmdarg.o inject.o -lkernel32 -luser32 -lcomdlg32 -lmsvcrt
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows -L%MinGW%\lib --enable-stdcall-fixup -e  Main -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_loader.exe loader.o cmdarg.o inject.o -lkernel32 -luser32 -lcomdlg32 -lmsvcrt
	)
call	Common	CleanUp
:noloaderexe

::goto	nocommondll
call	Common	ChangeDirectory	common.dll
call	Common	Compile	common.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e _DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_common.dll --out-implib %OUTPUT_PATH%\SDK\Lib\%Machine%\libWaffle_common.a common.o -lkernel32 -lmingwex
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e  DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_common.dll --out-implib %OUTPUT_PATH%\SDK\Lib\%Machine%\libWaffle_common.a common.o -lkernel32 -lmingwex
	)
call	Common	CleanUp
:nocommondll

::goto	noloaderdll
call	Common	ChangeDirectory	loader.dll
call	Common	Compile	loader.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e _DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_loader.dll loader.o -lkernel32 -luser32 -lshlwapi
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e  DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_loader.dll loader.o -lkernel32 -luser32 -lshlwapi
	)
call	Common	CleanUp
:noloaderdll