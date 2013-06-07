#define  UNICODE
#include <windows.h>
#include "core.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

VOID WINAPI ResetThread(
_In_      LPTHREAD_CONTEXT lpContext 
){
    THREAD_CONTEXT stContext;
    stContext.hThread = lpContext->hThread;
    stContext.lpstContext = lpContext->lpstContext;

    SuspendThread(stContext.hThread);
    SetThreadContext(stContext.hThread,stContext.lpstContext);
    ResumeThread(stContext.hThread);
    CloseHandle(stContext.hThread);
    GlobalFree(stContext.lpstContext);
}

VOID FASTCALL SetThreadEnvironment(
  _In_      HANDLE   hThread,
  _In_      PCONTEXT lpstContext
){
    THREAD_CONTEXT stThread = {hThread,lpstContext};

    SetThreadLocale(stNewEnvir.ThreadLocale);

    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ResetThread,&stThread,0,NULL);
    while (TRUE);
}

int WINAPI SetBreakpoint(LIBRARY_TABLE_OBJECT stLibraryTable[])
{
    //MessageBox(0,TEXT("Start Hooking"),TEXT("Debugger"),0);
    int i;
    for (i = 0; stLibraryTable[i].lpszLibrary; i++)
    {
        CopyLibraryEx(&stLibraryTable[i]);
    }
    _wsprintfA = GetFunctionAddressA(stLibraryTable[USER32].lpLibrary,"wsprintfA");
    _VirtualProtect = GetFunctionAddressA(stLibraryTable[KERNEL32].lpLibrary,"VirtualProtect");

    //stUser32Table[MESSAGEBOXA].lpDetourFunction = HookedMessageBoxA;
    //stUser32Table[MESSAGEBOXA].lpOriginalFunction = GetProcAddress(GetModuleHandle(TEXT("User32.dll")),"MessageBoxA");

    AddVectoredExceptionHandler(TRUE,BreakpointHandler);
    for (i = 0; stLibraryTable[i].lpszLibrary; i++)
    {
        LPHOOK_TABLE_OBJECT lpHookTable = stLibraryTable[i].lpHookTable;
        int j;
        for (j = 0; lpHookTable[j].lpszFunction; j++)
        {
            DWORD flOldProtect;
            //if  (_VirtualProtect(lpHookTable[j].lpOriginalFunction,1,PAGE_EXECUTE_READWRITE,&flOldProtect))
            //{
                _VirtualProtect(lpHookTable[j].lpOriginalFunction,1,PAGE_EXECUTE_READWRITE,&flOldProtect);
                *(char *)(lpHookTable[j].lpOriginalFunction) = 0xF4;    //hlt
                _VirtualProtect(lpHookTable[j].lpOriginalFunction,1,flOldProtect,&flOldProtect);
            //}
            //else
            //{
                //((LPMESSAGEBOXA)stUser32Table[MESSAGEBOXA].lpNewFunction)(0,lpHookTable[j].lpszFunction,"Unable to hook",0);
            //}
        }
    }

    //MessageBoxA(0,"MessageBoxA","Dll Copy Example",0);
    //((LPMESSAGEBOXA)stUser32Table[MESSAGEBOXA].lpNewFunction)(0,"lpMessageBoxA","Dll Copy Example",0);

    //VirtualProtect(stUser32Table[MESSAGEBOXA].lpOriginalFunction,1,flOldProtect,&flOldProtect);
    //RemoveVectoredExceptionHandler(hBreakpoint);

    //((LPMESSAGEBOXA)stUser32Table[MESSAGEBOXA].lpNewFunction)(0,"Finish Hooking","Debugger",0);
    return 0;
}

VOID WINAPI InitLibrary(
  _In_  DWORD  dwThreadId
){
    hHeap = HeapCreate(0,0,0);
    //∂¡»°≥Ã–Ú≈‰÷√
    //CryptCATAdminCalcHashFromFileHandle
    stOldEnvir.ACP = GetACP();
    stNewEnvir.ACP = CP_SHIFT_JIS;
    stOldEnvir.OEMCP = GetOEMCP();
    stNewEnvir.OEMCP = CP_SHIFT_JIS;
    stOldEnvir.ThreadLocale = GetThreadLocale();
    stNewEnvir.ThreadLocale = LOCALE_JA_JP;

    LPTSTR lpszCommandLineW = GetCommandLineW();
    int intSize = 4*lstrlenW(lpszCommandLineW);
    lpszCommandLineA = GlobalAlloc(GPTR,intSize);
    WideCharToMultiByte(stNewEnvir.ACP,0,lpszCommandLineW,-1,lpszCommandLineA,intSize,NULL,NULL);

    SetBreakpoint(stLibraryTable);

    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,dwThreadId);    //WinXP may return ERROR_ACCESS_DENIED

    CONTEXT stContext;
    stContext.ContextFlags = CONTEXT_FULL;
    GetThreadContext(hThread,&stContext);
    
    PCONTEXT lpstContext = GlobalAlloc(GPTR,sizeof(CONTEXT));
    RtlMoveMemory(lpstContext,&stContext,sizeof(stContext));
    #if defined(_WIN64)
    stContext.Rsp -= 8*sizeof(SIZE_T);        //Protect stack
    stContext.Rip = (SIZE_T)SetThreadEnvironment;
    stContext.Rcx = (SIZE_T)hThread;
    stContext.Rdx = (SIZE_T)lpstContext;
    #else
    stContext.Esp -= 8*sizeof(SIZE_T);        //Protect stack
    stContext.Eip = (SIZE_T)SetThreadEnvironment;
    stContext.Ecx = (SIZE_T)hThread;
    stContext.Edx = (SIZE_T)lpstContext;
    #endif  // defined(_WIN64)
    SetThreadContext(hThread,&stContext);
    ResumeThread(hThread);

    return;
}