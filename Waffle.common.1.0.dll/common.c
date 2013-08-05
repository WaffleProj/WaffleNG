#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

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

        if (WaffleOpenProcessSetting())
        {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WaffleInit, 0, 0, NULL);
        }
    }
    return TRUE;
}