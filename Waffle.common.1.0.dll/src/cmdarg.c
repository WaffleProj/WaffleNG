#include "..\common.h"

LIBRARY_EXPORT int WINAPI WaffleArgc(void)
{
    int intArg = 0;
    int i = -1;
    LPTSTR szCommandLine = GetCommandLine();
loop:
    i++;
    if (szCommandLine[i] == TEXT('\0'))
        goto end;
    else if ((szCommandLine[i] == TEXT(' ')) || (szCommandLine[i] == TEXT('\t')))
        goto loop;

    intArg++;
argloop:
    if (szCommandLine[i] == TEXT('\0'))
        goto end;
    else if ((szCommandLine[i] == TEXT(' ')) || (szCommandLine[i] == TEXT('\t')))
        goto loop;
    else if (szCommandLine[i] == TEXT('\"'))
    {
        for (i++; szCommandLine[i] != TEXT('\"'); i++);
        i++;
        goto argloop;
    }
    i++;
    goto argloop;
end:
    return intArg;
}

LIBRARY_EXPORT SIZE_T WINAPI WaffleArgv(
    _In_                int intPosition,
    _Out_writes_(nSize) LPTSTR lpString,
    _In_                int nSize
    )
{
    int intArg = 0;
    int i = -1, j = -1;
    LPTSTR szCommandLine = GetCommandLine();
loop:
    i++;
    if (szCommandLine[i] == TEXT('\0'))
        goto end;
    else if ((szCommandLine[i] == TEXT(' ')) || (szCommandLine[i] == TEXT('\t')))
        goto loop;

    intArg++;
argloop:
    if (szCommandLine[i] == TEXT('\0'))
        goto end;
    else if ((szCommandLine[i] == TEXT(' ')) || (szCommandLine[i] == TEXT('\t')))
        goto loop;
    else if (szCommandLine[i] == TEXT('\"'))
    {
        i++;
        goto deliloop;
    }
    if (intArg == intPosition)
    {
        if (nSize > 1)
        {
            j++;
            lpString[j] = szCommandLine[i];
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
    if (szCommandLine[i] == TEXT('\0'))
        goto end;
    else if (szCommandLine[i] == TEXT('\"'))
    {
        i++;
        goto argloop;
    }
    if (intArg == intPosition)
    {
        if (nSize > 1)
        {
            j++;
            lpString[j] = szCommandLine[i];
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

LIBRARY_EXPORT LPCTSTR WINAPI WaffleArgp(
    _In_    int intPosition
    )
{
    int intArg = 0;
    int i = -1;
    LPTSTR szCommandLine = GetCommandLine();
loop:
    i++;
    if (szCommandLine[i] == TEXT('\0'))
        return NULL;
    else if ((szCommandLine[i] == TEXT(' ')) || (szCommandLine[i] == TEXT('\t')))
        goto loop;

    intArg++;
    if (intArg == intPosition)
        return &szCommandLine[i];

argloop:
    if (szCommandLine[i] == TEXT('\0'))
        return NULL;
    else if ((szCommandLine[i] == TEXT(' ')) || (szCommandLine[i] == TEXT('\t')))
        goto loop;
    else if (szCommandLine[i] == TEXT('\"'))
    {
        for (i++; szCommandLine[i] != TEXT('\"'); i++);
        i++;
        goto argloop;
    }
    i++;
    goto argloop;
}