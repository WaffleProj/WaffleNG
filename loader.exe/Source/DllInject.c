#define  UNICODE
#include <windows.h>
#include "..\loader.h"

PROCESS_INFORMATION WINAPI InjectDll(LPCTSTR lpszTarget, LPTSTR lpszArgument, LPCTSTR lpszDirectory, LPCTSTR lpszDllFull)
{
    STARTUPINFO stStartUp;
    PROCESS_INFORMATION stProcessInfo;

    stStartUp.cb = sizeof(stStartUp);
    GetStartupInfo(&stStartUp);
    CreateProcess(lpszTarget,lpszArgument,NULL,NULL,TRUE,CREATE_SUSPENDED,0,lpszDirectory,&stStartUp,&stProcessInfo);
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