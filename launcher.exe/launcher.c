#define  UNICODE
#define  _UNICODE
#include "launcher.h"
#include <shellapi.h>
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

LPWAFFLEARGP    lpWaffleArgp;

VOID WINAPI Main()
{
    //Get base directory
    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL,szPath,sizeof(szPath)/sizeof(TCHAR));
    int i;
    for (i = lstrlen(szPath); szPath[i] != '\\'; i--);
    szPath[i] = '\0';

    //Load Waffle_common
    TCHAR szLibrary[MAX_PATH];
    wsprintf(szLibrary,TEXT("%s\\Component\\Waffle\\%s\\Waffle_common10.dll"),szPath,WAFFLE_PORT_MACHINE_STRING);
    HMODULE hLibrary = LoadLibrary(szLibrary);
    lpWaffleArgp = (LPVOID)GetProcAddress(hLibrary,"WaffleArgp");

    //Launcher
    TCHAR szWaffle[MAX_PATH];
    wsprintf(szWaffle,TEXT("%s\\Component\\Waffle\\%s\\Waffle.exe"),szPath,WAFFLE_PORT_MACHINE_STRING);
    HGLOBAL lpszArgument = GlobalAlloc(GPTR,(lstrlen(szWaffle) + lstrlen(lpWaffleArgp(2)) + 3 + 1) * sizeof(TCHAR));
    wsprintf(lpszArgument,TEXT("\"%s\" %s"),szWaffle,lpWaffleArgp(2));

    STARTUPINFO stStartUp;
    PROCESS_INFORMATION stProcessInfo;
    stStartUp.cb = sizeof(stStartUp);
    GetStartupInfo(&stStartUp);
    CreateProcess(szWaffle,lpszArgument,NULL,NULL,TRUE,0,0,0,&stStartUp,&stProcessInfo);

    GlobalFree(lpszArgument);
            
    ExitProcess(0);
    return;
}