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

extern LPVOID		stHookTable[];

typedef struct {
    LPVOID lpNewFunction;
    LPVOID lpOldFunction;
    LPCSTR lpszFunction;
    LPCSTR lpszModule;
} HOOK_TABLE_OBJECT;

typedef BOOL (WINAPI *LPGETCPINFO)(
  _In_   UINT CodePage,
  _Out_  LPCPINFO lpCPInfo
);

typedef int (WINAPI *LPMULTIBYTETOWIDECHAR)(
  _In_       UINT CodePage,
  _In_       DWORD dwFlags,
  _In_       LPCSTR lpMultiByteStr,
  _In_       int cbMultiByte,
  _Out_opt_  LPWSTR lpWideCharStr,
  _In_       int cchWideChar
);

typedef int (WINAPI *LPWIDECHARTOMULTIBYTE)(
  _In_       UINT CodePage,
  _In_       DWORD dwFlags,
  _In_       LPCWSTR lpWideCharStr,
  _In_       int cchWideChar,
  _Out_opt_  LPSTR lpMultiByteStr,
  _In_       int cbMultiByte,
  _In_opt_   LPCSTR lpDefaultChar,
  _Out_opt_  LPBOOL lpUsedDefaultChar
);

typedef LRESULT (WINAPI *LPSENDMESSAGEA)(
  _In_  HWND hWnd,
  _In_  UINT Msg,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
);

typedef LRESULT (WINAPI *LPCALLWINDOWPROCA)(
  _In_  WNDPROC lpPrevWndFunc,
  _In_  HWND hWnd,
  _In_  UINT Msg,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
);

typedef int (WINAPI *LPMESSAGEBOXA)(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
);

typedef int (__cdecl *LPWSPRINTFA)(
  _Out_  LPSTR lpOut,
  _In_   LPCSTR lpFmt,
  _In_    ...
);

extern HOOK_TABLE_OBJECT                stGetCPInfo;
extern HOOK_TABLE_OBJECT                stMultiByteToWideChar;
extern HOOK_TABLE_OBJECT                stWideCharToMultiByte;
extern HOOK_TABLE_OBJECT                stSendMessageA;
extern HOOK_TABLE_OBJECT                stCallWindowProcA;
extern HOOK_TABLE_OBJECT                stMessageBoxA;
extern LPMESSAGEBOXA                    _MessageBoxA;
extern LPWSPRINTFA                      _wsprintfA;

#ifdef __cplusplus
extern "C" {
#endif

int WINAPI SetHook(LPVOID stHookTable[]);
LPVOID WINAPI AnsiToUnicode(LPCSTR lpszText);
LPVOID WINAPI ProgramCPToWindowsCP(LPCSTR lpszText);
VOID WINAPI KeepLastErrorAndFree(LPVOID lpMem);

#ifdef __cplusplus
};
#endif

#endif /* __MOJIBAKE_CORE_H_ */