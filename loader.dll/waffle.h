#ifndef __MOJIBAKE_CORE_H_
#define __MOJIBAKE_CORE_H_
#include <waffle.h>
#include "detour.h"
#include "init.h"
#include "src\membp\membp.h"

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

#endif /* __MOJIBAKE_CORE_H_ */