#include "main.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

LPWAFFLE_SMART_ARRAY lpstLibrary;
LPWAFFLE_SMART_ARRAY lpstSkipLibrary;

BOOL WINAPI DllMain(
    _In_    HINSTANCE hinstDLL,
    _In_    DWORD fdwReason,
    _In_    LPVOID lpvReserved
    )
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        lpstLibrary = WaffleSmartArrayCreate(sizeof(WAFFLE_MODULE_NODE));
        lpstSkipLibrary = WaffleSmartArrayCreate(sizeof(WAFFLE_MODULE_NODE));
    }
    return TRUE;
}