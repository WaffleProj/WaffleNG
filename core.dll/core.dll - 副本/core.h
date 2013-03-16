#ifndef __MOJIBAKE_CORE_H_
#define __MOJIBAKE_CORE_H_

extern	HINSTANCE	hInstance;
extern	HGLOBAL		lpszCommandLineA;
#define	CP_SHIFT_JIS		932
#define	TM_RESUMEMAINTHREAD	WM_USER + 1

#ifdef __cplusplus
extern "C" {
#endif

VOID WINAPI InitLibrary(LPVOID lpParameter);

#ifdef __cplusplus
};
#endif

#endif /* __MOJIBAKE_CORE_H_ */