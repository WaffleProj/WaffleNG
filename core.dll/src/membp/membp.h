#ifndef __MEMBP_H_
#define __MEMBP_H_

#ifdef __cplusplus
extern "C" {
#endif

HMODULE WINAPI GetModuleAddressW(
  _In_  LPCWSTR lpszModule
);

HMODULE WINAPI CopyLibrary(
  _In_  HMODULE hModule
);

LPVOID WINAPI GetFunctionAddressA(
  _In_  HMODULE hDll,
  _In_  LPSTR lpszFuncName
);

LONG CALLBACK BreakpointHandler(
  _In_  PEXCEPTION_POINTERS ExceptionInfo
);

#ifdef __cplusplus
};
#endif

#endif /* __MEMBP_H_ */