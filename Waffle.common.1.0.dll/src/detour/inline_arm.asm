        area	.text, code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleGetCallersAddress	proc
		export	WaffleGetCallersAddress
		mov	r0,0
		;mov	r1,0
		mov	pc,lr
WaffleGetCallersAddress	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineHandler	proc
		export	WaffleInlineHandler
		bkpt	0
		mov	r0,0
		;mov	r1,0
		mov	pc,lr
WaffleInlineHandler	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineDetour	proc
		export	WaffleInlineDetour
		mov	r0,0
		;mov	r1,0
		mov	pc,lr
WaffleInlineDetour	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end