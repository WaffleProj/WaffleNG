#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

HINSTANCE hWaffleCommonDll;
TCHAR szWaffleCommonDll[MAX_PATH];
TCHAR szComponentPath[MAX_PATH];
LPWAFFLE_PROCESS_SETTING lpstProcessSetting;

BOOL WINAPI DllMain(
    _In_    HINSTANCE hinstDLL,
    _In_    DWORD fdwReason,
    _In_    LPVOID lpvReserved
    )
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);

        hWaffleCommonDll = hinstDLL;

        GetModuleFileName(hWaffleCommonDll, szWaffleCommonDll, sizeof(szWaffleCommonDll) / sizeof(szWaffleCommonDll[0]));

        lstrcpy(szComponentPath, szWaffleCommonDll);
        int i = lstrlen(szComponentPath);                                                   //E:\WaffleNightly\Component\Waffle\I386\Waffle.common.1.0.dll
        for (i--; szComponentPath[i] != TEXT('\\'); i--); szComponentPath[i] = TEXT('\0');  //E:\WaffleNightly\Component\Waffle\I386
        for (i--; szComponentPath[i] != TEXT('\\'); i--); szComponentPath[i] = TEXT('\0');  //E:\WaffleNightly\Component\Waffle
        for (i--; szComponentPath[i] != TEXT('\\'); i--); szComponentPath[i] = TEXT('\0');  //E:\WaffleNightly\Component

        if (WaffleOpenProcessSetting())
        {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WaffleInit, 0, 0, NULL);
        }
    }
    return TRUE;
}