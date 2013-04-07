#define  UNICODE
#include <windows.h>
#include "..\loader.h"

PROCESS_INFORMATION WINAPI InjectDll(LPCTSTR lpszTarget, LPTSTR lpszArgument, LPCTSTR lpszDirectory, LPCTSTR lpszDllFull)
{
    PVOID OldValue;
    STARTUPINFO stStartUp;
    PROCESS_INFORMATION stProcessInfo;

    stStartUp.cb = sizeof(stStartUp);
    GetStartupInfo(&stStartUp);
    if (lpWow64DisableWow64FsRedirection)
        lpWow64DisableWow64FsRedirection(&OldValue);
    CreateProcess(lpszTarget,lpszArgument,NULL,NULL,TRUE,CREATE_SUSPENDED,0,lpszDirectory,&stStartUp,&stProcessInfo);
    if (lpWow64RevertWow64FsRedirection)
        lpWow64RevertWow64FsRedirection(OldValue);
    LPVOID lpszRemoteDll = VirtualAllocEx(stProcessInfo.hProcess,NULL,MAX_PATH*sizeof(TCHAR),MEM_COMMIT,PAGE_READWRITE);
    if (lpszRemoteDll)
    {
        WriteProcessMemory(stProcessInfo.hProcess,lpszRemoteDll,lpszDllFull,MAX_PATH*sizeof(TCHAR),NULL);
        FARPROC lpLoadLibrary = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),"LoadLibraryW");
        HANDLE hThread = CreateRemoteThread(stProcessInfo.hProcess,NULL,0,(LPTHREAD_START_ROUTINE)lpLoadLibrary,lpszRemoteDll,0,NULL);
        CloseHandle(hThread);
    }
    
    return stProcessInfo;
}