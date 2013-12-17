		extern	VirtualProtect
		extern	FlushInstructionCache
		extern	GetCurrentProcess

		extern	WaffleFindDetourAddress
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		area	.rdata, data, readonly
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		area	.text, code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineHandler	proc
		export	WaffleInlineHandler
		dcw	0xDEFE	;__debugbreak
		mov	r0,0
		bx	lr
WaffleInlineHandler	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineDetour	proc
		export	WaffleInlineDetour
		mov	r0,0
		bx	lr
WaffleInlineDetour	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end