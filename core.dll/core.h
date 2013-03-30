#ifndef __MOJIBAKE_CORE_H_
#define __MOJIBAKE_CORE_H_
#include "..\mojibake.h"

#define	CP_SHIFT_JIS    932
#define LOCALE_JA_JP    0x0411

typedef struct {
    UINT ACP;
    UINT OEMCP;
    LCID ThreadLocale;
} ENVIRONMENT_BLOCK;

extern	HINSTANCE	hDLL;
extern	HANDLE		hHeap;

extern	ENVIRONMENT_BLOCK	stOldEnvir;
extern	ENVIRONMENT_BLOCK	stNewEnvir;

extern	HGLOBAL		lpszCommandLineA;
extern	UINT            ParentTid;

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

extern HOOK_TABLE_OBJECT		stGetCPInfo;
extern HOOK_TABLE_OBJECT		stMultiByteToWideChar;
extern HOOK_TABLE_OBJECT		stWideCharToMultiByte;

typedef BOOL (WINAPI *lpGetCPInfo)(
  _In_   UINT CodePage,
  _Out_  LPCPINFO lpCPInfo
);

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
VOID WINAPI KeepLastErrorAndFree(LPVOID lpMem);

#ifdef __cplusplus
};
#endif

#endif /* __MOJIBAKE_CORE_H_ */