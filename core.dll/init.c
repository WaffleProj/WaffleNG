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

int WINAPI HookedMessageBoxA(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
){

    return ((LPMESSAGEBOXA)stUser32Table[MESSAGEBOXA].lpNewFunction)(hWnd,"MessageBoxA has been hooked","Dll Copy Example",uType);
}

int WINAPI SetBreakpoint(LIBRARY_TABLE_OBJECT stLibraryTable[])
{
    DWORD flOldProtect;

    int i = 0,j;
    HOOK_TABLE_OBJECT *stFunction;
    while (stLibraryTable[i].lpszLibrary)
    {
        stLibraryTable[i].lpLibrary = CopyLibrary(GetModuleAddressW(stLibraryTable[i].lpszLibrary));
        stFunction = stLibraryTable[i].lpHookTable;
        j = 0;
        while (stFunction[j].lpszFunction)
        {
            stFunction[j].lpNewFunction = GetFunctionAddressA(stLibraryTable[i].lpLibrary,stFunction[j].lpszFunction);
            j++;
        }
        i++;
    }
    _wsprintfA = GetFunctionAddressA(stLibraryTable[USER32].lpLibrary,"wsprintfA");

    stUser32Table[MESSAGEBOXA].lpDetourFunction = HookedMessageBoxA;
    stUser32Table[MESSAGEBOXA].lpOriginalFunction = GetProcAddress(GetModuleHandle(TEXT("User32.dll")),"MessageBoxA");

    PVOID hBreakpoint = AddVectoredExceptionHandler(TRUE,BreakpointHandler);
    VirtualProtect(stUser32Table[MESSAGEBOXA].lpOriginalFunction,1,PAGE_READONLY,&flOldProtect);

    MessageBoxA(0,"MessageBoxA","Dll Copy Example",0);
    ((LPMESSAGEBOXA)stUser32Table[MESSAGEBOXA].lpNewFunction)(0,"lpMessageBoxA","Dll Copy Example",0);

    VirtualProtect(stUser32Table[MESSAGEBOXA].lpOriginalFunction,1,flOldProtect,&flOldProtect);
    RemoveVectoredExceptionHandler(hBreakpoint);
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