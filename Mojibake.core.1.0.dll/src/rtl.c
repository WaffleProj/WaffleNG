#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\mojibake.h"

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
            return 0;
        }
    }
    else
    {
        return 0;
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
        return lpszNewText;
    }
    else
        return 0;
}

VOID WINAPI KeepLastErrorAndFree(
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