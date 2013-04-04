		.386
		.model flat,stdcall
		option casemap:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;Using MASM32
include		windows.inc
include		kernel32.inc
includelib	kernel32.lib
include		user32.inc
includelib	user32.lib
include		psapi.inc
includelib	psapi.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
ifndef	MODULEINFO
MODULEINFO	struct
	lpBaseOfDll	dd	?
	SizeOfImage	dd	?
	EntryPoint	dd	?
MODULEINFO	ends
endif
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
hProcess	dd	?
hModule		dd	?

lpNewDll	dd	?
dwMessageBoxA	dd	?
		.const
szUser32	db	"User32.dll",0
szMessageBoxA	db	"MessageBoxA",0

szModuleInfoFmt	db	"lpBaseOfDll = %08x",0dh,0ah
		db	"SizeOfImage = %08x",0dh,0ah
		db	"EntryPoint = %08x",0dh,0ah,0

szMemInfoFmt	db	"BaseAddress = %08x",0dh,0ah
		db	"AllocationBase = %08x",0dh,0ah
		db	"AllocationProtect = %08x",0dh,0ah
		db	"RegionSize = %08x",0dh,0ah
		db	"State = %08x",0dh,0ah
		db	"Protect = %08x",0dh,0ah
		db	"Type = %08x",0dh,0ah,0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start		proc
		local	@stModuleInfo:MODULEINFO
		local	@stMemInfo:MEMORY_BASIC_INFORMATION
		;local	@szBuf[256]

		;Get dll base address and size
		invoke	GetCurrentProcess
		mov	hProcess,eax
		invoke	GetModuleHandle,addr szUser32
		.if	!eax
			invoke	LoadLibrary,addr szUser32
		.endif
		mov	hModule,eax
		invoke	GetModuleInformation,hProcess,hModule,addr @stModuleInfo,sizeof @stModuleInfo
		
		;invoke	wsprintf,addr @szBuf,addr szModuleInfoFmt,@stModuleInfo.lpBaseOfDll,@stModuleInfo.SizeOfImage,@stModuleInfo.EntryPoint
		;invoke	MessageBox,0,addr @szBuf,0,0
		
		;Reserve memory address
		invoke	VirtualAlloc,NULL,@stModuleInfo.SizeOfImage,MEM_RESERVE,PAGE_NOACCESS
		mov	lpNewDll,eax
		
		mov	edi,@stModuleInfo.lpBaseOfDll
		mov	esi,edi
		add	esi,@stModuleInfo.SizeOfImage
		.while	edi < esi
			invoke	VirtualQuery,edi,addr @stMemInfo,sizeof @stMemInfo
			.if	@stMemInfo.State == MEM_COMMIT
				;invoke	wsprintf,addr @szBuf,addr szMemInfoFmt,@stMemInfo.BaseAddress,@stMemInfo.AllocationBase,@stMemInfo.AllocationProtect,@stMemInfo.RegionSize,@stMemInfo.State,@stMemInfo.Protect,@stMemInfo.lType
				;invoke	MessageBox,0,addr @szBuf,0,0
				
				;Get offset of the memory
				mov	ebx,@stMemInfo.BaseAddress
				sub	ebx,@stMemInfo.AllocationBase
				add	ebx,lpNewDll

				invoke	VirtualAlloc,ebx,@stMemInfo.RegionSize,MEM_COMMIT,PAGE_READWRITE
				invoke	RtlMoveMemory,ebx,@stMemInfo.BaseAddress,@stMemInfo.RegionSize
				invoke	VirtualProtect,ebx,@stMemInfo.RegionSize,@stMemInfo.Protect,addr @stMemInfo.AllocationProtect
			.endif
			mov	edi,@stMemInfo.BaseAddress
			add	edi,@stMemInfo.RegionSize
		.endw

		;Get offset of MessageBoxA
		invoke	GetProcAddress,hModule,addr szMessageBoxA
		sub	eax,@stModuleInfo.lpBaseOfDll
		mov	dwMessageBoxA,eax

		;Call new MessageBoxA
		push	0
		push	0
		push	offset szUser32
		push	0
		mov	eax,lpNewDll
		add	eax,dwMessageBoxA
		call	eax
		
		invoke	ExitProcess,0
		ret
start		endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start