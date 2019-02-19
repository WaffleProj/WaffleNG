#include "..\main.h"

WAFFLE_HOOK_DLL_FUNCTION LPWAFFLE_SMART_ARRAY WINAPI WaffleSmartArrayCreate(
    _In_    SIZE_T nSize
)
{
    if (!nSize)
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
    lpHeader->nSize = nSize;
    lpHeader->nCount = 0;
    lpHeader->lpArray = NULL;

    return lpHeader;
}

WAFFLE_HOOK_DLL_FUNCTION BOOL WINAPI WaffleSmartArrayVerify(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader
)
{
    if (!lpHeader || !lpHeader->hHeap || !lpHeader->nSize)
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

WAFFLE_HOOK_DLL_FUNCTION BOOL WINAPI WaffleSmartArrayDestroy(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader
)
{
    if (!lpHeader || !lpHeader->hHeap)
    {
        return FALSE;
    }

    AcquireSRWLockExclusive(&lpHeader->hLock);
    ReleaseSRWLockExclusive(&lpHeader->hLock);

    return HeapDestroy(lpHeader->hHeap);
}

WAFFLE_HOOK_DLL_FUNCTION LPVOID WINAPI WaffleSmartArrayIndexToAddress(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader,
    _In_    SIZE_T Index
)
{
    return (LPVOID) (((SIZE_T) lpHeader->lpArray) + lpHeader->nSize * Index);
}

WAFFLE_HOOK_DLL_FUNCTION WAFFLE_SMART_ARRAY_SEARCH_RESULT WINAPI WaffleSmartArraySearch(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader,
    _In_    LPVOID lpElement,
    _In_    LPCOMPARE pCompare
)
{
    WAFFLE_SMART_ARRAY_SEARCH_RESULT stResult;
    stResult.Status = SEARCH_MISSING;
    stResult.Param1 = SIZE_MAX;
    stResult.Param2 = 0;

    if (!WaffleSmartArrayVerify(lpHeader) || !lpHeader->lpArray)
    {
        return stResult;
    }

    AcquireSRWLockShared(&lpHeader->hLock);

    for (SIZE_T i = 0; i < lpHeader->nCount - 1; i++)
    {
        if (!pCompare(lpElement, WaffleSmartArrayIndexToAddress(lpHeader, i)))
        {
            stResult.Status = SEARCH_SUCCESS;
            stResult.Param1 = i;
            stResult.Param2 = i;

            break;
        }
    }

    ReleaseSRWLockShared(&lpHeader->hLock);

    return stResult;
}

WAFFLE_HOOK_DLL_FUNCTION LPVOID WINAPI WaffleSmartArrayAdd(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader,
    _In_    LPVOID lpElement
)
{
    if (!WaffleSmartArrayVerify(lpHeader) || !lpElement)
    {
        return NULL;
    }

    AcquireSRWLockExclusive(&lpHeader->hLock);

    LPVOID lpNewArray = NULL;
    if (!lpHeader->nCount)
    {
        lpNewArray = HeapAlloc(lpHeader->hHeap, HEAP_ZERO_MEMORY, lpHeader->nSize * (lpHeader->nCount + 1));
        if (!lpNewArray)
        {
            ReleaseSRWLockExclusive(&lpHeader->hLock);
            return NULL;
        }
    }
    else
    {
        lpNewArray = HeapReAlloc(lpHeader->hHeap, HEAP_ZERO_MEMORY, lpHeader->lpArray, lpHeader->nSize * (lpHeader->nCount + 1));
        if (!lpNewArray)
        {
            ReleaseSRWLockExclusive(&lpHeader->hLock);
            return NULL;
        }
    }

    lpHeader->lpArray = lpNewArray;
    RtlMoveMemory(WaffleSmartArrayIndexToAddress(lpHeader, lpHeader->nCount), lpElement, lpHeader->nSize);
    lpHeader->nCount++;
    ReleaseSRWLockExclusive(&lpHeader->hLock);
    return WaffleSmartArrayIndexToAddress(lpHeader, lpHeader->nCount - 1);
}