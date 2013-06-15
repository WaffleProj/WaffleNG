		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_GetPEMagic	proc	_lpuszTarget
		local	@hFile,@hMapFile,@lpFile

		push	ebx
		push	esi
		push	edi
		invoke	CreateFile,_lpuszTarget,GENERIC_READ,NULL,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
		mov	@hFile,eax
		invoke	CreateFileMapping,eax,NULL,PAGE_READONLY,NULL,NULL,NULL
		mov	@hMapFile,eax
		invoke	MapViewOfFile,eax,FILE_MAP_READ,NULL,NULL,NULL
		mov	@lpFile,eax
		mov	ebx,eax
		.if	!ebx
			push	0
			jmp	@F
		.endif
		
		assume	ebx:ptr IMAGE_DOS_HEADER
		movzx	eax,[ebx].e_magic
		.if	eax !=	00005A4Dh
			push	-1
		.else
			mov	eax,[ebx].e_lfanew
			add	eax,@lpFile
			mov	ebx,eax
			assume	ebx:ptr IMAGE_NT_HEADERS
			mov	eax,[ebx].Signature
			.if	eax !=	00004550h
				push	-1
			.else
				movzx	eax,[ebx].FileHeader.Characteristics
				and	eax,2000h	;2000h	File is a DLL
				.if	eax
					push	-1
				.else
					movzx	eax,[ebx].OptionalHeader.Magic
					push	eax
				.endif
			.endif
		.endif
		assume	ebx:nothing
@@:
		invoke	UnmapViewOfFile,@lpFile
		invoke	CloseHandle,@hMapFile
		invoke	CloseHandle,@hFile
		pop	eax
		pop	edi
		pop	esi
		pop	ebx
		ret
_GetPEMagic	endp