#include "mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

ENVIRONMENT_SETTING stOldEnvir;
ENVIRONMENT_SETTING stNewEnvir;

WAFFLE_COMPONENT_FUNCTION SIZE_T WINAPI DetourBaseThreadInitThunk(
    _In_    LPWAFFLE_THREAD_CONTEXT lpstThread
    )
{
    //SetThreadLocale(stNewEnvir.ThreadLocale);
    return 0;
}

WAFFLE_COMPONENT_FUNCTION SIZE_T WINAPI ComponentInit(
    _In_    LPWAFFLE_PROCESS_SETTING lpstProcessSetting
    )
{
    //Read config files
    stOldEnvir.AnsiCodePage = GetACP();
    stNewEnvir.AnsiCodePage = WaffleGetOptionInt(NULL, NULL, TEXT("AnsiCodePage"), 932);

    stOldEnvir.OemCodePage = GetOEMCP();
    stNewEnvir.OemCodePage = WaffleGetOptionInt(NULL, NULL, TEXT("OemCodePage"), 932);

    stOldEnvir.ThreadLocale = GetThreadLocale();
    stNewEnvir.ThreadLocale = WaffleGetOptionInt(NULL, NULL, TEXT("ThreadLocale"), MAKELCID(MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN), SORT_DEFAULT));

    stOldEnvir.DefaultCharSet = DEFAULT_CHARSET;
    stNewEnvir.DefaultCharSet = (BYTE) WaffleGetOptionInt(NULL, NULL, TEXT("DefaultCharSet"), SHIFTJIS_CHARSET);

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