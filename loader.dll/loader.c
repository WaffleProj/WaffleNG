#define  UNICODE
#define _UNICODE
#include "loader.h"
#include <shlwapi.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

HINSTANCE hLoader;

VOID WINAPI InitLibrary(
  _In_  DWORD  dwThreadId
){
    ExitProcess(0);
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,dwThreadId);    //WinXP may return ERROR_ACCESS_DENIED
    ResumeThread(hThread);

    return;
}

SIZE_T WINAPI LoadDll(
  _In_  LPCTSTR lpszLibrary
)
{
    TCHAR szValueProcessSetting[64];
    GetEnvironmentVariable(szNameProcessSetting,szValueProcessSetting,sizeof(szValueProcessSetting));
    SetEnvironmentVariable(szNameProcessSetting,NULL);

    HANDLE hFileMapping = OpenFileMapping(PAGE_READWRITE,FALSE,szValueProcessSetting);
    LPWAFFLE_PROCESS_SETTING lpstProcessSetting = MapViewOfFile(hFileMapping,FILE_MAP_READ,0,0,0);
    if (!lpstProcessSetting)
    {
        TCHAR szBuf[16];
        wsprintf(szBuf,TEXT("%i"),GetLastError());
        MessageBox(0,szBuf,0,0);
        ExitProcess(0);
    }
    LPTSTR lpszPluginName = (LPTSTR)((SIZE_T)lpstProcessSetting + lpstProcessSetting->offsetPluginName);
    MessageBox(0,lpszPluginName,0,0);
    PostThreadMessage(lpstProcessSetting->dwThreadId,TM_GETTID,0,(LPARAM)InitLibrary);
    return 0;

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