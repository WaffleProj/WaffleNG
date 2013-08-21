@echo	off
set	Project=Mojibake
call	Common	StartUp	%1

::goto nocore
call	Common	ChangeDirectory	%Project%.core.1.0.dll
call	Common	Compile	mojibake.c
call	Common	Compile	src\gdi32.c
call	Common	Compile	src\kernel32.c
call	Common	Compile	src\ntdll.c
call	Common	Compile	src\rtl.c
call	Common	Compile	src\subclass.c
call	Common	Compile	src\user32.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e _DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.core.1.0.dll mojibake.o gdi32.o kernel32.o ntdll.o rtl.o subclass.o user32.o -lWaffle.common.1.0 -lkernel32 -luser32 -lgdi32 -lpsapi -ladvapi32
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e  DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.core.1.0.dll mojibake.o gdi32.o kernel32.o ntdll.o rtl.o subclass.o user32.o -lWaffle.common.1.0 -lkernel32 -luser32 -lgdi32 -lpsapi -ladvapi32
	)
call	Common	CleanUp
:nocore

popd