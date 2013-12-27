#include "shell.h"

volatile LONG nIClassFactory = 0;
volatile LONG nIClassFactoryLock = 0;
volatile LONG nIWaffleShell = 0;

STDAPI DllGetClassObject(
    _In_    REFCLSID rclsid,
    _In_    REFIID riid,
    _Out_   LPVOID *ppv
    )
{
    if (IsEqualCLSID(rclsid, &CLSID_IWaffleShell))
    {
        return IClassFactoryObject.lpVtbl->QueryInterface(&IClassFactoryObject, riid, ppv);
    }
    else
    {
        *ppv = NULL;
        return CLASS_E_CLASSNOTAVAILABLE;
    }
}

STDAPI DllCanUnloadNow(void)
{
    if (!nIClassFactory && !nIClassFactoryLock && !nIWaffleShell)
    {
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

BOOL WINAPI DllMain(
    _In_    HINSTANCE hinstDLL,
    _In_    DWORD fdwReason,
    _In_    LPVOID lpvReserved
    )
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hinstDLL);
    }
    }

    return TRUE;
}
