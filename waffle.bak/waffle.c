#define  UNICODE
#define  _UNICODE
#include "waffle.h"
#include <shellapi.h>
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

LPWAFFLEARGC    lpWaffleArgc;
LPWAFFLEARGV    lpWaffleArgv;
LPWAFFLEARGP    lpWaffleArgp;
LPWAFFLEGETMACHINETYPE  lpWaffleGetMachineType;

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
    lpWaffleArgc = (LPVOID)GetProcAddress(hLibrary,"WaffleArgc");
    lpWaffleArgv = (LPVOID)GetProcAddress(hLibrary,"WaffleArgv");
    lpWaffleArgp = (LPVOID)GetProcAddress(hLibrary,"WaffleArgp");
    lpWaffleGetMachineType = (LPVOID)GetProcAddress(hLibrary,"WaffleGetMachineType");

    //Pickup target
    int nArg = lpWaffleArgc();
    TCHAR szTarget[MAX_PATH];
    TCHAR szDirectory[MAX_PATH];
    if (nArg > 1)
    {
        lpWaffleArgv(3,szTarget,sizeof(szTarget));      //1.文件名 2.插件名 3.目标
        lstrcpy(szDirectory,szTarget);
        
        for (i = lstrlen(szTarget); szDirectory[i] != '\\'; i--);
        szDirectory[i] = '\0';
    }
    else if (nArg == 1)
    {
        MessageBox(0,TEXT("FIXME:A proper window"),0,0);
        ExitProcess(0);
        return;
        OPENFILENAME stOpenFile = {};
        stOpenFile.lStructSize = sizeof(stOpenFile);
        stOpenFile.lpstrFile = szTarget;
        stOpenFile.nMaxFile = sizeof(szTarget)/sizeof(TCHAR);
        stOpenFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        GetOpenFileName(&stOpenFile);
    }
    else // if (nArg == 0)
    {
        MessageBox(0,TEXT("argc failed."),0,0);
        ExitProcess(0);
        return;
    }

    TCHAR szLoader[MAX_PATH];
    lstrcpy(szLoader,szPath);
    switch (lpWaffleGetMachineType(szTarget))
    {
        case MACHINE_I386:
        {
            STARTUPINFO stStartUp;
            PROCESS_INFORMATION stProcessInfo;

            stStartUp.cb = sizeof(stStartUp);
            GetStartupInfo(&stStartUp);

            lstrcat(szLoader,TEXT("\\Component\\Waffle\\I386\\Waffle_loader.exe"));
            HGLOBAL lpszArgument = GlobalAlloc(GPTR,(lstrlen(szLoader) + lstrlen(lpWaffleArgp(2)) + 3 + 1) * sizeof(TCHAR));
            wsprintf(lpszArgument,TEXT("\"%s\" %s"),szLoader,lpWaffleArgp(2));
            CreateProcess(szLoader,lpszArgument,NULL,NULL,TRUE,0,0,szDirectory,&stStartUp,&stProcessInfo);
            GlobalFree(lpszArgument);
            break;
        }
        case MACHINE_AMD64:
        {
            STARTUPINFO stStartUp;
            PROCESS_INFORMATION stProcessInfo;

            stStartUp.cb = sizeof(stStartUp);
            GetStartupInfo(&stStartUp);
        
            lstrcat(szLoader,TEXT("\\Component\\Waffle\\AMD64\\Waffle_loader.exe"));
            HGLOBAL lpszArgument = GlobalAlloc(GPTR,(lstrlen(szLoader) + lstrlen(lpWaffleArgp(2)) + 3 + 1) * sizeof(TCHAR));
            wsprintf(lpszArgument,TEXT("\"%s\" %s"),szLoader,lpWaffleArgp(2));
            CreateProcess(szLoader,lpszArgument,NULL,NULL,TRUE,0,0,szDirectory,&stStartUp,&stProcessInfo);
            GlobalFree(lpszArgument);
            break;
        }
        case 0xFFFF:
        {
            MessageBox(0,TEXT("FIXME:Unable to open the target"),0,0);
            break;
        }
        default:
        {
            MessageBox(0,TEXT("FIXME:Unsupported file or .net program"),0,0);       //Can be .net program
        }
    }

    ExitProcess(0);
    return;
}