;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
FALSE			equ	0
TRUE			equ	1
PAGE_EXECUTE		equ	10h
PAGE_EXECUTE_READ	equ	20h
PAGE_EXECUTE_READWRITE	equ	40h
PAGE_EXECUTE_WRITECOPY	equ	80h
VirtualProtect		proto	:QWORD,:QWORD,:QWORD,:QWORD
FlushInstructionCache	proto	:QWORD,:QWORD,:QWORD
GetCurrentProcess	proto

WaffleFindDetourAddress	proto	:QWORD,:QWORD
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		option prologue:none
		option epilogue:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleGetCallersAddress	proc	CallersCaller

		mov	rax,qword ptr [rsp]	;This is Caller
		xor	rax,rax			;Have no idea how to find caller's caller
		test	rcx,rcx
		je	label1
		mov	qword ptr [rcx],rax
label1:
		ret
WaffleGetCallersAddress	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineHandler	proc

		;int	3
		mov	rcx,qword ptr [rsp]	;FunctionAddress
		sub	rcx,6
		mov	rdx,qword ptr [rsp+08h]	;CallerReturnAddress
		sub	rsp,20h
		call	WaffleFindDetourAddress
		add	rsp,20h
		
		test	rax,rax
		jz	label2
		mov	qword ptr [rsp],rax
		ret
label2:
		int	3
		ret
		db	"Unknown Address",0
WaffleInlineHandler	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		option prologue:PrologueDef
		option epilogue:EpilogueDef
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineDetour	proc	;lpFunction
			;local	flOldProtect
		
		mov	rax,FALSE
		ret

		;int	3
		mov	qword ptr [rsp+8h],rsi
		mov	qword ptr [rsp+10h],rdi
		mov	qword ptr [rsp+18h],rbx
		;mov	qword ptr [rsp+20h],flOldProtect

		.const
Signature	db	90h,90h,90h,90h,90h,8Bh,0FFh
		.code
		mov	rbx,rcx			;mov	rbx,lpFunction
		
		mov	rcx,14			;jmp [rip+0](6 bytes) QWORD (8 bytes)
		mov	rdi,rbx
label3:		
		mov	rsi,offset Signature + 1
		mov	rax,90h
		repne	scasb
		jne	label4			;Jump if there is no NOP in the first 14 bytes
		mov	rax,rdi			;Back up the rdi
		inc	rax
		repe	cmpsb
		mov	rdi,rax
		jne	label3			;Jump if it doesn't match

		mov	rsi,qword ptr [rsp+8h]
		mov	rdi,qword ptr [rsp+10h]
		mov	rbx,qword ptr [rsp+18h]
		mov	rax,FALSE
		ret
label4:
		mov	rcx,rbx
		mov	rdx,14
		mov	r8,PAGE_EXECUTE_READWRITE
		lea	r9,qword ptr [rsp+20h]
		sub	rsp,20h
		call	VirtualProtect
		add	rsp,20h
		
		.const
;Spin		db	0EBh,0FEh
;CallRip0	db	0FFh,15h,00h,00h,00h,00h
Patch		db	0EBh,0FEh,00h,00h,00h,00h,0FFh,15h
		.code
		mov	rsi,offset Patch
		mov	rdi,rbx
		movsw
		movsd

		mov	rax,WaffleInlineHandler
		stosq
		
		mov	rdi,rbx
		movsw
		
		sub	rsp,20h
		call	GetCurrentProcess
		add	rsp,20h
		
		mov	rcx,rax
		mov	rdx,rbx
		mov	r8,14
		sub	rsp,20h
		call	FlushInstructionCache
		add	rsp,20h
		
		mov	rcx,rbx
		mov	rdx,14
		mov	r8,qword ptr [rsp+20h]
		lea	r9,qword ptr [rsp+20h]
		sub	rsp,20h
		call	VirtualProtect
		add	rsp,20h
		
		mov	rsi,qword ptr [rsp+8h]
		mov	rdi,qword ptr [rsp+10h]
		mov	rbx,qword ptr [rsp+18h]
		mov	rax,TRUE
		ret
WaffleInlineDetour	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end