HOOK_TABLE_HEAD_OBJECT	struct
	lpDispatch	dd	?
HOOK_TABLE_HEAD_OBJECT	ends

HOOK_TABLE_BEGIN macro	_Laber:LABEL,_Dispatch:=<?>
ifidn	<%_Dispatch>,<?>
_Dispatch	proto
endif
_Laber		HOOK_TABLE_HEAD_OBJECT	<_Dispatch>
		endm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
HOOK_TABLE_OBJECT	struct
	lpNewFunction	dd	?
	lpOldFunction	dd	?
	lpszFunction	dd	?
	lpszModule	dd	?
HOOK_TABLE_OBJECT	ends

HOOK_TABLE	macro	_Laber:LABEL,_DLL,_Arg
		local	stFunction,NewFunction,szFunction,szModule
		local	protoFunction,apiFunction,count,protoDefine,protoDefineTemp
		local	temp_struct
		
		protoFunction	CatStr	<proto>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		apiFunction	CatStr	<api>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		count	textequ	%_Arg
		if	count ne 0
			protoDefine	textequ	<:dword>
			count	textequ	%count-1
			repeat	count
				protoDefineTemp	textequ	@CatStr(<protoDefine>,<,:dword>)
				protoDefine	textequ	protoDefineTemp
				count	textequ	%count-1
			endm
		else
			protoDefine	textequ	<>
		endif
protoFunction	typedef	proto	protoDefine
apiFunction	typedef	ptr	protoFunction
temp_struct	struct
	lpNewFunction	apiFunction	?
	lpOldFunction	apiFunction	?
	lpszFunction	dd	?
	lpszModule	dd	?
temp_struct	ends

		stFunction	CatStr	<st>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		NewFunction	CatStr	<New>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		szFunction	CatStr	<sz>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2)
		szModule	CatStr	<sz>,@SubStr(<_DLL>,2,@SizeStr(<_DLL>)-2)

		.const
NewFunction	proto	protoDefine
szFunction	db	@CatStr(<!'>,@SubStr(<_Laber>,2,@SizeStr(<_Laber>)-2),<!'>),0
		.data
externdef	stFunction:temp_struct
stFunction	temp_struct	<offset	NewFunction,0,offset szFunction,offset szModule>

		endm
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
HOOK_TABLE_END	macro
		HOOK_TABLE_OBJECT	<?>
		endm