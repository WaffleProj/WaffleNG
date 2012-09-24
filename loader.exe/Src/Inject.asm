		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_InjectDllX86	proc	uses ebx esi edi _lpuszTarget,_lpuszDll,_lpuszArgument
		local	@stStartUp:STARTUPINFO
		local	@stProcessInfo:PROCESS_INFORMATION
		local	@lpDllName
		local	@lpuszTargetAndArgu

		invoke	lstrlen,_lpuszTarget
		mov	ebx,eax
		invoke	lstrlen,_lpuszArgument
		add	ebx,eax
		inc	ebx		;¿Õ¸ñ
		inc	ebx		;Ä©Î²µÄ0
		mov	eax,2
		xchg	eax,ebx
		mul	ebx
		invoke	GlobalAlloc,GPTR,eax
		mov	@lpuszTargetAndArgu,eax
		invoke	lstrcpy,@lpuszTargetAndArgu,_lpuszTarget
		invoke	lstrlen,@lpuszTargetAndArgu
		mov	edi,@lpuszTargetAndArgu
		mul	ebx
		add	edi,eax
		mov	eax,' '
		stosd
		invoke	lstrcat,@lpuszTargetAndArgu,_lpuszArgument

		push	sizeof @stStartUp
		pop	@stStartUp.cb
		invoke	GetStartupInfo,addr @stStartUp
		invoke	CreateProcess,_lpuszTarget,@lpuszTargetAndArgu,NULL,NULL,FALSE,CREATE_SUSPENDED,0,NULL,addr @stStartUp,addr @stProcessInfo
		invoke	VirtualAllocEx,@stProcessInfo.hProcess,NULL,MAX_PATH,MEM_COMMIT,PAGE_READWRITE
		.if	eax
			mov	@lpDllName,eax
			invoke	WriteProcessMemory,@stProcessInfo.hProcess,@lpDllName,_lpuszDll,MAX_PATH * 2,NULL
			invoke	CreateRemoteThread,@stProcessInfo.hProcess,NULL,0,LoadLibraryW,@lpDllName,0,NULL
			invoke	CloseHandle,eax
		.endif
		;invoke	Sleep,1000
		invoke	ResumeThread,@stProcessInfo.hThread
		invoke	CloseHandle,@stProcessInfo.hProcess
		invoke	GlobalFree,@lpuszTargetAndArgu
		
		ret
_InjectDllX86	endp