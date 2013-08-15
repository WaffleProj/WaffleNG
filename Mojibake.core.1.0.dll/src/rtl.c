#include "..\mojibake.h"

LPWSTR WINAPI MBCSToUnicode(
    _In_    UINT AnsiCodePage,
    _In_    LPCSTR lpszText
    )
{
    if (lpszText)
    {
        int intText = lstrlenA(lpszText);
        if (intText)
        {
            intText++;
            LPWSTR lpuszText = (LPWSTR) WaffleAlloc(2 * intText);
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
    _In_    UINT FromCodePage,
    _In_    UINT ToCodePage,
    _In_    LPCSTR lpszText
    )
{
    if (lpszText)
    {
        int intText = lstrlenA(lpszText) + 1;
        LPWSTR lpuszText = (LPWSTR) WaffleAlloc(2 * intText);
        LPSTR lpszNewText = (LPSTR) WaffleAlloc(2 * intText);
        MultiByteToWideChar(FromCodePage, 0, lpszText, -1, lpuszText, intText);
        WideCharToMultiByte(ToCodePage, 0, lpuszText, -1, lpszNewText, intText, NULL, FALSE);
        MojibakeFree(lpuszText);
        return lpszNewText;
    }
    else
    {
        return NULL;
    }
}

LPWSTR WINAPI AnsiToUnicode(
    _In_    LPCSTR lpszText
    )
{
    if (lpszText)
    {
        int intText = lstrlenA(lpszText);
        if (intText)
        {
            intText++;
            LPWSTR lpuszText = (LPWSTR) WaffleAlloc(2 * intText);
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

LPSTR WINAPI ProgramCPToWindowsCP(
    _In_    LPCSTR lpszText
    )
{
    if (lpszText)
    {
        int intText = lstrlenA(lpszText) + 1;
        LPWSTR lpuszText = (LPWSTR) WaffleAlloc(2 * intText);
        LPSTR lpszNewText = (LPSTR) WaffleAlloc(2 * intText);
        MultiByteToWideChar(stNewEnvir.AnsiCodePage, 0, lpszText, -1, lpuszText, intText);
        WideCharToMultiByte(stOldEnvir.AnsiCodePage, 0, lpuszText, -1, lpszNewText, intText, NULL, FALSE);
        MojibakeFree(lpuszText);
        return lpszNewText;
    }
    else
    {
        return NULL;
    }
}

VOID WINAPI MojibakeFree(
    _In_    LPVOID lpMem
    )
{
    if (lpMem)
    {
        DWORD LastError = GetLastError();
        WaffleFree(lpMem);
        SetLastError(LastError);
    }
    return;
}