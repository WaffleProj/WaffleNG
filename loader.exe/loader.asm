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
uszEnvirVar	dw	16 dup (?)

uszTMP		dw	MAX_PATH dup (?)
uszCurrentFolder dw	MAX_PATH dup (?)
uszDllFull	dw	MAX_PATH dup (?)

nArgc		dd	?
nTargetArgc	dd	?

hTargetMainThread dd	?
hTargetProcess	dd	?

stMSG		MSG	<?>
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

uszParrentTID	ustr	("ParrentTID",0)

uszErrConsoleInit ustr	("E0001 Console failed to initialize.",0dh,0ah,0)
uszErrOpenTarget ustr	("E0002 Cannnot open target.",0dh,0ah,0)
uszErrUnrecognizedTarget ustr	("E0003 Unrecognized target.",0dh,0ah,0)
uszErrUnrecognizedOption ustr	("E0004 Unrecognized option.",0dh,0ah,0)

uszResumeThread ustr	("Resume Main Thread",0dh,0ah,0)
uszErrUnrecognizedMessage ustr	("E0011 Unrecognized Message.",0dh,0ah,0)

TM_FIRSTMESSAGE		equ	TM_RESUMEMAINTHREAD
TM_RESUMEMAINTHREAD	equ	WM_USER + 1
TM_LASTMESSAGE		equ	TM_RESUMEMAINTHREAD
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
		invoke	_SetConsole,addr uszTitle		;设置Console句柄
		.if	!eax
			invoke	MessageBox,0,addr uszErrConsoleInit,0,0
			invoke	ExitProcess,0
		.endif

		invoke	GetCurrentDirectory,sizeof uszCurrentFolder,addr uszCurrentFolder		;获得需要的目录信息
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

		invoke	_argc
		.if	!eax			;无参数 显示帮助
			Cout	offset uszHelp
			Cpause
			invoke	ExitProcess,0
		.else			;有参数 依次处理
			mov	nArgc,eax
			xor	eax,eax
			mov	nTargetArgc,eax
			.while	TRUE
				inc	nTargetArgc
				invoke	_argv,nTargetArgc,addr uszBuf,sizeof uszBuf
				mov	esi,offset uszBuf
				lodsw				;逐字符(word)判断
				.if	eax ==	'/'					;以'/'开头,假定为选项
					lodsd					;目前只涉及一个字符,所以lodsd还读取了字符串结尾的0
					.if	eax ==	'h'
						Cout	offset uszHelp
						Cpause
						invoke	ExitProcess,0
					.elseif	eax ==	'/'
					.else
						Cout	offset uszErrUnrecognizedOption
						invoke	ExitProcess,0
					.endif
				.else					;不以以'/'开头,假定为目标文件
					invoke	_GetPEMagic,addr uszBuf
					.if	eax == 0
						Cout	offset uszErrOpenTarget
						invoke	ExitProcess,0
					.elseif	eax == 0000010Bh
						Cout	offset uszX86

						invoke	GetCurrentThreadId			;环境变量
						invoke	wsprintf,addr uszEnvirVar,addr uszFmt,eax
						invoke	SetEnvironmentVariable,addr uszParrentTID,addr uszEnvirVar
						
						invoke	PeekMessage,addr stMSG,0,0,0,PM_NOREMOVE ;建立消息循环

						invoke	lstrcat,addr uszDllFull,addr uszX86Dll	;Dll
						
						inc	nTargetArgc				;命令行
						invoke	_argp,nTargetArgc
						
						invoke	_InjectDll,addr uszBuf,addr uszDllFull,eax
						mov	hTargetMainThread,edx
						mov	hTargetProcess,eax
						.break
					.elseif	eax == 0000020Bh
						Cout	offset uszX64
						invoke	ExitProcess,0
					.else
						Cout	offset uszErrUnrecognizedTarget
						invoke	ExitProcess,0
					.endif
				.endif
				mov	eax,nTargetArgc
				.if	eax == nArgc
					Cout	offset uszErrUnrecognizedTarget
					invoke	ExitProcess,0
				.endif
			.endw
		.endif
		.while	TRUE
			invoke	GetMessage,addr stMSG,0,TM_FIRSTMESSAGE,TM_LASTMESSAGE
			.break	.if	!eax		;WM_QUIT
			.break	.if	eax == -1	;发生了错误
			mov	eax,stMSG.message
			.if	eax ==	TM_RESUMEMAINTHREAD
				Cout	offset uszResumeThread
				invoke	ResumeThread,hTargetMainThread
				.break
			.else
				Cout	offset uszErrUnrecognizedMessage
			.endif
		.endw

		invoke	ExitProcess,0
		ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start