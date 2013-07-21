#define  UNICODE
#define  _UNICODE
#include "waffle.h"
#include <shellapi.h>
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
    int nArg = WaffleArgc();
    TCHAR szPluginName[64];
    TCHAR szTarget[MAX_PATH];
    TCHAR szDirectory[MAX_PATH];
    if (nArg > 1)
    {
        //1.文件名 2.插件名 3.目标
        WaffleArgv(2,szPluginName,sizeof(szPluginName));
        
        WaffleArgv(3,szTarget,sizeof(szTarget));
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
        MessageBox(0,TEXT("FIXME:argc failed."),0,0);
        ExitProcess(0);
        return;
    }

    TCHAR szLoader[MAX_PATH];
    lstrcpy(szLoader,szPath);
    WORD MachineType = WaffleGetMachineType(szTarget);
    if (MachineType == WAFFLE_PORT_MACHINE)
    {
                        TCHAR szValueProcessSetting[64];
                        TCHAR szPluginName[64];
                        if (WaffleArgv(2,szPluginName,sizeof(szPluginName)))
                        {
                            wsprintf(szValueProcessSetting,szFmtValueProcessSetting,GetCurrentThreadId(),GetTickCount());
                            HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,WAFFLE_PROCESS_SETTING_SIZE,szValueProcessSetting);
                            SetEnvironmentVariable(szNameProcessSetting,szValueProcessSetting);
                            LPWAFFLE_PROCESS_SETTING lpstProcessSetting = MapViewOfFile(hFileMapping,FILE_MAP_ALL_ACCESS,0,0,0);

                            lpstProcessSetting->wVersionMajor = WAFFLE_SDK_VERSION_MAJOR;
                            lpstProcessSetting->wVersionMinor = WAFFLE_SDK_VERSION_MINOR;
                            lpstProcessSetting->cbSize = sizeof(WAFFLE_PROCESS_SETTING);
                            lpstProcessSetting->dwThreadId = GetCurrentThreadId();
                            lpstProcessSetting->offsetPluginName = sizeof(WAFFLE_PROCESS_SETTING);
                            LPTSTR lpszPluginName = (LPTSTR)((SIZE_T)lpstProcessSetting + lpstProcessSetting->offsetPluginName);
                            lstrcpy(lpszPluginName,szPluginName);
                        }
                        else
                        {
                            MessageBox(0,TEXT("FIXME:No plugin"),0,0);
                            ExitProcess(0);
                        }

                        MSG stMSG;
                        PeekMessage(&stMSG,0,0,0,PM_NOREMOVE);

                        HGLOBAL lpszArgument = GlobalAlloc(GPTR,(lstrlen(szTarget) + lstrlen(WaffleArgp(4)) + 3 + 1) * sizeof(TCHAR));
                        lstrcpy(lpszArgument,TEXT("\""));
                        lstrcat(lpszArgument,szTarget);
                        lstrcat(lpszArgument,TEXT("\""));
                        lstrcat(lpszArgument,TEXT(" "));
                        lstrcat(lpszArgument,WaffleArgp(4));

                        TCHAR szDllFull[MAX_PATH];
                        lstrcpy(szDllFull,szPath);
                        lstrcat(szDllFull,TEXT("\\Waffle_loader.dll"));

                        PROCESS_INFORMATION stProcessInfo = WaffleInjectDll(szTarget,lpszArgument,szDirectory,szDllFull);
                        GlobalFree(lpszArgument);
                        
                        CONTEXT stContext = {};
                        stContext.ContextFlags = CONTEXT_FULL;
                        GetThreadContext(stProcessInfo.hThread,&stContext);
                        
                        while (TRUE)
                        {
                            GetMessage(&stMSG,0,TM_FIRSTMESSAGE,TM_LASTMESSAGE);
                            if (stMSG.message == TM_GETTID)
                            {
                                HANDLE hThread = CreateRemoteThread(stProcessInfo.hProcess,NULL,0,(LPTHREAD_START_ROUTINE)stMSG.lParam,(LPVOID)((SIZE_T)(stProcessInfo.dwThreadId)),0,NULL);
                                CloseHandle(hThread);
                                break;
                            }
                        }
    }
    else
    {
        switch (MachineType)
        {
            case WAFFLE_PORT_MACHINE_I386:
            {
                STARTUPINFO stStartUp;
                PROCESS_INFORMATION stProcessInfo;

                stStartUp.cb = sizeof(stStartUp);
                GetStartupInfo(&stStartUp);

                lstrcat(szLoader,TEXT("\\..\\I386\\Waffle.exe"));
                HGLOBAL lpszArgument = GlobalAlloc(GPTR,(lstrlen(szLoader) + lstrlen(WaffleArgp(2)) + 3 + 1) * sizeof(TCHAR));
                wsprintf(lpszArgument,TEXT("\"%s\" %s"),szLoader,WaffleArgp(2));
                CreateProcess(szLoader,lpszArgument,NULL,NULL,TRUE,0,0,szDirectory,&stStartUp,&stProcessInfo);
                GlobalFree(lpszArgument);
                break;
            }
            case WAFFLE_PORT_MACHINE_AMD64:
            {
                STARTUPINFO stStartUp;
                PROCESS_INFORMATION stProcessInfo;

                stStartUp.cb = sizeof(stStartUp);
                GetStartupInfo(&stStartUp);
            
                lstrcat(szLoader,TEXT("\\..\\AMD64\\Waffle.exe"));
                HGLOBAL lpszArgument = GlobalAlloc(GPTR,(lstrlen(szLoader) + lstrlen(WaffleArgp(2)) + 3 + 1) * sizeof(TCHAR));
                wsprintf(lpszArgument,TEXT("\"%s\" %s"),szLoader,WaffleArgp(2));
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
    }

    ExitProcess(0);
    return;
}