		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_DispatchGdi32	proc
		add	dword ptr [esp],2	;修正返回值
		push	ebp
		mov	ebp,esp
		sub	esp,4	;制造一个局部变量
		pushad
		
		mov	edi,offset TABLE_Gdi32
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
_DispatchGdi32	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyCreateFontA	proc	uses ebx esi edi nHeight,nWidth,nEscapement,nOrientation,fnWeight,fdwItalic,fdwUnderline,fdwStrikeOut,fdwCharSet,fdwOutputPrecision,fdwClipPrecision,fdwQuality,fdwPitchAndFamily,lpszFace
		local	@dwFace
		local	@lpuszFace

		pushad
		mov	edi,offset TABLE_Gdi32
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		.if	lpszFace
			invoke	lstrlen,lpszFace
			inc	eax
			mov	@dwFace,eax
			shl	eax,1
			invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
			mov	@lpuszFace,eax
			invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpszFace,-1,@lpuszFace,@dwFace
		.else
			mov	@lpuszFace,0
		.endif
		;invoke	MessageBoxW,0,@lpuszFace,0,0
		popad
		
		invoke	CreateFontW,nHeight,nWidth,nEscapement,nOrientation,fnWeight,fdwItalic,fdwUnderline,fdwStrikeOut,fdwCharSet,fdwOutputPrecision,fdwClipPrecision,fdwQuality,fdwPitchAndFamily,@lpuszFace
		
		pushad
		mov	edi,offset TABLE_Gdi32
		invoke	HeapFree,[edi].hHeap,NULL,@lpuszFace
		popad
		ret
MyCreateFontA	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MyTextOutA	proc	uses ebx esi edi hdc,nXStart,nYStart,lpString,cchString
		local	@dwString
		local	@lpuszString

		pushad
		mov	edi,offset TABLE_Gdi32
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		.if	lpString
			mov	eax,cchString
			inc	eax
			mov	@dwString,eax
			shl	eax,1
			invoke	HeapAlloc,[edi].hHeap,HEAP_ZERO_MEMORY,eax
			mov	@lpuszString,eax
			invoke	MultiByteToWideChar,[edi].dwACP,NULL,lpString,cchString,@lpuszString,@dwString
		.else
			mov	@lpuszString,0
		.endif
		;invoke	MessageBoxW,0,@lpuszString,0,0
		popad
		
		invoke	TextOutW,hdc,nXStart,nYStart,@lpuszString,cchString
		
		pushad
		mov	edi,offset TABLE_Gdi32
		invoke	HeapFree,[edi].hHeap,NULL,@lpuszString
		popad
		ret
MyTextOutA	endp