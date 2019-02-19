#include "..\common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

WAFFLE_COMMON_DLL_FUNCTION BOOL WINAPI WaffleCreateProcess(
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

    PVOID OldValue = 0;
    WaffleDisableWow64FsRedirection(&OldValue);
    BOOL Result = CreateProcess(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    DWORD LastError = GetLastError();
    WaffleRevertWow64FsRedirection(OldValue);

    SetLastError(LastError);
    return Result;
}

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleInjectDll(
    _In_    HANDLE hProcess,
    _In_    HANDLE hThread,
    _In_    LPCTSTR lpszDllFull
)
{
    LPVOID lpszRemoteDll = VirtualAllocEx(hProcess, NULL, MAX_PATH * sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);
    if (lpszRemoteDll)
    {
        LPTHREAD_START_ROUTINE lpLoadLibrary = (LPTHREAD_START_ROUTINE) WaffleGetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), TEXT("LoadLibraryW"));

        WriteProcessMemory(hProcess, lpszRemoteDll, lpszDllFull, lstrlen(lpszDllFull) * sizeof(TCHAR), NULL);
        HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, lpLoadLibrary, lpszRemoteDll, 0, NULL);
        if (!hRemoteThread)
        {
            VirtualFreeEx(hProcess, lpszRemoteDll, 0, MEM_RELEASE);
            return;
        }

        WaitForSingleObject(hRemoteThread, INFINITE);
        VirtualFreeEx(hProcess, lpszRemoteDll, 0, MEM_RELEASE);
        CloseHandle(hRemoteThread);
    }
}

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleExecuteTo(
    _In_        HANDLE hProcess,
    _In_        HANDLE hThread,
    _In_        LPBYTE lpProgramCounter
)
{
#ifdef WAFFLE_PORT_ENTRY_POINT_LOOP
    if (!lpProgramCounter)
    {
        return;
    }

    CONTEXT stContext;
    stContext.ContextFlags = CONTEXT_FULL;
    DWORD flOldProtect;
    BYTE OriginalInstruction[WAFFLE_PORT_ENTRY_POINT_LOOP_SIZE];

    if (!VirtualProtectEx(hProcess, lpProgramCounter, WAFFLE_PORT_ENTRY_POINT_LOOP_SIZE, PAGE_EXECUTE_READWRITE, &flOldProtect))
    {
        return;
    }
    if (!ReadProcessMemory(hProcess, lpProgramCounter, OriginalInstruction, WAFFLE_PORT_ENTRY_POINT_LOOP_SIZE, NULL))
    {
        VirtualProtectEx(hProcess, lpProgramCounter, WAFFLE_PORT_ENTRY_POINT_LOOP_SIZE, flOldProtect, &flOldProtect);
        return;
    }
    WriteProcessMemory(hProcess, lpProgramCounter, WAFFLE_PORT_ENTRY_POINT_LOOP, WAFFLE_PORT_ENTRY_POINT_LOOP_SIZE, NULL);
    FlushInstructionCache(hProcess, lpProgramCounter, WAFFLE_PORT_ENTRY_POINT_LOOP_SIZE);
    ResumeThread(hThread);

    for (;;)
    {
        Sleep(100);
        SuspendThread(hThread);
        GetThreadContext(hThread, &stContext);
        if ((LPBYTE) WAFFLE_PORT_PROGRAM_COUNTER_TO_PHYSICAL_ADDRESS(stContext.WAFFLE_PORT_PROGRAM_POINTER) == lpProgramCounter)
        {
            WriteProcessMemory(hProcess, lpProgramCounter, OriginalInstruction, WAFFLE_PORT_ENTRY_POINT_LOOP_SIZE, NULL);
            FlushInstructionCache(hProcess, lpProgramCounter, WAFFLE_PORT_ENTRY_POINT_LOOP_SIZE);
            VirtualProtectEx(hProcess, lpProgramCounter, WAFFLE_PORT_ENTRY_POINT_LOOP_SIZE, flOldProtect, &flOldProtect);
            break;
        }
        else
        {
            ResumeThread(hThread);
        }
    }
#endif
}

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleExecute(
    _Out_       LPWAFFLE_PROCESS_SETTING lpstPS,
    _In_        LPCTSTR lpApplicationName,
    _Inout_opt_ LPTSTR lpCommandLine,
    _In_opt_    LPCTSTR lpCurrentDirectory
)
{
    PROCESS_INFORMATION stProcessInfo;

    WaffleCreateProcess(lpApplicationName, lpCommandLine, NULL, NULL, TRUE, CREATE_SUSPENDED, 0, lpCurrentDirectory, NULL, &stProcessInfo);

#ifdef _M_IX86  //Workaround for XP
    WaffleExecuteTo(stProcessInfo.hProcess, stProcessInfo.hThread, (LPBYTE) WAFFLE_PORT_PROGRAM_COUNTER_TO_PHYSICAL_ADDRESS(WaffleGetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), TEXT("RtlInitializeExceptionChain"))));
    WaffleExecuteTo(stProcessInfo.hProcess, stProcessInfo.hThread, (LPBYTE) WAFFLE_PORT_PROGRAM_COUNTER_TO_PHYSICAL_ADDRESS(WaffleGetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), TEXT("BaseThreadInitThunk"))));
#endif

    lpstPS->dwThreadId = stProcessInfo.dwThreadId;
    lpstPS->dwProcessId = stProcessInfo.dwProcessId;
    WaffleGetFileHash(lpApplicationName, lpstPS->szProcessHash);

    TCHAR szInjectDll[MAX_PATH];
    wsprintf(szInjectDll, TEXT("%s\\Waffle\\%s\\Hook.dll"), lpstPS->szComponentDirectory, WAFFLE_PORT_MACHINE_STRING);
    WaffleInjectDll(stProcessInfo.hProcess, stProcessInfo.hThread, szInjectDll);
    wsprintf(szInjectDll, TEXT("%s\\Waffle\\%s\\Waffle.dll"), lpstPS->szComponentDirectory, WAFFLE_PORT_MACHINE_STRING);
    WaffleInjectDll(stProcessInfo.hProcess, stProcessInfo.hThread, szInjectDll);
    wsprintf(szInjectDll, TEXT("%s\\Waffle\\%s\\Loader.dll"), lpstPS->szComponentDirectory, WAFFLE_PORT_MACHINE_STRING);
    WaffleInjectDll(stProcessInfo.hProcess, stProcessInfo.hThread, szInjectDll);

    CloseHandle(stProcessInfo.hThread);
    CloseHandle(stProcessInfo.hProcess);
}