#include "..\common.h"

LIBRARY_EXPORT BOOL WINAPI WaffleAddFunction(
    _In_    LPWAFFLE_LIBRARY_ARRAY lpstNewLibrary,
    _In_    LPCTSTR lpszFunction,
    _In_    HMODULE hDetour,
    _In_    LPCTSTR lpszDetour
    )
{
    if (lpszFunction[0] == TEXT('#')) //This is a comment
    {
        return FALSE;
    }
    LPBYTE lpSource = WaffleGetProcAddress(lpstNewLibrary->hSource, lpszFunction);
    if (!lpSource)  //This function doesn't exist
    {
        return FALSE;
    }
    if (!lpstNewLibrary->lpstFunction)
    {
        lpstNewLibrary->lpstFunction = (LPWAFFLE_FUNCTION_ARRAY) WaffleAlloc(sizeof(WAFFLE_FUNCTION_ARRAY));
        if (!lpstNewLibrary->lpstFunction)
        {
            MessageBox(0, TEXT("FIXME:Unable to allocate memory for function array"), 0, 0);
            ExitProcess(0);
        }
    }
    else
    {
        int i;
        for (i = lpstNewLibrary->lpstFunction[0].dwBehind; i >= 0; i--)
        {
            lpstNewLibrary->lpstFunction[i].dwBehind++;
        }
        lpstNewLibrary->lpstFunction = (LPWAFFLE_FUNCTION_ARRAY) WaffleReAlloc(lpstNewLibrary->lpstFunction, sizeof(WAFFLE_FUNCTION_ARRAY)*(lpstNewLibrary->lpstFunction[0].dwBehind + 1));
        if (!lpstNewLibrary->lpstFunction)
        {
            MessageBox(0, TEXT("FIXME:Unable to add elements in function array"), 0, 0);
            ExitProcess(0);
        }
    }

    lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].dwBehind = 0;
    lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpszFunction = lpszFunction;
    lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpSource = lpSource;
    lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpBackup = WaffleGetProcAddress(lpstNewLibrary->hBackup, lpszFunction);
    if (!hDetour)
    {
        lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpDetour = 0;
    }
    else if (!lpszDetour)
    {
        DWORD nSize = lstrlen(lpszFunction) + sizeof(TEXT("Detour"));
        LPTSTR lpszDefaultDetour = (LPTSTR) WaffleAlloc(nSize*sizeof(TCHAR));
        wsprintf(lpszDefaultDetour, TEXT("Detour%s"), lpszFunction);
        lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpDetour = WaffleGetProcAddress(hDetour, lpszDefaultDetour);
        if (!lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpDetour)
        {
            TCHAR szMissing[256];
            wsprintf(szMissing, TEXT("FIXME:Cannot find %s for %s"), lpszDefaultDetour, lpszFunction);
            MessageBox(0, szMissing, 0, 0);
        }
        WaffleFree(lpszDefaultDetour);
    }
    else
    {
        lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpDetour = WaffleGetProcAddress(hDetour, lpszDetour);
        if (!lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpDetour)
        {
            TCHAR szMissing[256];
            wsprintf(szMissing, TEXT("FIXME:Cannot find %s for %s"), lpszDetour, lpszFunction);
            MessageBox(0, szMissing, 0, 0);
        }
    }
    return TRUE;
}

LIBRARY_EXPORT DWORD WINAPI WaffleCreateFunctionArray(
    _In_    LPWAFFLE_LIBRARY_ARRAY lpstNewLibrary
    )
{
    //GetAllKey
    LPTSTR lpszKey = WaffleGetOptionSectionKeys(TEXT("Detour.ini"), lpstNewLibrary->lpszLibrary);
    //get the list
    DWORD nFunction = 0;

    LPTSTR lpszNextKey = lpszKey;
    DWORD nSizeOfKey = lstrlen(lpszNextKey);
    while (nSizeOfKey)
    {
        int i;
        for (i = 0; lpszNextKey[i] != TEXT('=') && lpszNextKey[i] != TEXT('\0'); i++);  //FIXME
        lpszNextKey[i] = TEXT('\0');

        DWORD nSizeOfFunction = lstrlen(lpszNextKey);
        if (WaffleAddFunction(lpstNewLibrary, lpszNextKey, WaffleLoadComponent(lpszNextKey + nSizeOfFunction + 1), NULL))
        {
            nFunction++;
        }

        lpszNextKey = lpszNextKey + nSizeOfKey + 1;
        nSizeOfKey = lstrlen(lpszNextKey);
    }
    return nFunction;
}

LIBRARY_EXPORT LPVOID WINAPI WaffleGetBackupAddressW(
    _In_    LPCWSTR lpszLibrary,
    _In_    LPCWSTR lpszFunction
    )
{
    int i;
    for (i = lpstProcessSetting->lpstLibrary[0].dwBehind; i >= 0; i--)
    {
        if (!WafflelstrcmpiW(lpszLibrary, lpstProcessSetting->lpstLibrary[i].lpszLibrary))
        {
            return WaffleGetProcAddressW(lpstProcessSetting->lpstLibrary[i].hBackup, lpszFunction); //uses WideCharToMultiByte
            /*
            int j;
            for (j = lpstProcessSetting->lpstLibrary[i].lpstFunction[0].dwBehind; j >= 0; j--)
            {
            if (!Wafflelstrcmp(lpszFunction, lpstProcessSetting->lpstLibrary[i].lpstFunction[j].lpszFunction))
            {
            return lpstProcessSetting->lpstLibrary[i].lpstFunction[j].lpBackup;
            }
            }
            break;
            */
        }
    }
    return NULL;
}

LIBRARY_EXPORT LPVOID WINAPI WaffleGetBackupAddressA(
    _In_    LPCWSTR lpszLibrary,
    _In_    LPCSTR lpszFunction
    )
{
    int i;
    for (i = lpstProcessSetting->lpstLibrary[0].dwBehind; i >= 0; i--)
    {
        if (!WafflelstrcmpiW(lpszLibrary, lpstProcessSetting->lpstLibrary[i].lpszLibrary))
        {
            return WaffleGetProcAddressA(lpstProcessSetting->lpstLibrary[i].hBackup, lpszFunction); //uses WideCharToMultiByte
            /*
            int j;
            for (j = lpstProcessSetting->lpstLibrary[i].lpstFunction[0].dwBehind; j >= 0; j--)
            {
            if (!Wafflelstrcmp(lpszFunction, lpstProcessSetting->lpstLibrary[i].lpstFunction[j].lpszFunction))
            {
            return lpstProcessSetting->lpstLibrary[i].lpstFunction[j].lpBackup;
            }
            }
            break;
            */
        }
    }
    return NULL;
}

LIBRARY_EXPORT SIZE_T WINAPI WaffleFindDetourAddress(
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