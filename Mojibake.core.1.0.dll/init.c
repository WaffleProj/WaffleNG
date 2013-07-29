#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

HANDLE      hHeap;

LIBRARY_EXPORT SIZE_T WINAPI ThreadInit(
    _In_    LPTHREAD_CONTEXT lpstThread
    )
{
    SetThreadLocale(stNewEnvir.ThreadLocale);
    return 0;
}

LIBRARY_EXPORT SIZE_T WINAPI ComponentInit(
    _In_    LPWAFFLE_PROCESS_SETTING lpstProcessSetting
    )
{
    hHeap = HeapCreate(0, 0, 0);
    //∂¡»°≥Ã–Ú≈‰÷√
    //CryptCATAdminCalcHashFromFileHandle
    stOldEnvir.ACP = GetACP();
    stNewEnvir.ACP = CP_SHIFT_JIS;
    stOldEnvir.OEMCP = GetOEMCP();
    stNewEnvir.OEMCP = CP_SHIFT_JIS;
    stOldEnvir.ThreadLocale = GetThreadLocale();
    stNewEnvir.ThreadLocale = LOCALE_JA_JP;

    WaffleSetLibraryTable(stLibraryTable);

    return 0;
}
