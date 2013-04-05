#ifndef __MEMBP_H_
#define __MEMBP_H_

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

typedef struct {
    LPVOID lpNewFunction;
    LPVOID lpOldFunction;
    LPCSTR lpszFunction;
    LPCSTR lpszModule;
} HOOK_TABLE_OBJECT;

#ifdef __cplusplus
};
#endif

#endif /* __MEMBP_H_ */