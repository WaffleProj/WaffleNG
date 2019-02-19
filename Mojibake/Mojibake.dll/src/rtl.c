#include "..\mojibake.h"

LPWSTR WINAPI MBCSToUnicode(
    _In_        UINT AnsiCodePage,
    _In_opt_    LPCSTR lpszText
)
{
    if (lpszText)
    {
        int intText = lstrlenA(lpszText);
        if (intText)
        {
            intText++;
            LPWSTR lpuszText = (LPWSTR) WaffleAlloc(4 * intText);
            MultiByteToWideChar(AnsiCodePage, 0, lpszText, -1, lpuszText, intText);
            return lpuszText;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

LPSTR WINAPI MBCSToMBCS(
    _In_        UINT FromCodePage,
    _In_        UINT ToCodePage,
    _In_opt_    LPCSTR lpszText
)
{
    if (lpszText)
    {
        int intText = lstrlenA(lpszText) + 1;
        LPWSTR lpuszText = (LPWSTR) WaffleAlloc(4 * intText);
        LPSTR lpszNewText = (LPSTR) WaffleAlloc(4 * intText);
        MultiByteToWideChar(FromCodePage, 0, lpszText, -1, lpuszText, intText);
        WideCharToMultiByte(ToCodePage, 0, lpuszText, -1, lpszNewText, 4 * intText, NULL, FALSE);
        MojibakeFree(lpuszText);
        return lpszNewText;
    }
    else
    {
        return NULL;
    }
}

LPWSTR WINAPI AnsiToUnicode(
    _In_opt_    LPCSTR lpszText
)
{
    if (lpszText)
    {
        int intText = lstrlenA(lpszText);
        if (intText)
        {
            intText++;
            LPWSTR lpuszText = (LPWSTR) WaffleAlloc(4 * intText);
            MultiByteToWideChar(stNewEnvir.AnsiCodePage, 0, lpszText, -1, lpuszText, intText);
            return lpuszText;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

LPSTR WINAPI UnicodeToAnsi(
    _In_opt_    LPCWSTR lpuszText
)
{
    if (lpuszText)
    {
        int intText = lstrlenW(lpuszText);
        if (intText)
        {
            intText++;
            LPSTR lpszText = (LPSTR) WaffleAlloc(4 * intText);
            WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszText, -1, lpszText, 4 * intText, NULL, FALSE);
            return lpszText;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

LPSTR WINAPI ProgramCPToWindowsCP(
    _In_opt_    LPCSTR lpszText
)
{
    if (lpszText)
    {
        int intText = lstrlenA(lpszText) + 1;
        LPWSTR lpuszText = (LPWSTR) WaffleAlloc(4 * intText);
        LPSTR lpszNewText = (LPSTR) WaffleAlloc(4 * intText);
        MultiByteToWideChar(stNewEnvir.AnsiCodePage, 0, lpszText, -1, lpuszText, intText);
        WideCharToMultiByte(stOldEnvir.AnsiCodePage, 0, lpuszText, -1, lpszNewText, 4 * intText, NULL, FALSE);
        MojibakeFree(lpuszText);
        return lpszNewText;
    }
    else
    {
        return NULL;
    }
}

_Ret_maybenull_
_Success_(return == 0)
LPVOID WINAPI MojibakeFree(
    _In_opt_    LPVOID lpMem
)
{
    if (lpMem)
    {
        DWORD LastError = GetLastError();
        lpMem = WaffleFree(lpMem);
        SetLastError(LastError);
    }
    return lpMem;
}