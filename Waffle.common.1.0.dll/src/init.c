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
    _In_    LPWAFFLE_THREAD_CONTEXT lpstThread
    )
{
    SuspendThread(lpstThread->hThread);
    SetThreadContext(lpstThread->hThread, lpstThread->lpstContext);
    ResumeThread(lpstThread->hThread);
    CloseHandle(lpstThread->hThread);
    GlobalFree(lpstThread->lpstContext);
    GlobalFree(lpstThread);
}

VOID __fastcall SetThreadEnvironment(
    _In_    LPWAFFLE_THREAD_CONTEXT lpstThread
    )
{
    static LPTHREADINIT ThreadInit;
    if (!ThreadInit)
    {
        ThreadInit = (LPTHREADINIT) WaffleGetProcAddress(hComponent, TEXT("ThreadInit"));
    }

    ThreadInit(lpstThread);

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ResetThread, lpstThread, 0, NULL);
    for (;;);
}

LIBRARY_EXPORT HMODULE WINAPI WaffleLoadComponent(
    _In_    LPCTSTR lpszComponent
    )
{
    HMODULE hComponent = GetModuleHandle(lpszComponent);
    if (hComponent)
    {
        return hComponent;
    }

    TCHAR szComponent[MAX_PATH];
    wsprintf(szComponent, TEXT("%s\\%s\\%s\\%s"), lpstProcessSetting->szComponentDirectory, lpstProcessSetting->szComponent, WAFFLE_PORT_MACHINE_STRING, lpszComponent);
    hComponent = LoadLibrary(szComponent);
    LPCOMPONENTINIT ComponentInit = (LPCOMPONENTINIT) WaffleGetProcAddress(hComponent, TEXT("ComponentInit"));
    if (!ComponentInit)
    {
        FreeLibrary(hComponent);
        return NULL;
    }
    else
    {
        ComponentInit(lpstProcessSetting);
    }
    return hComponent;
}

LIBRARY_EXPORT SIZE_T WINAPI WaffleInit(
    _In_    LPVOID lpReserved
    )
{
    TCHAR szExecutable[MAX_PATH];
    GetModuleFileName(NULL, szExecutable, sizeof(szExecutable) / sizeof(szExecutable[0]));
    WaffleSetOptionString(TEXT("ProgramName"), szExecutable, FALSE);

    hComponent = WaffleLoadComponent(TEXT("Mojibake.core.1.0.dll"));
    if (!hComponent)
    {
        MessageBox(0, TEXT("FIXME:Invalid Component"), 0, 0);
        WaffleResumeMainThread();
        return 0;
    }

    int nLibrary = WaffleCreateLibraryArray();
    if (nLibrary > 0)
    {
        AddVectoredExceptionHandler(TRUE, WaffleExceptionHandler);
        for (nLibrary--; nLibrary >= 0; nLibrary--)
        {
            DWORD countFunction;
            for (countFunction = 0; WaffleSetDetour(nLibrary, countFunction); countFunction++);
        }
    }

    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, lpstProcessSetting->dwThreadId);    //WinXP may return ERROR_ACCESS_DENIED

    CONTEXT stContext;
    stContext.ContextFlags = CONTEXT_FULL;
    GetThreadContext(hThread, &stContext);

    PCONTEXT lpstContext = (PCONTEXT) GlobalAlloc(GPTR, sizeof(CONTEXT));
    LPWAFFLE_THREAD_CONTEXT lpstThread = (LPWAFFLE_THREAD_CONTEXT) GlobalAlloc(GPTR, sizeof(WAFFLE_THREAD_CONTEXT));
    lpstThread->hThread = hThread;
    lpstThread->lpstContext = lpstContext;
    lpstThread->lpstProcessSetting = lpstProcessSetting;

    RtlMoveMemory(lpstContext, &stContext, sizeof(stContext));
    stContext.WAFFLE_PORT_STACK_POINTER -= 8 * sizeof(SIZE_T);        //Protect stack
    stContext.WAFFLE_PORT_PROGRAM_POINTER = (SIZE_T) SetThreadEnvironment;
    stContext.WAFFLE_PORT_FASTCALL_ARGUMENT = (SIZE_T) lpstThread;
    SetThreadContext(hThread, &stContext);
    ResumeThread(hThread);

    return 0;
}