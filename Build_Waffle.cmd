@echo	off
set	Project=Waffle
call	Common	StartUp	%1

::goto	nocommondll
call	Common	ChangeDirectory	%Project%.common.1.0.dll
call	Common	Compile	common.c
call	Common	Compile	src\cmdarg.c
call	Common	Compile	src\pemachine.c
call	Common	Compile	src\inject.c
call	Common	Compile	src\filesystem.c
call	Common	Compile	src\setting.c
call	Common	Compile	src\init.c
call	Common	Compile	src\hash.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e _DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.common.1.0.dll common.o cmdarg.o pemachine.o inject.o filesystem.o setting.o init.o hash.o -lmingwex -lkernel32 -luser32 -ladvapi32
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e  DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.common.1.0.dll common.o cmdarg.o pemachine.o inject.o filesystem.o setting.o init.o hash.o -lmingwex -lkernel32 -luser32 -ladvapi32
	)
call	Common	CleanUp
:nocommondll

::goto nowaffle
call	Common	ChangeDirectory	%Project%.exe
call	Common	Compile	waffle.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -e _Main -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.exe waffle.o -lWaffle.common.1.0 -lmingwex -lmsvcrt %libgcc% -lkernel32 -luser32 -lcomdlg32
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -e  Main -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.exe waffle.o -lWaffle.common.1.0 -lmingwex -lmsvcrt %libgcc% -lkernel32 -luser32 -lcomdlg32
	)
call	Common	CleanUp
:nowaffleexe

popd