@echo	off
set	Project=Waffle
call	Common	StartUp	%1

::goto	nocommondll
call	Common	ChangeDirectory	%Project%.common.1.0.dll
call	Common	Compile	common.c
call	Common	Compile	src\cmdarg.c
call	Common	Compile	src\componentarray.c
call	Common	Compile	src\filesystem.c
call	Common	Compile	src\functionarray.c
call	Common	Compile	src\hash.c
call	Common	Compile	src\init.c
call	Common	Compile	src\inject.c
call	Common	Compile	src\libraryarray.c
call	Common	Compile	src\logfile.c
call	Common	Compile	src\option.c
call	Common	Compile	src\platform.c
call	Common	Compile	src\setting.c
call	Common	Compile	src\rtl\rtl.c
call	Common	Compile	src\detour\detour.c
call	Common	Compile	src\detour\inline_c.c
call	Common	Compile	src\detour\veh.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e _DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.common.1.0.dll common.o cmdarg.o componentarray.o filesystem.o functionarray.o hash.o init.o inject.o libraryarray.o logfile.o option.o platform.o rtl.o setting.o detour.o inline_c.o veh.o -lkernel32 -luser32 -ladvapi32 -lpsapi
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e  DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.common.1.0.dll common.o cmdarg.o componentarray.o filesystem.o functionarray.o hash.o init.o inject.o libraryarray.o logfile.o option.o platform.o rtl.o setting.o detour.o inline_c.o veh.o -lkernel32 -luser32 -ladvapi32 -lpsapi
	)
call	Common	CleanUp
:nocommondll

::goto nowaffle
call	Common	ChangeDirectory	%Project%.exe
call	Common	Compile	waffle.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e _Main -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.exe waffle.o -lWaffle.common.1.0 -lkernel32 -luser32 -lcomdlg32
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e  Main -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.exe waffle.o -lWaffle.common.1.0 -lkernel32 -luser32 -lcomdlg32
	)
call	Common	CleanUp
:nowaffleexe

popd