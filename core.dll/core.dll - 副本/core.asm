		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
include		shlwapi.inc
includelib	user32.lib
includelib	kernel32.lib
includelib	gdi32.lib
includelib	shlwapi.lib

DllMain		proto	:HINSTANCE,:DWORD,:LPVOID
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 常量定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
STIF_SUPPORT_HEX	equ	1
TM_RESUMEMAINTHREAD	equ	WM_USER + 1
CP_SHIFT_JIS		equ	932
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
externdef	hInstance:dword
hInstance	dd	?
externdef	lpszCommandLineA:dword
lpszCommandLineA dd	?

nParentThread	dd	?
szEnvirVar	db	16 dup (?)
		.const
szParentTID	db	'ParentTID',0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		Hooker.asm
		.data
User32		HOOK_TABLE_BEGIN	_DispatchUser32
"MessageBoxExA"		HOOK_TABLE	5
"SetWindowTextA"	HOOK_TABLE	2
		HOOK_TABLE_END

Kernel32	HOOK_TABLE_BEGIN	_DispatchKernel32
"CreateDirectoryA"		HOOK_TABLE	2
"CreateFileA"			HOOK_TABLE	7
"DeleteFileA"			HOOK_TABLE	1
"FindFirstFileA"		HOOK_TABLE	2
"GetACP"			HOOK_TABLE	0
"GetCommandLineA"		HOOK_TABLE	0
"GetFileAttributesA"		HOOK_TABLE	1
"GetModuleFileNameA"		HOOK_TABLE	3
"GetModuleHandleA"		HOOK_TABLE	1
"K32GetModuleFileNameExA"	HOOK_TABLE	4
"LoadLibraryA"			HOOK_TABLE	1
"SetCurrentDirectoryA"		HOOK_TABLE	1
		HOOK_TABLE_END

Gdi32		HOOK_TABLE_BEGIN	_DispatchGdi32
"CreateFontA"	HOOK_TABLE	14
"TextOutA"	HOOK_TABLE	5
		HOOK_TABLE_END
		.code
include		user32.asm
include		kernel32.asm
include		gdi32.asm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; InitLibrary	proc	uses ebx esi edi _lParam

		; invoke	GetModuleHandle,NULL
		; mov	hTarget,eax

		; invoke	GetCommandLineW
		; mov	ebx,eax
		; invoke	lstrlenW,eax
		; shl	eax,1
		; mov	esi,eax
		; invoke	GlobalAlloc,GPTR,esi
		; mov	lpszCommandLineA,eax
		; invoke	WideCharToMultiByte,CP_SHIFT_JIS,NULL,ebx,-1,lpszCommandLineA,esi,NULL,NULL
		
		; invoke	GetEnvironmentVariable,addr szParentTID,addr szEnvirVar,16
		; invoke	SetEnvironmentVariable,addr szParentTID,NULL
		; invoke	StrToIntEx,addr szEnvirVar,STIF_SUPPORT_HEX,addr nParentThread

		; invoke	_TableHook,addr TABLE_User32
		; invoke	_TableHook,addr TABLE_Kernel32
		; invoke	_TableHook,addr TABLE_Gdi32

		; invoke	PostThreadMessage,nParentThread,TM_RESUMEMAINTHREAD,0,0
		
		; ret

; InitLibrary	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; DllEntry	proc	_hInstance,_dwReason,_dwReserved

		; .if	_dwReason == DLL_PROCESS_ATTACH
			; push	_hInstance
			; pop	hInstance
			; invoke	CreateThread,NULL,0,offset _Main,NULL,NULL,NULL
			; invoke	CloseHandle,eax
		; .endif
		; mov	eax,TRUE
		; ret

; DllEntry	Endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		; End	DllEntry
		end	DllMain