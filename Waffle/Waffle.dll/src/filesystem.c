#include "..\common.h"

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleGetModuleDirectory(
    _In_opt_            HMODULE hModule,
    _Out_writes_(nSize) LPTSTR lpFilename,
    _In_                DWORD nSize
)
{
    if (!lpFilename)
    {
        return;
    }

    DWORD i = GetModuleFileName(hModule, lpFilename, nSize);
    if (i)
    {
        while ((lpFilename[i--] != TEXT('\\')));
        lpFilename[i + 1] = TEXT('\0');
    }
    else
    {
        lpFilename[0] = TEXT('\0');
    }
}

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleDisableWow64FsRedirection(
    _Inout_ PVOID *OldValue
)
{
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    if (hKernel32)
    {
        LPWOW64DISABLEWOW64FSREDIRECTION lpWow64DisableWow64FsRedirection = (LPWOW64DISABLEWOW64FSREDIRECTION) WaffleGetProcAddress(hKernel32, TEXT("Wow64DisableWow64FsRedirection"));
        if (lpWow64DisableWow64FsRedirection)
        {
            lpWow64DisableWow64FsRedirection(OldValue);
        }
    }
}

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleRevertWow64FsRedirection(
    _In_    PVOID OldValue
)
{
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    if (hKernel32)
    {
        LPWOW64REVERTWOW64FSREDIRECTION lpWow64RevertWow64FsRedirection = (LPWOW64REVERTWOW64FSREDIRECTION) WaffleGetProcAddress(hKernel32, TEXT("Wow64RevertWow64FsRedirection"));
        if (lpWow64RevertWow64FsRedirection)
        {
            lpWow64RevertWow64FsRedirection(OldValue);
        }
    }
}