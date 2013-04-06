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
includelib	psapi.lib
include		Hook.inc
DllMain		proto	:HINSTANCE,:DWORD,:LPVOID
__DEBUG__	equ	TRUE
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
if		__DEBUG__
		.data?
hDebug		dd	?
stCS		CRITICAL_SECTION	<?>
		.const
szDebug		db	"DebugOutput.log",0
szCrLf		db	0dh,0ah,0
endif
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.const
szKernel32	db	"Kernel32.dll",0
szUser32	db	"User32.dll",0
szGdi32		db	"Gdi32.dll",0
szPsapi		db	"Psapi.dll"
		.data
externdef	stHookTable:HOOK_TABLE_HEAD_OBJECT
stHookTable	HOOK_TABLE_BEGIN	_HookDispatch
"CreateDirectoryA"		HOOK_TABLE	"Kernel32",2
"CreateFileA"			HOOK_TABLE	"Kernel32",7
"DeleteFileA"			HOOK_TABLE	"Kernel32",1
"FindFirstFileA"		HOOK_TABLE	"Kernel32",2
"FindNextFileA"			HOOK_TABLE	"Kernel32",2
"GetACP"			HOOK_TABLE	"Kernel32",0
"GetCommandLineA"		HOOK_TABLE	"Kernel32",0
"GetCPInfo"			HOOK_TABLE	"Kernel32",2
"GetFileAttributesA"		HOOK_TABLE	"Kernel32",1
"GetModuleFileNameA"		HOOK_TABLE	"Kernel32",3
"GetModuleHandleA"		HOOK_TABLE	"Kernel32",1
"LoadLibraryA"			HOOK_TABLE	"Kernel32",1
"LoadLibraryExA"		HOOK_TABLE	"Kernel32",3
"MultiByteToWideChar"		HOOK_TABLE	"Kernel32",6
"SetCurrentDirectoryA"		HOOK_TABLE	"Kernel32",1
"SetFileAttributesA"		HOOK_TABLE	"Kernel32",2
"WideCharToMultiByte"		HOOK_TABLE	"Kernel32",8

;CharLower
"CallWindowProcA"		HOOK_TABLE	"User32",5
"CreateWindowExA"		HOOK_TABLE	"User32",12
"MessageBoxExA"			HOOK_TABLE	"User32",5
;OemToCharA
"SendMessageA"			HOOK_TABLE	"User32",4
"SetWindowTextA"		HOOK_TABLE	"User32",2

"CreateFontA"			HOOK_TABLE	"Gdi32",14
"TextOutA"			HOOK_TABLE	"Gdi32",5

"GetModuleFileNameExA"		HOOK_TABLE	"Psapi",4
		HOOK_TABLE_END
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		Hook.asm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	DllMain