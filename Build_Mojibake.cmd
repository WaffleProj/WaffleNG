@echo	off
set	Project=Mojibake
call	Common	StartUp	%1

::goto nocore
call	Common	ChangeDirectory	mojibake.dll
call	Common	Compile	mojibake.c
call	Common	Compile	detour.c
call	Common	Compile	init.c
call	Common	Compile	src\membp\membp.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e _DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_main.dll mojibake.o detour.o init.o membp.o -lkernel32 -luser32 -lgdi32 -lpsapi -lshlwapi -lmingwex
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib --enable-stdcall-fixup -kill-at -e  DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_main.dll mojibake.o detour.o init.o membp.o -lkernel32 -luser32 -lgdi32 -lpsapi -lshlwapi -lmingwex
	)
call	Common	CleanUp
:nocore