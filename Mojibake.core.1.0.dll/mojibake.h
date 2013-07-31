#ifndef __MOJIBAKE_MAIN_MOJIBAKE_H_
#define __MOJIBAKE_MAIN_MOJIBAKE_H_

#define WAFFLE_COMPONENT_BUILD
#include <waffle.h>
#include "src\rtl.h"

extern HANDLE       hHeap;

typedef struct
{
    UINT ACP;
    UINT OEMCP;
    LCID ThreadLocale;
} ENVIRONMENT_BLOCK, *LPENVIRONMENT_BLOCK;

extern ENVIRONMENT_BLOCK    stOldEnvir;
extern ENVIRONMENT_BLOCK    stNewEnvir;

#ifdef __cplusplus
extern "C" {
#endif

    LPWSTR WINAPI AnsiToUnicode(
        _In_    LPCSTR lpszText
        );
    LPSTR WINAPI ProgramCPToWindowsCP(
        _In_    LPCSTR lpszText
        );
    VOID WINAPI KeepLastErrorAndFree(
        _In_    LPVOID lpMem
        );

#ifdef __cplusplus
};
#endif

#endif /* __MOJIBAKE_MAIN_MOJIBAKE_H_ */