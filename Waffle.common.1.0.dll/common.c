#define  UNICODE
#define _UNICODE
#include "common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

HINSTANCE hWaffleCommonDll;
TCHAR szWaffleCommonDll[MAX_PATH];
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

        if (WaffleOpenProcessSetting())
        {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WaffleInit, 0, 0, NULL);
        }
    }
    return TRUE;
}