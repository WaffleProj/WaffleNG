#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

static HMODULE hComponent;

VOID WINAPI ResetThread(
    _In_    LPTHREAD_CONTEXT lpstThread
    )
{
    SuspendThread(lpstThread->hThread);
    SetThreadContext(lpstThread->hThread, lpstThread->lpstContext);
    ResumeThread(lpstThread->hThread);
    CloseHandle(lpstThread->hThread);
    GlobalFree(lpstThread->lpstContext);
    GlobalFree(lpstThread);
}

void __fastcall SetThreadEnvironment(
    _In_    LPTHREAD_CONTEXT lpstThread
    )
{
    static LPTHREADINIT ThreadInit;
    if (!ThreadInit)
    {
        ThreadInit = (LPTHREADINIT) GetProcAddress(hComponent, "ThreadInit");
    }

    ThreadInit(lpstThread);

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ResetThread, lpstThread, 0, NULL);
    for (;;);
}

LIBRARY_EXPORT SIZE_T WINAPI WaffleInit(
    _In_    LPVOID lpReserved
    )
{
    TCHAR szLibrary[MAX_PATH];
    wsprintf(szLibrary, TEXT("%s\\%s\\%s\\Mojibake.core.1.0.dll"), szComponentPath, lpstProcessSetting->szPlugin, WAFFLE_PORT_MACHINE_STRING);
    hComponent = LoadLibrary(szLibrary);
    LPCOMPONENTINIT ComponentInit = (LPCOMPONENTINIT) GetProcAddress(hComponent, "ComponentInit");
    if (!ComponentInit)
    {
        MessageBox(0, TEXT("FIXME:Invalid Component"), 0, 0);
        WaffleResumeMainThread();
    }
    else
    {
        ComponentInit(lpstProcessSetting);

        WaffleSetBreakpoint();

        HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, lpstProcessSetting->dwThreadId);    //WinXP may return ERROR_ACCESS_DENIED

        CONTEXT stContext;
        stContext.ContextFlags = CONTEXT_FULL;
        GetThreadContext(hThread, &stContext);

        PCONTEXT lpstContext = (PCONTEXT) GlobalAlloc(GPTR, sizeof(CONTEXT));
        LPTHREAD_CONTEXT lpstThread = (LPTHREAD_CONTEXT) GlobalAlloc(GPTR, sizeof(THREAD_CONTEXT));
        lpstThread->hThread = hThread;
        lpstThread->lpstContext = lpstContext;

        RtlMoveMemory(lpstContext, &stContext, sizeof(stContext));
        stContext.WAFFLE_PORT_STACK_POINTER -= 8 * sizeof(SIZE_T);        //Protect stack
        stContext.WAFFLE_PORT_PROGRAM_POINTER = (SIZE_T) SetThreadEnvironment;
        stContext.WAFFLE_PORT_FASTCALL_ARGUMENT = (SIZE_T) lpstThread;
        SetThreadContext(hThread, &stContext);
        ResumeThread(hThread);
    }
    return 0;
}