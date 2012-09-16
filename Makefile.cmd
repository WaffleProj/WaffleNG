::ShellEntry:=\
@goto	InitShell

VPATH = Src:Lib

asmfile := $(subst /,\,$(wildcard *.asm Lib/*.asm Src/*.asm Header/*.asm))
cfile	:= $(subst /,\,$(wildcard *.c Lib/*.c Src/*.c Header/*.c))
cppfile := $(subst /,\,$(wildcard *.cpp Lib/*.cpp Src/*.cpp Header/*.cpp))
rcfile	:= $(subst /,\,$(wildcard Resource/*.rc))

asmobj	:= $(subst Src\,,$(subst Lib\,,$(asmfile:.asm=.asmobj)))
cobj	:= $(subst Src\,,$(subst Lib\,,$(cfile:.c=.cobj)))
cppobj	:= $(subst Src\,,$(subst Lib\,,$(cppfile:.cpp=.cppobj)))
rcres	:= $(rcfile:.rc=.res)

allobj	:= $(asmobj) $(cobj) $(cppobj)
fullpathobj := $(foreach object,$(allobj),$(project_root)\Bin\$(target)Obj\$(object))
linkobj := $(foreach object,$(fullpathobj),FILE $(object))
linkres := RESOURCE $(project_root)\$(rcres)
#=================================================================================================
Build:$(project_name).$(BinExtName)
$(project_name).$(BinExtName):$(project_name).obj
	jwlink @JWLINK FILE $(project_root)\Bin\$(target)Obj\$(project_name).obj
$(project_name).obj:$(asmfile)
	jwasm	/Fo $(project_root)\Bin\$(target)Obj\$(project_name).obj $(project_name).asm
#=================================================================================================
%.res:Resource\%.rc
	rc	$(RC) $^
#=================================================================================================
.IGNORE:Clean
Clean:
	del	$(foreach folder,$(if $(CD),$(CD)\,)$(subst /,\,$(wildcard Bin/*Obj)),$(folder)\*.*)
	echo	Done.
#=================================================================================================
Help:
	$(FullPath) :invoke ShowHelp
#=================================================================================================
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:ShellMain
	title	Advanced Makefile v0.10
	call	:CheckParameter %*
	if not	%ERRORLEVEL%==0 goto ExitFunction
	call	:AnalysisPath %MainFile%
	if not	%ERRORLEVEL%==0 goto ExitFunction
	set	project_name=%FileName%
	set	project_root=%FileDir%
	call	:SetEnvironment
	call	:CheckProject
	if not	%ERRORLEVEL%==0 goto ExitFunction
	if not "%res%"=="" mingw32-make -j %NUMBER_OF_PROCESSORS% -s -f %FullPath% %project_name%.res
	mingw32-make -j %NUMBER_OF_PROCESSORS% -s -f %FullPath%
	if not ERRORLEVEL 1 goto _Success
:_Err
	pause
	goto	ExitFunction
:_Success	
	call :BuildCountInc %FileDir%\BUILD
	echo	Finish.
	call	:Sleep 5000
	goto	ExitFunction
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
::用户子程序
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:CheckParameter
	if	"%1" == ":invoke"	(
		call	:%2 %3 %4 %5 %6 %7 %8 %9
		set	ERRORLEVEL=1
		goto	ExitFunction
		)

	if	"%1" == ""	(
		call	:ShowHelp
		call	:Sleep 2000
		set	ERRORLEVEL=1
		goto	ExitFunction
	) else (
		set	MainFile=%1
		)
		
	if	"%2" == ""	(
		set	target=x86
	) else (
		set	target=%2
		)
		
	if	"%3" == ""	(
		set	output=windows
	) else (
		set	output=%3
		)
		
	if	"%4" == ""	(
		set	debug=off
	) else (
		set	debug=%4
		)

	if not	"%5" == ""	(
		call	:ShowHelp
		set	ERRORLEVEL=1
		goto	ExitFunction
		)
	
	set	ERRORLEVEL=0
	goto	ExitFunction
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:SetEnvironment
	set	masm32=D:\masm32
	::被删除了
	set	japheth=D:\JWasm
	set	vc=D:\Program Files (x86)\Microsoft Visual Studio 11.0
	set	mingw32=D:\MinGW
	::等mingw-w64有一个比较简单的安装程序后再说吧
	set	sdk7=D:\MicrosoftSDKs\Windows\v7.1

	set	proj_include=%FileDir%;%FileDir%\Lib;%FileDir%\Src;%FileDir%\Header
	set	proj_lib=%FileDir%;%FileDir%\Lib;%FileDir%\Src;%FileDir%\Header
	
	set	DebugML=
	set	DebugLink=
	set	ExtraLink=
	if	"%target%" == "x86"	(
		if	"%debug%" == "on" set DebugML=/Zi
		if	"%debug%" == "on" set DebugLink=/DEBUG /DEBUGTYPE:CV
		if	"%output%" == "dll" set ExtraLink=/Dll
		if	"%output%" == "native" set ExtraLink=/driver /base:0x10000 /align:32
		set	BinExtName=exe
		if	"%output%" == "dll" set BinExtName=dll
		if	"%output%" == "native" set BinExtName=sys
		set	SubSystem=windows
		if	"%output%" == "console" set SubSystem=console
		if	"%output%" == "native" set SubSystem=native
	) else (
		if	"%debug%" == "on" set DebugML=/Zi
		if	"%debug%" == "on" set DebugLink=/DEBUG /DEBUGTYPE:CV
		if	"%output%" == "dll" set ExtraLink=/Dll
		if	"%output%" == "native" set ExtraLink=/driver /base:0x10000 /align:32
		set	BinExtName=exe
		if	"%output%" == "dll" set BinExtName=dll
		if	"%output%" == "native" set BinExtName=sys
		set	SubSystem=windows
		if	"%output%" == "console" set SubSystem=console
		if	"%output%" == "native" set SubSystem=native
		)

	
	set	ML=%DebugML% /nologo /c /Cp /coff
	set	JWASM=%DebugML% /nologo /c /Cp /coff
	set	CL=
	
	set	JWLINK=OPTION quiet FORMAT window pe RUNTIME %SubSystem% NAME %FileDir%\Bin\%FileName%.%BinExtName%
	set	LINK=%DebugLink% %ExtraLink% /nologo /subsystem:%SubSystem% /out:%FileDir%\Bin\%FileName%.%BinExtName%
	
	set	WRC=
	set	RC=/nologo

	goto	_%target%_Environment
:_x86_Environment
	set	include=%proj_include%;%japheth%\WinInc\Include;%japheth%\KmdKit\include;%japheth%\KmdKit\macros;%vc%\VC\include;%mingw32%\include;sdk7%\Include;%include%
	set	lib=%proj_lib%;%sdk7%\Lib;%japheth%\WinInc\Lib;%vc%\VC\Lib;%mingw32%\lib;%lib%
	set	path=%masm32%\bin;%japheth%\Bin;%vc%\VC\Bin;%vc%\Common7\IDE;%mingw32%\bin;%sdk7%\Bin;%path%
	goto	ExitFunction
:_x64_Environment
	set	include=%proj_include%;%japheth%\WinInc\Include;%japheth%\KmdKit\include;%japheth%\KmdKit\macros;%vc%\VC\include;%sdk7%\Include;%include%
	set	lib=%proj_lib%;%sdk7%\Lib\x64;%japheth%\WinInc\Lib64;%vc%\VC\Lib\amd64;%lib%
	set	path=%japheth%\Bin;%vc%\VC\Bin\amd64;%mingw32%\bin;%sdk7%\Bin\x64;%path%
	goto	ExitFunction
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:CheckProject
	if not exist "%FileDir%\%FileName%.%FileExtName%" (
		echo	error 	: cannot open file ^<%FileDir%\%FileName%.%FileExtName%^>
		set	ERRORLEVEL=1
		goto	ExitFunction
		)
	md	%FileDir%\Bin\%target%Obj 2>NUL
	md	%FileDir%\Header 2>NUL
	md	%FileDir%\Lib 2>NUL
	md	%FileDir%\Resource 2>NUL
	md	%FileDir%\Src 2>NUL
	set	res=
	if exist %FileDir%\Resource\%FileName%.res set res=%FileDir\%Resource\%FileName%.res
	if exist %FileDir%\Resource\%FileName%.rc set res=%FileDir\%Resource\%FileName%.res
	if exist %FileDir%\Resource\%FileName%.ico (
		echo	#include "Resource.h" > %FileDir%\Resource\%FileName%.rc
		echo	1000 ICON "%FileName%.ico" >> %FileDir%\Resource\%FileName%.rc
		set	res=%FileDir\%Resource\%FileName%.res
		)

	goto	ExitFunction
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:AnalysisPath
	::初始化变量
	set	PathLength=
	set	PathExtLength=
	set	PathFilenameLength=
	set	PathDirLength=

	set	esi=
	set	edi=
	set	eax=

	set	FileExtName=
	set	FileName=
	set	FileDir=
	
	set	QuotationMark="
	::只在.之后开始判断文件名时期待出现另一个双引号
	::获得参数1长度,字符串从esi倒序排列到edi
	set	esi=%1
	set	eax=%esi:~0,1%
:_PathLength
	if	!eax!==!QuotationMark!	(
		set	eax=%esi:~1,1%
		set	esi=%esi:~1%
		goto	_PathLength
		)
	if not	"%eax%"==""	(
		set /a	PathLength+=1
		set	edi=%eax%%edi%
		set	eax=%esi:~1,1%
		set	esi=%esi:~1%
		goto	_PathLength
		)
	::获得拓展名
	set	eax=%edi:~0,1%
:_PathExtLength
	if	"%eax%"==""	(
		echo	error 	: unrecognized file format
		set	ERRORLEVEL=1
		goto	ExitFunction
		)
	if not	"%eax%"=="."	(
		set /a	PathExtLength+=1
		set	FileExtName=%eax%%FileExtName%
		set	eax=%edi:~1,1%
		set	edi=%edi:~1%
		goto	_PathExtLength
		)
	::获得文件名
	set	edi=%edi:~1%
	if	"%edi%"=="" goto :_FinishAnalysis
	set	eax=%edi:~0,1%
:_PathFilenameLength
	if	"%eax%"==""	(
		set	edi=
		set	eax=
		goto	_RelativePath
		)
	if not	"%eax%"=="\"	(
		if	!eax!==!QuotationMark!	(
			goto	_FinishAnalysis
		) else (
			set /a	PathFilenameLength+=1
			set	FileName=%eax%%FileName%
			set	eax=%edi:~1,1%
			set	edi=%edi:~1%
			goto	_PathFilenameLength
			)
		)
:_RelativePath
	::可能是相对目录,也可能是根目录,还有正常执行的
	set	eax=%edi:~0,1%
	if	"%eax%"==":"	(
		set	edi=\%edi%
		)
	::获得目录
	if	"%edi%"=="" goto :_FinishAnalysis
	set	eax=%edi:~0,1%
:_PathDirLength
	if not	"%eax%"==""	(
		if	!eax!==!QuotationMark!	(
			goto	_FinishAnalysis
		) else (
			set /a	PathDirLength+=1
			set	FileDir=%eax%%FileDir%
			set	eax=%edi:~1,1%
			set	edi=%edi:~1%
			goto	_PathDirLength
			)
		)
:_FinishAnalysis
	if	"%FileDir%"=="" set "FileDir=%CD%"
	
	set	eax=%FileDir:~-1,1%
	if	"%eax%"=="\" set FileDir=%FileDir:~0,-1%
	
	set	eax=%FileDir:~1,1%
	if not	"%eax%"==":" set "FileDir=%CD%\%FileDir%"
	goto	ExitFunction
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:BuildCountInc
	if not	exist %1 echo 0 > %1
	set BuildCountLine=1
	for /f	%%i in (%1) do (
		if !BuildCountLine!==1 (
			set BuildNumber=%%i
			set /a line=%BuildCountLine%+1
			)
		)
	set	BuildCountLine=
	set /a	BuildNumber+=1
	echo	%BuildNumber% > %1
	goto	ExitFunction
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:ShowHelp
	echo	Advanced Makefile v0.10, %LastModified%, Simplify your life.
	echo	Copyright ^(c^) 2012 Excalibur
	echo.
	echo	usage
	echo	^	%ShellName% MainFile ^<target^> ^<output^> ^<debug^>
	echo.
	echo	options
	echo	^	target	: {x86^|x64}			Default=x86
	echo	^	output	: {windows^|console^|dll^|native}	Default=windows
	echo	^	debug	: {on^|off}			Default=off

	goto	ExitFunction
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
::共享子程序
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:InitShell
	::初始化语言环境
	@echo	off
	setlocal EnableDelayedExpansion
	setlocal EnableExtensions
	set	FullPath=%~dpnx0
	set	ShellName=%~nx0
	set	LastModified=%~t0
	::NUMBER_OF_PROCESSORS
	call	:ShellMain %*
	goto	ExitFunction
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:Sleep
	ping /n 1 /w %1 1.0.0.0 > nul
	goto	ExitFunction
::>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
:ExitFunction