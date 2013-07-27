#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

LIBRARY_EXPORT BOOL WINAPI WaffleCreateProcess(
    _In_opt_    LPCTSTR lpApplicationName,
    _Inout_opt_ LPTSTR lpCommandLine,
    _In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_        BOOL bInheritHandles,
    _In_        DWORD dwCreationFlags,
    _In_opt_    LPVOID lpEnvironment,
    _In_opt_    LPCTSTR lpCurrentDirectory,
    _In_opt_    LPSTARTUPINFO lpStartupInfo,
    _Out_opt_   LPPROCESS_INFORMATION lpProcessInformation
    )
{
    STARTUPINFO stStartUp;
    PROCESS_INFORMATION stProcessInfo;

    if (!lpStartupInfo)
    {
        stStartUp.cb = sizeof(stStartUp);
        GetStartupInfo(&stStartUp);

        lpStartupInfo = &stStartUp;
    }
    if (!lpProcessInformation)
    {
        lpProcessInformation = &stProcessInfo;
    }

    //Disable redirection if the target is in the "system32"
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    PVOID OldValue = 0;

    //Open the target
    LPWOW64DISABLEWOW64FSREDIRECTION lpWow64DisableWow64FsRedirection = (LPWOW64DISABLEWOW64FSREDIRECTION) GetProcAddress(hKernel32, "Wow64DisableWow64FsRedirection");
    if (lpWow64DisableWow64FsRedirection)
    {
        lpWow64DisableWow64FsRedirection(&OldValue);
    }

    BOOL Result = CreateProcess(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    DWORD LastError = GetLastError();

    LPWOW64REVERTWOW64FSREDIRECTION lpWow64RevertWow64FsRedirection = (LPWOW64REVERTWOW64FSREDIRECTION) GetProcAddress(hKernel32, "Wow64RevertWow64FsRedirection");
    if (lpWow64RevertWow64FsRedirection)
    {
        lpWow64RevertWow64FsRedirection(OldValue);
    }
    
    SetLastError(LastError);
    return Result;
}

LIBRARY_EXPORT void WINAPI WaffleInjectDll(
    _In_    HANDLE hProcess,
    _In_    LPCTSTR lpszDllFull
    )
{
    LPVOID lpszRemoteDll = VirtualAllocEx(hProcess, NULL, MAX_PATH*sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);
    if (lpszRemoteDll)
    {
        FARPROC lpLoadLibrary = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryW");

        WriteProcessMemory(hProcess, lpszRemoteDll, lpszDllFull, lstrlen(lpszDllFull)*sizeof(TCHAR), NULL);
        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE) lpLoadLibrary, lpszRemoteDll, 0, NULL);
        WaitForSingleObject(hThread, INFINITE);

        VirtualFreeEx(hProcess, lpszRemoteDll, 0, MEM_RELEASE);
        CloseHandle(hThread);
    }
}

LIBRARY_EXPORT void WINAPI WaffleExecute(
    _In_opt_    LPCTSTR lpApplicationName,
    _Inout_opt_ LPTSTR lpCommandLine,
    _In_opt_    LPCTSTR lpCurrentDirectory,
    _Out_opt_   LPWAFFLE_PROCESS_SETTING lpstProcessSetting
    )
{
        PROCESS_INFORMATION stProcessInfo;

        WaffleCreateProcess(lpApplicationName, lpCommandLine, NULL, NULL, TRUE, CREATE_SUSPENDED, 0, lpCurrentDirectory, NULL, &stProcessInfo);

        lpstProcessSetting->dwThreadId = stProcessInfo.dwThreadId;
        lpstProcessSetting->dwProcessId = stProcessInfo.dwProcessId;
        CloseHandle(stProcessInfo.hThread);

        WaffleInjectDll(stProcessInfo.hProcess, szWaffleCommonDll);

        CloseHandle(stProcessInfo.hProcess);
}