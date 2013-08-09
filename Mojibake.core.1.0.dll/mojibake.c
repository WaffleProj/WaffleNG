#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

ENVIRONMENT_SETTING stOldEnvir;
ENVIRONMENT_SETTING stNewEnvir;

LIBRARY_EXPORT SIZE_T WINAPI ThreadInit(
    _In_    LPWAFFLE_THREAD_CONTEXT lpstThread
    )
{
    //SetThreadLocale(stNewEnvir.ThreadLocale);
    return 0;
}

LIBRARY_EXPORT SIZE_T WINAPI ComponentInit(
    _In_    LPWAFFLE_PROCESS_SETTING lpstProcessSetting
    )
{
    //Read config files
    stOldEnvir.AnsiCodePage = GetACP();
    stNewEnvir.AnsiCodePage = WaffleGetOptionInt(TEXT("AnsiCodePage"), stOldEnvir.AnsiCodePage);

    stOldEnvir.OemCodePage = GetOEMCP();
    stNewEnvir.OemCodePage = WaffleGetOptionInt(TEXT("OemCodePage"), stOldEnvir.OemCodePage);

    LPWORD lpNlsAnsiCodePage = (LPWORD) WaffleGetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), TEXT("NlsAnsiCodePage"));
    if (lpNlsAnsiCodePage)
    {
        stOldEnvir.NlsAnsiCodePage = *lpNlsAnsiCodePage;
        stNewEnvir.NlsAnsiCodePage = (WORD) WaffleGetOptionInt(TEXT("NlsAnsiCodePage"), stOldEnvir.NlsAnsiCodePage);
        *lpNlsAnsiCodePage = stNewEnvir.NlsAnsiCodePage;
    }

    stOldEnvir.ThreadLocale = GetThreadLocale();
    stNewEnvir.ThreadLocale = WaffleGetOptionInt(TEXT("ThreadLocale"), stOldEnvir.ThreadLocale);

    stOldEnvir.DefaultCharSet = DEFAULT_CHARSET;
    stNewEnvir.DefaultCharSet = (BYTE) WaffleGetOptionInt(TEXT("DefaultCharSet"), stOldEnvir.DefaultCharSet);

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