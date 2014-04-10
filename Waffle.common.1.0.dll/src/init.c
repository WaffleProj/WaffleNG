#include "..\common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

LIBRARY_EXPORT SIZE_T WINAPI WaffleInit(
    _In_    LPVOID lpReserved
    )
{
    TCHAR szExecutable[MAX_PATH];
    if (!GetModuleFileName(NULL, szExecutable, lengthof(szExecutable)))
    {
        MessageBox(0, TEXT("Unable to locate executable file directory."), TEXT("Waffle"), 0);
        ExitProcess(0);
    }
    WaffleSetOptionString(NULL, NULL, TEXT("ProgramName"), szExecutable, FALSE);

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

    WaffleResumeMainThread();

    return 0;
}