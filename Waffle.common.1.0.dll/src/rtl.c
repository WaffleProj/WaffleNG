#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"

LIBRARY_EXPORT int WINAPI WafflelstrcmpiW(
    _In_    LPCWSTR lpString1,
    _In_    LPCWSTR lpString2
    )
{
    if (lpString1 && lpString2)
    {
        int i;
        for (i = 0; !(((lpString1[i] | 0x20) != (lpString2[i] | 0x20)) || (lpString1[i] == 0) || (lpString2[i] == 0)); i++);
        return lpString1[i] - lpString2[i];
    }
    else
    {
        return (int) (lpString1 - lpString2);
    }
}

LIBRARY_EXPORT int WINAPI WafflelstrcmpiA(
    _In_    LPCSTR lpString1,
    _In_    LPCSTR lpString2
    )
{
    if (lpString1 && lpString2)
    {
        int i;
        for (i = 0; !(((lpString1[i] | 0x20) != (lpString2[i] | 0x20)) || (lpString1[i] == 0) || (lpString2[i] == 0)); i++);
        return lpString1[i] - lpString2[i];
    }
    else
    {
        return (int) (lpString1 - lpString2);
    }
}

LIBRARY_EXPORT int WINAPI WafflelstrcmpW(
    _In_    LPCWSTR lpString1,
    _In_    LPCWSTR lpString2
    )
{
    if (lpString1 && lpString2)
    {
        int i;
        for (i = 0; !((lpString1[i] != lpString2[i]) || (lpString1[i] == 0) || (lpString2[i] == 0)); i++);
        return lpString1[i] - lpString2[i];
    }
    else
    {
        return (int) (lpString1 - lpString2);
    }
}

LIBRARY_EXPORT int WINAPI WafflelstrcmpA(
    _In_    LPCSTR lpString1,
    _In_    LPCSTR lpString2
    )
{
    if (lpString1 && lpString2)
    {
        int i;
        for (i = 0; !((lpString1[i] != lpString2[i]) || (lpString1[i] == 0) || (lpString2[i] == 0)); i++);
        return lpString1[i] - lpString2[i];
    }
    else
    {
        return (int) (lpString1 - lpString2);
    }
}
LIBRARY_EXPORT int WINAPI WaffleStrToIntW(
    _In_    LPCWSTR lpString,
    _In_    int nDefault
    )
{
    int i = 0;
    BOOL bHex = FALSE;
    int nResult = 0;
    if (lpString[0] == L'0' && (lpString[1] | 0x20) == L'x')
    {
        bHex = TRUE;
        i = 2;
    }
    else if (lpString[0] == L'-')
    {
        i = 1;
    }
    while (lpString[i])
    {
        int Digital;
        if ((lpString[i] >= L'0') && (lpString[i] <= L'9'))
        {
            Digital = lpString[i] - L'0';
        }
        else if (((lpString[i] | 0x20) >= L'a') && ((lpString[i] | 0x20) <= L'f'))
        {
            Digital = (lpString[i] | 0x20) - L'a' + 10;
        }
        else
        {
            break;
        }

        if (!bHex)
        {
            nResult = nResult * 10 + Digital;
        }
        else
        {
            nResult = nResult * 16 + Digital;
        }

        i++;
    }
    if ((i == 0) || (i == 2 && bHex) || (i == 1 && lpString[0] == L'-'))
    {
        return nDefault;
    }
    else if (lpString[0] == L'-')
    {
        return -nResult;
    }
    else
    {
        return nResult;
    }
}

LIBRARY_EXPORT int WINAPI WaffleStrToIntA(
    _In_    LPCSTR lpString,
    _In_    int nDefault
    )
{
    int i = 0;
    BOOL bHex = FALSE;
    int nResult = 0;
    if (lpString[0] == '0' && (lpString[1] | 0x20) == 'x')
    {
        bHex = TRUE;
        i = 2;
    }
    else if (lpString[0] == '-')
    {
        i = 1;
    }
    while (lpString[i])
    {
        int Digital;
        if ((lpString[i] >= '0') && (lpString[i] <= '9'))
        {
            Digital = lpString[i] - '0';
        }
        else if (((lpString[i] | 0x20) >= 'a') && ((lpString[i] | 0x20) <= 'f'))
        {
            Digital = (lpString[i] | 0x20) - 'a' + 10;
        }
        else
        {
            break;
        }

        if (!bHex)
        {
            nResult = nResult * 10 + Digital;
        }
        else
        {
            nResult = nResult * 16 + Digital;
        }

        i++;
    }
    if ((i == 0) || (i == 2 && bHex) || (i == 1 && lpString[0] == '-'))
    {
        return nDefault;
    }
    else if (lpString[0] == '-')
    {
        return -nResult;
    }
    else
    {
        return nResult;
    }
}

LIBRARY_EXPORT VOID WaffleCreateRWLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    if (!lpstRWLock->hReader)
    {
        InitializeCriticalSectionAndSpinCount(&lpstRWLock->csRead, 5120);
        InitializeCriticalSectionAndSpinCount(&lpstRWLock->csWrite, 3072);
        lpstRWLock->hReader = CreateEvent(NULL, TRUE, FALSE, NULL);
        lpstRWLock->dwReader = 0;
    }
}

LIBRARY_EXPORT VOID WaffleReleaseRWLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    if (lpstRWLock->hReader)
    {
        WaitForSingleObject(lpstRWLock->hReader, INFINITE);
        CloseHandle(lpstRWLock->hReader);
        lpstRWLock->hReader = NULL;
        DeleteCriticalSection(&lpstRWLock->csWrite);
        DeleteCriticalSection(&lpstRWLock->csRead);
    }
}

LIBRARY_EXPORT VOID WaffleEnterWriterLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    EnterCriticalSection(&lpstRWLock->csRead);
    for (EnterCriticalSection(&lpstRWLock->csWrite); lpstRWLock->dwReader; EnterCriticalSection(&lpstRWLock->csWrite))
    {
        LeaveCriticalSection(&lpstRWLock->csWrite);
        WaitForSingleObject(lpstRWLock->hReader, INFINITE);
    }
    LeaveCriticalSection(&lpstRWLock->csRead);
}

LIBRARY_EXPORT VOID WaffleLeaveWriterLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    LeaveCriticalSection(&lpstRWLock->csWrite);
}

LIBRARY_EXPORT VOID WaffleEnterReaderLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    EnterCriticalSection(&lpstRWLock->csRead);
    EnterCriticalSection(&lpstRWLock->csWrite);
    lpstRWLock->dwReader++;
    if (lpstRWLock->dwReader == 1)
    {
        ResetEvent(lpstRWLock->hReader);    //nonsignaled, need to wait
    }
    LeaveCriticalSection(&lpstRWLock->csWrite);
    LeaveCriticalSection(&lpstRWLock->csRead);
}

LIBRARY_EXPORT VOID WaffleLeaveReaderLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    EnterCriticalSection(&lpstRWLock->csWrite);
    lpstRWLock->dwReader--;
    if (lpstRWLock->dwReader == 0)
    {
        SetEvent(lpstRWLock->hReader);      //signaled
    }
    LeaveCriticalSection(&lpstRWLock->csWrite);
}