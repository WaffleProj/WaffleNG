		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_DispatchKernel32 proc
		add	dword ptr [esp],2	;修正返回值
		push	ebp
		mov	ebp,esp
		sub	esp,4	;制造一个局部变量
		pushad
		
		mov	edi,offset TABLE_Kernel32
		add	edi,sizeof HOOK_TABLE_HEAD_OBJECT
		xor	eax,eax
		mov	ecx,-1
		cld
		repnz	scasb

		assume	edi:ptr HOOK_TABLE_OBJECT
		.while	[edi].hookFunction
			mov	eax,[edi].hookFunction
			mov	dword ptr [ebp-4],eax	;保存对应的函数
			
			mov	eax,[edi].lpFunction
			.if	eax == dword ptr [ebp+4]
				popad
				add	esp,4
				mov	esp,ebp
				pop	ebp
				
				add	esp,4
				jmp	dword ptr [esp-12]	
				;[esp]=正常的返回地址
				;[esp-4]=Call _Dispatch
				;[esp-8]=ebp
				;[esp-12]=local
			.endif
			add	edi,8
			xor	eax,eax
			mov	ecx,-1
			cld
			repnz	scasb
		.endw
		assume	edi:nothing

		;没找到就跳到原函数
		popad
		add	esp,4
		mov	esp,ebp
		pop	ebp
		
		add	esp,4
		jmp	dword ptr [esp-4]
		;[esp]=正常的返回地址
		;[esp-4]=Call _Dispatch
_DispatchKernel32 endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyCreateFileA	proc	uses ebx esi edi lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile
		local	@dwFileName
		local	@lpuszFileName

		pushad
		mov	edi,offset TABLE_Kernel32
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		.if	lpFileName
			invoke	lstrlen,lpFileName
			inc	eax
			mov	@dwFileName,eax
			shl	eax,1
			invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
			mov	@lpuszFileName,eax
			invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpFileName,-1,@lpuszFileName,@dwFileName
		.else
			mov	@lpuszFileName,0
		.endif
		popad
		
		invoke	CreateFileW,@lpuszFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile
		
		pushad
		mov	edi,offset TABLE_Kernel32
		invoke	HeapFree,[edi].hHeap,NULL,@lpuszFileName
		popad
		ret
MyCreateFileA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MySetCurrentDirectoryA	proc	uses ebx esi edi lpPathName

		invoke	lpSetCurrentDirectoryA,lpPathName
		ret
MySetCurrentDirectoryA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyGetCommandLineA	proc	uses ebx esi edi

		mov	eax,lpszCommandLineA
		ret
MyGetCommandLineA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyGetModuleFileNameA	proc	uses ebx esi edi hModule,lpFilename,nSize
		local	@lpuszFilename

		pushad
		mov	edi,offset TABLE_Kernel32
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		
		mov	eax,nSize
		shl	eax,2
		invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
		mov	@lpuszFilename,eax
		popad
		
		.if	lpFilename
			invoke	GetModuleFileNameW,hModule,@lpuszFilename,nSize
		.else
			invoke	GetModuleFileNameW,hModule,NULL,nSize
		.endif

		pushad
		mov	edi,offset TABLE_Kernel32
		invoke	WideCharToMultiByte,[edi].dwACP,NULL,@lpuszFilename,-1,lpFilename,nSize,NULL,NULL
		invoke	HeapFree,[edi].hHeap,NULL,@lpuszFilename
		popad
		ret
MyGetModuleFileNameA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyK32GetModuleFileNameExA proc	uses ebx esi edi hProcess,hModule,lpFilename,nSize
		local	@lpuszFilename

		pushad
		mov	edi,offset TABLE_Kernel32
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		
		mov	eax,nSize
		shl	eax,2
		invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
		mov	@lpuszFilename,eax
		popad
		
		.if	lpFilename
			invoke	K32GetModuleFileNameExW,hProcess,hModule,@lpuszFilename,nSize		;需要修改winbase.inc以及kernel32.lib
		.else
			invoke	K32GetModuleFileNameExW,hProcess,hModule,NULL,nSize
		.endif

		pushad
		mov	edi,offset TABLE_Kernel32
		invoke	WideCharToMultiByte,[edi].dwACP,NULL,@lpuszFilename,-1,lpFilename,nSize,NULL,NULL
		invoke	HeapFree,[edi].hHeap,NULL,@lpuszFilename
		popad
		ret
MyK32GetModuleFileNameExA endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyCreateDirectoryA	proc	uses ebx esi edi lpPathName,lpSecurityAttributes
			local	@dwPathName
			local	@lpuszPathName

			pushad
			mov	edi,offset TABLE_Kernel32
			assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
			.if	lpPathName
				invoke	lstrlen,lpPathName
				inc	eax
				mov	@dwPathName,eax
				shl	eax,1
				invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
				mov	@lpuszPathName,eax
				invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpPathName,-1,@lpuszPathName,@dwPathName
			.else
				mov	@lpuszPathName,0
			.endif
			popad
			
			invoke	CreateDirectoryW,@lpuszPathName,lpSecurityAttributes
			
			pushad
			mov	edi,offset TABLE_Kernel32
			invoke	HeapFree,[edi].hHeap,NULL,@lpuszPathName
			popad
			ret
MyCreateDirectoryA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyGetFileAttributesA	proc	uses ebx esi edi lpFileName
			local	@dwFileName
			local	@lpuszFileName

			pushad
			mov	edi,offset TABLE_Kernel32
			assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
			.if	lpFileName
				invoke	lstrlen,lpFileName
				inc	eax
				mov	@dwFileName,eax
				shl	eax,1
				invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
				mov	@lpuszFileName,eax
				invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpFileName,-1,@lpuszFileName,@dwFileName
			.else
				mov	@lpuszFileName,0
			.endif
			popad
			
			invoke	GetFileAttributesW,@lpuszFileName
			
			pushad
			mov	edi,offset TABLE_Kernel32
			invoke	HeapFree,[edi].hHeap,NULL,@lpuszFileName
			popad
			ret
MyGetFileAttributesA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyDeleteFileA	proc	uses ebx esi edi lpFileName
		local	@dwFileName
		local	@lpuszFileName

		pushad
		mov	edi,offset TABLE_Kernel32
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		.if	lpFileName
			invoke	lstrlen,lpFileName
			inc	eax
			mov	@dwFileName,eax
			shl	eax,1
			invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
			mov	@lpuszFileName,eax
			invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpFileName,-1,@lpuszFileName,@dwFileName
		.else
			mov	@lpuszFileName,0
		.endif
		popad
		
		invoke	DeleteFileW,@lpuszFileName
		
		pushad
		mov	edi,offset TABLE_Kernel32
		invoke	HeapFree,[edi].hHeap,NULL,@lpuszFileName
		popad
		ret
MyDeleteFileA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyFindFirstFileA	proc	uses ebx esi edi lpFileName,lpFindFileData
			local	@dwFileName
			local	@lpuszFileName

			pushad
			mov	edi,offset TABLE_Kernel32
			assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
			.if	lpFileName
				invoke	lstrlen,lpFileName
				inc	eax
				mov	@dwFileName,eax
				shl	eax,1
				invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
				mov	@lpuszFileName,eax
				invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpFileName,-1,@lpuszFileName,@dwFileName
			.else
				mov	@lpuszFileName,0
			.endif
			popad
			
			invoke	FindFirstFileW,@lpuszFileName,lpFindFileData
			
			pushad
			mov	edi,offset TABLE_Kernel32
			invoke	HeapFree,[edi].hHeap,NULL,@lpuszFileName
			popad
			ret
MyFindFirstFileA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyLoadLibraryA	proc	uses ebx esi edi lpFileName
		local	@dwFileName
		local	@lpuszFileName

		pushad
		mov	edi,offset TABLE_Kernel32
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		.if	lpFileName
			invoke	lstrlen,lpFileName
			inc	eax
			mov	@dwFileName,eax
			shl	eax,1
			invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
			mov	@lpuszFileName,eax
			invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpFileName,-1,@lpuszFileName,@dwFileName
		.else
			mov	@lpuszFileName,0
		.endif
		popad
		
		invoke	LoadLibraryW,@lpuszFileName
		
		pushad
		mov	edi,offset TABLE_Kernel32
		invoke	HeapFree,[edi].hHeap,NULL,@lpuszFileName
		popad
		ret
MyLoadLibraryA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyGetModuleHandleA	proc	uses ebx esi edi lpModuleName
			local	@dwModuleName
			local	@lpuszModuleName

			pushad
			mov	edi,offset TABLE_Kernel32
			assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
			.if	lpModuleName
				invoke	lstrlen,lpModuleName
				inc	eax
				mov	@dwModuleName,eax
				shl	eax,1
				invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
				mov	@lpuszModuleName,eax
				invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpModuleName,-1,@lpuszModuleName,@dwModuleName
			.else
				mov	@lpuszModuleName,0
			.endif
			popad
			
			invoke	GetModuleHandleW,@lpuszModuleName
			
			pushad
			mov	edi,offset TABLE_Kernel32
			invoke	HeapFree,[edi].hHeap,NULL,@lpuszModuleName
			popad
			ret
MyGetModuleHandleA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyGetACP		proc	uses ebx esi edi

			mov	edi,offset TABLE_Kernel32
			assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
			mov	eax,[edi].dwACP
			ret
MyGetACP		endp