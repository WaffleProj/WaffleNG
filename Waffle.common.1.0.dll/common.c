﻿#include "common.h"
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
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WaffleInit, 0, 0, NULL);
            if (hThread)
            {
                CloseHandle(hThread);
            }
            else
            {
                MessageBox(0, TEXT("FIXME:WaffleInit failed"), 0, 0);
                ExitProcess(0);
            }
        }
    }
    return TRUE;
}