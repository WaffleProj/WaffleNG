@echo	off
set	Project=Waffle
call	Common	StartUp	%1

::goto	nocommondll
call	Common	ChangeDirectory	common10.dll
call	Common	Compile	common.c
call	Common	Compile	src\cmdarg.c
call	Common	Compile	src\pemachine.c
call	Common	Compile	src\inject.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e _DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_common10.dll common.o cmdarg.o pemachine.o inject.o -lmingwex -lkernel32 -lmingwex
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e  DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_common10.dll common.o cmdarg.o pemachine.o inject.o -lmingwex -lkernel32 -lmingwex
	)
call	Common	CleanUp
:nocommondll

::goto nowaffle
call	Common	ChangeDirectory	waffle.exe
call	Common	Compile	waffle.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -e _Main -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.exe waffle.o -lWaffle_common10 -lmingwex -lmsvcrt %libgcc% -lkernel32 -luser32 -lcomdlg32
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -e  Main -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%.exe waffle.o -lWaffle_common10 -lmingwex -lmsvcrt %libgcc% -lkernel32 -luser32 -lcomdlg32
	)
call	Common	CleanUp
:nowaffleexe

::goto	noloaderdll
call	Common	ChangeDirectory	loader.dll
call	Common	Compile	loader.c
if	"%Machine%" == "I386"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e _DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_loader.dll loader.o -lWaffle_common10 -lmingwex -lkernel32 -luser32 -lshlwapi
	)
if	"%Machine%" == "AMD64"	(
ld	--subsystem windows --dll -L%MinGW%\lib -L%OUTPUT_PATH%\Component\Waffle\%Machine% --enable-stdcall-fixup -kill-at -e  DllMain -o %OUTPUT_PATH%\Component\%Project%\%Machine%\%Project%_loader.dll loader.o -lWaffle_common10 -lmingwex -lkernel32 -luser32 -lshlwapi
	)
call	Common	CleanUp
:noloaderdll

popd