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
; 常量定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		
		.data?
uszBuf		dw	MAX_PATH dup (?)

nArgc		dd	?
nTargetArgc	dd	?
		.const
uszTitle	ustr	("Waffle v0.10",0)
uszHelp		ustr	("Waffle v0.10, Sep 23 2012, Windows API Filtering Layer",0dh,0ah)
uszHelp2	ustr	("(c) 2012 Excalibur. All rights reserved.",0dh,0ah)
uszHelp3	ustr	("Blah blah blah.",0dh,0ah,0dh,0ah)
uszHelp4	ustr	("usage: loader [ options ] target_full_path [ arguments ]",0dh,0ah,0)

uszFmt		ustr	("%08X",0)

uszX86		ustr	("X86",0dh,0ah,0)
uszX64		ustr	("X64 does not yet support. Sorry",21h,0dh,0ah,0)

uszErrConsoleInit ustr	("Console failed to initialize.",0dh,0ah,0)
uszErrOpenTarget ustr	("Cannnot open target.",0dh,0ah,0)
uszErrUnrecognizedTarget ustr	("Unrecognized target.",0dh,0ah,0)
uszErrUnrecognizedOption ustr	("Unrecognized option.",0dh,0ah,0)
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		Console.inc

include		_CmdLine.asm
include		PECheck.asm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke	_SetConsole,addr uszTitle
		.if	!eax
			invoke	MessageBox,0,addr uszErrConsoleInit,0,0
			invoke	ExitProcess,0
		.endif
		invoke	_argc		;无参数 显示帮助
		.if	!eax
			Cout	offset uszHelp
			Cpause
		.else
			mov	nArgc,eax
			xor	eax,eax
			mov	nTargetArgc,eax
			.while	TRUE	;处理每个参数
				inc	nTargetArgc
				invoke	_argv,nTargetArgc,addr uszBuf,sizeof uszBuf
				mov	esi,offset uszBuf
				lodsw
				.if	eax ==	'/'	;如果以'/'开头则判断选项
					lodsw
					.if	eax ==	'h'
						Cout	offset uszHelp
						Cpause
						.break
					.elseif	eax ==	'd'
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
						inc	nTargetArgc		;获得命令行
						invoke	_argp,nTargetArgc
						mov	nTargetArgc,eax
						Cout	nTargetArgc
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