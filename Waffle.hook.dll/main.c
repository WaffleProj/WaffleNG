#include "main.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

LPWAFFLE_LIBRARY_ARRAY lpstLibrary;

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