#include "..\common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

LIBRARY_EXPORT SIZE_T WINAPI WaffleInit(
    _In_    LPVOID lpReserved
    )
{
    MessageBox(0, TEXT("WaffleInit"), 0, 0);

    lpstProcessSetting->hGlobalMutex = CreateMutex(NULL, FALSE, NULL);

    WaffleAddComponent(TEXT("Waffle.common.1.0.dll"));  //so we can use WaffleAlloc

    TCHAR szExecutable[MAX_PATH];
    GetModuleFileName(NULL, szExecutable, sizeof(szExecutable) / sizeof(szExecutable[0]));
    WaffleSetOptionString(TEXT("ProgramName"), szExecutable, FALSE);

    WaffleLoadDetourOption();

    int x, y;   //we should use the signed data
    AddVectoredExceptionHandler(TRUE, WaffleExceptionHandler);

    if (lpstProcessSetting->lpstLibrary)
    {
        for (x = lpstProcessSetting->lpstLibrary[0].dwBehind; x >= 0; x--)
        {
            for (y = 0; WaffleSetDetour(x, y); y++);
        }
    }



    //return 0; //for attaching debugger
    //MessageBoxA(0, "Attach", 0, 0);

    HANDLE hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME, FALSE, lpstProcessSetting->dwThreadId);
    ResumeThread(hThread);
    CloseHandle(hThread);

    return 0;
}