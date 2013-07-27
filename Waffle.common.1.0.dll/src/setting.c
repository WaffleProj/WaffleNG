#define  UNICODE
#define  _UNICODE
#include "..\common.h"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

LIBRARY_EXPORT LPWAFFLE_PROCESS_SETTING WINAPI WaffleOpenProcessSetting(void)
{
    if (!lpstProcessSetting)
    {
        TCHAR szValueProcessSetting[64];
        GetEnvironmentVariable(szNameProcessSetting, szValueProcessSetting, sizeof(szValueProcessSetting));
        SetEnvironmentVariable(szNameProcessSetting, NULL);

        HANDLE hFileMapping = OpenFileMapping(FILE_MAP_WRITE, FALSE, szValueProcessSetting);

        lpstProcessSetting = MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);

        if (lpstProcessSetting)
        {
            lpstProcessSetting->lpszPlugin = (LPTSTR) ((SIZE_T) lpstProcessSetting + lpstProcessSetting->offsetszPlugin);
        }
    }
    return lpstProcessSetting;
}

LIBRARY_EXPORT LPWAFFLE_PROCESS_SETTING WINAPI WaffleCreateProcessSetting(
    _In_    LPWAFFLE_PROCESS_SETTING lpstNewSetting
    )
{
    TCHAR szValueProcessSetting[64];
    wsprintf(szValueProcessSetting, szFmtValueProcessSetting, GetCurrentThreadId(), GetTickCount());
    HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, WAFFLE_PROCESS_SETTING_SIZE, szValueProcessSetting);
    SetEnvironmentVariable(szNameProcessSetting, szValueProcessSetting);
    LPWAFFLE_PROCESS_SETTING lpstProcessSetting = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (lpstProcessSetting)
    {
        lpstProcessSetting->wVersionMajor = WAFFLE_SDK_VERSION_MAJOR;
        lpstProcessSetting->wVersionMinor = WAFFLE_SDK_VERSION_MINOR;
        lpstProcessSetting->cbSize = sizeof(WAFFLE_PROCESS_SETTING);
        lpstProcessSetting->offsetszPlugin = sizeof(WAFFLE_PROCESS_SETTING);
        lpstProcessSetting->lpszPlugin = (LPTSTR) ((SIZE_T) lpstProcessSetting + lpstProcessSetting->offsetszPlugin);
        lstrcpy(lpstProcessSetting->lpszPlugin, lpstNewSetting->lpszPlugin);
    }

    return lpstProcessSetting;
}

LIBRARY_EXPORT void WINAPI WaffleResumeMainThread(void)
{
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, lpstProcessSetting->dwThreadId);    //WinXP may return ERROR_ACCESS_DENIED
    ResumeThread(hThread);
}