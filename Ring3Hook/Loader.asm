		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
include		user32.inc
includelib	user32.lib
include		kernel32.inc
includelib	kernel32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
stStartUp	STARTUPINFO	<?>
stProcessInfo	PROCESS_INFORMATION	<?>
lpLoadLibrary	dd	?
lpDllName	dd	?
szInjectDllFull	db	MAX_PATH dup (?)
szTargetFull	db	MAX_PATH dup (?)

		.const
szErrOpen	db	'注入失败,无法打开远程线程',0
szSuccess	db	'注入成功',0
szDllKernel	db	'Kernel32.dll',0
szLoadLibrary	db	'LoadLibraryA',0
szInjectDll	db	'\Hooker.dll',0
szTarget	db	'\Target.exe',0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
;********************************************************************
; 准备工作：获取dll的全路径文件名、获取LoadLibrary函数地址等
;********************************************************************
		invoke	GetCurrentDirectory,MAX_PATH,addr szInjectDllFull
		invoke	lstrcat,addr szInjectDllFull,addr szInjectDll
		
		invoke	GetCurrentDirectory,MAX_PATH,addr szTargetFull
		invoke	lstrcat,addr szTargetFull,addr szTarget
		
		invoke	GetModuleHandle,addr szDllKernel
		invoke	GetProcAddress,eax,offset szLoadLibrary
		mov	lpLoadLibrary,eax
;********************************************************************
; 创建进程
;********************************************************************
		invoke	GetStartupInfo,addr stStartUp
		invoke	CreateProcess,NULL,addr szTargetFull,NULL,NULL,FALSE,CREATE_SUSPENDED,0,NULL,addr stStartUp,addr stProcessInfo
		.if	eax
;********************************************************************
; 在进程中分配空间并将DLL文件名拷贝过去，然后创建一个LoadLibrary线程
;********************************************************************
			invoke	VirtualAllocEx,stProcessInfo.hProcess,NULL,MAX_PATH,MEM_COMMIT,PAGE_READWRITE
			.if	eax
				mov	lpDllName,eax
				invoke	WriteProcessMemory,stProcessInfo.hProcess,eax,offset szInjectDllFull,MAX_PATH,NULL
				invoke	CreateRemoteThread,stProcessInfo.hProcess,NULL,0,lpLoadLibrary,lpDllName,0,NULL
				invoke	CloseHandle,eax
			.endif
			invoke	MessageBox,NULL,addr szSuccess,NULL,NULL
			invoke	ResumeThread,stProcessInfo.hThread
			invoke	CloseHandle,stProcessInfo.hProcess
		.else
			invoke	MessageBox,NULL,addr szErrOpen,NULL,MB_OK or MB_ICONWARNING
		.endif
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start
