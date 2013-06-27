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
    int nCommandLine = 0;
    if (nArg > 1)
    {
        for (i = 1; i <= nArg; i++)
        {
            argv(i,szTarget,sizeof(szTarget));
            if  (!lstrcmpi(szTarget,TEXT("/target")))
            {
                i++;    //如果没有忘记带路径的话
                argv(i,szTarget,sizeof(szTarget));
                lstrcpy(szDirectory,szTarget);
                
                int j;
                for (j = lstrlen(szTarget); szDirectory[j] != '\\'; j--);
                szDirectory[j] = '\0';

                i++;
                nCommandLine = i;

                break;
            }
        }
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

    HGLOBAL lpszArgument = GlobalAlloc(GPTR,1024);
    lstrcpy(lpszArgument,TEXT("\""));
    lstrcat(lpszArgument,szTarget);
    lstrcat(lpszArgument,TEXT("\""));
    lstrcat(lpszArgument,TEXT(" "));
    lstrcat(lpszArgument,argp(nCommandLine));

    TCHAR szDllFull[MAX_PATH];
    lstrcpy(szDllFull,szPath);
    lstrcat(szDllFull,TEXT("\\waffle.dll"));

    PROCESS_INFORMATION stProcessInfo = InjectDll(szTarget,lpszArgument,szDirectory,szDllFull);
    
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