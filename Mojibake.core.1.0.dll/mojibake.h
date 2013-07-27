#ifndef __MOJIBAKE_MAIN_MOJIBAKE_H_
#define __MOJIBAKE_MAIN_MOJIBAKE_H_

#define WAFFLE_COMPONENT_BUILD
#include <waffle.h>
#include "detour.h"
#include "init.h"
#include "src\exception\exception.h"

#define	CP_SHIFT_JIS    932
#define LOCALE_JA_JP    0x0411

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