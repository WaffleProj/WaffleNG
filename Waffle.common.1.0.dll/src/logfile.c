#include "..\common.h"

LIBRARY_EXPORT VOID WINAPI WaffleWriteLogFileA(
    _In_    LPCSTR lpszMessage
    )
{
    LPWSTR lpuszMessage = (LPWSTR) WaffleAlloc((WafflelstrlenA(lpszMessage) + 1) * 2);
    int i;
    for (i = 0; lpszMessage[i]; lpuszMessage[i] = lpszMessage[i], i++);
    WaffleWriteLogFileW(lpuszMessage);
}

LIBRARY_EXPORT VOID WINAPI WaffleWriteLogFileW(
    _In_    LPCWSTR lpszMessage
    )
{
    static HANDLE hLogFile;
    static CRITICAL_SECTION csLogFile;
    if (!hLogFile)
    {
        WaitForSingleObject(lpstProcessSetting->hGlobalMutex, INFINITE);
        if (!hLogFile)  //make sure we need to do so
        {
            TCHAR szLogFile[MAX_PATH];
            if (GetModuleFileName(NULL, szLogFile, lengthof(szLogFile)))
            {
                InitializeCriticalSection(&csLogFile);
                lstrcat(szLogFile, TEXT(".log"));
                hLogFile = CreateFile(szLogFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
                if (hLogFile == INVALID_HANDLE_VALUE)
                {
                    MessageBox(0, TEXT("FIXME:Unable to create the log file"), 0, 0);
                    DeleteCriticalSection(&csLogFile);
                    ReleaseMutex(lpstProcessSetting->hGlobalMutex);
                    return;
                }
                DWORD dwNumberOfBytesWritten;
                WriteFile(hLogFile, "\xFF\xFE", 2, &dwNumberOfBytesWritten, NULL);
            }
            else
            {
                MessageBox(0, TEXT("FIXME:Unable to get the executable path"), 0, 0);
                ReleaseMutex(lpstProcessSetting->hGlobalMutex);
                return;
            }
        }
        ReleaseMutex(lpstProcessSetting->hGlobalMutex);
    }

    EnterCriticalSection(&csLogFile);

    DWORD dwNumberOfBytesWritten;
    WriteFile(hLogFile, "\n\0\n\0", 4, &dwNumberOfBytesWritten, NULL);
    WriteFile(hLogFile, lpszMessage, lstrlen(lpszMessage) * sizeof(TCHAR), &dwNumberOfBytesWritten, NULL);
    FlushFileBuffers(hLogFile);

    LeaveCriticalSection(&csLogFile);
}