#define  UNICODE
#define  _UNICODE
#include "..\common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

LIBRARY_EXPORT BOOL WINAPI WaffleCreateProcess(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
){
    if (fdwReason == DLL_PROCESS_ATTACH) 
    {
        DisableThreadLibraryCalls(hinstDLL);
    }
    return TRUE;
}

LIBRARY_EXPORT PROCESS_INFORMATION WINAPI WaffleInjectDll(LPCTSTR lpszTarget, LPTSTR lpszArgument, LPCTSTR lpszDirectory, LPCTSTR lpszDllFull, LPWAFFLE_PROCESS_SETTING lpstProcessSetting)
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

    lpstProcessSetting->dwThreadId = stProcessInfo.dwThreadId;
    lpstProcessSetting->dwProcessId = stProcessInfo.dwProcessId;

    LPVOID lpszRemoteDll = VirtualAllocEx(stProcessInfo.hProcess,NULL,MAX_PATH*sizeof(TCHAR),MEM_COMMIT,PAGE_READWRITE);
    if (lpszRemoteDll)
    {
        FARPROC lpLoadLibrary = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),"LoadLibraryW");
        HANDLE hThread;
    
        WriteProcessMemory(stProcessInfo.hProcess,lpszRemoteDll,szWaffleCommonDll,MAX_PATH*sizeof(TCHAR),NULL);
        hThread = CreateRemoteThread(stProcessInfo.hProcess,NULL,0,(LPTHREAD_START_ROUTINE)lpLoadLibrary,lpszRemoteDll,0,NULL);
        WaitForSingleObject(hThread,INFINITE);

        WriteProcessMemory(stProcessInfo.hProcess,lpszRemoteDll,lpszDllFull,MAX_PATH*sizeof(TCHAR),NULL);
        hThread = CreateRemoteThread(stProcessInfo.hProcess,NULL,0,(LPTHREAD_START_ROUTINE)lpLoadLibrary,lpszRemoteDll,0,NULL);
        WaitForSingleObject(hThread,INFINITE);

        VirtualFreeEx(stProcessInfo.hProcess,lpszRemoteDll,0,MEM_RELEASE);
        //确保LoadLibrary使用了这个地址
        CloseHandle(hThread);
    }
    
    return stProcessInfo;
}