#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

HANDLE      hHeap;
ENVIRONMENT_SETTING stOldEnvir;
ENVIRONMENT_SETTING stNewEnvir;

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
    stOldEnvir.ANSICodePage = GetACP();
    stNewEnvir.ANSICodePage = WaffleGetOptionInt(lpstProcessSetting, TEXT("ANSICodePage"), stOldEnvir.ANSICodePage);

    stOldEnvir.OEMCodePage = GetOEMCP();
    stNewEnvir.OEMCodePage = WaffleGetOptionInt(lpstProcessSetting, TEXT("OEMCodePage"), stOldEnvir.OEMCodePage);

    stOldEnvir.ThreadLocale = GetThreadLocale();
    stNewEnvir.ThreadLocale = WaffleGetOptionInt(lpstProcessSetting, TEXT("ThreadLocale"), stOldEnvir.ThreadLocale);

    stOldEnvir.DefaultCharSet = DEFAULT_CHARSET;
    stNewEnvir.DefaultCharSet = (BYTE) WaffleGetOptionInt(lpstProcessSetting, TEXT("DefaultCharSet"), stOldEnvir.DefaultCharSet);

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