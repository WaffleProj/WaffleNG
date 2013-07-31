#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

LIBRARY_EXPORT LPWAFFLE_PROCESS_SETTING WINAPI WaffleCreateProcessSetting(VOID)
{
    TCHAR szValueProcessSetting[64];
    wsprintf(szValueProcessSetting, szFmtValueProcessSetting, GetCurrentThreadId(), GetTickCount());
    HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, WAFFLE_PROCESS_SETTING_SIZE, szValueProcessSetting);
    SetEnvironmentVariable(szNameProcessSetting, szValueProcessSetting);
    LPWAFFLE_PROCESS_SETTING lpstProcessSetting = (LPWAFFLE_PROCESS_SETTING) MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (lpstProcessSetting)
    {
        lpstProcessSetting->dwVersionMajor = WAFFLE_SDK_VERSION_MAJOR;
        lpstProcessSetting->dwVersionMinor = WAFFLE_SDK_VERSION_MINOR;
        lpstProcessSetting->cbSize = sizeof(WAFFLE_PROCESS_SETTING);
    }

    return lpstProcessSetting;
}

LIBRARY_EXPORT LPWAFFLE_PROCESS_SETTING WINAPI WaffleOpenProcessSetting(VOID)
{
    static LPWAFFLE_PROCESS_SETTING lpstProcessSetting;
    if (!lpstProcessSetting)
    {
        TCHAR szValueProcessSetting[64];
        GetEnvironmentVariable(szNameProcessSetting, szValueProcessSetting, sizeof(szValueProcessSetting));
        SetEnvironmentVariable(szNameProcessSetting, NULL);

        HANDLE hFileMapping = OpenFileMapping(FILE_MAP_WRITE, FALSE, szValueProcessSetting);

        lpstProcessSetting = (LPWAFFLE_PROCESS_SETTING) MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);
    }
    return lpstProcessSetting;
}

LIBRARY_EXPORT VOID WINAPI WaffleResumeMainThread(VOID)
{
    LPWAFFLE_PROCESS_SETTING lpstProcessSetting = WaffleOpenProcessSetting();
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, lpstProcessSetting->dwThreadId);    //WinXP may return ERROR_ACCESS_DENIED
    ResumeThread(hThread);
}