#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
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
    _In_    int intPosition,
    _In_    LPTSTR lpString,
    _In_    int intSize     //好像没用到intSize判断缓冲区?
    )
{
    int intArg = 0;
    BOOL FLAG;
    int i = -1, j = -1;
    LPTSTR szCommandLine = GetCommandLine();
loop:
    i++;
    if (szCommandLine[i] == TEXT('\0'))
        goto end;
    else if ((szCommandLine[i] == TEXT(' ')) || (szCommandLine[i] == TEXT('\t')))
        goto loop;

    intArg++;
    if (intArg == intPosition)
        FLAG = TRUE;
    else
        FLAG = FALSE;
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
    if ((intSize > 2) && (FLAG))
    {
        j++;
        lpString[j] = szCommandLine[i];
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
    if ((intSize > 2) && (FLAG))
    {
        j++;
        lpString[j] = szCommandLine[i];
    }
    i++;
    goto deliloop;
end:
    j++;
    lpString[j] = TEXT('\0');
    return j;   //返回复制的字符数(不含末尾0)
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