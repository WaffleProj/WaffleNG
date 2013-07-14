#ifndef __MOJIBAKE_MAIN_MOJIBAKE_H_
#define __MOJIBAKE_MAIN_MOJIBAKE_H_
#include <waffle.h>
#define  LIBRARY_EXPORTS
#include "detour.h"
#include "init.h"
#include "src\exception\exception.h"

#define	CP_SHIFT_JIS    932
#define LOCALE_JA_JP    0x0411

#ifdef __cplusplus
extern "C" {
#endif

LPVOID WINAPI AnsiToUnicode(LPCSTR lpszText);
LPVOID WINAPI ProgramCPToWindowsCP(LPCSTR lpszText);
VOID WINAPI KeepLastErrorAndFree(LPVOID lpMem);

#ifdef __cplusplus
};
#endif

#endif /* __MOJIBAKE_MAIN_MOJIBAKE_H_ */