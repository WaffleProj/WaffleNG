#define  UNICODE
#define  _UNICODE
#include "..\common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

LIBRARY_EXPORT SIZE_T WINAPI WaffleInit(
    _In_    LPVOID lpReserved
    )
{
    TCHAR szPath[MAX_PATH];
    WaffleGetModuleDirectory(hWaffleCommonDll, szPath, sizeof(szPath) / sizeof(szPath[0]));

    TCHAR szLibrary[MAX_PATH];
    wsprintf(szLibrary, TEXT("%s\\..\\..\\%s\\%s\\Mojibake.core.1.0.dll"), szPath, lpstProcessSetting->lpszPluginName, WAFFLE_PORT_MACHINE_STRING);
    HMODULE hDll = LoadLibrary(szLibrary);
    LPCOMPONENTINIT ComponentInit = (LPVOID) GetProcAddress(hDll, "ComponentInit");
    if (!ComponentInit)
    {
        MessageBox(0, TEXT("FIXME:Invalid Component"), 0, 0);
        WaffleResumeMainThread();
    }
    else
    {
        ComponentInit(lpstProcessSetting);
    }
    return 0;
}