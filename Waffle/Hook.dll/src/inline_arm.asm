		extern	VirtualProtect
		extern	FlushInstructionCache
		extern	GetCurrentProcess

		extern	WaffleHookDBLookup
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		area	.rdata, data, readonly
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Patch		b	.
		mov	r0,pc		;second instruction
		ldr	pc,[pc,#0]
		dcd	WaffleInlineHandler
		push	{r0-r3}		;first instruction
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		area	.text, code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineHandler	proc
		export	WaffleInlineHandler
		
		push	{r12,lr}
		sub	sp,sp,#8
		
		str	r0,[sp]
		mrs	r1,cpsr
		str	r1,[sp,#4]
		
		sub	r0,r0,#6
		orr	r0,r0,#1
		mov	r1,lr
		blx	WaffleHookDBLookup
		
		tst	r0,r0
		it	ne
		orrne	r0,r0,#1
		strne	r0,[sp]
		bne	label1
		;dcw	0xDEFE	;__debugbreak
		bkpt	0
label1
		ldr	r1,[sp,#4]
		msr	cpsr_fc,r1
		add	sp,sp,#8
		pop	{r12,lr}
		pop	{r0-r3}
		ldr	pc,[sp,#-32]
WaffleInlineHandler	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaffleInlineDetour	proc	;lpFunction
		export	WaffleInlineDetour
		
		and	r0,r0,#-2
		push	{r0,r4,lr}
		sub	sp,sp,#4
		
		ldr	r0,[sp,#4]	;mov	r0,lpFunction
		mov	r4,#12		;mov	r4,Length
		eor	r1,r1,r1	;xor	r1,r1
label2
		ldrh	r2,[r0,r1]	;mov	r2,word ptr [r0+r1]
		add	r1,r1,#2	;add	r1,2
		tst	r2,r2		;.if	!r2
		streq	r2,[sp,#4]	;	mov	dword ptr [sp+12],r2
		beq	label4		;	jmp	label4
					;.endif
		cmp	r1,r4		;.if	r1 != r4
		bne	label2		;	jmp	label2
					;.endif
		
		ldr	r0,[sp,#4]	;mov	r0,lpFunction
		mov	r1,r4		;mov	r1,Length
		mov	r2,0x40		;mov	r2,PAGE_EXECUTE_READWRITE
		mov	r3,sp		;lea	r3,[sp]
		blx	VirtualProtect
		
		ldr	r0,[sp,#4]	;mov	r0,lpFunction
		eor	r1,r1,r1	;xor	r1,r1
		ldr	r2,=Patch
label3
		ldr	r3,[r2,r1]
		str	r3,[r0,r1]
		add	r1,r1,#4
		cmp	r1,r4
		bne	label3
		ldrh	r3,[r2,r1]
		strh	r3,[r0]
		
		blx	GetCurrentProcess
		
		mov	r0,r0		;mov	r0,r0
		ldr	r1,[sp,#4]	;mov	r1,lpFunction
		mov	r2,r4		;mov	r2,Length
		blx	FlushInstructionCache
		
		ldr	r0,[sp,#4]	;mov	r0,lpFunction
		mov	r1,r4		;mov	r1,Length
		ldr	r2,[sp]		;mov	r2,[sp]
		mov	r3,sp		;lea	r3,[sp]
		blx	VirtualProtect
		
		mov	r0,#1		;mov	r0,TRUE
		str	r0,[sp,#4]	;mov	lpFunction,r0
label4		
		add	sp,sp,#4
		pop	{r0,r4,lr}
		bx	lr
WaffleInlineDetour	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end