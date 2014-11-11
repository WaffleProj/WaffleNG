#include "..\main.h"

typedef INT(WINAPI *LPCOMPARE) (LPVOID Element1, LPVOID Element2);

typedef enum
{
    SEARCH_SUCCESS = 0,
    SEARCH_BETWEEN,
    SEARCH_MISSING
} WAFFLE_SMART_ARRAY_SEARCH_RESULT_ENUM, *LPWAFFLE_SMART_ARRAY_SEARCH_RESULT_ENUM;

typedef struct
{
    WAFFLE_SMART_ARRAY_SEARCH_RESULT_ENUM   Status;
    SIZE_T  Param1;
    SIZE_T  Param2;
} WAFFLE_SMART_ARRAY_SEARCH_RESULT, *LPWAFFLE_SMART_ARRAY_SEARCH_RESULT;

typedef struct
{
    HANDLE      hHeap;
    SRWLOCK     hLock;
    LPCOMPARE   pCompare;
    SIZE_T      nSize;
    SIZE_T      nCount;
    LPVOID *    lpArray;
} WAFFLE_SMART_ARRAY, *LPWAFFLE_SMART_ARRAY;

LIBRARY_EXPORT LPWAFFLE_SMART_ARRAY WINAPI WaffleSmartArrayCreate(
    _In_    SIZE_T nSize,
    _In_    LPCOMPARE pCompare
    )
{
    if (!nSize || !pCompare)
    {
        return NULL;
    }

    HANDLE hHeap = HeapCreate(0, 0, 0);
    if (!hHeap)
    {
        return NULL;
    }

    LPWAFFLE_SMART_ARRAY lpHeader = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(*lpHeader));
    if (!lpHeader)
    {
        HeapDestroy(hHeap);
        return NULL;
    }

    lpHeader->hHeap = hHeap;
    InitializeSRWLock(&lpHeader->hLock);
    lpHeader->pCompare = pCompare;
    lpHeader->nSize = nSize;
    lpHeader->nCount = 0;
    lpHeader->lpArray = NULL;

    return lpHeader;
}

LIBRARY_EXPORT BOOL WINAPI WaffleSmartArrayVerify(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader
    )
{
    if (!lpHeader || !lpHeader->hHeap || !lpHeader->pCompare || !lpHeader->nSize)
    {
        return FALSE;
    }

    if (lpHeader->nCount && !lpHeader->lpArray)
    {
        return FALSE;
    }

    if (!lpHeader->nCount && lpHeader->lpArray)
    {
        return FALSE;
    }

    return TRUE;
}

LIBRARY_EXPORT BOOL WINAPI WaffleSmartArrayDestroy(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader
    )
{
    if (!WaffleSmartArrayVerify(lpHeader))
    {
        return FALSE;
    }

    return HeapDestroy(lpHeader->hHeap);
}

LIBRARY_EXPORT WAFFLE_SMART_ARRAY_SEARCH_RESULT WINAPI WaffleSmartArraySearch(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader,
    _In_    LPVOID lpElement
    )
{
    WAFFLE_SMART_ARRAY_SEARCH_RESULT stResult;
    stResult.Status = SEARCH_MISSING;
    stResult.Param1 = SIZE_MAX;
    stResult.Param2 = 0;

    if (!WaffleSmartArrayVerify(lpHeader))
    {
        return stResult;
    }

    SIZE_T Min = 0;
    SIZE_T Max = lpHeader->nCount - 1;
    LPVOID * lpArray = lpHeader->lpArray;
    for (SIZE_T Index = Min + (Max - Min) / 2; Min <= Max; Index = Min + (Max - Min) / 2)
    {
        INT nResult = lpHeader->pCompare(lpElement, lpArray[Index]);
        if (nResult > 0)
        {
            Min = Index + 1;
        }
        else if (nResult < 0)
        {
            Max = Index - 1;
        }
        else if (nResult == 0)
        {
            stResult.Status = SEARCH_SUCCESS;
            stResult.Param1 = Index;
            stResult.Param2 = Index;

            return stResult;
        }
    }

    stResult.Status = SEARCH_BETWEEN;
    stResult.Param1 = Min;
    stResult.Param2 = Max;

    return stResult;
}

LIBRARY_EXPORT BOOL WINAPI WaffleSmartArrayAddUnsafe(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader,
    _In_    LPVOID lpElement
    )
{
    if (!WaffleSmartArrayVerify(lpHeader) || !lpElement)
    {
        return FALSE;
    }

    AcquireSRWLockExclusive(&lpHeader->hLock);

    if (!lpHeader->nCount)
    {
        lpHeader->lpArray = (LPVOID *) HeapAlloc(lpHeader->hHeap, HEAP_ZERO_MEMORY, sizeof(lpHeader->lpArray[0]) * 1);
        if (!lpHeader->lpArray)
        {
            ReleaseSRWLockExclusive(&lpHeader->hLock);
            return FALSE;
        }

        lpHeader->lpArray[0] = HeapAlloc(lpHeader->hHeap, HEAP_ZERO_MEMORY, lpHeader->nSize);
        if (!lpHeader->lpArray)
        {
            HeapDestroy(lpHeader->lpArray);
            lpHeader->lpArray = NULL;
            ReleaseSRWLockExclusive(&lpHeader->hLock);
            return FALSE;
        }

        RtlMoveMemory(lpHeader->lpArray, lpElement, lpHeader->nSize);
        lpHeader->nCount++;
        ReleaseSRWLockExclusive(&lpHeader->hLock);
        return TRUE;
    }

    WAFFLE_SMART_ARRAY_SEARCH_RESULT stResult = WaffleSmartArraySearch(lpHeader, lpElement);
    switch (stResult.Status)
    {
        case SEARCH_BETWEEN:
        {
            LPVOID lpNewElement = HeapAlloc(lpHeader->hHeap, 0, lpHeader->nSize);
            if (!lpNewElement)
            {
                ReleaseSRWLockExclusive(&lpHeader->hLock);
                return FALSE;
            }

            LPVOID * lpNewArray = HeapReAlloc(lpHeader->hHeap, HEAP_ZERO_MEMORY, lpHeader->lpArray, sizeof(lpHeader->lpArray[0]) * (lpHeader->nCount + 1));
            if (!lpNewArray)
            {
                HeapFree(lpHeader->hHeap, 0, lpNewElement);
                ReleaseSRWLockExclusive(&lpHeader->hLock);
                return FALSE;
            }

            RtlMoveMemory(lpNewElement, lpElement, lpHeader->nSize);

            break;
        }
        case SEARCH_SUCCESS:
        {
            break;
        }
        case SEARCH_MISSING:
        {
            break;
        }
    }
}