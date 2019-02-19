#include "..\main.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

WAFFLE_HOOK_DLL_FUNCTION BOOL WINAPI WaffleJumpDetection(
    _In_    FARPROC lpFunction
    )
{
    LPBYTE lpSource = (LPBYTE) (SIZE_T) lpFunction;
#if (WAFFLE_PORT_MACHINE == WAFFLE_PORT_MACHINE_AMD64) || (WAFFLE_PORT_MACHINE == WAFFLE_PORT_MACHINE_I386)
    if ((lpSource[0] & 0xF0) == 0x70)
    {
        return TRUE;
    }
    switch (lpSource[0])
    {
        case 0x0F:
        {
            switch (lpSource[1])
            {
                case 0x83:
                case 0x87:
                {
                    return TRUE;
                    break;
                }
            }
            break;
        }
        case 0xE3:
        case 0xE9:
        case 0xEB:
        case 0xEA:
        case 0xFF:
        {
            return TRUE;
            break;
        }
        default:
        {
            switch (lpSource[1])
            {
                case 0xFF:
                {
                    return TRUE;
                    break;
                }
            }
            break;
        }
    }
#elif (WAFFLE_PORT_MACHINE == WAFFLE_PORT_MACHINE_ARMNT)
    WORD WordInst = ((LPWORD) lpSource)[0];
    DWORD DwordInst = ((LPDWORD) lpSource)[0];
    if ((WordInst & 0xF000) == 0xD000)
    {
        return TRUE;
    }
    else if ((WordInst & 0xF800) == 0xE000)
    {
        return TRUE;
    }
    else if ((DwordInst & 0xF800C000) == 0xF0008000)
    {
        return TRUE;
    }
#endif
    return FALSE;
}

WAFFLE_HOOK_DLL_FUNCTION FARPROC WINAPI WaffleRegisterHookOnAddress(
    _In_    FARPROC lpOldFunction,
    _In_    FARPROC lpNewFunction
    )
{
    if (!WaffleHookDBAddFunction(lpOldFunction, lpNewFunction))
    {
        return NULL;
    }

    do
    {
        if (!WaffleJumpDetection(lpOldFunction))
        {
            if (WaffleInlineDetour((LPBYTE) (SIZE_T) lpOldFunction))
            {
                break;
            }
        }

        if (!WaffleExceptionDetour((LPBYTE) (SIZE_T) lpOldFunction))
        {
            return NULL;
        }
    } while (FALSE);

    return lpOldFunction;
}

WAFFLE_HOOK_DLL_FUNCTION FARPROC WINAPI WaffleRegisterHookOnAPI(
    _In_opt_    LPCWSTR lpModuleName,
    _In_        LPCSTR lpProcName,
    _In_        FARPROC lpNewFunction
    )
{
    HMODULE hModule = GetModuleHandleW(lpModuleName);
    if (!hModule)
    {
        return NULL;
    }

    if (!lpProcName)
    {
        return NULL;
    }
    FARPROC lpOldFunction = GetProcAddress(hModule, lpProcName);
    if (!lpOldFunction)
    {
        return NULL;
    }

    return WaffleRegisterHookOnAddress(lpOldFunction, lpNewFunction);
}

WAFFLE_HOOK_DLL_FUNCTION FARPROC WINAPI WaffleRegisterHookOnCOMOffset(
    _In_    REFCLSID rclsid,
    _In_    REFIID riid,
    _In_    SIZE_T vtableOffset,
    _In_    FARPROC lpNewFunction
    )
{
    LPCOCREATEINSTANCE _CoCreateInstance = NULL;
    HMODULE hOle32 = LoadLibrary(TEXT("Ole32.dll"));
    if (!hOle32)
    {
        return NULL;
    }

    _CoCreateInstance = (LPCOCREATEINSTANCE) GetProcAddress(hOle32, "CoCreateInstance");
    if (!_CoCreateInstance)
    {
        FreeLibrary(hOle32);
        return NULL;
    }

    IUnknown * ppv = NULL;
    if (FAILED(_CoCreateInstance(rclsid, NULL, CLSCTX_INPROC_SERVER, riid, &ppv)))
    {
        FreeLibrary(hOle32);
        return NULL;
    }

    FARPROC lpPointer = WaffleRegisterHookOnAddress(((FARPROC *) ppv->lpVtbl)[vtableOffset], lpNewFunction);

    ppv->lpVtbl->Release(ppv);
    FreeLibrary(hOle32);

    return lpPointer;
}

WAFFLE_HOOK_DLL_FUNCTION FARPROC WINAPI WaffleUnregisterHook(
    _In_    FARPROC lpOldFunction
    )
{
    return NULL;
}