#define  UNICODE
#include <windows.h>
#include "core.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

typedef struct {
    HANDLE   hThread;
    PCONTEXT lpstContext;
} THREAD_CONTEXT,*PTHREAD_CONTEXT;

VOID WINAPI ResetThread(
_In_      PTHREAD_CONTEXT lpContext 
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

    return _MessageBoxA(hWnd,"MessageBoxA has been hooked","Dll Copy Example",uType);
}

int WINAPI SetHook(LPVOID stHookTable[])
{
    DWORD flOldProtect;

    HMODULE hCopyLibrary = CopyLibrary(GetModuleAddressW(L"User32.dll"));
    _MessageBoxA = GetFunctionAddressA(hCopyLibrary,"MessageBoxA");
    _wsprintfA = GetFunctionAddressA(hCopyLibrary,"wsprintfA");

    stMessageBoxA.lpNewFunction = HookedMessageBoxA;
    stMessageBoxA.lpOldFunction = GetProcAddress(GetModuleHandle(TEXT("User32.dll")),"MessageBoxA");
    VirtualProtect(stMessageBoxA.lpOldFunction,1,PAGE_READONLY,&flOldProtect);
    PVOID hBreakpoint = AddVectoredExceptionHandler(TRUE,BreakpointHandler);

    _MessageBoxA(0,"lpMessageBoxA","Dll Copy Example",0);
    MessageBoxA(0,"MessageBoxA","Dll Copy Example",0);

    VirtualProtect(stMessageBoxA.lpOldFunction,1,flOldProtect,&flOldProtect);
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

    SetHook(stHookTable);

    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,dwThreadId);    //WinXP may return ERROR_ACCESS_DENIED

    CONTEXT stContext;
    stContext.ContextFlags = CONTEXT_FULL;
    GetThreadContext(hThread,&stContext);
    
    PCONTEXT lpstContext = GlobalAlloc(GPTR,sizeof(CONTEXT));
    RtlMoveMemory(lpstContext,&stContext,sizeof(stContext));
    #if defined(_WIN64)
    stContext.Rsp -= 64;        //Protect stack
    stContext.Rip = (SIZE_T)SetThreadEnvironment;
    stContext.Rcx = (SIZE_T)hThread;
    stContext.Rdx = (SIZE_T)lpstContext;
    #else
    stContext.Esp -= 32;        //Protect stack
    stContext.Eip = (SIZE_T)SetThreadEnvironment;
    stContext.Ecx = (SIZE_T)hThread;
    stContext.Edx = (SIZE_T)lpstContext;
    #endif  // defined(_WIN64)
    SetThreadContext(hThread,&stContext);
    ResumeThread(hThread);

    return;
}