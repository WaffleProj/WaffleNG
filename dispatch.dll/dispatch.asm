		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
includelib	user32.lib
includelib	kernel32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
hInstance	dd	?
hTarget		dd	?
		.const
szLog		db	'Heap.log',0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Main		proc	uses ebx esi edi _lParam

		invoke	GetModuleHandle,NULL
		mov	hTarget,eax
		
		;invoke	_HotPatching,offset szKernel32,offset szHeapCreate,_HeapCreate,offset lpHeapCreate
		;invoke	_HotPatching,offset szKernel32,offset szHeapDestroy,_HeapDestroy,offset lpHeapDestroy
		invoke	MessageBox,0,0,0,0
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