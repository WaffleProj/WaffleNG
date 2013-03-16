#ifndef __MOJIBAKE_CORE_H_
#define __MOJIBAKE_CORE_H_

#define _In_
#define _In_opt_
#define _Out_
#define _Out_opt_

#define	TM_RESUMEMAINTHREAD	WM_USER + 1
#define	CP_SHIFT_JIS 932

extern	HINSTANCE	hDLL;
extern	HANDLE		hHeap;
extern	UINT		NewCodePage;
extern	UINT		OldCodePage;
extern	HGLOBAL		lpszCommandLineA;

typedef struct {
    LPVOID lpDispatch;
} HOOK_TABLE_HEAD_OBJECT;

extern HOOK_TABLE_HEAD_OBJECT		stHookTable;

typedef struct {
    LPVOID lpNewFunction;
    LPVOID lpOldFunction;
    LPCSTR lpszFunction;
    LPCSTR lpszModule;
} HOOK_TABLE_OBJECT;

extern HOOK_TABLE_OBJECT		stMultiByteToWideChar;
extern HOOK_TABLE_OBJECT		stWideCharToMultiByte;

typedef int (WINAPI *lpMultiByteToWideChar)(
  _In_       UINT CodePage,
  _In_       DWORD dwFlags,
  _In_       LPCSTR lpMultiByteStr,
  _In_       int cbMultiByte,
  _Out_opt_  LPWSTR lpWideCharStr,
  _In_       int cchWideChar
);

typedef int (WINAPI *lpWideCharToMultiByte)(
  _In_       UINT CodePage,
  _In_       DWORD dwFlags,
  _In_       LPCWSTR lpWideCharStr,
  _In_       int cchWideChar,
  _Out_opt_  LPSTR lpMultiByteStr,
  _In_       int cbMultiByte,
  _In_opt_   LPCSTR lpDefaultChar,
  _Out_opt_  LPBOOL lpUsedDefaultChar
);

#ifdef __cplusplus
extern "C" {
#endif

int WINAPI _SetHook(HOOK_TABLE_HEAD_OBJECT * stHookTable);
LPVOID WINAPI AnsiToUnicode(LPCSTR lpszText);

#ifdef __cplusplus
};
#endif

#endif /* __MOJIBAKE_CORE_H_ */