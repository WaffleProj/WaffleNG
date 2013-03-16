#ifndef __MOJIBAKE_LOADER_H_
#define __MOJIBAKE_LOADER_H_

#define _In_
#define _In_opt_
#define _Out_
#define _Out_opt_

#define	TM_FIRSTMESSAGE		TM_RESUMEMAINTHREAD
#define	TM_RESUMEMAINTHREAD	WM_USER + 1
#define	TM_LASTMESSAGE		TM_RESUMEMAINTHREAD

#ifdef __cplusplus
extern "C" {
#endif

#undef  RtlZeroMemory
VOID WINAPI RtlZeroMemory(
  _In_  VOID UNALIGNED *Destination,
  _In_  SIZE_T Length
);

int WINAPI argc();
LPCTSTR WINAPI argv(int intPosition, LPTSTR lpString, int intSize);
LPCTSTR WINAPI argp(int intPosition);
WORD WINAPI GetPEMagic(LPVOID lpFile);
HANDLE WINAPI InjectDll(LPCTSTR lpszTarget, LPTSTR lpszArgument, LPCTSTR lpszDirectory, LPCTSTR lpszDllFull);

#ifdef __cplusplus
};
#endif

#endif /* __MOJIBAKE_LOADER_H_ */