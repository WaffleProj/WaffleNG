#include "mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

ENVIRONMENT_SETTING stOldEnvir;
ENVIRONMENT_SETTING stNewEnvir;
LOGFONTA lfGothic;
TEXTMETRICA tmGothic;
DWORD dwGothic;

LIBRARY_EXPORT SIZE_T WINAPI DetourBaseThreadInitThunk(
    _In_    LPWAFFLE_THREAD_CONTEXT lpstThread
    )
{
    //SetThreadLocale(stNewEnvir.ThreadLocale);
    return 0;
}

int CALLBACK MojibakeEnumFontsProcA(
    _In_    const LOGFONTA *lplf,
    _In_    const TEXTMETRICA *lptm,
    _In_    DWORD dwType,
    _In_    LPARAM lpData
    )
{
    RtlMoveMemory(&lfGothic, lplf, sizeof(lfGothic));
    RtlMoveMemory(&tmGothic, lptm, sizeof(tmGothic));
    tmGothic.tmPitchAndFamily = tmGothic.tmPitchAndFamily | TMPF_TRUETYPE & ~TMPF_FIXED_PITCH;
    tmGothic.tmCharSet = SHIFTJIS_CHARSET;
    dwGothic = dwType;
    lstrcpyA(lfGothic.lfFaceName, "\x82\x6C\x82\x72\x20\x83\x53\x83\x56\x83\x62\x83\x4E");  //ＭＳ ゴシック
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

    //Get fake font info
    EnumFontsA(GetDC(NULL), "MS Gothic", MojibakeEnumFontsProcA, 0);

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