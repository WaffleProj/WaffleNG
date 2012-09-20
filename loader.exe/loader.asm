		.386
		.model flat,stdcall
		option casemap:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
UNICODE		equ	TRUE

include		windows.inc
include		winnt.inc
includelib	kernel32.lib
includelib	user32.lib

include		Console.inc
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		
		.data?
uszBuf		dw	MAX_PATH dup (?)

hFile		dd	?
hMapFile	dd	?
lpFile		dd	?
		.const
uszTitle	dw	"Waffle v0.10",0
uszHelp		dw	"This is a help.",0
uszError	dw	"error",0
uszFmt		dw	"%08X",0

uszDosSign	dw	"Dos Sign:",0
uszNTHead	dw	"NT Head:",0
uszNTSign	dw	"NT Sign:",0
uszNTTarget	dw	"NT Target:",0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		_CmdLine.asm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke	_SetConsole,0;addr uszTitle
		invoke	_argc
		.if	eax == 1
			Cout	uszHelp
		.elseif	eax == 2
			invoke	_argv,1,addr uszBuf,sizeof uszBuf
			invoke	CreateFile,addr uszBuf,GENERIC_READ,NULL,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
			mov	hFile,eax
			invoke	CreateFileMapping,eax,NULL,PAGE_READONLY,NULL,NULL,NULL
			mov	hMapFile,eax
			invoke	MapViewOfFile,eax,FILE_MAP_READ,NULL,NULL,NULL
			mov	lpFile,eax
			mov	ebx,eax
			.if	!ebx
				invoke	ExitProcess,0
			.endif
			
			assume	ebx:ptr IMAGE_DOS_HEADER
			Cout	uszDosSign
			invoke	wsprintf,addr uszBuf,addr uszFmt,[ebx].e_magic
			invoke	lstrlen,addr uszBuf
			Coutn	uszBuf,eax
			Creturn

			Cout	uszNTHead
			invoke	wsprintf,addr uszBuf,addr uszFmt,[ebx].e_lfanew
			invoke	lstrlen,addr uszBuf
			Coutn	uszBuf,eax
			Creturn
			
			mov	eax,[ebx].e_lfanew
			add	eax,lpFile
			mov	ebx,eax
			assume	ebx:ptr IMAGE_NT_HEADERS
			
			Cout	uszNTSign
			invoke	wsprintf,addr uszBuf,addr uszFmt,[ebx].Signature
			invoke	lstrlen,addr uszBuf
			Coutn	uszBuf,eax
			Creturn
			
			Cout	uszNTTarget
			movzx	eax,[ebx].OptionalHeader.Magic
			invoke	wsprintf,addr uszBuf,addr uszFmt,[ebx].OptionalHeader.Magic
			invoke	lstrlen,addr uszBuf
			Coutn	uszBuf,eax
			Creturn
			assume	ebx:nothing
		.else
			Cout	uszError
		.endif
		invoke	ExitProcess,0
		ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start