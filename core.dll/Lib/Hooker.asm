USE_HOOK_DISPATCHER	equ	1
		.const
bOriginalCode	db	90h,90h,90h,90h,90h,8Bh,0FFh
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_HotPatching	proc	uses ebx edi esi,_lpszDllName,_lpszProc,_lpNew,_lplpOri
		local	@hDll,@lpOldProc
		local	@OldProtect

;********************************************************************
;参数检查
;********************************************************************
		.if	!(_lpszDllName && _lpszProc && _lpNew)
			mov	eax,ERROR_INVALID_ADDRESS
			ret
		.endif
;********************************************************************
;获得函数地址
;********************************************************************
		invoke	GetModuleHandle,_lpszDllName
		.if	!eax
			mov	eax,ERROR_INVALID_NAME
			ret
		.else
			mov	@hDll,eax
		.endif

		invoke	GetProcAddress,eax,_lpszProc
		.if	!eax
			mov	eax,ERROR_INVALID_FUNCTION
			ret
		.else
			mov	@lpOldProc,eax
			mov	ebx,_lplpOri
			mov	[ebx],eax
			add	dword ptr [ebx],2	;返回值已经绕过了函数头部
		.endif
;********************************************************************
;判断是否可以Hot-Patching
;
;C2 XXXX	retn	XXXX		Addr-8
;90		nop			Addr-5
;90		nop
;90		nop
;90		nop
;90		nop
;8BFF		mov	edi,edi		Addr
;55		push	ebp		Addr+1
;8BEC		mov	ebp,esp		Addr+3
;********************************************************************
;bOriginalCode	db	90h,90h,90h,90h,90h,8Bh,0FFh
		mov	eax,@lpOldProc
		lea	esi,[eax-5]
		mov	edi,offset bOriginalCode
		mov	ecx,sizeof bOriginalCode
		cld
		repe	cmpsb
		.if	!ZERO?
			mov	eax,ERROR_NOT_SUPPORTED
			ret
		.endif
;********************************************************************
;修改分页保护属性:R E->RWE
;********************************************************************
		mov	eax,@lpOldProc
		lea	edi,[eax-5]
		invoke	VirtualProtect,edi,7,PAGE_EXECUTE_READWRITE,addr @OldProtect
		.if	!eax
			mov	eax,ERROR_ACCESS_DENIED
			ret
		.endif
;********************************************************************
;写入远程跳转
;bJmpFar	db	0E9h
;bCallNear	db	0E8h
;********************************************************************
ifdef		USE_HOOK_DISPATCHER
		mov	eax,0E8h
else
		mov	eax,0E9h
endif
		cld
		stosb
;********************************************************************
;计算Jmp的偏移
;********************************************************************
		mov	eax,_lpNew
		sub	eax,edi
		sub	eax,4
		cld
		stosd
;********************************************************************
;写入短跳转
;bJmpShort		db	0EBh,0F9h
;********************************************************************
		mov	word ptr [edi],0F9EBh
;********************************************************************
;恢复分页保护属性:RWE->R E
;********************************************************************
		mov	eax,@lpOldProc
		lea	edi,[eax-5]
		mov	ebx,@OldProtect
		invoke	VirtualProtect,edi,7,ebx,addr @OldProtect
		.if	!eax
			mov	eax,ERROR_CALL_NOT_IMPLEMENTED
			ret
		.endif

		mov	eax,ERROR_SUCCESS
		ret

_HotPatching	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_UnPatching	proc	uses ebx edi esi,_lpszDllName,_lpszProc
		local	@hDll,@lpOldProc
		local	@OldProtect

;********************************************************************
;参数检查
;********************************************************************
		.if	!(_lpszDllName && _lpszProc)
			mov	eax,ERROR_INVALID_ADDRESS
			ret
		.endif
;********************************************************************
;获得函数地址
;********************************************************************
		invoke	GetModuleHandle,_lpszDllName
		.if	!eax
			mov	eax,ERROR_INVALID_NAME
			ret
		.else
			mov	@hDll,eax
		.endif

		invoke	GetProcAddress,eax,_lpszProc
		.if	!eax
			mov	eax,ERROR_INVALID_FUNCTION
			ret
		.else
			mov	@lpOldProc,eax
		.endif
;********************************************************************
;修改分页保护属性:R E->RWE
;********************************************************************
		invoke	VirtualProtect,@lpOldProc,2,PAGE_EXECUTE_READWRITE,addr @OldProtect
		.if	!eax
			mov	eax,ERROR_ACCESS_DENIED
			ret
		.endif
;********************************************************************
;解钩
;********************************************************************
		mov	ebx,@lpOldProc
		mov	word ptr [ebx],0FF8Bh
;********************************************************************
;恢复分页保护属性:RWE->R E
;********************************************************************
		mov	edx,@OldProtect
		invoke	VirtualProtect,@lpOldProc,2,edx,addr @OldProtect
		.if	!eax
			mov	eax,ERROR_CALL_NOT_IMPLEMENTED
			ret
		.endif

		mov	eax,ERROR_SUCCESS
		ret

_UnPatching	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Dispatcher	proc
; 堆栈示意	ebp=载入时的esp
;[ebp]		原调用EBP
;[ebp+4]	Hook调用压入的函数地址
;[ebp+8]	原调用地址
;[ebp+12]	原调用的第一个参数
;[ebp+16]	原调用的第二个参数
;[ebp+20]	原调用的第三个参数
;[ebp+24]	原调用的第四个参数
		
		push	ebp
		mov	ebp,esp		
		
		;push	_P4
		push	[ebp+24]
		push	0
		push	[ebp+16]
		;push	_P1
		push	[ebp+12]
		call	lpMessageBoxA
		
		pop	ebp
		add	esp,4+4+4*4	;Hook调用地址+原调用地址+参数
		push	[esp-4-4*4]	;压入原调用地址
		ret
_Dispatcher	endp