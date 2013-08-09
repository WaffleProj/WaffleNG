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
    LIBRARY_EXPORT DWORD WINAPI DetourGetCurrentDirectoryA(
        _In_    DWORD nBufferLength,
        _Out_   LPSTR lpBuffer
        )
    {
        DWORD Result = 0;
        if (lpBuffer)
        {
            LPWSTR lpuBuffer = (LPWSTR) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, nBufferLength*sizeof(WCHAR));
            Result = GetCurrentDirectory(nBufferLength, lpuBuffer);
            DWORD LastError = GetLastError();
            WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuBuffer, -1, lpBuffer, nBufferLength - 1, NULL, NULL);
            HeapFree(hHeap, 0, lpuBuffer);
            SetLastError(LastError);
        }
        return Result;
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

    LIBRARY_EXPORT BOOL WINAPI DetourCreateDirectoryA(
        _In_        LPCSTR lpPathName,
        _In_opt_    LPSECURITY_ATTRIBUTES lpSecurityAttributes
        )
    {
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
        WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, FindFileData.cFileName, -1, lpFindFileData->cFileName, sizeof(lpFindFileData->cFileName), NULL, NULL);
        WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, FindFileData.cAlternateFileName, -1, lpFindFileData->cAlternateFileName, sizeof(lpFindFileData->cAlternateFileName), NULL, NULL);

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
        WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, FindFileData.cFileName, -1, lpFindFileData->cFileName, sizeof(lpFindFileData->cFileName), NULL, NULL);
        WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, FindFileData.cAlternateFileName, -1, lpFindFileData->cAlternateFileName, sizeof(lpFindFileData->cAlternateFileName), NULL, NULL);

        SetLastError(LastError);
        return Result;
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

    LIBRARY_EXPORT DWORD WINAPI DetourGetModuleFileNameA(
        _In_opt_    HMODULE hModule,
        _Out_       LPSTR lpFilename,
        _In_        DWORD nSize
        )
    {
        LPWSTR lpuszFilename = (LPWSTR) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(WCHAR) * nSize);

        GetModuleFileName(hModule, lpuszFilename, nSize);

        DWORD LastError = GetLastError();
        WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszFilename, -1, lpFilename, nSize, NULL, NULL);
        HeapFree(hHeap, 0, lpuszFilename);
        int Result = lstrlenA(lpFilename);
        SetLastError(LastError);
        return Result;
    }

    LIBRARY_EXPORT HMODULE WINAPI DetourGetModuleHandleA(
        _In_opt_    LPCSTR lpModuleName
        )
    {
        LPWSTR lpuszModuleName = NULL;
        if (lpModuleName)
        {
            lpuszModuleName = AnsiToUnicode(lpModuleName);
        }
        HMODULE Result = GetModuleHandle(lpuszModuleName);

        if (lpModuleName)
        {
            KeepLastErrorAndFree(lpuszModuleName);
        }
        return Result;
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

    LIBRARY_EXPORT LPSTR WINAPI DetourGetCommandLineA(void)   //由于这个函数直接返回指针无需释放,所以只能这么做
    {
        static LPSTR lpszCommandLineA;
        if (!lpszCommandLineA)
        {
            LPWSTR lpszCommandLineW = GetCommandLineW();
            int intSize = 4 * lstrlenW(lpszCommandLineW);
            lpszCommandLineA = (LPSTR) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, intSize);
            WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpszCommandLineW, -1, lpszCommandLineA, intSize, NULL, NULL);
        }
        return lpszCommandLineA;
    }

    LIBRARY_EXPORT UINT WINAPI DetourGetACP(void)
    {
        return stNewEnvir.AnsiCodePage;
    }

    LIBRARY_EXPORT UINT WINAPI DetourGetOEMCP(void)
    {
        return stNewEnvir.OemCodePage;
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

        switch (CodePage)
        {
        case CP_ACP:
            CodePage = stNewEnvir.AnsiCodePage;
            break;
        case CP_OEMCP:
            CodePage = stNewEnvir.OemCodePage;
            break;
        }

        return BackupGetCPInfo(CodePage, lpCPInfo);
    }

    LIBRARY_EXPORT LCID WINAPI DetourGetThreadLocale(void)
    {
        return stNewEnvir.ThreadLocale;
    }

    LIBRARY_EXPORT LCID WINAPI DetourGetUserDefaultLCID(void)
    {
        return stNewEnvir.ThreadLocale;
    }

    LIBRARY_EXPORT LCID WINAPI DetourGetSystemDefaultLCID(void)
    {
        return stNewEnvir.ThreadLocale;
    }

    LIBRARY_EXPORT LANGID WINAPI DetourGetUserDefaultLangID(void)
    {
        return LANGIDFROMLCID(stNewEnvir.ThreadLocale);
    }

    LIBRARY_EXPORT LANGID WINAPI DetourGetSystemDefaultLangID(void)
    {
        return LANGIDFROMLCID(stNewEnvir.ThreadLocale);
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
            CodePage = stNewEnvir.AnsiCodePage;
            break;
        case CP_OEMCP:
            CodePage = stNewEnvir.OemCodePage;
            break;
        }

        return BackupMultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
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
            CodePage = stNewEnvir.AnsiCodePage;
            break;
        case CP_OEMCP:
            CodePage = stNewEnvir.OemCodePage;
            break;
        }

        return BackupWideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
    }

    LIBRARY_EXPORT int WINAPI DetourCompareStringA(
        _In_    LCID Locale,
        _In_    DWORD dwCmpFlags,
        _In_    LPCSTR lpString1,
        _In_    int cchCount1,
        _In_    LPCSTR lpString2,
        _In_    int cchCount2
        )
    {
        /*
        static LPCOMPARESTRINGA BackupCompareStringA;
        if (!BackupCompareStringA)
        {
        BackupCompareStringA = (LPCOMPARESTRINGA) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("CompareStringA"));
        }
        */

        LPWSTR lpuString1 = AnsiToUnicode(lpString1);
        LPWSTR lpuString2 = AnsiToUnicode(lpString2);
        //MessageBox(0, lpuString1, lpuString2, 0);
        //int Result = BackupCompareStringA(Locale, dwCmpFlags, lpString1, cchCount1, lpString2, cchCount2);
        int Result = CompareString(Locale, dwCmpFlags, lpuString1, cchCount1, lpuString2, cchCount2);

        DWORD LastError = GetLastError();
        HeapFree(hHeap, 0, lpuString1);
        HeapFree(hHeap, 0, lpuString2);
        SetLastError(LastError);
        return Result;
    }

    LIBRARY_EXPORT BOOL WINAPI DetourIsDBCSLeadByte(
        _In_    BYTE TestChar
        )
    {
        return IsDBCSLeadByteEx(stNewEnvir.AnsiCodePage, TestChar);
    }

    LIBRARY_EXPORT BOOL WINAPI DetourIsDBCSLeadByteEx(
        _In_    UINT CodePage,
        _In_    BYTE TestChar
        )
    {
        static LPISDBCSLEADBYTEEX BackupIsDBCSLeadByteEx;
        if (!BackupIsDBCSLeadByteEx)
        {
            BackupIsDBCSLeadByteEx = (LPISDBCSLEADBYTEEX) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("IsDBCSLeadByteEx"));
        }

        switch (CodePage)
        {
        case CP_ACP:
            CodePage = stNewEnvir.AnsiCodePage;
            break;
        case CP_OEMCP:
            CodePage = stNewEnvir.OemCodePage;
            break;
        }

        return BackupIsDBCSLeadByteEx(CodePage, TestChar);
    }

    /*
    ExceptionCode = 0eedfade
    ExceptionFlags = 00000001
    NumberOfArguments = 00000007
    Arguments[0] = 00000000000186EC
    Arguments[1] = 000000000812E668
    Arguments[2] = 0000000000017854
    Arguments[3] = 000000000037F3E8
    Arguments[4] = 0000000000000000
    Arguments[5] = 000000000037F3DC
    Arguments[6] = 000000000037F3C0

    CPU Disasm
    Address   Hex dump          Command                                  Comments
    0001375C  /$ /5A            pop     edx
    0001375D  |. |54            push    esp                             ; mov lpArguments[6],esp    ;
    0001375E  |. |55            push    ebp                             ; mov lpArguments[5],ebp    ;
    0001375F  |. |57            push    edi                             ; mov lpArguments[4],edi    ;
    00013760  |. |56            push    esi                             ; mov lpArguments[3],esi    ;
    00013761  |. |53            push    ebx                             ; mov lpArguments[2],ebx    ;
    00013762  |. |50            push    eax                             ; mov lpArguments[1],eax    ;
    00013763  |. |52            push    edx                             ; mov lpArguments[0],edx    ; return address
    00013764  |. |54            push    esp                             ; push lpArguments
    00013765  |. |6A 07         push    7                               ; push nNumberOfArguments
    00013767  |. |6A 01         push    1                               ; push dwExceptionFlags
    00013769  |. |68 DEFAED0E   push    0EEDFADE                        ; push dwExceptionCode
    0001376E  |. |52            push    edx                             ; call RaiseException
    0001376F  \.-|E9 44DBFFFF   jmp     <jmp.&kernel32.RaiseException>

    */
    LIBRARY_EXPORT void WINAPI DetourRaiseException(
        _In_    DWORD dwExceptionCode,
        _In_    DWORD dwExceptionFlags,
        _In_    DWORD nNumberOfArguments,
        _In_    const ULONG_PTR *lpArguments
        )
    {
        static LPRAISEEXCEPTION BackupRaiseException;
        if (!BackupRaiseException)
        {
            BackupRaiseException = (LPRAISEEXCEPTION) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("RaiseException"));
        }

        TCHAR szExceptionRecord[4096];

        wsprintf(szExceptionRecord, TEXT("ExceptionCode = %08x\nExceptionFlags = %08x\nNumberOfArguments = %08x"), dwExceptionCode, dwExceptionFlags, nNumberOfArguments);

        DWORD i;
        for (i = 0; i < nNumberOfArguments; i++)
        {
            TCHAR szBuf[256];
            wsprintf(szBuf, TEXT("\nArguments[%u] = %016I64X"), i, (UINT64) (lpArguments[i]));
            lstrcat(szExceptionRecord, szBuf);
        }
        //MessageBox(0, szExceptionRecord, 0, 0);
        WaffleWriteLogFile(szExceptionRecord);

        BackupRaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
    }
#ifdef __cplusplus
};
#endif