		.386
		.model flat,stdcall
		option casemap:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 头文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
UNICODE		equ	TRUE
_WIN32_WINNT	equ	0501h

include		windows.inc
include		winnt.inc
includelib	kernel32.lib
includelib	user32.lib

include		Unicode.inc
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		
		.data?
uszBuf		dw	MAX_PATH dup (?)

uszTMP		dw	MAX_PATH dup (?)
uszCurrentFolder dw	MAX_PATH dup (?)
uszDllFull	dw	MAX_PATH dup (?)

nArgc		dd	?
nTargetArgc	dd	?
		.const
uszTitle	ustr	("Waffle v0.10",0)
uszHelp		ustr	("Waffle v0.10, Sep 23 2012, Windows API Filtering Layer",0dh,0ah)
uszHelp2	ustr	("(c) 2012 Excalibur. All rights reserved.",0dh,0ah)
uszHelp3	ustr	("Blah blah blah.",0dh,0ah,0dh,0ah)
uszHelp4	ustr	("usage: loader [ options ] target_full_path [ arguments ]",0dh,0ah,0)

uszFmt		ustr	("%08X",0)

uszX86		ustr	("Target: x86",0dh,0ah,0)
uszX86Dll	ustr	("x86\dispatch.dll",0)
uszX64		ustr	("x64 does not yet support. Sorry",21h,0dh,0ah,0)
uszX64Dll	ustr	("x64\dispatch.dll",0)

uszErrConsoleInit ustr	("E0001 Console failed to initialize.",0dh,0ah,0)
uszErrOpenTarget ustr	("E0002 Cannnot open target.",0dh,0ah,0)
uszErrUnrecognizedTarget ustr	("E0003 Unrecognized target.",0dh,0ah,0)
uszErrUnrecognizedOption ustr	("E0004 Unrecognized option.",0dh,0ah,0)
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		Console.inc

include		_CmdLine.asm
include		PECheck.asm
include		Inject.asm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke	_SetConsole,addr uszTitle
		.if	!eax
			invoke	MessageBox,0,addr uszErrConsoleInit,0,0
			invoke	ExitProcess,0
		.endif
		invoke	GetCurrentDirectory,sizeof uszCurrentFolder,addr uszCurrentFolder
		invoke	lstrlen,addr uszCurrentFolder
		mov	edi,offset uszCurrentFolder
		mov	ecx,2
		mul	ecx
		add	edi,eax
		xor	eax,eax
		mov	ax,'\'
		.if	word ptr [edi-2] != ax
			stosd
		.endif

		invoke	lstrcpy,addr uszDllFull,addr uszCurrentFolder

		invoke	_argc		;无参数 显示帮助
		.if	!eax
			Cout	offset uszHelp
			Cpause
		.else
			mov	nArgc,eax
			xor	eax,eax
			.while	TRUE	;处理每个参数
				inc	nTargetArgc
				invoke	_argv,nTargetArgc,addr uszBuf,sizeof uszBuf
				mov	esi,offset uszBuf
				lodsw
				.if	eax ==	'/'	;如果以'/'开头则判断选项
					lodsd		;只限一个字符
					.if	eax ==	'h'
						Cout	offset uszHelp
						Cpause
						.break
					.elseif	eax ==	'/'
					.else
						Cout	offset uszErrUnrecognizedOption
						.break
					.endif
				.else				;否则认为是目标文件
					invoke	_GetPEMagic,addr uszBuf
					.if	eax == 0
						Cout	offset uszErrOpenTarget
					.elseif	eax == 0000010Bh
						Cout	offset uszX86
						invoke	lstrcat,addr uszDllFull,addr uszX86Dll	;准备Dll字符串
						inc	nTargetArgc		;获得命令行
						invoke	_argp,nTargetArgc
						invoke	_InjectDllX86,addr uszBuf,addr uszDllFull,eax
					.elseif	eax == 0000020Bh
						Cout	offset uszX64
					.else
						Cout	offset uszErrUnrecognizedTarget
					.endif
					.break
				.endif
				mov	eax,nTargetArgc
				.if	eax == nArgc
					Cout	offset uszErrUnrecognizedTarget
					invoke	ExitProcess,0
				.endif
			.endw
		.endif
		invoke	ExitProcess,0
		ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start