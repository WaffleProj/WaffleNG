#define  UNICODE
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include "loader.h"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

LPWOW64DISABLEWOW64FSREDIRECTION lpWow64DisableWow64FsRedirection;
LPWOW64REVERTWOW64FSREDIRECTION lpWow64RevertWow64FsRedirection;

VOID WINAPI Main()
{
    PVOID OldValue;
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    lpWow64DisableWow64FsRedirection = (LPVOID)GetProcAddress(hKernel32,"Wow64DisableWow64FsRedirection");
    lpWow64RevertWow64FsRedirection = (LPVOID)GetProcAddress(hKernel32,"Wow64RevertWow64FsRedirection");

    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL,szPath,sizeof(szPath)/sizeof(TCHAR));
    int i;
    for(i = lstrlen(szPath); szPath[i] != '\\'; i--);
    szPath[i] = '\0';

    TCHAR szTarget[MAX_PATH] = {};
    if (argc() > 1)
    {
        argv(2,(LPTSTR)&szTarget,sizeof(szTarget));
    }
    else
    {
        OPENFILENAME stOpenFile = {};
        stOpenFile.lStructSize = sizeof(stOpenFile);
        stOpenFile.lpstrFile = szTarget;
        stOpenFile.nMaxFile = sizeof(szTarget)/sizeof(TCHAR);
        stOpenFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        GetOpenFileName(&stOpenFile);
    }

    if (lpWow64DisableWow64FsRedirection)
        lpWow64DisableWow64FsRedirection(&OldValue);
    HANDLE hFile = CreateFile(szTarget,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if (lpWow64RevertWow64FsRedirection)
        lpWow64RevertWow64FsRedirection(OldValue);
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
    }
    else
    {
        printf("[FFFF]At this moment we can not load PE64 file.\n");
    }

    ExitProcess(0);
    return;
}