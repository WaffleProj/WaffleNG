#include "..\common.h"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleInitProcessSetting(
    _In_    LPWAFFLE_PROCESS_SETTING lpstPS
    )
{
    if (lpstPS)
    {
        if (!lpstPS->cbSize) lpstPS->cbSize = sizeof(WAFFLE_PROCESS_SETTING);
        if (!lpstPS->dwVersionMajor) lpstPS->dwVersionMajor = WAFFLE_SDK_VERSION_MAJOR;
        if (!lpstPS->dwVersionMinor) lpstPS->dwVersionMinor = WAFFLE_SDK_VERSION_MINOR;

        lpstPS->hGlobalMutex = CreateMutex(NULL, FALSE, NULL);

        lpstPS->lpstComponent = NULL;
        // So we can use WaffleAlloc
        WaffleAddComponent(TEXT("Waffle.common.1.0.dll"));
        lpstPS->lpstLibrary = NULL;

        // So we can use WaffleGetOptionString
        if (!lstrcmp(lpstPS->szComponent, TEXT(""))) lstrcpy(lpstPS->szComponent, TEXT("Waffle"));
        if (!lstrcmp(lpstPS->szComponentDirectory, TEXT("")))
        {
            WaffleGetModuleDirectory(GetModuleHandle(TEXT("Waffle.common.1.0.dll")), lpstPS->szComponentDirectory, lengthof(lpstPS->szComponentDirectory));
            int i = lstrlen(lpstPS->szComponentDirectory);
            for (i--; lpstPS->szComponentDirectory[i] != TEXT('\\'); i--); lpstPS->szComponentDirectory[i] = TEXT('\0');
            for (i--; lpstPS->szComponentDirectory[i] != TEXT('\\'); i--); lpstPS->szComponentDirectory[i] = TEXT('\0');
        }
    }
}

WAFFLE_COMMON_DLL_FUNCTION LPWAFFLE_PROCESS_SETTING WINAPI WaffleCreateProcessSetting(void)
{
    if (!lpstProcessSetting)
    {
        lpstProcessSetting = (LPWAFFLE_PROCESS_SETTING) WaffleAlloc(WAFFLE_PROCESS_SETTING_SIZE);
        WaffleInitProcessSetting(lpstProcessSetting);
    }

    return lpstProcessSetting;
}

WAFFLE_COMMON_DLL_FUNCTION LPWAFFLE_PROCESS_SETTING WINAPI WaffleShareProcessSetting(void)
{
    LPWAFFLE_PROCESS_SETTING lpstPS = NULL;

    if (lpstProcessSetting)
    {
        TCHAR szValueProcessSetting[64];
        wsprintf(szValueProcessSetting, szFmtValueProcessSetting, GetCurrentThreadId(), GetTickCount());
        HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, WAFFLE_PROCESS_SETTING_SIZE, szValueProcessSetting);
        if (!hFileMapping)
        {
            return NULL;
        }
        SetEnvironmentVariable(szNameProcessSetting, szValueProcessSetting);
        lpstPS = (LPWAFFLE_PROCESS_SETTING) MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

        RtlMoveMemory(lpstPS, lpstProcessSetting, WAFFLE_PROCESS_SETTING_SIZE);
    }

    return lpstPS;
}

WAFFLE_COMMON_DLL_FUNCTION LPWAFFLE_PROCESS_SETTING WINAPI WaffleOpenProcessSetting(void)
{
    if (!lpstProcessSetting)
    {
        TCHAR szValueProcessSetting[64];
        DWORD nSize = lengthof(szValueProcessSetting);
        DWORD nWrite = GetEnvironmentVariable(szNameProcessSetting, szValueProcessSetting, nSize);
        if (nWrite && (nWrite < nSize))
        {
            SetEnvironmentVariable(szNameProcessSetting, NULL);

            HANDLE hFileMapping = OpenFileMapping(FILE_MAP_WRITE, FALSE, szValueProcessSetting);
            if (hFileMapping)
            {
                LPWAFFLE_PROCESS_SETTING lpstPS = (LPWAFFLE_PROCESS_SETTING) MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);

                if (lpstPS)
                {
                    WaffleCreateProcessSetting();
                    if (lpstProcessSetting)
                    {
                        RtlMoveMemory(lpstProcessSetting, lpstPS, WAFFLE_PROCESS_SETTING_SIZE);
                        WaffleInitProcessSetting(lpstProcessSetting);
                    }

                    UnmapViewOfFile(lpstPS);
                }

                CloseHandle(hFileMapping);
            }
        }
    }

    return lpstProcessSetting;
}

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleResumeMainThread(void)
{
    HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, lpstProcessSetting->dwThreadId);
    if (hThread)
    {
        ResumeThread(hThread);
        CloseHandle(hThread);
    }
}