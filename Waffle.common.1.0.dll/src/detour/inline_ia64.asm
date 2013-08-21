.text;
.global	WaffleGetCallersAddress;
.global	WaffleInlineHandler;
.global	WaffleInlineDetour;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
.proc	WaffleGetCallersAddress;
WaffleGetCallersAddress::

		mov		r8 = 0
		br.ret.sptk.few	rp
.endp	WaffleGetCallersAddress;
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