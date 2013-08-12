#include "..\common.h"

LIBRARY_EXPORT VOID WINAPI WaffleWriteLogFile(
    _In_    LPCTSTR lpszMessage
    )
{
    static HANDLE hLogFile;
    static CRITICAL_SECTION csLogFile;
    if (!hLogFile)
    {
        WaitForSingleObject(lpstProcessSetting->hGlobalMutex, INFINITE);
        if (!hLogFile)  //make sure we need to do so
        {
            InitializeCriticalSection(&csLogFile);
            TCHAR szLogFile[MAX_PATH];
            GetModuleFileName(NULL, szLogFile, sizeof(szLogFile) / sizeof(szLogFile[0]));
            lstrcat(szLogFile, TEXT(".log"));
            hLogFile = CreateFile(szLogFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
            if (!hLogFile)
            {
                MessageBox(0, TEXT("FIXME:Unable to create the log file"), 0, 0);
                DeleteCriticalSection(&csLogFile);
                return;
            }
            DWORD dwNumberOfBytesWritten;
            WriteFile(hLogFile, "\xFF\xFE", 2, &dwNumberOfBytesWritten, NULL);
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