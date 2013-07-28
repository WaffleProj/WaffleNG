#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\mojibake.h"

LIBRARY_EXPORT int DetourShellAboutW(
    _In_opt_    HWND hWnd,
    _In_        LPCTSTR szApp,
    _In_opt_    LPCTSTR szOtherStuff,
    _In_opt_    HICON hIcon
    )
{
    return ((LPSHELLABOUTW) stShell32Table[SHELLABOUTW].lpNewFunction)(hWnd, TEXT("Mojibake"), TEXT("Other stuff"), hIcon);
}