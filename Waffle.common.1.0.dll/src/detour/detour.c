#include "..\..\common.h"

LIBRARY_EXPORT BOOL WINAPI WaffleSetDetour(
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
        bDetour = WaffleInlineDetour(lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpSource);
    }
    if (!bDetour)
    {
        bDetour = WaffleExceptionDetour(lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpSource);
    }
    return bDetour;
}

LIBRARY_EXPORT BOOL WINAPI WaffleAddDetour(
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

LIBRARY_EXPORT VOID WINAPI WaffleLoadDetourOption(void)
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