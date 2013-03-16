		.const
uszFmt		ustr	("0x%X",0)

uszInjectDll	ustr	("core.dll",0)

uszParentTID	ustr	("ParentTID",0)
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_InjectDll	proc	uses ebx esi edi _lpuszTarget,_lpuszArgument,_lpuszCurrentDirectory
		local	@stStartUp:STARTUPINFO
		local	@stProcessInfo:PROCESS_INFORMATION
		local	@lpDllName
		local	@lpuszTargetAndArgu

		invoke	GetCurrentThreadId				;环境变量
		invoke	wsprintf,addr uszEnvirVar,addr uszFmt,eax
		invoke	SetEnvironmentVariable,addr uszParentTID,addr uszEnvirVar
		
		invoke	PeekMessage,addr stMSG,0,0,0,PM_NOREMOVE	;建立消息循环

		invoke	lstrcpy,addr uszDllFull,addr uszCurrentFolder
		invoke	lstrcat,addr uszDllFull,addr uszInjectDll		;dll
		
		invoke	lstrlen,_lpuszTarget
		mov	ebx,eax
		invoke	lstrlen,_lpuszArgument
		add	ebx,eax
		inc	ebx		;空格
		inc	ebx		;末尾的0
		mov	eax,2
		xchg	eax,ebx
		shl	eax,2
		invoke	GlobalAlloc,GPTR,eax
		mov	@lpuszTargetAndArgu,eax
		
		invoke	lstrcpy,@lpuszTargetAndArgu,addr uszQuotation
		invoke	lstrcat,@lpuszTargetAndArgu,addr uszBuf
		invoke	lstrcat,@lpuszTargetAndArgu,addr uszQuotation
		invoke	lstrlen,@lpuszTargetAndArgu
		mov	edi,@lpuszTargetAndArgu
		mul	ebx
		add	edi,eax
		mov	eax,' '
		stosd	;空格和NUL
		invoke	lstrcat,@lpuszTargetAndArgu,_lpuszArgument

		push	sizeof @stStartUp
		pop	@stStartUp.cb
		invoke	GetStartupInfo,addr @stStartUp
		invoke	CreateProcess,_lpuszTarget,@lpuszTargetAndArgu,NULL,NULL,TRUE,CREATE_SUSPENDED,0,_lpuszCurrentDirectory,addr @stStartUp,addr @stProcessInfo
		invoke	VirtualAllocEx,@stProcessInfo.hProcess,NULL,MAX_PATH * 2,MEM_COMMIT,PAGE_READWRITE
		.if	eax
			mov	@lpDllName,eax
			invoke	WriteProcessMemory,@stProcessInfo.hProcess,@lpDllName,addr uszDllFull,MAX_PATH * 2,NULL
			;使用GetModuleHandle和GetProcAddress来获得LoadLibrary的具体地址,而不是IAT
			invoke	CreateRemoteThread,@stProcessInfo.hProcess,NULL,0,LoadLibraryW,@lpDllName,0,NULL
			invoke	CloseHandle,eax
		.endif
		invoke	GlobalFree,@lpuszTargetAndArgu
		
		mov	edx,@stProcessInfo.hProcess
		mov	eax,@stProcessInfo.hThread
		
		ret
_InjectDll	endp