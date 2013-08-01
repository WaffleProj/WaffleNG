#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\mojibake.h"

#ifdef __cplusplus
extern "C" {
#endif
LIBRARY_EXPORT BOOL WINAPI DetourCreateDirectoryA(
    _In_        LPCSTR lpPathName,
    _In_opt_    LPSECURITY_ATTRIBUTES lpSecurityAttributes
    ){
        LPWSTR lpuszPathName = AnsiToUnicode(lpPathName);
        BOOL Result = CreateDirectory(lpuszPathName, lpSecurityAttributes);

        KeepLastErrorAndFree(lpuszPathName);
        return Result;
}

LIBRARY_EXPORT HANDLE WINAPI DetourCreateFileA(
    _In_        LPCSTR lpFileName,
    _In_        DWORD dwDesiredAccess,
    _In_        DWORD dwShareMode,
    _In_opt_    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_        DWORD dwCreationDisposition,
    _In_        DWORD dwFlagsAndAttributes,
    _In_opt_    HANDLE hTemplateFile
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    //MessageBox(0,lpuszFileName,TEXT("DetourCreateFileA"),0);
    HANDLE Result = CreateFile(lpuszFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

    KeepLastErrorAndFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT BOOL WINAPI DetourDeleteFileA(
    _In_    LPCSTR lpFileName
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    BOOL Result = DeleteFile(lpuszFileName);

    KeepLastErrorAndFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT HANDLE WINAPI DetourFindFirstFileA(
    _In_    LPCSTR lpFileName,
    _Out_   LPWIN32_FIND_DATAA lpFindFileData
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    WIN32_FIND_DATA FindFileData;
    HANDLE Result = FindFirstFile(lpuszFileName, &FindFileData);

    DWORD LastError = GetLastError();

    lpFindFileData->dwFileAttributes = FindFileData.dwFileAttributes;
    lpFindFileData->ftCreationTime = FindFileData.ftCreationTime;
    lpFindFileData->ftLastAccessTime = FindFileData.ftLastAccessTime;
    lpFindFileData->ftLastWriteTime = FindFileData.ftLastWriteTime;
    lpFindFileData->nFileSizeHigh = FindFileData.nFileSizeHigh;
    lpFindFileData->nFileSizeLow = FindFileData.nFileSizeLow;
    lpFindFileData->dwReserved0 = FindFileData.dwReserved0;
    lpFindFileData->dwReserved1 = FindFileData.dwReserved1;
    WideCharToMultiByte(stNewEnvir.ACP, 0, FindFileData.cFileName, -1, lpFindFileData->cFileName, sizeof(lpFindFileData->cFileName), NULL, NULL);
    WideCharToMultiByte(stNewEnvir.ACP, 0, FindFileData.cAlternateFileName, -1, lpFindFileData->cAlternateFileName, sizeof(lpFindFileData->cAlternateFileName), NULL, NULL);

    GlobalFree(lpuszFileName);
    SetLastError(LastError);
    return Result;
}

LIBRARY_EXPORT BOOL WINAPI DetourFindNextFileA(
    _In_    HANDLE hFindFile,
    _Out_   LPWIN32_FIND_DATAA lpFindFileData
    )
{
    WIN32_FIND_DATA FindFileData;
    BOOL Result = FindNextFile(hFindFile, &FindFileData);

    DWORD LastError = GetLastError();

    lpFindFileData->dwFileAttributes = FindFileData.dwFileAttributes;
    lpFindFileData->ftCreationTime = FindFileData.ftCreationTime;
    lpFindFileData->ftLastAccessTime = FindFileData.ftLastAccessTime;
    lpFindFileData->ftLastWriteTime = FindFileData.ftLastWriteTime;
    lpFindFileData->nFileSizeHigh = FindFileData.nFileSizeHigh;
    lpFindFileData->nFileSizeLow = FindFileData.nFileSizeLow;
    lpFindFileData->dwReserved0 = FindFileData.dwReserved0;
    lpFindFileData->dwReserved1 = FindFileData.dwReserved1;
    WideCharToMultiByte(stNewEnvir.ACP, 0, FindFileData.cFileName, -1, lpFindFileData->cFileName, sizeof(lpFindFileData->cFileName), NULL, NULL);
    WideCharToMultiByte(stNewEnvir.ACP, 0, FindFileData.cAlternateFileName, -1, lpFindFileData->cAlternateFileName, sizeof(lpFindFileData->cAlternateFileName), NULL, NULL);

    SetLastError(LastError);
    return Result;
}

LIBRARY_EXPORT UINT WINAPI DetourGetACP(void)
{
    return stNewEnvir.ACP;
}

LIBRARY_EXPORT LPSTR WINAPI DetourGetCommandLineA(void)   //由于这个函数直接返回指针无需释放,所以只能这么做
{
    static LPSTR lpszCommandLineA;
    if (!lpszCommandLineA)
    {
        LPWSTR lpszCommandLineW = GetCommandLineW();
        int intSize = 4 * lstrlenW(lpszCommandLineW);
        lpszCommandLineA = (LPSTR) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, intSize);
        WideCharToMultiByte(stNewEnvir.ACP, 0, lpszCommandLineW, -1, lpszCommandLineA, intSize, NULL, NULL);
    }
    return lpszCommandLineA;
}

LIBRARY_EXPORT BOOL WINAPI DetourGetCPInfo(
    _In_    UINT CodePage,
    _Out_   LPCPINFO lpCPInfo
    )
{
    static LPGETCPINFO BackupGetCPInfo;
    if (!BackupGetCPInfo)
    {
        BackupGetCPInfo = (LPGETCPINFO) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("GetCPInfo"));
    }

    if (!CodePage)
    {
        CodePage = stNewEnvir.ACP;
    }

    return BackupGetCPInfo(CodePage, lpCPInfo);
}

LIBRARY_EXPORT DWORD WINAPI DetourGetFileAttributesA(
    _In_    LPCSTR lpFileName
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    DWORD Result = GetFileAttributes(lpuszFileName);

    KeepLastErrorAndFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT DWORD WINAPI DetourGetModuleFileNameA(
    _In_opt_    HMODULE hModule,
    _Out_       LPSTR lpFilename,
    _In_        DWORD nSize
    )
{
    LPWSTR lpuszFilename = 0;
    if (lpFilename)
    {
        lpuszFilename = (LPWSTR) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 4 * nSize);
    }

    GetModuleFileName(hModule, lpuszFilename, nSize);

    DWORD LastError = GetLastError();
    WideCharToMultiByte(stNewEnvir.ACP, 0, lpuszFilename, -1, lpFilename, nSize, NULL, NULL);
    HeapFree(hHeap, 0, lpuszFilename);
    int Result = lstrlenA(lpFilename);
    SetLastError(LastError);
    return Result;
}

LIBRARY_EXPORT HMODULE WINAPI DetourGetModuleHandleA(
    _In_opt_    LPCSTR lpModuleName
    )
{
    LPWSTR lpuszModuleName = AnsiToUnicode(lpModuleName);
    HMODULE Result = GetModuleHandle(lpuszModuleName);

    KeepLastErrorAndFree(lpuszModuleName);
    return Result;
}

LIBRARY_EXPORT UINT WINAPI DetourGetOEMCP(void)
{
    return stNewEnvir.OEMCP;
}

LIBRARY_EXPORT HMODULE WINAPI DetourLoadLibraryA(
    _In_    LPCSTR lpFileName
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    HMODULE Result = LoadLibrary(lpuszFileName);

    KeepLastErrorAndFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT HMODULE WINAPI DetourLoadLibraryExA(
    _In_        LPCSTR lpFileName,
    _Reserved_  HANDLE hFile,
    _In_        DWORD dwFlags
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    HMODULE Result = LoadLibraryEx(lpuszFileName, hFile, dwFlags);

    KeepLastErrorAndFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT int WINAPI DetourMultiByteToWideChar(
    _In_        UINT CodePage,
    _In_        DWORD dwFlags,
    _In_        LPCSTR lpMultiByteStr,
    _In_        int cbMultiByte,
    _Out_opt_   LPWSTR lpWideCharStr,
    _In_        int cchWideChar
    )
{
    static LPMULTIBYTETOWIDECHAR BackupMultiByteToWideChar;
    if (!BackupMultiByteToWideChar)
    {
        BackupMultiByteToWideChar = (LPMULTIBYTETOWIDECHAR) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("MultiByteToWideChar"));
    }

    switch (CodePage)
    {
    case CP_ACP:
        CodePage = stNewEnvir.ACP;
        break;
    case CP_OEMCP:
        CodePage = stNewEnvir.OEMCP;
        break;
    }

    return BackupMultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

LIBRARY_EXPORT BOOL WINAPI DetourSetCurrentDirectoryA(
    _In_    LPCSTR lpPathName
    )
{
    LPWSTR lpuszPathName = AnsiToUnicode(lpPathName);
    BOOL Result = SetCurrentDirectory(lpuszPathName);

    KeepLastErrorAndFree(lpuszPathName);
    return Result;
}

LIBRARY_EXPORT BOOL WINAPI DetourSetFileAttributesA(
    _In_    LPCSTR lpFileName,
    _In_    DWORD dwFileAttributes
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    BOOL Result = SetFileAttributes(lpuszFileName, dwFileAttributes);

    KeepLastErrorAndFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT int WINAPI DetourWideCharToMultiByte(
    _In_        UINT CodePage,
    _In_        DWORD dwFlags,
    _In_        LPCWSTR lpWideCharStr,
    _In_        int cchWideChar,
    _Out_opt_   LPSTR lpMultiByteStr,
    _In_        int cbMultiByte,
    _In_opt_    LPCSTR lpDefaultChar,
    _Out_opt_   LPBOOL lpUsedDefaultChar
    )
{
    static LPWIDECHARTOMULTIBYTE BackupWideCharToMultiByte;
    if (!BackupWideCharToMultiByte)
    {
        BackupWideCharToMultiByte = (LPWIDECHARTOMULTIBYTE) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("WideCharToMultiByte"));
    }

    switch (CodePage)
    {
    case CP_ACP:
        CodePage = stNewEnvir.ACP;
        break;
    case CP_OEMCP:
        CodePage = stNewEnvir.OEMCP;
        break;
    }

    return BackupWideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
}
#ifdef __cplusplus
};
#endif