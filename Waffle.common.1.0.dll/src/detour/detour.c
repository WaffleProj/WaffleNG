#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\..\common.h"

LIBRARY_EXPORT BOOL WINAPI WaffleSetDetour(
    _In_    DWORD dwLibrary,
    _In_    DWORD dwFunction
    )
{
    BOOL bDetour = FALSE;

    if (!lpstProcessSetting->lpstLibrary)
    {
        return bDetour;
    }
    else if (dwLibrary > lpstProcessSetting->lpstLibrary[0].dwBehind)
    {
        return bDetour;
    }
    else if (dwFunction > lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[0].dwBehind)
    {
        return bDetour;
    }
    else if (!lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpDetour)
    {
        return bDetour;
    }
    else if (!lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpSource)
    {
        return bDetour;
    }

    if (!bDetour)
    {
        bDetour = WaffleInlineDetour(lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpSource);
    }
    if (!bDetour)
    {
        bDetour = WaffleExceptionDetour(lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpSource);
    }
    return bDetour;
}