#define  UNICODE
#define _UNICODE
#include <windows.h>
#include <shlwapi.h>
#include "loader.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

HINSTANCE hLoader;

VOID WINAPI InitLibrary(
  _In_  DWORD  dwThreadId
){
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,dwThreadId);    //WinXP may return ERROR_ACCESS_DENIED
    ResumeThread(hThread);

    return;
}

SIZE_T WINAPI LoadDll(
  _In_  LPCTSTR lpszLibrary
)
{
    TCHAR szPath[MAX_PATH];
    GetModuleFileName(hLoader,szPath,sizeof(szPath)/sizeof(TCHAR));
    int i;
    for (i = lstrlen(szPath); szPath[i] != '\\'; i--);
    szPath[i] = '\0';

    TCHAR szLibrary[MAX_PATH];
    lstrcpy(szLibrary,szPath);
    lstrcat(szLibrary,TEXT("\\"));
    lstrcat(szLibrary,lpszLibrary);
    HMODULE hDll = LoadLibrary(szLibrary);
    LPCOMPONENTINIT ComponentInit = (LPCOMPONENTINIT)GetProcAddress(hDll,"ComponentInit");
    ComponentInit();
    return 0;
}

BOOL WINAPI DllMain(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
){
    if (fdwReason == DLL_PROCESS_ATTACH) 
    {
        DisableThreadLibraryCalls(hinstDLL);
        hLoader = hinstDLL;
        CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LoadDll,TEXT("Waffle_waffle.dll"),0,NULL);
        //LoadDll(TEXT("waffle.dll"));
    }
    return TRUE;
}