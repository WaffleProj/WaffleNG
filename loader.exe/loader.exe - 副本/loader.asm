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
includelib	comdlg32.lib

include		Unicode.inc
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		
		.data?
uszBuf		dw	MAX_PATH dup (?)

uszCurrentFolder dw	MAX_PATH dup (?)
stOpenFile	OPENFILENAME	<?>

lpArg		dd	?
lpArgFull	dd	?

stStartUp	STARTUPINFO	<?>
stProcessInfo	PROCESS_INFORMATION	<?>
		.const
uszX86Inject	ustr	("x86\inject.exe",0)
uszX64Inject	ustr	("x64\inject.exe",0)
uszSpace	ustr	(" ",0)
uszQuotation	ustr	('"',0)

uszErrOpenTarget 	ustr	("[0001]Cannnot read the file.",0)
uszErrUnrecognizedTarget ustr	("[0002]This is not a legal PE file.",0)
uszErrX64Target		ustr	("[FFFF]At this moment we can not load PE64 file.",0)
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		_CmdLine.asm
include		PECheck.asm
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
		.if	!eax			;无参数 打开文件
			push	sizeof stOpenFile
			pop	stOpenFile.lStructSize
			push	offset uszBuf
			pop	stOpenFile.lpstrFile
			push	lengthof uszBuf
			pop	stOpenFile.nMaxFile
			mov	stOpenFile.Flags,OFN_PATHMUSTEXIST or OFN_FILEMUSTEXIST
			invoke	GetOpenFileName,addr stOpenFile
		.else				;有参数 依次处理
			invoke	_argv,1,addr uszBuf,sizeof uszBuf
		.endif
			
		invoke	_GetPEMagic,addr uszBuf
		.if	eax == 0
			invoke	MessageBox,0,addr uszErrOpenTarget,0,0
		.elseif	eax == IMAGE_NT_OPTIONAL_HDR32_MAGIC
			invoke	_argp,2
			mov	lpArg,eax
			invoke	lstrlen,addr uszCurrentFolder
			mov	ebx,eax
			add	ebx,lengthof uszX86Inject
			inc	ebx	;空格
			invoke	lstrlen,addr uszBuf
			add	ebx,eax
			inc	ebx	;空格
			invoke	lstrlen,lpArg
			add	ebx,eax
			inc	ebx	;NUL
			
			mov	eax,ebx
			shl	eax,2
			invoke	GlobalAlloc,GPTR,eax
			mov	lpArgFull,eax

			invoke	lstrcpy,lpArgFull,addr uszCurrentFolder
			invoke	lstrcat,lpArgFull,addr uszX86Inject
			invoke	lstrcat,lpArgFull,addr uszSpace
			invoke	lstrcat,lpArgFull,addr uszQuotation
			invoke	lstrcat,lpArgFull,addr uszBuf
			invoke	lstrcat,lpArgFull,addr uszQuotation
			invoke	lstrcat,lpArgFull,addr uszSpace
			invoke	lstrcat,lpArgFull,lpArg

			invoke	lstrcpy,addr uszBuf,addr uszCurrentFolder
			invoke	lstrcat,addr uszBuf,addr uszX86Inject
			
			push	sizeof stStartUp
			pop	stStartUp.cb
			invoke	GetStartupInfo,addr stStartUp
			invoke	CreateProcess,addr uszBuf,lpArgFull,NULL,NULL,FALSE,NULL,NULL,NULL,addr stStartUp,addr stProcessInfo
			
			invoke	CloseHandle,stProcessInfo.hProcess
			invoke	CloseHandle,stProcessInfo.hThread
		.elseif	eax == IMAGE_NT_OPTIONAL_HDR64_MAGIC
			invoke	MessageBox,0,addr uszErrX64Target,0,0
		.else
			invoke	MessageBox,0,addr uszErrUnrecognizedTarget,0,0
		.endif

		invoke	ExitProcess,0
		ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start