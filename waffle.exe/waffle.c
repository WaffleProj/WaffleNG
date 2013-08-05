#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <waffle.h>
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

VOID WINAPI Main(VOID)
{
    //Get base directory
    TCHAR szPath[MAX_PATH];
    WaffleGetModuleDirectory(NULL, szPath, sizeof(szPath) / sizeof(szPath[0]));

    //Pickup target
    int nArg = WaffleArgc();
    TCHAR szComponent[MAX_PATH];
    TCHAR szTarget[MAX_PATH];
    TCHAR szDirectory[MAX_PATH];
    RtlZeroMemory(szComponent, sizeof(szComponent));
    RtlZeroMemory(szTarget, sizeof(szTarget));
    RtlZeroMemory(szDirectory, sizeof(szDirectory));
    if (nArg >= 3)
    {
        //1.文件名 2.插件名 3.目标
        WaffleArgv(2, szComponent, sizeof(szComponent));

        WaffleArgv(3, szTarget, sizeof(szTarget));
    }
    else
    {
        OPENFILENAME stOpenFile;
        RtlZeroMemory(&stOpenFile, sizeof(stOpenFile));

        stOpenFile.lStructSize = sizeof(stOpenFile);
        stOpenFile.lpstrFile = szTarget;
        stOpenFile.nMaxFile = sizeof(szTarget) / sizeof(TCHAR);
        stOpenFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        GetOpenFileName(&stOpenFile);

        lstrcpy(szComponent, TEXT("Mojibake"));
    }

    lstrcpy(szDirectory, szTarget);
    int i = lstrlen(szTarget);
    for (; szDirectory[i] != TEXT('\\'); i--);
    szDirectory[i] = TEXT('\0');

    WORD MachineType = WaffleGetMachineType(szTarget);
    if (MachineType == WAFFLE_PORT_MACHINE)
    {
        LPWAFFLE_PROCESS_SETTING lpstProcessSetting = WaffleCreateProcessSetting();
        lstrcpy(lpstProcessSetting->szComponent, szComponent);

        lstrcpy(lpstProcessSetting->szComponentDirectory, szPath);
        int i = lstrlen(lpstProcessSetting->szComponentDirectory);                                                                              //E:\WaffleNightly\Component\Waffle\I386\ 
        for (i--; lpstProcessSetting->szComponentDirectory[i] != TEXT('\\'); i--); lpstProcessSetting->szComponentDirectory[i] = TEXT('\0');    //E:\WaffleNightly\Component\Waffle
        for (i--; lpstProcessSetting->szComponentDirectory[i] != TEXT('\\'); i--); lpstProcessSetting->szComponentDirectory[i] = TEXT('\0');    //E:\WaffleNightly\Component

        LPTSTR lpszArgument = (LPTSTR) GlobalAlloc(GPTR, (lstrlen(szTarget) + lstrlen(WaffleArgp(4)) + 3 + 1) * sizeof(TCHAR));
        if (lpszArgument)
        {
            wsprintf(lpszArgument, TEXT("\"%s\" %s"), szTarget, WaffleArgp(4));
            WaffleExecute(lpstProcessSetting, szTarget, lpszArgument, szDirectory);
            GlobalFree(lpszArgument);
        }
        else
        {
            MessageBox(0, TEXT("FIXME:Unable to allocate memory"), 0, 0);
            ExitProcess(0);
        }
    }
    else
    {
        TCHAR szLoader[MAX_PATH];
        lstrcpy(szLoader, szPath);
        int i = lstrlen(szLoader);                                              //E:\WaffleNightly\Component\Waffle\I386\ 
        for (i--; szLoader[i] != TEXT('\\'); i--); szLoader[i] = TEXT('\0');    //E:\WaffleNightly\Component\Waffle

        STARTUPINFO stStartUp;
        PROCESS_INFORMATION stProcessInfo;

        stStartUp.cb = sizeof(stStartUp);
        GetStartupInfo(&stStartUp);

        switch (MachineType)
        {
        case WAFFLE_PORT_MACHINE_I386:
            {
                lstrcat(szLoader, TEXT("\\I386\\Waffle.exe"));
                break;
            }
        case WAFFLE_PORT_MACHINE_AMD64:
            {
                lstrcat(szLoader, TEXT("\\AMD64\\Waffle.exe"));
                break;
            }
        case WAFFLE_PORT_MACHINE_ARMNT:
            {
                lstrcat(szLoader, TEXT("\\ARMNT\\Waffle.exe"));
                break;
            }
        case 0xFFFF:
            {
                MessageBox(0, TEXT("FIXME:Unable to open the target"), 0, 0);
                ExitProcess(0);
                break;
            }
        default:
            {
                MessageBox(0, TEXT("FIXME:Unsupported file or .net program"), 0, 0);       //Could be .net program
                ExitProcess(0);
            }
        }

        LPTSTR lpszArgument;
        if (nArg >= 3)
        {
            lpszArgument = (LPTSTR) GlobalAlloc(GPTR, (lstrlen(szLoader) + lstrlen(WaffleArgp(2)) + 3 + 1) * sizeof(TCHAR));
            if (lpszArgument)
            {
                wsprintf(lpszArgument, TEXT("\"%s\" %s"), szLoader, WaffleArgp(2));
            }
            else
            {
                MessageBox(0, TEXT("FIXME:Unable to allocate memory"), 0, 0);
                ExitProcess(0);
            }
        }
        else
        {
            lpszArgument = (LPTSTR) GlobalAlloc(GPTR, (lstrlen(szLoader) + lstrlen(szComponent) + lstrlen(szTarget) + 6 + 1) * sizeof(TCHAR));
            if (lpszArgument)
            {
                wsprintf(lpszArgument, TEXT("\"%s\" %s \"%s\""), szLoader, szComponent, szTarget);
            }
            else
            {
                MessageBox(0, TEXT("FIXME:Unable to allocate memory"), 0, 0);
                ExitProcess(0);
            }
        }
        CreateProcess(szLoader, lpszArgument, NULL, NULL, TRUE, 0, 0, szDirectory, &stStartUp, &stProcessInfo);
        CloseHandle(stProcessInfo.hThread);
        CloseHandle(stProcessInfo.hProcess);
        GlobalFree(lpszArgument);
    }

    ExitProcess(0);
    return;
}