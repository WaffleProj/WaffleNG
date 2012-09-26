		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		D:\masm32\include\windows.inc
include		D:\masm32\include\user32.inc
includelib	user32.lib
include		D:\masm32\include\kernel32.inc
includelib	kernel32.lib
include		D:\masm32\include\Shlwapi.inc
includelib	Shlwapi.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
hInstance	dd	?
hTarget		dd	?
nParentThread	dd	?
szEnvirVar	db	16 dup (?)
szConvert	db	16 dup (?)

hSnap		dd	?
stSnap		THREADENTRY32 <?>
		.const
szFmt		db	'%08X',0
szParrentTID	db	'ParrentTID',0
szh		db	'0x',0
ifndef	STIF_SUPPORT_HEX
	STIF_SUPPORT_HEX	equ	1
endif
TM_RESUMEMAINTHREAD	equ	WM_USER + 1
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Main		proc	uses ebx esi edi _lParam

		invoke	GetModuleHandle,NULL
		mov	hTarget,eax
		
		;invoke	_HotPatching,offset szKernel32,offset szHeapCreate,_HeapCreate,offset lpHeapCreate
		;invoke	_HotPatching,offset szKernel32,offset szHeapDestroy,_HeapDestroy,offset lpHeapDestroy
		invoke	GetEnvironmentVariable,addr szParrentTID,addr szEnvirVar,16
		invoke	SetEnvironmentVariableW,addr szParrentTID,NULL
		invoke	lstrcpy,addr szConvert,addr szh
		invoke	lstrcat,addr szConvert,addr szEnvirVar
		invoke	StrToIntEx,addr szConvert,STIF_SUPPORT_HEX,addr nParentThread
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