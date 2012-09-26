		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
include		ShLwApi.INC
includelib	user32.lib
includelib	kernel32.lib
includelib	Shlwapi.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 常量定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
ifdef	_WINDOWS_
THREADENTRY32 STRUCT
    dwSize              DWORD ?
    cntUsage            DWORD ?
    th32ThreadID        DWORD ?
    th32OwnerProcessID  DWORD ?
    tpBasePri           DWORD ?
    tpDeltaPri          DWORD ?
    dwFlags             DWORD ?
THREADENTRY32 ENDS
endif
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
hInstance	dd	?
hTarget		dd	?

nParentThread	dd	?
szEnvirVar	db	16 dup (?)

		.const
szParrentTID	db	'ParrentTID',0
szh		db	'0x',0
ifndef	STIF_SUPPORT_HEX
	STIF_SUPPORT_HEX	equ	1
endif
TM_RESUMEMAINTHREAD	equ	WM_USER + 1

		.data?
lpMessageBoxA	dd	?
		.const
szUser32	db	'user32.dll',0
szMessageBoxA	db	'MessageBoxA',0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		Hooker.asm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Main		proc	uses ebx esi edi _lParam

		invoke	GetModuleHandle,NULL
		mov	hTarget,eax
		
		invoke	_HotPatching,offset szUser32,offset szMessageBoxA,_Dispatcher,offset lpMessageBoxA

		invoke	GetEnvironmentVariable,addr szParrentTID,addr szEnvirVar,16
		invoke	SetEnvironmentVariable,addr szParrentTID,NULL
		invoke	StrToIntEx,addr szEnvirVar,STIF_SUPPORT_HEX,addr nParentThread
		invoke	PostThreadMessage,nParentThread,TM_RESUMEMAINTHREAD,0,0
		ret

_Main		endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
DllEntry	proc	_hInstance,_dwReason,_dwReserved

		.if	_dwReason == DLL_PROCESS_ATTACH
			push	_hInstance
			pop	hInstance
			invoke	CreateThread,NULL,0,offset _Main,NULL,NULL,NULL
			invoke	CloseHandle,eax
		.endif
		mov	eax,TRUE
		ret

DllEntry	Endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		End	DllEntry