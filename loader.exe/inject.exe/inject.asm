		.386
		.model flat,stdcall
		option casemap:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 头文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
UNICODE		equ	TRUE
;_WIN32_WINNT	equ	0501h

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
uszTarget	dw	MAX_PATH dup (?)
uszTargetFolder	dw	MAX_PATH dup (?)
uszEnvirVar	dw	16 dup (?)

uszCurrentFolder dw	MAX_PATH dup (?)
uszDllFull	dw	MAX_PATH dup (?)

hMainThread	dd	?

stMSG		MSG	<?>
stOpenFile	OPENFILENAME	<?>
		.const
uszQuotation	ustr	('"',0)
TM_FIRSTMESSAGE		equ	TM_RESUMEMAINTHREAD
TM_RESUMEMAINTHREAD	equ	WM_USER + 1
TM_LASTMESSAGE		equ	TM_RESUMEMAINTHREAD
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		_CmdLine.asm
include		DllInject.asm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke	GetModuleFileName,NULL,addr uszCurrentFolder,sizeof uszCurrentFolder		;获得需要的目录信息
		invoke	lstrlen,addr uszCurrentFolder
		mov	edi,offset uszCurrentFolder
		mov	ecx,2
		mul	ecx
		add	edi,eax
		xor	eax,eax
		mov	ax,'\'
		mov	ecx,-1
		std
		repnz	scasw	;Releas|e\x86
		xor	eax,eax
		cld
		add	edi,4	;Release\|x86
		stosw

		invoke	_argc
		.if	!eax
			invoke	ExitProcess,0
		.endif

		invoke	_argv,1,addr uszBuf,sizeof uszBuf			;目标程序
		invoke	_argv,1,addr uszTargetFolder,sizeof uszTargetFolder	;启动目录
		invoke	lstrlen,addr uszTargetFolder
		mov	ecx,eax
		mov	edi,offset uszTargetFolder
		mov	edx,2
		mul	edx
		add	edi,eax
		xor	eax,eax
		mov	ax,'\'
		std
		repnz	scasw	;Releas|e\x86
		.if	ZERO?
			xor	eax,eax
			cld
			add	edi,2	;Release|\x86
			stosw
			invoke	_argp,2							;命令行
			invoke	_InjectDll,addr uszBuf,eax,addr uszTargetFolder
		.else
			invoke	_argp,2							;命令行
			invoke	_InjectDll,addr uszBuf,eax,NULL
		.endif
		mov	hMainThread,eax
		invoke	CloseHandle,edx

		.while	TRUE
			invoke	GetMessage,addr stMSG,0,TM_FIRSTMESSAGE,TM_LASTMESSAGE
			.break	.if	eax == -1	;发生了错误
			mov	eax,stMSG.message
			.if	eax ==	TM_RESUMEMAINTHREAD
				invoke	ResumeThread,hMainThread
				invoke	CloseHandle,hMainThread
				.break
			.endif
		.endw

		invoke	ExitProcess,0
		ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start