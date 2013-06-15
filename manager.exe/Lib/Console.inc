ifndef	UNICODE
	.err	<@FileName only works in Unicode environment.>
endif
ifdef	__JWASM__
;********************************************************************
;对关键词dw进行重定义,使其可以像db一样定义Unicode字符串
		option renamekeyword:<dw>=dw_
dw		macro	lbl:LABEL,parms:VARARG
		local	wstr
		wstr	textequ	<>
		for	parm,<parms>
			ifidn	<">,@SubStr(parm,1,1)
				% forc chr$, <@SubStr(parm,2,@SizeStr(parm)-2)>
				ifnb	wstr
					wstr	CatStr wstr,<,>
				endif
				wstr	CatStr wstr,<'&chr$'>
				endm
			elseifidn <'>,@SubStr(parm,1,1)
				% forc chr$, <@SubStr(parm,2,@SizeStr(parm)-2)>
				ifnb	wstr
					wstr	CatStr wstr,<,>
				endif
				wstr	CatStr wstr,<'&chr$'>
				endm
			else
				ifnb	wstr
					wstr	CatStr wstr,<,>
				endif
				wstr	CatStr wstr,<parm>
			endif
			endm
		lbl	dw_	wstr
		endm
endif
;********************************************************************
;设置Console
		.data?
hStdIn		dd	?	;控制台输入句柄
hStdOut		dd	?	;控制台输出句柄
hStdErr		dd	?	;控制台错误句柄
dwBytesRead	dd	?
dwBytesWrite	dd	?

_ProtoCout	typedef	proto	:dword,:dword,:dword,:dword,:dword
_ApiCout	typedef	ptr	_ProtoCout
_Cout		_ApiCout ?
		.const
uszBOM		dw	0FEFFh,0
uszCrLf		dw	0dh,0ah,0
uszPressAnyKeyToContinue dw	'P','r','e','s','s',' ','s','o','m','e',' ','k','e','y','s',' ','t','o',' ','c','o','n','t','i','n','u','e','.','.','.',0
ifdef	_WINDOWS_
	ENABLE_INSERT_MODE	equ	0020h
	ENABLE_EXTENDED_FLAGS	equ	0080h
endif
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_CtrlHandler	proc	_dwCtrlType

		pushad
		mov	eax,_dwCtrlType
		.if	eax == CTRL_C_EVENT || eax == CTRL_BREAK_EVENT || eax == CTRL_CLOSE_EVENT
			invoke	ExitProcess,0
		.endif
		popad
		mov	eax,TRUE
		ret

_CtrlHandler	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_SetConsole	proc	uses ebx edi esi,_lpTitle

		invoke	SetConsoleTitle,_lpTitle
		invoke	GetStdHandle,STD_INPUT_HANDLE
		mov	hStdIn,eax
		invoke	GetStdHandle,STD_OUTPUT_HANDLE
		mov	hStdOut,eax
		invoke	GetStdHandle,STD_ERROR_HANDLE
		mov	hStdErr,eax
		invoke	GetConsoleMode,hStdOut,addr dwBytesWrite
		.if	eax
			push	WriteConsole
			pop	_Cout
		.else
			push	WriteFile
			pop	_Cout
			invoke	WriteFile,hStdOut,addr uszBOM,2,addr dwBytesWrite,NULL
		.endif
		invoke	SetConsoleCtrlHandler,addr _CtrlHandler,TRUE
		invoke	SetConsoleMode,hStdIn,ENABLE_INSERT_MODE or ENABLE_EXTENDED_FLAGS
		ret
_SetConsole	endp
;********************************************************************
;判断是否重定向,选择适当的输出方式
Cout		macro	lpuszString

		push	NULL
		push	offset dwBytesWrite
		mov	eax,_Cout
		mov	ecx,WriteConsole
		mov	edx,WriteFile
		.if	eax == ecx
			invoke	lstrlen,lpuszString
			push	eax
		.elseif	eax == edx
			invoke	lstrlen,lpuszString
			mov	ecx,2
			mul	ecx
			push	eax
		.else
			;.err	[Line:@Line] ConsoleRediect is undefined.
			push	NULL
		.endif
		push	lpuszString
		push	hStdOut
		call	_Cout
		endm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Coutn		macro	uszString,nChar

		push	NULL
		push	offset dwBytesWrite
		
		push	nChar	;可能会覆盖
		mov	eax,_Cout
		mov	ecx,WriteConsole
		mov	edx,WriteFile
		.if	eax == ecx
			;直接使用之前的nChar
		.elseif	eax == edx
			pop	eax
			mov	ecx,2
			mul	ecx
			push	eax
		.else
			;.err	[Line:@Line] ConsoleRediect is undefined.
			pop	eax
			push	NULL
		.endif
		push	offset uszString
		push	hStdOut
		call	_Cout
		endm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Creturn		textequ	<Cout	offset uszCrLf>
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Cpause		macro
		mov	eax,_Cout
		.if	eax == WriteConsole
			Cout	offset uszPressAnyKeyToContinue
			invoke	ReadConsole,hStdIn,addr dwBytesRead,1,NULL,NULL
			Creturn
		.endif
		endm