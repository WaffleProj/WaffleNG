#define  UNICODE
#define  _UNICODE
#include "..\common.h"

LIBRARY_EXPORT LPWAFFLE_PROCESS_SETTING WINAPI WaffleOpenProcessSetting()
{
    if (!lpstProcessSetting)
    {
        TCHAR szValueProcessSetting[64];
        GetEnvironmentVariable(szNameProcessSetting,szValueProcessSetting,sizeof(szValueProcessSetting));
        SetEnvironmentVariable(szNameProcessSetting,NULL);

        HANDLE hFileMapping = OpenFileMapping(FILE_MAP_WRITE,FALSE,szValueProcessSetting);

        lpstProcessSetting = MapViewOfFile(hFileMapping,FILE_MAP_WRITE,0,0,0);
    
        if (lpstProcessSetting)
        {
            lpstProcessSetting->lpszPluginName = (LPTSTR)((SIZE_T)lpstProcessSetting + lpstProcessSetting->offsetszPluginName);
        }
    }
    return lpstProcessSetting;
}

LIBRARY_EXPORT void WINAPI WaffleResumeMainThread()
{
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,lpstProcessSetting->dwThreadId);    //WinXP may return ERROR_ACCESS_DENIED
    ResumeThread(hThread);
}