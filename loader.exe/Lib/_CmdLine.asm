;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Sample code for < Win32ASM Programming 3rd Edition>
; by 罗云彬, http://www.win32asm.com.cn
; Edited by Excalibur, Support Unicode, Get the point
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; _CmdLine.asm
; 命令行参数分析的通用子程序
; 功能：
; _argc ---> 对命令行参数进行数量统计
; _argv ---> 取某个命令行参数
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
CHAR_BLANK	equ	20h	;定义空格
CHAR_DELI	equ	'"'	;定义分隔符
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 取命令行参数个数 (arg count)
; 参数个数必定大于等于 1, 参数 1 为当前执行文件名
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_argc		proc	uses ebx esi edi
		local	@dwArgc

		mov	@dwArgc,0
		invoke	GetCommandLine
		mov	esi,eax
		cld
_argc_loop:
;********************************************************************
; 忽略参数之间的空格
;********************************************************************
		ifdef	UNICODE
			lodsw
			or	ax,ax
			jz	_argc_end
			cmp	ax,CHAR_BLANK
			jz	_argc_loop
		else
			lodsb
			or	al,al
			jz	_argc_end
			cmp	al,CHAR_BLANK
			jz	_argc_loop
		endif
;********************************************************************
; 一个参数开始
;********************************************************************
		ifdef	UNICODE
			dec	esi
			dec	esi
		else
			dec	esi
		endif
		inc	@dwArgc
_argc_loop1:
		ifdef	UNICODE
			lodsw
			or	ax,ax
			jz	_argc_end
			cmp	ax,CHAR_BLANK
			jz	_argc_loop		;参数结束
			cmp	ax,CHAR_DELI
			jnz	_argc_loop1		;继续处理参数内容
		else
			lodsb
			or	al,al
			jz	_argc_end
			cmp	al,CHAR_BLANK
			jz	_argc_loop		;参数结束
			cmp	al,CHAR_DELI
			jnz	_argc_loop1		;继续处理参数内容
		endif
;********************************************************************
; 如果一个参数中的一部分有空格,则用 " " 包括
;********************************************************************
		@@:
		ifdef	UNICODE
			lodsw
			or	ax,ax
			jz	_argc_end
			cmp	ax,CHAR_DELI
			jnz	@B
			jmp	_argc_loop1
		else
			lodsb
			or	al,al
			jz	_argc_end
			cmp	al,CHAR_DELI
			jnz	@B
			jmp	_argc_loop1
		endif
_argc_end:
		mov	eax,@dwArgc
		ret

_argc		endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 取指定位置的命令行参数
;  argv 0 = 执行文件名
;  argv 1 = 参数1 ...
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_argv		proc	uses ebx esi edi _dwArgv,_lpReturn,_dwSize
		local	@dwArgv,@dwFlag

		inc	_dwArgv
		mov	@dwArgv,0
		mov	edi,_lpReturn

		invoke	GetCommandLine
		mov	esi,eax
		cld
_argv_loop:
;********************************************************************
; 忽略参数之间的空格
;********************************************************************
		ifdef	UNICODE
			lodsw
			or	ax,ax
			jz	_argv_end
			cmp	ax,CHAR_BLANK
			jz	_argv_loop
		else
			lodsb
			or	al,al
			jz	_argv_end
			cmp	al,CHAR_BLANK
			jz	_argv_loop
		endif
;********************************************************************
; 一个参数开始
; 如果和要求的参数符合,则开始复制到返回缓冲区
;********************************************************************
		ifdef	UNICODE
			dec	esi
			dec	esi
		else
			dec	esi
		endif
		inc	@dwArgv
		mov	@dwFlag,FALSE
		mov	eax,_dwArgv
		cmp	eax,@dwArgv
		jnz	@F
		mov	@dwFlag,TRUE
		@@:
_argv_loop1:
		ifdef	UNICODE
			lodsw
			or	ax,ax
			jz	_argv_end
			cmp	ax,CHAR_BLANK
			jz	_argv_loop		;参数结束
			cmp	ax,CHAR_DELI
			jz	_argv_loop2
			cmp	_dwSize,2
			jle	@F
			cmp	@dwFlag,TRUE
			jne	@F
			stosw
			dec	_dwSize
			dec	_dwSize
		else
			lodsb
			or	al,al
			jz	_argv_end
			cmp	al,CHAR_BLANK
			jz	_argv_loop		;参数结束
			cmp	al,CHAR_DELI
			jz	_argv_loop2
			cmp	_dwSize,1
			jle	@F
			cmp	@dwFlag,TRUE
			jne	@F
			stosb
			dec	_dwSize
		endif
		@@:
		jmp	_argv_loop1		;继续处理参数内容

_argv_loop2:
		ifdef	UNICODE
			lodsw
			or	ax,ax
			jz	_argv_end
			cmp	ax,CHAR_DELI
			jz	_argv_loop1
			cmp	_dwSize,2
			jle	@F
			cmp	@dwFlag,TRUE
			jne	@F
			stosw
			dec	_dwSize
			dec	_dwSize
		else
			lodsb
			or	al,al
			jz	_argv_end
			cmp	al,CHAR_DELI
			jz	_argv_loop1
			cmp	_dwSize,1
			jle	@F
			cmp	@dwFlag,TRUE
			jne	@F
			stosb
			dec	_dwSize
		endif
		@@:
		jmp	_argv_loop2
_argv_end:
		xor	eax,eax
		ifdef	UNICODE
			stosw
		else
			stosb
		endif
		ret

_argv		endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 返回指定位置的命令行参数地址
;  argp 0 = 执行文件名
;  argp 1 = 参数1 ...
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_argp		proc	uses ebx esi edi _dwArgp
		local	@dwArgp

		xor	eax,eax
		mov	@dwArgp,eax
		mov	ecx,_dwArgp
		inc	ecx
		invoke	GetCommandLine
		mov	esi,eax
		cld
_argp_loop:
;********************************************************************
; 忽略参数之间的空格
;********************************************************************
		ifdef	UNICODE
			lodsw
			or	ax,ax
			jz	_argp_end
			cmp	ax,CHAR_BLANK
			jz	_argp_loop
		else
			lodsb
			or	al,al
			jz	_argp_end
			cmp	al,CHAR_BLANK
			jz	_argp_loop
		endif
;********************************************************************
; 一个参数开始
;********************************************************************
		ifdef	UNICODE
			dec	esi
			dec	esi
		else
			dec	esi
		endif
		inc	@dwArgp
		.if	ecx ==	@dwArgp
			jmp	_argp_end
		.endif
;********************************************************************
; 读取字符
;********************************************************************
		ifdef	UNICODE
			.while	TRUE
				lodsw
				cmp	ax,CHAR_BLANK
				jz	_argp_loop
				.if	ax ==	CHAR_DELI
					inc	eax
					.while	ax
						.break	.if ax == CHAR_DELI
						lodsw
					.endw
				.endif
				.break	.if ax == NULL
			.endw
		else
			.while	TRUE
				lodsb
				cmp	al,CHAR_BLANK
				jz	_argp_loop
				.if	al ==	CHAR_DELI
					inc	eax
					.while	al
						.break	.if al == CHAR_DELI
						lodsb
					.endw
				.endif
				.break	.if al == NULL
			.endw
		endif
;********************************************************************
; 结束
;********************************************************************
_argp_end:
		ifdef	UNICODE
			.if	ax
				mov	eax,esi
			.else
				xor	eax,eax
			.endif
		else
			.if	al
				mov	eax,esi
			.else
				xor	eax,eax
			.endif

		endif
		ret

_argp		endp
