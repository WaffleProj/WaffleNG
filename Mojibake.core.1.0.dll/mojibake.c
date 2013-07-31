#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

HANDLE      hHeap;
ENVIRONMENT_BLOCK   stOldEnvir;
ENVIRONMENT_BLOCK   stNewEnvir;

LIBRARY_EXPORT SIZE_T WINAPI ThreadInit(
    _In_    LPWAFFLE_THREAD_CONTEXT lpstThread
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

    //Read config files
    stOldEnvir.ACP = GetACP();
    stNewEnvir.ACP = WaffleGetOptionInt(lpstProcessSetting, TEXT("ACP"), stOldEnvir.ACP);

    stOldEnvir.OEMCP = GetOEMCP();
    stNewEnvir.OEMCP = WaffleGetOptionInt(lpstProcessSetting, TEXT("OEMCP"), stOldEnvir.OEMCP);

    stOldEnvir.ThreadLocale = GetThreadLocale();
    stNewEnvir.ThreadLocale = WaffleGetOptionInt(lpstProcessSetting, TEXT("ThreadLocale"), stOldEnvir.ThreadLocale);

    return 0;
}

BOOL WINAPI DllMain(
    _In_    HINSTANCE hinstDLL,
    _In_    DWORD fdwReason,
    _In_    LPVOID lpvReserved
    )
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
    }
    return TRUE;
}