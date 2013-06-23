#define  UNICODE
#define  _UNICODE
#include <windows.h>
#include <shellapi.h>
#include "waffle.h"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

VOID WINAPI Main()
{
    //Get base directory
    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL,szPath,sizeof(szPath)/sizeof(TCHAR));
    int i;
    for (i = lstrlen(szPath); szPath[i] != '\\'; i--);
    szPath[i] = '\0';

    //Pickup target
    int nArg = argc();
    TCHAR szTarget[MAX_PATH];
    TCHAR szDirectory[MAX_PATH];
    if (nArg > 1)
    {
        for (i = 1; i <= nArg; i++)
        {
            argv(i,szTarget,sizeof(szTarget));
            if  (!lstrcmpi(szTarget,TEXT("/target")))
            {
                i++;    //如果没有忘记带路径的话
                argv(i,szTarget,sizeof(szTarget));
                lstrcpy(szDirectory,szTarget);
                
                int j;
                for (j = lstrlen(szTarget); szDirectory[j] != '\\'; j--);
                szDirectory[j] = '\0';
            
                break;
            }
        }
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
    WORD Magic = GetMachineType(szTarget);
    if  (Magic == MACHINE_I386)
    {
        STARTUPINFO stStartUp;
        PROCESS_INFORMATION stProcessInfo;

        stStartUp.cb = sizeof(stStartUp);
        GetStartupInfo(&stStartUp);

        lstrcat(szLoader,TEXT("\\Component\\Waffle\\I386\\loader.exe"));
        CreateProcess(szLoader,(LPWSTR)argp(2),NULL,NULL,TRUE,0,0,szDirectory,&stStartUp,&stProcessInfo);
    }
    else if (Magic == MACHINE_AMD64)
    {
        STARTUPINFO stStartUp;
        PROCESS_INFORMATION stProcessInfo;

        stStartUp.cb = sizeof(stStartUp);
        GetStartupInfo(&stStartUp);
    
        lstrcat(szLoader,TEXT("\\Component\\Waffle\\AMD64\\loader.exe"));
        CreateProcess(szLoader,(LPWSTR)argp(2),NULL,NULL,TRUE,0,0,szDirectory,&stStartUp,&stProcessInfo);
    }
    else
    {
        MessageBox(0,TEXT("FIXME:Unsupported file or .net program"),0,0);       //Can be .net program
    }

    ExitProcess(0);
    return;
}