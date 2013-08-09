#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"

LIBRARY_EXPORT VOID WINAPI WaffleGetModuleDirectory(
    _In_opt_    HMODULE hModule,
    _Out_       LPTSTR lpFilename,
    _In_        DWORD nSize
    )
{
    GetModuleFileName(hModule, lpFilename, nSize);
    int i;
    for (i = lstrlen(lpFilename); lpFilename[i] != TEXT('\\'); i--);
    lpFilename[i] = TEXT('\0');
}

LIBRARY_EXPORT VOID WINAPI WaffleDisableWow64FsRedirection(
    _Inout_ PVOID *OldValue
    )
{
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    LPWOW64DISABLEWOW64FSREDIRECTION lpWow64DisableWow64FsRedirection = (LPWOW64DISABLEWOW64FSREDIRECTION) WaffleGetProcAddress(hKernel32, TEXT("Wow64DisableWow64FsRedirection"));
    if (lpWow64DisableWow64FsRedirection)
    {
        lpWow64DisableWow64FsRedirection(OldValue);
    }
}

LIBRARY_EXPORT VOID WINAPI WaffleRevertWow64FsRedirection(
    _In_    PVOID OldValue
    )
{
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));

    LPWOW64REVERTWOW64FSREDIRECTION lpWow64RevertWow64FsRedirection = (LPWOW64REVERTWOW64FSREDIRECTION) WaffleGetProcAddress(hKernel32, TEXT("Wow64RevertWow64FsRedirection"));
    if (lpWow64RevertWow64FsRedirection)
    {
        lpWow64RevertWow64FsRedirection(OldValue);
    }
}