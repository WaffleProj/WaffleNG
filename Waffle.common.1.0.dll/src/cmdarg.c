#include "..\common.h"

WAFFLE_COMMON_DLL_FUNCTION int WINAPI WaffleArgc(
    _In_opt_    LPCTSTR lpCmdLine
    )
{
    int intArg = 0;
    int i = -1;
    if (!lpCmdLine)
    {
        lpCmdLine = GetCommandLine();
    }
loop:
    i++;
    if (lpCmdLine[i] == TEXT('\0'))
        goto end;
    else if ((lpCmdLine[i] == TEXT(' ')) || (lpCmdLine[i] == TEXT('\t')))
        goto loop;

    intArg++;
argloop:
    if (lpCmdLine[i] == TEXT('\0'))
        goto end;
    else if ((lpCmdLine[i] == TEXT(' ')) || (lpCmdLine[i] == TEXT('\t')))
        goto loop;
    else if (lpCmdLine[i] == TEXT('\"'))
    {
        for (i++; lpCmdLine[i] != TEXT('\"'); i++);
        i++;
        goto argloop;
    }
    i++;
    goto argloop;
end:
    return intArg;
}

WAFFLE_COMMON_DLL_FUNCTION SIZE_T WINAPI WaffleArgv(
    _In_opt_            LPCTSTR lpCmdLine,
    _In_                int intPosition,
    _Out_writes_(nSize) LPTSTR lpString,
    _In_                int nSize
    )
{
    int intArg = 0;
    int i = -1, j = -1;
    if (!lpCmdLine)
    {
        lpCmdLine = GetCommandLine();
    }
loop:
    i++;
    if (lpCmdLine[i] == TEXT('\0'))
        goto end;
    else if ((lpCmdLine[i] == TEXT(' ')) || (lpCmdLine[i] == TEXT('\t')))
        goto loop;

    intArg++;
argloop:
    if (lpCmdLine[i] == TEXT('\0'))
        goto end;
    else if ((lpCmdLine[i] == TEXT(' ')) || (lpCmdLine[i] == TEXT('\t')))
        goto loop;
    else if (lpCmdLine[i] == TEXT('\"'))
    {
        i++;
        goto deliloop;
    }
    if (intArg == intPosition)
    {
        if (nSize > 1)
        {
            j++;
            lpString[j] = lpCmdLine[i];
            nSize--;
        }
        else
        {
            goto end;
        }
    }
    i++;
    goto argloop;
deliloop:
    if (lpCmdLine[i] == TEXT('\0'))
        goto end;
    else if (lpCmdLine[i] == TEXT('\"'))
    {
        i++;
        goto argloop;
    }
    if (intArg == intPosition)
    {
        if (nSize > 1)
        {
            j++;
            lpString[j] = lpCmdLine[i];
            nSize--;
        }
        else
        {
            goto end;
        }
    }
    i++;
    goto deliloop;
end:
    j++;
    lpString[j] = TEXT('\0');
    return j + 1;
}

WAFFLE_COMMON_DLL_FUNCTION LPCTSTR WINAPI WaffleArgp(
    _In_opt_    LPCTSTR lpCmdLine,
    _In_        int intPosition
    )
{
    int intArg = 0;
    int i = -1;
    if (!lpCmdLine)
    {
        lpCmdLine = GetCommandLine();
    }
loop:
    i++;
    if (lpCmdLine[i] == TEXT('\0'))
        return NULL;
    else if ((lpCmdLine[i] == TEXT(' ')) || (lpCmdLine[i] == TEXT('\t')))
        goto loop;

    intArg++;
    if (intArg == intPosition)
        return &lpCmdLine[i];

argloop:
    if (lpCmdLine[i] == TEXT('\0'))
        return NULL;
    else if ((lpCmdLine[i] == TEXT(' ')) || (lpCmdLine[i] == TEXT('\t')))
        goto loop;
    else if (lpCmdLine[i] == TEXT('\"'))
    {
        for (i++; lpCmdLine[i] != TEXT('\"'); i++);
        i++;
        goto argloop;
    }
    i++;
    goto argloop;
}