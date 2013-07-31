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
LIBRARY_EXPORT int DetourShellAboutW(
    _In_opt_    HWND hWnd,
    _In_        LPCTSTR szApp,
    _In_opt_    LPCTSTR szOtherStuff,
    _In_opt_    HICON hIcon
    )
{
    static LPSHELLABOUTW BackupShellAboutW;
    if (!BackupShellAboutW)
    {
        BackupShellAboutW = (LPSHELLABOUTW) WaffleGetBackupAddress(TEXT("shell32.dll"), TEXT("ShellAboutW"));
    }
    return BackupShellAboutW(hWnd, TEXT("Mojibake"), TEXT("Other stuff"), hIcon);
}
#ifdef __cplusplus
};
#endif