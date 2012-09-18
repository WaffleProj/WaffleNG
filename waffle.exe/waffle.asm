		.386
		.model flat,stdcall
		option casemap:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
UNICODE		equ	TRUE

include		windows.inc
includelib	kernel32.lib
includelib	user32.lib

include		Console.inc
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		
		.data?
hStdIn		dd	?	;控制台输入句柄
hStdOut		dd	?	;控制台输出句柄
hStdErr		dd	?	;控制台错误句柄
dwBytesRead	dd	?
dwBytesWrite	dd	?

uszBuf		dw	MAX_PATH dup (?)
		.const
uszTitle	dw	"Waffle v0.10",0
uszTest		dw	"ABC",0
uszTest2	dw	"DEF",0
uszFmt		dw	"%08X",0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
include		_CmdLine.asm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke	_SetConsole,addr uszTitle
		invoke	_argc
		mov	ebx,eax
		invoke	wsprintf,addr uszBuf,addr uszFmt,eax
		invoke	lstrlen,addr uszBuf
		Coutn	uszBuf,eax
		Creturn
		.while	ebx
			dec	ebx
			invoke	_argv,ebx,addr uszBuf,sizeof uszBuf
			invoke	lstrlen,addr uszBuf
			Coutn	uszBuf,eax
			Creturn
		.endw
		ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start