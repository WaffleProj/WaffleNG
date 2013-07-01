#define  UNICODE
#define  _UNICODE
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include "loader.h"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

VOID WINAPI Main()
{
    //Get base directory
    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL,szPath,sizeof(szPath)/sizeof(TCHAR));
    int i;
    for (i = lstrlen(szPath); szPath[i] != '\\'; i--);
    szPath[i] = '\0';

    //Pickup target
    int nArg = argc();
    TCHAR szTarget[MAX_PATH];
    TCHAR szDirectory[MAX_PATH];
    if (nArg > 1)
    {
        argv(3,szTarget,sizeof(szTarget));      //1.文件名 2.插件名 3.目标
        lstrcpy(szDirectory,szTarget);
        
        for (i = lstrlen(szTarget); szDirectory[i] != '\\'; i--);
        szDirectory[i] = '\0';
    }
    else if (nArg == 1)
    {
        MessageBox(0,TEXT("FIXME:A proper window"),0,0);
        ExitProcess(0);
        return;
        OPENFILENAME stOpenFile = {};
        stOpenFile.lStructSize = sizeof(stOpenFile);
        stOpenFile.lpstrFile = szTarget;
        stOpenFile.nMaxFile = sizeof(szTarget)/sizeof(TCHAR);
        stOpenFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        GetOpenFileName(&stOpenFile);
    }
    else // if (nArg == 0)
    {
        MessageBox(0,TEXT("argc failed."),0,0);
        ExitProcess(0);
        return;
    }

    TCHAR szEnvirVar[64];   //SHA-1也放的下了
    wsprintf(szEnvirVar,TEXT("0x%X"),GetCurrentThreadId());
    SetEnvironmentVariable(TEXT("ParentTID"),szEnvirVar);

    MSG stMSG;
    PeekMessage(&stMSG,0,0,0,PM_NOREMOVE);

    HGLOBAL lpszArgument = GlobalAlloc(GPTR,(lstrlen(szTarget) + lstrlen(argp(4)) + 3 + 1) * sizeof(TCHAR));
    lstrcpy(lpszArgument,TEXT("\""));
    lstrcat(lpszArgument,szTarget);
    lstrcat(lpszArgument,TEXT("\""));
    lstrcat(lpszArgument,TEXT(" "));
    lstrcat(lpszArgument,argp(4));

    TCHAR szDllFull[MAX_PATH];
    lstrcpy(szDllFull,szPath);
    lstrcat(szDllFull,TEXT("\\loader.dll"));

    PROCESS_INFORMATION stProcessInfo = InjectDll(szTarget,lpszArgument,szDirectory,szDllFull);
    GlobalFree(lpszArgument);
    
    CONTEXT stContext = {};
    stContext.ContextFlags = CONTEXT_FULL;
    GetThreadContext(stProcessInfo.hThread,&stContext);
    
    while (TRUE)
    {
        GetMessage(&stMSG,0,TM_FIRSTMESSAGE,TM_LASTMESSAGE);
        if (stMSG.message == TM_GETTID)
        {
            HANDLE hThread = CreateRemoteThread(stProcessInfo.hProcess,NULL,0,(LPTHREAD_START_ROUTINE)stMSG.lParam,(LPVOID)((SIZE_T)(stProcessInfo.dwThreadId)),0,NULL);
            CloseHandle(hThread);
            break;
        }
    }

    ExitProcess(0);
    return;
}