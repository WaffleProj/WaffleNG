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

		mov	rcx,qword ptr [rsp]	;HotpatchReturnAddress
		mov	rdx,qword ptr [rsp+16]	;CallerReturnAddress
		sub	rsp,20h
		call	WaffleFindDetourAddress
		add	rsp,20h
		
		test	rax,rax
		je	label2
		mov	qword ptr [rsp],rax
		ret
label2:
		inc	qword ptr [rsp]
		inc	qword ptr [rsp]
		ret
WaffleInlineHandler	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		option prologue:PrologueDef
		option epilogue:EpilogueDef
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineDetour	proc	lpFunction
			local	flOldProtect

		mov	eax,FALSE
		ret
WaffleInlineDetour	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end