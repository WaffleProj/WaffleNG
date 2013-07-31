#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

BOOL WINAPI DllMain(
    _In_    HINSTANCE hinstDLL,
    _In_    DWORD fdwReason,
    _In_    LPVOID lpvReserved
    )
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);

        LPTSTR lpszComponentDirectory = WaffleGetComponentDirectory();
        GetModuleFileName(hinstDLL, lpszComponentDirectory, MAX_PATH);
        int i = lstrlen(lpszComponentDirectory);                                                            //E:\WaffleNightly\Component\Waffle\I386\Waffle.common.1.0.dll
        for (i--; lpszComponentDirectory[i] != TEXT('\\'); i--); lpszComponentDirectory[i] = TEXT('\0');    //E:\WaffleNightly\Component\Waffle\I386
        for (i--; lpszComponentDirectory[i] != TEXT('\\'); i--); lpszComponentDirectory[i] = TEXT('\0');    //E:\WaffleNightly\Component\Waffle
        for (i--; lpszComponentDirectory[i] != TEXT('\\'); i--); lpszComponentDirectory[i] = TEXT('\0');    //E:\WaffleNightly\Component

        if (WaffleOpenProcessSetting())
        {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WaffleInit, 0, 0, NULL);
        }
    }
    return TRUE;
}