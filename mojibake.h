#ifndef __MOJIBAKE_H_
#define __MOJIBAKE_H_

#define _In_
#define _In_opt_
#define _Out_
#define _Out_opt_
#define _Reserved_

#define	TM_FIRSTMESSAGE		TM_SETMAINIP
#define	TM_SETMAINIP     	WM_USER + 1
#define	TM_RESUMETMAINIP	WM_USER + 2
#define	TM_LASTMESSAGE		TM_RESUMETMAINIP

#ifdef __cplusplus
extern "C" {
#endif

#undef  RtlZeroMemory
VOID WINAPI RtlZeroMemory(
  _In_  VOID UNALIGNED *Destination,
  _In_  SIZE_T Length
);

#undef  RtlMoveMemory
VOID WINAPI RtlMoveMemory(
  _In_  VOID UNALIGNED *Destination,
  _In_  const VOID UNALIGNED *Source,
  _In_  SIZE_T Length
);

#ifdef __cplusplus
};
#endif

#endif /* __MOJIBAKE_H_ */