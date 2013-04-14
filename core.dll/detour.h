#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include "src\detour\kernel32.h"
#include "src\detour\user32.h"
#include "src\detour\gdi32.h"
#include "src\detour\psapi.h"

typedef struct {
    LPCWSTR lpszLibrary;
    LPVOID  lpHookTable;
    LPVOID  lpLibrary;
    HMODULE hModule;
    LPVOID  lpEndOfModule;
} LIBRARY_TABLE_OBJECT,*LPLIBRARY_TABLE_OBJECT;

typedef struct {
    LPCSTR lpszFunction;
    LPVOID lpDetourFunction;
    LPVOID lpNewFunction;
    LPVOID lpOriginalFunction;
} HOOK_TABLE_OBJECT,*LPHOOK_TABLE_OBJECT;

typedef struct {
    UINT ACP;
    UINT OEMCP;
    LCID ThreadLocale;
} ENVIRONMENT_BLOCK,*LPENVIRONMENT_BLOCK;

typedef int (__cdecl *LPWSPRINTFA)(
  _Out_  LPSTR lpOut,
  _In_   LPCSTR lpFmt,
  _In_    ...
);

extern	HINSTANCE	hDLL;
extern	HANDLE		hHeap;

extern	ENVIRONMENT_BLOCK	stOldEnvir;
extern	ENVIRONMENT_BLOCK	stNewEnvir;

extern	HGLOBAL		lpszCommandLineA;

extern LIBRARY_TABLE_OBJECT     stHookTable[];

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

#ifdef __cplusplus
};
#endif

#endif /* __GLOBAL_H_ */