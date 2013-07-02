#ifndef __WAFFLE_LOADER_EXE_H_
#define __WAFFLE_LOADER_EXE_H_
#include <waffle.h>

typedef BOOL (WINAPI *LPWOW64DISABLEWOW64FSREDIRECTION)(
  _Out_  PVOID *OldValue
);

typedef BOOL (WINAPI *LPWOW64REVERTWOW64FSREDIRECTION)(
  _In_  PVOID OldValue
);

extern LPWOW64DISABLEWOW64FSREDIRECTION lpWow64DisableWow64FsRedirection;
extern LPWOW64REVERTWOW64FSREDIRECTION lpWow64RevertWow64FsRedirection;

#ifdef __cplusplus
extern "C" {
#endif

int WINAPI argc();
LPCTSTR WINAPI argv(int intPosition, LPTSTR lpString, int intSize);
LPCTSTR WINAPI argp(int intPosition);
WORD WINAPI GetPEMagic(LPVOID lpFile);
PROCESS_INFORMATION WINAPI InjectDll(LPCTSTR lpszTarget, LPTSTR lpszArgument, LPCTSTR lpszDirectory, LPCTSTR lpszDllFull);

#ifdef __cplusplus
};
#endif

#endif /* __WAFFLE_LOADER_EXE_H_ */