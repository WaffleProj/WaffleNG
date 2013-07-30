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
    if ((i == 0) || (i == 2 && bHex))
    {
        return nDefault;
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
    if ((i == 0) || (i == 2 && bHex))
    {
        return nDefault;
    }
    else
    {
        return nResult;
    }
}