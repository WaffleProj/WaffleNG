#ifndef __MEMBP_H_
#define __MEMBP_H_
#include "..\core.h"

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

extern LPMESSAGEBOXA lpMessageBoxA;
extern LPWSPRINTFA lpwsprintfA;
extern HMODULE hDll;
extern HOOK_TABLE_OBJECT stMessageBoxA;

#ifdef __cplusplus
extern "C" {
#endif

HMODULE WINAPI CopyLibrary(
  _In_  LPSTR lpszDllName
);

LPVOID WINAPI GetProcAddr(
  _In_  HMODULE hDll,
  _In_  LPSTR lpszFuncName
);

LONG CALLBACK BreakpointHandler(
  _In_  PEXCEPTION_POINTERS ExceptionInfo
);

int WINAPI HookedMessageBoxA(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
);

#ifdef __cplusplus
};
#endif

#endif /* __MEMBP_H_ */