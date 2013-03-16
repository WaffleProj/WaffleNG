		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
includelib	user32.lib
includelib	kernel32.lib
includelib	gdi32.lib
includelib	shlwapi.lib
include		Hook.inc
DllMain		proto	:HINSTANCE,:DWORD,:LPVOID
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
externdef	hDLL:dword
hDLL		dd	?
externdef	hHeap:dword
hHeap		dd	?

externdef	NewCodePage:dword
NewCodePage	dd	?
externdef	OldCodePage:dword
OldCodePage	dd	?

externdef	lpszCommandLineA:dword
lpszCommandLineA dd	?
		.const
szKernel32	db	"Kernel32.dll",0
szUser32	db	"User32.dll",0
szGdi32		db	"Gdi32.dll",0
		.data
externdef	stHookTable:HOOK_TABLE_HEAD_OBJECT
stHookTable	HOOK_TABLE_BEGIN	_HookDispatch
"CreateDirectoryA"		HOOK_TABLE	"Kernel32",2
"CreateFileA"			HOOK_TABLE	"Kernel32",7
"DeleteFileA"			HOOK_TABLE	"Kernel32",1
"FindFirstFileA"		HOOK_TABLE	"Kernel32",2
"GetACP"			HOOK_TABLE	"Kernel32",0
"GetCommandLineA"		HOOK_TABLE	"Kernel32",0
"GetFileAttributesA"		HOOK_TABLE	"Kernel32",1
"GetModuleFileNameA"		HOOK_TABLE	"Kernel32",3
"GetModuleHandleA"		HOOK_TABLE	"Kernel32",1
"K32GetModuleFileNameExA"	HOOK_TABLE	"Kernel32",4
"LoadLibraryA"			HOOK_TABLE	"Kernel32",1
"MultiByteToWideChar"		HOOK_TABLE	"Kernel32",6
"SetCurrentDirectoryA"		HOOK_TABLE	"Kernel32",1
"WideCharToMultiByte"		HOOK_TABLE	"Kernel32",8


"MessageBoxExA"			HOOK_TABLE	"User32",5
"SetWindowTextA"		HOOK_TABLE	"User32",2

"CreateFontA"			HOOK_TABLE	"Gdi32",14
"TextOutA"			HOOK_TABLE	"Gdi32",5
		HOOK_TABLE_END
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		Hook.asm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	DllMain