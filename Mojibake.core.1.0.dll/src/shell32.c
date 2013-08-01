#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\mojibake.h"

#ifdef __cplusplus
extern "C" {
#endif
LIBRARY_EXPORT int DetourShellAboutA(
    _In_opt_    HWND hWnd,
    _In_        LPCSTR szApp,
    _In_opt_    LPCSTR szOtherStuff,
    _In_opt_    HICON hIcon
    )
{
    LPWSTR uszApp = AnsiToUnicode(szApp);
    LPWSTR uszOtherStuff = AnsiToUnicode(szOtherStuff);
    int Result = ShellAbout(hWnd, uszApp, uszOtherStuff, hIcon);

    DWORD LastError = GetLastError();
    HeapFree(hHeap, 0, uszApp);
    HeapFree(hHeap, 0, uszOtherStuff);
    SetLastError(LastError);
    return Result;
}
#ifdef __cplusplus
};
#endif