@echo	off
set	Project=Mojibake
call	Common	StartUp	%1

::goto nocore
call	Common	ChangeDirectory	%Project%.core.1.0.dll
call	Common	Compile	mojibake.c
call	Common	Compile	detour.c
call	Common	Compile	init.c
call	Common	Compile	src\exception\exception.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e _DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.core.1.0.dll mojibake.o detour.o init.o exception.o -lWaffle.common.1.0 -lkernel32 -luser32 -lgdi32 -lpsapi
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e  DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.core.1.0.dll mojibake.o detour.o init.o exception.o -lWaffle.common.1.0 -lkernel32 -luser32 -lgdi32 -lpsapi
	)
call	Common	CleanUp
:nocore

popd