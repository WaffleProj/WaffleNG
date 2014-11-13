#include "..\..\common.h"

WAFFLE_COMMON_DLL_FUNCTION BOOL WINAPI WaffleJumpDetection(
    _In_    LPBYTE lpSource
    )
{
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

WAFFLE_COMMON_DLL_FUNCTION BOOL WINAPI WaffleSetDetour(
    _In_    DWORD dwLibrary,
    _In_    DWORD dwFunction
    )
{
    BOOL bDetour = FALSE;

    if (!lpstProcessSetting->lpstLibrary)
    {
        return bDetour;
    }
    else if (dwLibrary > lpstProcessSetting->lpstLibrary[0].dwBehind)
    {
        return bDetour;
    }
    else if (dwFunction > lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[0].dwBehind)
    {
        return bDetour;
    }
    else if (!lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpDetour)
    {
        return bDetour;
    }
    else if (!lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpSource)
    {
        return bDetour;
    }

    //MessageBox(0, lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpszFunction, lpstProcessSetting->lpstLibrary[dwLibrary].lpszLibrary, 0);
    if (!bDetour)
    {
        if (!WaffleJumpDetection(lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpSource))
        {
            bDetour = WaffleInlineDetour(lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpSource);
        }
    }
    if (!bDetour)
    {
        bDetour = WaffleExceptionDetour(lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpSource);
    }
    return bDetour;
}

WAFFLE_COMMON_DLL_FUNCTION BOOL WINAPI WaffleAddDetour(
    _In_    LPBYTE lpSource,
    _In_    LPCTSTR lpszFunction,
    _In_    HMODULE hDetour
    )
{
    HMODULE hModule;
    if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR) lpSource, &hModule))
    {
        return FALSE;
    }

    WaffleAddLibrary(hModule);
    WAFFLE_FUNCTION_ARRAY stNewFunction;
    stNewFunction.lpszFunction = lpszFunction;
    stNewFunction.lpSource = lpSource;
    WaffleAddFunction(&stNewFunction, hDetour);
    return TRUE;
}

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleLoadDetourOption(void)
{
    LPTSTR lpszSection = WaffleGetOptionSectionNames(TEXT("Detour.ini"));
    if (!lpszSection)
    {
        MessageBox(0, TEXT("FIXME:Unable to allocate more memory to create library array"), 0, 0);
        return;
    }

    LPTSTR lpszNextSection = lpszSection;
    DWORD nSizeOfSection;
    for (nSizeOfSection = lstrlen(lpszNextSection); nSizeOfSection; lpszNextSection = lpszNextSection + nSizeOfSection + 1, nSizeOfSection = lstrlen(lpszNextSection))
    {
        //Check if we already loaded this library
        //for now we just load everything because we don't have a function to work with LoadLibrary
        //HMODULE hLibrary = GetModuleHandle(lpszNext);
        HMODULE hLibrary = LoadLibrary(lpszNextSection);
        if (hLibrary)
        {
            LPTSTR lpszKey = WaffleGetOptionSectionKeys(TEXT("Detour.ini"), lpszNextSection);

            LPTSTR lpszNextKey = lpszKey;
            DWORD nSizeOfKey;
            for (nSizeOfKey = lstrlen(lpszNextKey); nSizeOfKey; lpszNextKey = lpszNextKey + nSizeOfKey + 1, nSizeOfKey = lstrlen(lpszNextKey))
            {
                int i;
                for (i = 0; lpszNextKey[i] != TEXT('=') && lpszNextKey[i] != TEXT('\0'); i++);  //FIXME
                lpszNextKey[i] = TEXT('\0');

                DWORD nSizeOfFunction = lstrlen(lpszNextKey);
                if (lpszNextKey[0] != TEXT('#')) //This is a comment
                {
                    WaffleAddDetour(WaffleGetProcAddress(hLibrary, lpszNextKey), lpszNextKey, WaffleLoadComponent(lpszNextKey + nSizeOfFunction + 1));
                }
            }
        }
    }

    WaffleFree(lpszSection);
}