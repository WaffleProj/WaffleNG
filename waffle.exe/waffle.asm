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

uszBuf		dw	100 dup (?)
		.const
uszTitle	dw	"Waffle v0.10",0
usz0Dh		dw	0dh
uszBOM		dw	0FEFFh
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
lop:
		Cout	offset uszTest,3
		Cout	offset usz0Dh,1
		invoke	Sleep,1000
		Cout	offset uszTest2,3
		Cout	offset usz0Dh,1
		invoke	Sleep,1000
		jmp	lop
		invoke	Sleep,5000
		ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start