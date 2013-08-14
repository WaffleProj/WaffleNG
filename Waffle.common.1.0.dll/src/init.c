#include "..\common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

VOID WINAPI ResetThread(
    _In_    LPWAFFLE_THREAD_CONTEXT lpstThread
    )
{
    SuspendThread(lpstThread->hThread);
    SetThreadContext(lpstThread->hThread, lpstThread->lpstContext);
    ResumeThread(lpstThread->hThread);
    CloseHandle(lpstThread->hThread);
    WaffleFree(lpstThread->lpstContext);
    WaffleFree(lpstThread);
}

VOID __fastcall SetThreadEnvironment(
    _In_    LPWAFFLE_THREAD_CONTEXT lpstThread
    )
{
    /*
    static LPTHREADINIT ThreadInit;
    if (!ThreadInit)
    {
    ThreadInit = (LPTHREADINIT) WaffleGetProcAddress(hComponent, TEXT("ThreadInit"));
    }

    ThreadInit(lpstThread);
    */

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ResetThread, lpstThread, 0, NULL);
    for (;;);
}

LIBRARY_EXPORT SIZE_T WINAPI WaffleInit(
    _In_    LPVOID lpReserved
    )
{
    lpstProcessSetting->hGlobalMutex = CreateMutex(NULL, FALSE, NULL);

    WaffleAddComponent(TEXT("Waffle.common.1.0.dll"));  //so we can use WaffleAlloc

    TCHAR szExecutable[MAX_PATH];
    GetModuleFileName(NULL, szExecutable, sizeof(szExecutable) / sizeof(szExecutable[0]));
    WaffleSetOptionString(TEXT("ProgramName"), szExecutable, FALSE);

    WaffleLoadDetourOption();

    DWORD x, y;
    AddVectoredExceptionHandler(TRUE, WaffleExceptionHandler);
    for (x = lpstProcessSetting->lpstLibrary[0].dwBehind; x >= 0; x--)
    {
        for (y = 0; WaffleSetDetour(x, y); y++);
    }


    //return 0; //for attaching debugger

    HANDLE hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME, FALSE, lpstProcessSetting->dwThreadId);

    CONTEXT stContext;
    stContext.ContextFlags = CONTEXT_FULL;
    GetThreadContext(hThread, &stContext);

    PCONTEXT lpstContext = (PCONTEXT) WaffleAlloc(sizeof(CONTEXT));
    RtlMoveMemory(lpstContext, &stContext, sizeof(stContext));

    LPWAFFLE_THREAD_CONTEXT lpstThread = (LPWAFFLE_THREAD_CONTEXT) WaffleAlloc(sizeof(WAFFLE_THREAD_CONTEXT));
    lpstThread->hThread = hThread;
    lpstThread->lpstContext = lpstContext;
    lpstThread->lpstProcessSetting = lpstProcessSetting;

    stContext.WAFFLE_PORT_STACK_POINTER -= 8 * sizeof(SIZE_T);        //Protect stack
    stContext.WAFFLE_PORT_PROGRAM_POINTER = (SIZE_T) SetThreadEnvironment;
    stContext.WAFFLE_PORT_FASTCALL_ARGUMENT = (SIZE_T) lpstThread;
    SetThreadContext(hThread, &stContext);

    ResumeThread(hThread);

    return 0;
}