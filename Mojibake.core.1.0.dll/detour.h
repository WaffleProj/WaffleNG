#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include "src\detour\kernel32.h"
#include "src\detour\user32.h"
#include "src\detour\gdi32.h"
#include "src\detour\psapi.h"
#include "src\detour\shell32.h"

extern HINSTANCE    hDLL;
extern HANDLE       hHeap;
extern LPSTR        lpszCommandLineA;

typedef struct
{
    UINT ACP;
    UINT OEMCP;
    LCID ThreadLocale;
} ENVIRONMENT_BLOCK, *LPENVIRONMENT_BLOCK;

extern ENVIRONMENT_BLOCK    stOldEnvir;
extern ENVIRONMENT_BLOCK    stNewEnvir;

typedef struct
{
    LPCSTR lpszFunction;
    LPVOID lpDetourFunction;
    LPVOID lpNewFunction;
    LPVOID lpOriginalFunction;
} HOOK_TABLE_OBJECT, *LPHOOK_TABLE_OBJECT;

extern	HOOK_TABLE_OBJECT       stKernel32Table [];
#define CREATEDIRECTORYA        0
#define CREATEFILEA             1
#define DELETEFILEA             2
#define FINDFIRSTFILEA          3
#define FINDNEXTFILEA           4
#define GETACP                  5
#define GETCOMMANDLINEA         6
#define GETCPINFO               7
#define GETFILEATTRIBUTESA      8
#define GETMODULEFILENAMEA      9
#define GETMODULEHANDLEA        10
#define GETOEMCP                11
#define LOADLIBRARYA            12
#define LOADLIBRARYEXA          13
#define MULTIBYTETOWIDECHAR     14
#define SETCURRENTDIRECTORYA    15
#define SETFILEATTRIBUTESA      16
#define WIDECHARTOMULTIBYTE     17
extern	HOOK_TABLE_OBJECT       stUser32Table [];
#define CALLWINDOWPROCA         0
#define CREATEWINDOWEXA         1
#define MESSAGEBOXA             2
#define MESSAGEBOXEXA           3
#define SENDMESSAGEA            4
#define SETWINDOWTEXTA          5
extern	HOOK_TABLE_OBJECT       stGdi32Table [];
#define CREATEFONTA             0
#define TEXTOUTA                1
extern	HOOK_TABLE_OBJECT       stPsapiTable [];
#define GETMODULEFILENAMEEXA    0
extern	HOOK_TABLE_OBJECT       stShell32Table [];
#define SHELLABOUTW             0

typedef struct
{
    LPCWSTR             lpszLibrary;
    LPHOOK_TABLE_OBJECT lpHookTable;
    HMODULE             lpLibrary;
    HMODULE             hModule;
    LPVOID              lpEndOfModule;
} LIBRARY_TABLE_OBJECT, *LPLIBRARY_TABLE_OBJECT;

extern LIBRARY_TABLE_OBJECT    stLibraryTable [];
#define KERNEL32                0
#define USER32                  1
#define GDI32                   2
#define PSAPI                   3
#define NTDLL                   4
#define SHELL32                 5

typedef int (__cdecl *LPWSPRINTFA)(
    _Out_  LPSTR lpOut,
    _In_   LPCSTR lpFmt,
    _In_   ...
    );

extern LPWSPRINTFA _wsprintfA;

typedef BOOL(WINAPI *LPVIRTUALPROTECT)(
    _In_   LPVOID lpAddress,
    _In_   SIZE_T dwSize,
    _In_   DWORD flNewProtect,
    _Out_  PDWORD lpflOldProtect
    );

extern LPVIRTUALPROTECT _VirtualProtect;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
};
#endif

#endif /* __GLOBAL_H_ */