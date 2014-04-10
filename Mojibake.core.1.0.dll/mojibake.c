#include "mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

ENVIRONMENT_SETTING stOldEnvir;
ENVIRONMENT_SETTING stNewEnvir;

LIBRARY_EXPORT SIZE_T WINAPI DetourBaseThreadInitThunk(
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
    stNewEnvir.AnsiCodePage = WaffleGetOptionInt(NULL, NULL, TEXT("AnsiCodePage"), stOldEnvir.AnsiCodePage);

    stOldEnvir.OemCodePage = GetOEMCP();
    stNewEnvir.OemCodePage = WaffleGetOptionInt(NULL, NULL, TEXT("OemCodePage"), stOldEnvir.OemCodePage);

    LPWORD lpNlsAnsiCodePage = (LPWORD) WaffleGetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), TEXT("NlsAnsiCodePage"));
    if (lpNlsAnsiCodePage)
    {
        stOldEnvir.NlsAnsiCodePage = *lpNlsAnsiCodePage;
        stNewEnvir.NlsAnsiCodePage = (WORD) WaffleGetOptionInt(NULL, NULL, TEXT("NlsAnsiCodePage"), stOldEnvir.NlsAnsiCodePage);
        *lpNlsAnsiCodePage = stNewEnvir.NlsAnsiCodePage;
    }

    stOldEnvir.ThreadLocale = GetThreadLocale();
    stNewEnvir.ThreadLocale = WaffleGetOptionInt(NULL, NULL, TEXT("ThreadLocale"), stOldEnvir.ThreadLocale);

    stOldEnvir.DefaultCharSet = DEFAULT_CHARSET;
    stNewEnvir.DefaultCharSet = (BYTE) WaffleGetOptionInt(NULL, NULL, TEXT("DefaultCharSet"), stOldEnvir.DefaultCharSet);

    //Subclass system controls
    MojibakeSubclass();

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