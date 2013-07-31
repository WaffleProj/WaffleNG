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
        ThreadInit = (LPTHREADINIT) WaffleGetProcAddress(hComponent, "ThreadInit");
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
    LPWAFFLE_PROCESS_SETTING lpstProcessSetting = WaffleOpenProcessSetting();
    wsprintf(szComponent, TEXT("%s\\%s\\%s\\%s"), WaffleGetComponentDirectory(), lpstProcessSetting->szPlugin, WAFFLE_PORT_MACHINE_STRING, lpszComponent);
    hComponent = LoadLibrary(szComponent);
    LPCOMPONENTINIT ComponentInit = (LPCOMPONENTINIT) WaffleGetProcAddress(hComponent, "ComponentInit");
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
    LPWAFFLE_PROCESS_SETTING lpstProcessSetting = WaffleOpenProcessSetting();
    {
        TCHAR szExecutable[MAX_PATH];
        GetModuleFileName(NULL, szExecutable, sizeof(szExecutable) / sizeof(szExecutable[0]));
        WaffleSetOptionString(lpstProcessSetting, TEXT("ProgramName"), szExecutable, FALSE);
    }

    {
        hComponent = WaffleLoadComponent(TEXT("Mojibake.core.1.0.dll"));
        if (!hComponent)
        {
            MessageBox(0, TEXT("FIXME:Invalid Component"), 0, 0);
            WaffleResumeMainThread();
            return 0;
        }
    }

    TCHAR szFilter[MAX_PATH];
    wsprintf(szFilter, TEXT("%s\\%s\\Config\\Filter.ini"), WaffleGetComponentDirectory(), lpstProcessSetting->szPlugin);
    LPTSTR lpszLibrary;
    {
        DWORD nSize = 4;
        lpszLibrary = (LPTSTR) GlobalAlloc(GPTR, nSize*sizeof(TCHAR));
        if (!lpszLibrary)
        {
            MessageBox(0, TEXT("FIXME:Unable to allocate more memory"), 0, 0);
            WaffleResumeMainThread();
            return 0;
        }
        while (GetPrivateProfileSectionNames(lpszLibrary, nSize, szFilter) == nSize - 2)
        {
            nSize = nSize * 2;
            lpszLibrary = (LPTSTR) GlobalReAlloc(lpszLibrary, nSize*sizeof(TCHAR), GHND);
            if (!lpszLibrary)
            {
                MessageBox(0, TEXT("FIXME:Unable to allocate more memory"), 0, 0);
                WaffleResumeMainThread();
                return 0;
            }
        }
    }

    int nLibrary = 0;
    {
        LPTSTR lpszNext = lpszLibrary;
        DWORD nSize = lstrlen(lpszNext);
        while (nSize)
        {
            //Check if we already loaded this library
            //for now we just load everything because we don't have a function to work with LoadLibrary
            //HMODULE hLibrary = GetModuleHandle(lpszNext);
            HMODULE hLibrary = LoadLibrary(lpszNext);
            if (hLibrary)
            {
                nLibrary++;
            }

            WAFFLE_LIBRARY_ARRAY stLibrary;
            stLibrary.lpszLibrary = lpszNext;
            WaffleCopyLibrary(&stLibrary);
            WaffleAddLibrary(&stLibrary);

            lpszNext = lpszNext + nSize + 1;
            nSize = lstrlen(lpszNext);
        }
    }

    //WaffleSetBreakpoint();

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

    MessageBox(0, 0, 0, 0);
    WaffleSetBreakpoint();

    return 0;
}