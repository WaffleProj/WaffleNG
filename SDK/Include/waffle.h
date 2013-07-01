#ifndef __SDK_WAFFLE_H_
#define __SDK_WAFFLE_H_

#define _In_
#define _In_opt_
#define _Out_
#define _Out_opt_
#define _Reserved_

#define MACHINE_AMD64   0x8664  //IMAGE_FILE_MACHINE_AMD64
#define MACHINE_I386    0x14C   //IMAGE_FILE_MACHINE_I386

#ifdef  LIBRARY_EXPORTS
#define LIBRARY_API     __declspec(dllexport)
#else
#define LIBRARY_API     __declspec(dllimport)
#endif

#define	TM_FIRSTMESSAGE		TM_GETTID
#define	TM_GETTID     	        WM_USER + 1
#define	TM_LASTMESSAGE		TM_GETTID

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

#endif /* __SDK_WAFFLE_H_ */