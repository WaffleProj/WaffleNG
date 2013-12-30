#include "..\common.h"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

LIBRARY_EXPORT LPWAFFLE_PROCESS_SETTING WINAPI WaffleCreateProcessSetting(void)
{
    TCHAR szValueProcessSetting[64];
    wsprintf(szValueProcessSetting, szFmtValueProcessSetting, GetCurrentThreadId(), GetTickCount());
    HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, WAFFLE_PROCESS_SETTING_SIZE, szValueProcessSetting);
    if (!hFileMapping)
    {
        return NULL;
    }
    SetEnvironmentVariable(szNameProcessSetting, szValueProcessSetting);
    LPWAFFLE_PROCESS_SETTING lpstPS = (LPWAFFLE_PROCESS_SETTING) MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (lpstPS)
    {
        lpstPS->dwVersionMajor = WAFFLE_SDK_VERSION_MAJOR;
        lpstPS->dwVersionMinor = WAFFLE_SDK_VERSION_MINOR;
        lpstPS->cbSize = sizeof(WAFFLE_PROCESS_SETTING);
    }

    return lpstPS;
}

LIBRARY_EXPORT LPWAFFLE_PROCESS_SETTING WINAPI WaffleOpenProcessSetting(void)
{
    if (!lpstProcessSetting)
    {
        TCHAR szValueProcessSetting[64];
        DWORD nSize = sizeof(szValueProcessSetting) / sizeof(szValueProcessSetting[0]);
        DWORD nWrite = GetEnvironmentVariable(szNameProcessSetting, szValueProcessSetting, nSize);
        if (nWrite && (nWrite < nSize))
        {
            SetEnvironmentVariable(szNameProcessSetting, NULL);

            HANDLE hFileMapping = OpenFileMapping(FILE_MAP_WRITE, FALSE, szValueProcessSetting);
            if (hFileMapping)
            {
                lpstProcessSetting = (LPWAFFLE_PROCESS_SETTING)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);
            }
        }
    }
    return lpstProcessSetting;
}

LIBRARY_EXPORT VOID WINAPI WaffleResumeMainThread(void)
{
    HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, lpstProcessSetting->dwThreadId);
    if (hThread)
    {
        ResumeThread(hThread);
        CloseHandle(hThread);
    }
}