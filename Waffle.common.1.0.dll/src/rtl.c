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