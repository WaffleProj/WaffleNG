#include <waffle.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

BOOL WINAPI DllMain(
    _In_    HINSTANCE hinstDLL,
    _In_    DWORD fdwReason,
    _In_    LPVOID lpvReserved
    )
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);

#ifdef _DEBUG
        MessageBox(0, TEXT("Please attach the debugger."), TEXT("Loader.dll"), 0);
        if (IsDebuggerPresent())
        {
			__try
			{
				DebugBreak();
			}
			__except (GetExceptionCode() == EXCEPTION_BREAKPOINT ?
				EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				// Doing nothing
			}
        }
#endif // _DEBUG
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WaffleInit, 0, 0, NULL);
        if (hThread)
        {
            CloseHandle(hThread);
        }
        else
        {
            MessageBox(0, TEXT("FIXME:Unable to create WaffleInit thread"), 0, 0);
            ExitProcess(0);
        }
    }
    return TRUE;
}