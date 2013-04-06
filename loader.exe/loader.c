#define  UNICODE
#include <windows.h>
#include <shellapi.h>
#include "loader.h"

#include <stdio.h>

CONTEXT stContext,stNewContext; //Avoid ___chkstk_ms

VOID WINAPI Main()
{
    PVOID OldValue;
    char szTest[4096];

    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL,szPath,sizeof(szPath)/sizeof(TCHAR));
    int i;
    for(i = lstrlen(szPath); szPath[i] != '\\'; i--);
    szPath[i] = '\0';

    TCHAR szTarget[MAX_PATH];
    if (argc() > 1)
    {
        argv(2,(LPTSTR)&szTarget,sizeof(szTarget));
    }
    else
    {
        OPENFILENAME stOpenFile;
        RtlZeroMemory(&stOpenFile,sizeof(stOpenFile));
        stOpenFile.lStructSize = sizeof(stOpenFile);
        stOpenFile.lpstrFile = szTarget;
        stOpenFile.nMaxFile = sizeof(szTarget)/sizeof(TCHAR);
        stOpenFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        GetOpenFileName(&stOpenFile);
    }

    Wow64DisableWow64FsRedirection(&OldValue);
    HANDLE hFile = CreateFile(szTarget,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    Wow64RevertWow64FsRedirection(OldValue);
    HANDLE hMapFile = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
    LPVOID lpFile = MapViewOfFile(hMapFile,FILE_MAP_READ,0,0,0);
    if (!lpFile)
    {
        printf("[0001]Cannnot read the file.\n");
        ExitProcess(0);
        return;
    }

    TCHAR szEnvirVar[64];   //SHA-1也放的下了
    wsprintf(szEnvirVar,TEXT("0x%X"),GetCurrentThreadId());
    SetEnvironmentVariable(TEXT("ParentTID"),szEnvirVar);

    WORD Magic = GetPEMagic(lpFile);
    UnmapViewOfFile(lpFile);
    CloseHandle(hMapFile);
    CloseHandle(hFile);         //关闭文件才能CreateProcess

    //IMAGE_NT_OPTIONAL_HDR_MAGIC
    if  (Magic == IMAGE_NT_OPTIONAL_HDR_MAGIC)
    {
        MSG stMSG;
        PeekMessage(&stMSG,0,0,0,PM_NOREMOVE);
    
        HGLOBAL lpszArgument = GlobalAlloc(GPTR,1024);
        lstrcpy(lpszArgument,TEXT("\""));
        lstrcpy(lpszArgument,szTarget);
        lstrcpy(lpszArgument,TEXT("\""));
        lstrcpy(lpszArgument,TEXT(" "));
        lstrcpy(lpszArgument,argp(2));
    
        TCHAR szDirectory[MAX_PATH];
        for(i = lstrlen(szTarget); szTarget[i] != '\\'; i--);
        szDirectory[i] = '\0';
        for(i--; i; i--)
            szDirectory[i] = szTarget[i];
        szDirectory[0] = szTarget[0];
    
        TCHAR szDllFull[MAX_PATH];
        lstrcpy(szDllFull,szPath);
        lstrcat(szDllFull,TEXT("\\core.dll"));

        PROCESS_INFORMATION stProcessInfo = InjectDll(szTarget,lpszArgument,szDirectory,szDllFull);
        
        //CONTEXT stContext;
        RtlZeroMemory(&stContext,sizeof(stContext));
        stContext.ContextFlags = CONTEXT_FULL;
        GetThreadContext(stProcessInfo.hThread,&stContext);
        
        while (TRUE)
        {
            GetMessage(&stMSG,0,TM_FIRSTMESSAGE,TM_LASTMESSAGE);
            if (stMSG.message == TM_SETMAINIP)
            {
                //CONTEXT stNewContext;
                RtlZeroMemory(&stNewContext,sizeof(stNewContext));
                stNewContext.ContextFlags = CONTEXT_FULL;
                GetThreadContext(stProcessInfo.hThread,&stNewContext);
                #if defined(_WIN64)
                stNewContext.Rip = stMSG.lParam;
                #else
                stNewContext.Eip = stMSG.lParam;
                #endif  // defined(_WIN64)
                SetThreadContext(stProcessInfo.hThread,&stNewContext);
                ResumeThread(stProcessInfo.hThread);
            }
            else if (stMSG.message == TM_RESUMETMAINIP)
            {
                SuspendThread(stProcessInfo.hThread);
                SetThreadContext(stProcessInfo.hThread,&stContext);
                ResumeThread(stProcessInfo.hThread);
                CloseHandle(stProcessInfo.hThread);
                break;
            }
        }
    }
    else
        printf("[FFFF]At this moment we can not load PE64 file.\n");

    ExitProcess(0);
    return;
}