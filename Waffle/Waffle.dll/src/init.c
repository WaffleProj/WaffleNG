#include "..\common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

WAFFLE_COMMON_DLL_FUNCTION SIZE_T WINAPI WaffleInit(
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
#ifdef _DEBUG
    MessageBox(0, TEXT("Ready to resume main thread"), 0, 0);
#endif // _DEBUG

    WaffleResumeMainThread();

    return 0;
}