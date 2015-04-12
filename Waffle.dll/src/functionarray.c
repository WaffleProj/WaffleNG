#include "..\common.h"

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleAddFunction(
    _In_    LPWAFFLE_FUNCTION_ARRAY lpstNewFunction,
    _In_    HMODULE hDetour
    )
{
    //Check argument
    if (!lpstNewFunction->lpSource)
    {
        return;
    }

    //Do nothing if we have one
    LPWAFFLE_LIBRARY_ARRAY lpstLibrary = &lpstProcessSetting->lpstLibrary[WaffleFindLibrary(lpstNewFunction->lpSource)];
    LPWAFFLE_FUNCTION_ARRAY lpstFunction = lpstLibrary->lpstFunction;
    if (lpstFunction)
    {
        int j;
        for (j = lpstFunction[0].dwBehind; j >= 0; j--)
        {
            if (lpstNewFunction->lpSource == lpstFunction[j].lpSource)
            {
                return;
            }
        }
    }

    //Allocate the memory for new library
    if (!lpstFunction)
    {
        lpstFunction = (LPWAFFLE_FUNCTION_ARRAY) WaffleAlloc(sizeof(WAFFLE_FUNCTION_ARRAY));
        if (!lpstFunction)
        {
            MessageBox(0, TEXT("FIXME:Unable to allocate memory for function array"), 0, 0);
            ExitProcess(0);
        }
    }
    else
    {
        int i;
        for (i = lpstFunction[0].dwBehind; i >= 0; i--)
        {
            lpstFunction[i].dwBehind++;
        }
        lpstFunction = (LPWAFFLE_FUNCTION_ARRAY) WaffleReAlloc(lpstFunction, sizeof(WAFFLE_FUNCTION_ARRAY) * (lpstFunction[0].dwBehind + 1));
        if (!lpstFunction)
        {
            MessageBox(0, TEXT("FIXME:Unable to add elements in function array"), 0, 0);
            ExitProcess(0);
        }
    }
    lpstLibrary->lpstFunction = lpstFunction;

    //get the information
    lpstNewFunction->dwBehind = 0;
    //lpstNewFunction->lpBackup = (LPBYTE) (lpstLibrary->hSourceEnd - (SIZE_T) lpstNewFunction->lpSource + lpstLibrary->hBackupEnd);
    lpstNewFunction->lpBackup = (LPBYTE) ((SIZE_T) lpstNewFunction->lpSource - (SIZE_T) lpstLibrary->hSource + (SIZE_T) lpstLibrary->hBackup);

    DWORD nSize = lstrlen(lpstNewFunction->lpszFunction) + sizeof(TEXT("Detour")) / sizeof(TCHAR);
    LPTSTR lpszDetour = (LPTSTR) WaffleAlloc(nSize * sizeof(TCHAR));
    wsprintf(lpszDetour, TEXT("Detour%s"), lpstNewFunction->lpszFunction);
    lpstNewFunction->lpDetour = WaffleGetProcAddress(hDetour, lpszDetour);
    if (!lpstNewFunction->lpDetour)
    {
        TCHAR szMissing[256];
        wsprintf(szMissing, TEXT("FIXME:Cannot find %s"), lpszDetour);
        MessageBox(0, szMissing, 0, 0);
    }
    WaffleFree(lpszDetour);

    RtlMoveMemory(&lpstFunction[lpstFunction[0].dwBehind], lpstNewFunction, sizeof(WAFFLE_FUNCTION_ARRAY));
}
WAFFLE_COMMON_DLL_FUNCTION LPVOID WINAPI WaffleGetBackupAddress(
    _In_    LPCTSTR lpszLibrary,
    _In_    LPCTSTR lpszFunction
    )
{
    HMODULE hModule = GetModuleHandle(lpszLibrary);
    if (!hModule)
    {
        return NULL;
    }
    LPBYTE lpFunction = WaffleGetProcAddress(hModule, lpszFunction);

    int i;
    for (i = lpstProcessSetting->lpstLibrary[0].dwBehind; i >= 0; i--)
    {
        int j;
        for (j = lpstProcessSetting->lpstLibrary[i].lpstFunction[0].dwBehind; j >= 0; j--)
        {
            if (lpFunction == lpstProcessSetting->lpstLibrary[i].lpstFunction[j].lpSource)
            {
                return lpstProcessSetting->lpstLibrary[i].lpstFunction[j].lpBackup;
            }
        }
    }
    return NULL;
}

WAFFLE_COMMON_DLL_FUNCTION SIZE_T WINAPI WaffleFindDetourAddress(
    _In_    PVOID ExceptionAddress,
    _In_    PVOID CallerAddress
    )
{
    if (!ExceptionAddress)
    {
        return 0;
    }

    int i;
    for (i = lpstProcessSetting->lpstLibrary[0].dwBehind; i >= 0; i--)
    {
        LPWAFFLE_FUNCTION_ARRAY lpstFunction = lpstProcessSetting->lpstLibrary[i].lpstFunction;

        //Is this exception happened in a backuped library?
        if (((SIZE_T) ExceptionAddress >= (SIZE_T) lpstProcessSetting->lpstLibrary[i].hSource) && ((SIZE_T) ExceptionAddress <= lpstProcessSetting->lpstLibrary[i].hSourceEnd))
        {
            //Is this exception called by one of our component?
            if (WaffleFindComponent(CallerAddress) >= 0)
            {
                return (SIZE_T) ExceptionAddress - (SIZE_T) lpstProcessSetting->lpstLibrary[i].hSource + (SIZE_T) lpstProcessSetting->lpstLibrary[i].hBackup;
            }

            //Do we have a detour function?
            int j;
            for (j = lpstFunction[0].dwBehind; j >= 0; j--)
            {
                if (lpstFunction[j].lpSource == ExceptionAddress)
                {
                    if (lpstFunction[j].lpDetour)
                    {
                        return (SIZE_T) lpstFunction[j].lpDetour;
                    }
                    break;
                }
            }
            return (SIZE_T) ExceptionAddress - (SIZE_T) lpstProcessSetting->lpstLibrary[i].hSource + (SIZE_T) lpstProcessSetting->lpstLibrary[i].hBackup;
        }
    }

    return 0;
}