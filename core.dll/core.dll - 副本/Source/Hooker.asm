HOOK_TABLE_HEAD_OBJECT	struct
	lpDispatch	dd	?
	hHeap		dd	?
	dwACP		dd	?
HOOK_TABLE_HEAD_OBJECT	ends

HOOK_TABLE_BEGIN macro	_Laber:LABEL,_Dispatch:=<?>
		local	table,szFile,dllFile,lpDispatch
		table		CatStr	<TABLE_>,<_Laber>
		szFile		CatStr	<sz>,<_Laber>
		dllFile		CatStr	<_Laber>,<.dll>
		lpDispatch	CatStr	<lp>,<_Laber>,<Dispatch>

%table		equ	this byte
ifidn	<%_Dispatch>,<?>
_Dispatch	proto
endif
lpDispatch	dd	_Dispatch
		dd	?
		dd	?

szFile		db	@CatStr(<!'>,<%dllFile>,<!'>),0
		endm

HOOK_TABLE_OBJECT	struct
	hookFunction	dd	?
	lpFunction	dd	?
	szFunction	db	?
HOOK_TABLE_OBJECT	ends

HOOK_TABLE	macro	_Laber:LABEL,_Arg:=<0>
		local	protoFunction,apiFunction,hookFunction,myFunction,lpFunction,szFunction,count,protoDefine,protoDefineTemp
		protoFunction	CatStr	<proto>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		apiFunction	CatStr	<api>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		hookFunction	CatStr	<hook>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		MyFunction	CatStr	<My>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		lpFunction	CatStr	<lp>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		szFunction	CatStr	<sz>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)

		count	textequ	%_Arg
		if	count ne 0
			protoDefine	textequ	<:dword>
			count	textequ	%count-1
			repeat	count
				protoDefineTemp	textequ	@CatStr(<protoDefine>,<,:dword>)
				protoDefine	textequ	protoDefineTemp
				count	textequ	%count-1
			endm
		else
			protoDefine	textequ	<>
		endif
protoFunction	typedef	proto	protoDefine
apiFunction	typedef	ptr	protoFunction
hookFunction	dd	offset	MyFunction
lpFunction	apiFunction	?
szFunction	db	@CatStr(<!'>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2),<!'>),0
		endm
		
HOOK_TABLE_END	macro
		dd	?
		endm

USE_HOOK_DISPATCHER	equ	1
		.const
bOriginalCode	db	90h,90h,90h,90h,90h,8Bh,0FFh
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_HotPatch	proc	uses ebx edi esi,_hModule,_lpszProc,_lplpOri,_lpNew,_dwFlags
		local	@lpOldProc
		local	@OldProtect

;********************************************************************
;参数检查
;********************************************************************
		.if	!(_hModule && _lpszProc && _lpNew)
			mov	eax,ERROR_INVALID_ADDRESS
			ret
		.endif
;********************************************************************
;获得函数地址
;********************************************************************
		invoke	GetProcAddress,_hModule,_lpszProc
		.if	!eax
			mov	eax,ERROR_INVALID_FUNCTION
			ret
		.else
			mov	@lpOldProc,eax
			mov	ebx,_lplpOri
			mov	[ebx],eax		;查表的时候由于下面这一句,所以[ebp+4]需要追加2进行对比
			add	dword ptr [ebx],2	;绕过函数头部的Near Jump
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
comment	*
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
	*
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
		.if	_dwFlags == USE_HOOK_DISPATCHER
			mov	eax,0E8h
		.else
			mov	eax,0E9h
		.endif
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

_HotPatch	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_RemoveHotPatch	proc	uses ebx edi esi,_hModule,_lpszProc
		local	@lpOldProc
		local	@OldProtect

;********************************************************************
;参数检查
;********************************************************************
		.if	!(_hModule && _lpszProc)
			mov	eax,ERROR_INVALID_ADDRESS
			ret
		.endif
;********************************************************************
;获得函数地址
;********************************************************************
		invoke	GetProcAddress,_hModule,_lpszProc
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
		mov	word ptr [ebx],0FF8Bh	;mov	edi,edi
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

_RemoveHotPatch	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_TableHook	proc	uses ebx edi esi _lpTable
		local	@hModule,@lpDispatch

		mov	edi,_lpTable
		add	edi,sizeof HOOK_TABLE_HEAD_OBJECT	;Dll Name
		invoke	LoadLibrary,edi
		mov	@hModule,eax

		mov	edi,_lpTable
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		.if	![edi].lpDispatch
			add	edi,sizeof HOOK_TABLE_HEAD_OBJECT
			xor	eax,eax
			mov	ecx,-1
			cld
			repnz	scasb

			assume	edi:ptr HOOK_TABLE_OBJECT
			.while	[edi].hookFunction
				;invoke	_HotPatch,@hModule,offset szMessageBoxA,offset lpMessageBoxA,_MessageBoxA,NULL
				push	NULL
				push	[edi].hookFunction	;_MessageBoxA
				lea	eax,[edi].lpFunction
				push	eax			;lpMessageBoxA
				lea	eax,[edi].szFunction
				push	eax			;offset szMessageBoxA
				push	@hModule
				call	_HotPatch
				.if	eax !=	ERROR_SUCCESS
					pushad
					invoke	MessageBox,0,addr [edi].szFunction,0,0
					popad
				.endif
				
				add	edi,sizeof HOOK_TABLE_OBJECT - sizeof HOOK_TABLE_OBJECT.szFunction
				xor	eax,eax
				mov	ecx,-1
				cld
				repnz	scasb
			.endw
		.else
			assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
			push	[edi].lpDispatch
			pop	@lpDispatch
			
			add	edi,sizeof HOOK_TABLE_HEAD_OBJECT
			xor	eax,eax
			mov	ecx,-1
			cld
			repnz	scasb
			
			assume	edi:ptr HOOK_TABLE_OBJECT
			.while	[edi].hookFunction
				;invoke	_HotPatch,@hModule,offset szMessageBoxA,offset lpMessageBoxA,_Dispatch,USE_HOOK_DISPATCHER
				push	USE_HOOK_DISPATCHER
				push	@lpDispatch	;_Dispatch
				lea	eax,[edi].lpFunction
				push	eax			;lpMessageBoxA
				lea	eax,[edi].szFunction
				push	eax			;offset szMessageBoxA
				push	@hModule
				call	_HotPatch
				.if	eax !=	ERROR_SUCCESS
					pushad
					invoke	MessageBox,0,addr [edi].szFunction,0,0
					popad
				.endif
				
				add	edi,sizeof HOOK_TABLE_OBJECT - sizeof HOOK_TABLE_OBJECT.szFunction
				xor	eax,eax
				mov	ecx,-1
				cld
				repnz	scasb
			.endw
		.endif

		mov	edi,_lpTable		;初始化
		assume	edi:ptr HOOK_TABLE_HEAD_OBJECT
		invoke	HeapCreate,0,0,0
		mov	[edi].hHeap,eax
		mov	[edi].dwACP,CP_SHIFT_JIS
		
		assume	edi:nothing
		
		ret
		
_TableHook	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Dispatch2	proc
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
		push	esi
		push	edi
		push	ebx
		
		;push	_P4
		push	[ebp+24]
		push	0
		push	[ebp+16]
		;push	_P1
		push	[ebp+12]
		;call	lpMessageBoxExA
		
		pop	ebx
		pop	edi
		pop	esi
		mov	esp,ebp
		pop	ebp
		add	esp,4+4+4*4	;Hook调用地址+原调用地址+参数
		push	[esp-4-4*4]	;压入原调用地址
		ret
_Dispatch2	endp