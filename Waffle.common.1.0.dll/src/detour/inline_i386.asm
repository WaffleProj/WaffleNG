		.386
		.model flat,stdcall
		option casemap:none		
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
FALSE			equ	0
TRUE			equ	1
PAGE_EXECUTE		equ	10h
PAGE_EXECUTE_READ	equ	20h
PAGE_EXECUTE_READWRITE	equ	40h
PAGE_EXECUTE_WRITECOPY	equ	80h
VirtualProtect		proto	:DWORD,:DWORD,:DWORD,:DWORD
FlushInstructionCache	proto	:DWORD,:DWORD,:DWORD
GetCurrentProcess	proto

WaffleFindDetourAddress	proto	:DWORD,:DWORD
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		option  prologue:none
		option  epilogue:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineHandler	proc

		mov	ecx,dword ptr [esp]	;HotpatchReturnAddress
		mov	edx,dword ptr [esp+4]	;CallerReturnAddress
		invoke	WaffleFindDetourAddress,ecx,edx
		.if	eax
			mov	dword ptr [esp],eax
		.else
			inc	dword ptr [esp]
			inc	dword ptr [esp]
		.endif
		ret
WaffleInlineHandler	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		option  prologue:PrologueDef
		option  epilogue:EpilogueDef
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineDetour	proc	uses ebx esi edi lpFunction
			local	flOldProtect

		.const
Signature	db	90h,90h,90h,90h,90h,8Bh,0FFh
		.code
		mov	esi,offset Signature
		mov	ebx,lpFunction
		sub	ebx,5
		mov	edi,ebx
		mov	ecx,7
		repne	cmpsb
		.if	!ZERO?
			mov	eax,FALSE
			ret
		.endif

		;large address aware?
		mov	eax,lpFunction
		sub	eax,WaffleInlineHandler
		.if	OVERFLOW?
			neg	eax
		.endif
		shl	eax,1
		.if	ZERO?	;if the highest bit is 1, the difference is greater than 7FFFFFFFh
			mov	eax,FALSE
			ret
		.endif

		invoke	VirtualProtect,lpFunction,7,PAGE_EXECUTE_READWRITE, addr flOldProtect

		.const
Hotpatch	db	0E8h,0EBh,0F9h
		.code
		mov	esi,offset Hotpatch
		mov	edi,ebx
		movsb

		mov	eax,WaffleInlineHandler
		sub	eax,lpFunction
		stosd

		movsw

		invoke	GetCurrentProcess
		invoke	FlushInstructionCache,eax,ebx,7
		invoke	VirtualProtect,ebx,7,flOldProtect,addr flOldProtect

		mov	eax,TRUE
		ret
WaffleInlineDetour	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end