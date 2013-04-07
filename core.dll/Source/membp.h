#ifndef __MEMBP_H_
#define __MEMBP_H_
#include "..\core.h"

#ifdef __cplusplus
extern "C" {
#endif

HMODULE WINAPI CopyLibrary(
  _In_  LPTSTR lpszDllName
);

LPVOID WINAPI GetFunctionAddressA(
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

HMODULE WINAPI GetModuleAddressW(
  _In_  LPCWSTR lpszModule
);

#ifdef __cplusplus
};
#endif

#endif /* __MEMBP_H_ */