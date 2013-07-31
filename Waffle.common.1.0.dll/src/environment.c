#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"

LIBRARY_EXPORT LPTSTR WINAPI WaffleGetComponentDirectory(VOID)
{
    static TCHAR szComponentDirectory[MAX_PATH];
    return szComponentDirectory;
}
