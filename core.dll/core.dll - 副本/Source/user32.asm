		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_DispatchUser32	proc
		add	dword ptr [esp],2	;修正返回值
		push	ebp
		mov	ebp,esp
		sub	esp,4	;制造一个局部变量
		pushad
		
		mov	edi,offset TABLE_User32
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
_DispatchUser32	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyMessageBoxExA	proc	uses ebx esi edi hWnd,lpText,lpCaption,uType,wLanguageId
		local	@dwText,@dwCaption
		local	@lpuszText,@lpuszCaption

		;invoke	MessageBoxW,0,0,0,0
		;ret
		
		pushad
		mov	edi,offset TABLE_User32
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		.if	lpText
			invoke	lstrlen,lpText
			inc	eax
			mov	@dwText,eax
			shl	eax,1
			invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
			mov	@lpuszText,eax
			invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpText,-1,@lpuszText,@dwText
		.else
			mov	@lpuszText,0
		.endif
		.if	lpCaption
			invoke	lstrlen,lpCaption
			inc	eax
			mov	@dwCaption,eax
			shl	eax,1
			invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
			mov	@lpuszCaption,eax
			invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpCaption,-1,@lpuszCaption,@dwCaption
		.else
			mov	@lpuszCaption,0
		.endif
		popad
		
		invoke	MessageBoxExW,hWnd,@lpuszText,@lpuszCaption,uType,wLanguageId	;需要修改winuser.inc中对MessageBoxExW的定义
		;invoke	lpMessageBoxExA,hWnd,lpText,lpCaption,uType,wLanguageId
		
		pushad
		mov	edi,offset TABLE_User32
		invoke	HeapFree,[edi].hHeap,NULL,@lpuszText
		invoke	HeapFree,[edi].hHeap,NULL,@lpuszCaption
		popad
		ret
MyMessageBoxExA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MySetWindowTextA	proc	uses ebx esi edi hWnd,lpString
			local	@dwString
			local	@lpuszString

			pushad
			mov	edi,offset TABLE_User32
			assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
			.if	lpString
				invoke	lstrlen,lpString
				inc	eax
				mov	@dwString,eax
				shl	eax,1
				invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
				mov	@lpuszString,eax
				invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpString,-1,@lpuszString,@dwString
			.else
				mov	@lpuszString,0
			.endif
			popad
			
			invoke	SetWindowTextW,hWnd,@lpuszString
			
			pushad
			mov	edi,offset TABLE_User32
			invoke	HeapFree,[edi].hHeap,NULL,@lpuszString
			popad
			ret
MySetWindowTextA	endp