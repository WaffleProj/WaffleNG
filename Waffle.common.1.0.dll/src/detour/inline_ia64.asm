.text;
.global	WaffleInlineHandler;
.global	WaffleInlineDetour;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
.proc	WaffleInlineHandler;
WaffleInlineHandler::

		break		0
		mov		r8 = 0
		br.ret.sptk.few	rp
.endp	WaffleInlineHandler;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
.proc	WaffleInlineDetour;
WaffleInlineDetour::

		mov		r8 = 0
		br.ret.sptk.few	rp
.endp	WaffleInlineDetour;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>