#define  UNICODE
#include <windows.h>
#include "..\loader.h"

PROCESS_INFORMATION WINAPI InjectDll(LPCTSTR lpszTarget, LPTSTR lpszArgument, LPCTSTR lpszDirectory, LPCTSTR lpszDllFull)
{
    STARTUPINFO stStartUp;
    PROCESS_INFORMATION stProcessInfo;

    stStartUp.cb = sizeof(stStartUp);
    GetStartupInfo(&stStartUp);

    //Disable redirection if the target is in the "system32"
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    PVOID OldValue;

    //Open the target
    LPWOW64DISABLEWOW64FSREDIRECTION lpWow64DisableWow64FsRedirection = (LPVOID)GetProcAddress(hKernel32,"Wow64DisableWow64FsRedirection");
    if (lpWow64DisableWow64FsRedirection)
    {
        lpWow64DisableWow64FsRedirection(&OldValue);
    }
    CreateProcess(lpszTarget,lpszArgument,NULL,NULL,TRUE,CREATE_SUSPENDED,0,lpszDirectory,&stStartUp,&stProcessInfo);
    LPWOW64REVERTWOW64FSREDIRECTION lpWow64RevertWow64FsRedirection = (LPVOID)GetProcAddress(hKernel32,"Wow64RevertWow64FsRedirection");
    if (lpWow64RevertWow64FsRedirection)
    {
        lpWow64RevertWow64FsRedirection(OldValue);
    }

    LPVOID lpszRemoteDll = VirtualAllocEx(stProcessInfo.hProcess,NULL,MAX_PATH*sizeof(TCHAR),MEM_COMMIT,PAGE_READWRITE);
    if (lpszRemoteDll)
    {
        WriteProcessMemory(stProcessInfo.hProcess,lpszRemoteDll,lpszDllFull,MAX_PATH*sizeof(TCHAR),NULL);
        FARPROC lpLoadLibrary = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),"LoadLibraryW");
        HANDLE hThread = CreateRemoteThread(stProcessInfo.hProcess,NULL,0,(LPTHREAD_START_ROUTINE)lpLoadLibrary,lpszRemoteDll,0,NULL);
        //VirtualFreeEx(stProcessInfo.hProcess,lpszRemoteDll,0,MEM_RELEASE);
        //确保LoadLibrary使用了这个地址
        CloseHandle(hThread);
    }
    
    return stProcessInfo;
}