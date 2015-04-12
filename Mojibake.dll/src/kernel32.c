#include "..\mojibake.h"

_Success_(return)
LIBRARY_EXPORT BOOL WINAPI DetourCreateProcessW(
_In_opt_    LPCWSTR lpApplicationName,
_Inout_opt_ LPWSTR lpCommandLine,
_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
_In_        BOOL bInheritHandles,
_In_        DWORD dwCreationFlags,
_In_opt_    LPVOID lpEnvironment,
_In_opt_    LPCWSTR lpCurrentDirectory,
_In_opt_    LPSTARTUPINFOW lpStartupInfo,
_Out_       LPPROCESS_INFORMATION lpProcessInformation
)
{
    static LPCREATEPROCESSW BackupCreateProcessW;
    if (!BackupCreateProcessW)
    {
        BackupCreateProcessW = (LPCREATEPROCESSW) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("CreateProcessW"));
    }

    TCHAR szMessage[1024];
    wsprintf(szMessage, TEXT("This program wants to launch another program:\nAppName = %s\nCmdLine = %s\n\nIs this what you want to do?"), lpApplicationName, lpCommandLine);
    if (MessageBox(0, szMessage, 0, MB_YESNO | MB_ICONWARNING) == IDYES)
    {
        return BackupCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    }
    else
    {
        SetLastError(ERROR_ACCESS_DENIED);
        return FALSE;
    }
}

_Success_(return)
LIBRARY_EXPORT BOOL WINAPI DetourCreateProcessA(
_In_opt_    LPCSTR lpApplicationName,
_Inout_opt_ LPSTR lpCommandLine,
_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
_In_        BOOL bInheritHandles,
_In_        DWORD dwCreationFlags,
_In_opt_    LPVOID lpEnvironment,
_In_opt_    LPCSTR lpCurrentDirectory,
_In_opt_    LPSTARTUPINFOA lpStartupInfo,
_Out_       LPPROCESS_INFORMATION lpProcessInformation
)
{
    LPWSTR lpuszApplicationName = NULL;
    LPWSTR lpuszCommandLine = NULL;
    LPWSTR lpuszCurrentDirectory = NULL;
    LPSTARTUPINFOW lpuStartupInfo = NULL;
    STARTUPINFOW StartupInfo;
    if (lpApplicationName)
    {
        lpuszApplicationName = AnsiToUnicode(lpApplicationName);
    }
    if (lpCommandLine)
    {
        lpuszCommandLine = AnsiToUnicode(lpCommandLine);
    }
    if (lpCurrentDirectory)
    {
        lpuszCurrentDirectory = AnsiToUnicode(lpCurrentDirectory);
    }
    if (lpStartupInfo)
    {
        lpuStartupInfo = &StartupInfo;
        RtlMoveMemory(&StartupInfo, lpStartupInfo, sizeof(STARTUPINFOW));
        lpuStartupInfo->lpReserved = AnsiToUnicode(lpStartupInfo->lpReserved);
        lpuStartupInfo->lpDesktop = AnsiToUnicode(lpStartupInfo->lpDesktop);
        lpuStartupInfo->lpTitle = AnsiToUnicode(lpStartupInfo->lpTitle);
    }

    BOOL Result = DetourCreateProcessW(lpuszApplicationName, lpuszCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpuszCurrentDirectory, lpuStartupInfo, lpProcessInformation);

    if (lpApplicationName)
    {
        MojibakeFree(lpuszApplicationName);
    }
    if (lpCommandLine)
    {
        MojibakeFree(lpuszCommandLine);
    }
    if (lpCurrentDirectory)
    {
        MojibakeFree(lpuszCurrentDirectory);
    }
    if (lpStartupInfo)
    {
        MojibakeFree(lpuStartupInfo->lpReserved);
        MojibakeFree(lpuStartupInfo->lpDesktop);
        MojibakeFree(lpuStartupInfo->lpTitle);
    }
    return Result;
}

LIBRARY_EXPORT DWORD WINAPI DetourGetCurrentDirectoryA(
    _In_    DWORD nBufferLength,
    _Out_writes_to_opt_(nBufferLength, (return + 1))   LPSTR lpBuffer
    )
{
    DWORD Result = 0;
    if (lpBuffer)
    {
        LPWSTR lpuBuffer = (LPWSTR) WaffleAlloc((nBufferLength + 1)*sizeof(lpuBuffer[0]));
        if (lpuBuffer)
        {
            Result = GetCurrentDirectory(nBufferLength, lpuBuffer);

            DWORD LastError = GetLastError();
            WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuBuffer, -1, lpBuffer, nBufferLength - 1, NULL, NULL);
            MojibakeFree(lpuBuffer);
            SetLastError(LastError);
        }
        else
        {
            lpBuffer[0] = TEXT('\0');
        }
    }
    return Result;
}

LIBRARY_EXPORT BOOL WINAPI DetourSetCurrentDirectoryA(
    _In_    LPCSTR lpPathName
    )
{
    LPWSTR lpuszPathName = AnsiToUnicode(lpPathName);
    BOOL Result = SetCurrentDirectory(lpuszPathName);

    MojibakeFree(lpuszPathName);
    return Result;
}

LIBRARY_EXPORT BOOL WINAPI DetourCreateDirectoryA(
    _In_        LPCSTR lpPathName,
    _In_opt_    LPSECURITY_ATTRIBUTES lpSecurityAttributes
    )
{
    LPWSTR lpuszPathName = AnsiToUnicode(lpPathName);
    BOOL Result = CreateDirectory(lpuszPathName, lpSecurityAttributes);

    MojibakeFree(lpuszPathName);
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

    MojibakeFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT BOOL WINAPI DetourDeleteFileA(
    _In_    LPCSTR lpFileName
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    BOOL Result = DeleteFile(lpuszFileName);

    MojibakeFree(lpuszFileName);
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

    RtlMoveMemory(lpFindFileData, &FindFileData, offsetof(WIN32_FIND_DATAA, dwReserved1));
    WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, FindFileData.cFileName, -1, lpFindFileData->cFileName, sizeof(lpFindFileData->cFileName), NULL, NULL);
    WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, FindFileData.cAlternateFileName, -1, lpFindFileData->cAlternateFileName, sizeof(lpFindFileData->cAlternateFileName), NULL, NULL);

    MojibakeFree(lpuszFileName);
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

    RtlMoveMemory(lpFindFileData, &FindFileData, offsetof(WIN32_FIND_DATAA, dwReserved1));
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

    MojibakeFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT BOOL WINAPI DetourSetFileAttributesA(
    _In_    LPCSTR lpFileName,
    _In_    DWORD dwFileAttributes
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    BOOL Result = SetFileAttributes(lpuszFileName, dwFileAttributes);

    MojibakeFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT DWORD WINAPI DetourGetFullPathNameA(
    _In_    LPCSTR lpFileName,
    _In_    DWORD nBufferLength,
    _Out_   LPSTR lpBuffer,
    _Out_   LPSTR *lpFilePart
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    LPWSTR lpuBuffer = (LPWSTR) WaffleAlloc(sizeof(WCHAR) * nBufferLength);
    LPWSTR lpuFilePart = NULL;

    LPWSTR *lplpuFilePart = NULL;
    if (lpFilePart)
    {
        lplpuFilePart = &lpuFilePart;
    }

    GetFullPathName(lpuszFileName, nBufferLength, lpuBuffer, lplpuFilePart);

    DWORD LastError = GetLastError();
    WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuBuffer, -1, lpBuffer, nBufferLength - 1, NULL, NULL);
    MojibakeFree(lpuszFileName);
    MojibakeFree(lpuBuffer);
    int Result = lstrlenA(lpBuffer);        //This is not the correct method to get the return value, but it should work in the most of cases.
    if (lpFilePart && lpuFilePart)
    {
        *lpFilePart = lpBuffer + Result;
        int i;
        for (i = Result; (*lpFilePart)[i] != '\\'; i--);
        if ((*lpFilePart)[i - 1] == ':') //This is a driver
        {
            *lpFilePart = NULL;
        }
        else
        {
            *lpFilePart = *lpFilePart + i;
        }
    }
    SetLastError(LastError);
    return Result;
}

LIBRARY_EXPORT DWORD WINAPI DetourGetModuleFileNameA(
    _In_opt_    HMODULE hModule,
    _Out_       LPSTR lpFilename,
    _In_        DWORD nSize
    )
{
    LPWSTR lpuszFilename = (LPWSTR) WaffleAlloc(sizeof(WCHAR) * nSize);

    GetModuleFileName(hModule, lpuszFilename, nSize);

    DWORD LastError = GetLastError();
    WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszFilename, -1, lpFilename, nSize - 1, NULL, NULL);
    MojibakeFree(lpuszFilename);
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
        MojibakeFree(lpuszModuleName);
    }
    return Result;
}

LIBRARY_EXPORT HMODULE WINAPI DetourLoadLibraryA(
    _In_    LPCSTR lpFileName
    )
{
    LPWSTR lpuszFileName = AnsiToUnicode(lpFileName);
    HMODULE Result = LoadLibrary(lpuszFileName);

    MojibakeFree(lpuszFileName);
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

    MojibakeFree(lpuszFileName);
    return Result;
}

LIBRARY_EXPORT LPSTR WINAPI DetourGetCommandLineA(void)
{
    static LPSTR lpszCommandLineA;
    if (!lpszCommandLineA)
    {
        LPWSTR lpszCommandLineW = GetCommandLine();
        int nSize = 4 * (lstrlenW(lpszCommandLineW) + 1);
        lpszCommandLineA = (LPSTR) WaffleAlloc(nSize);
        WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpszCommandLineW, -1, lpszCommandLineA, nSize, NULL, NULL);
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

/*
LIBRARY_EXPORT int WINAPI DetourCompareStringA(
_In_    LCID Locale,
_In_    DWORD dwCmpFlags,
_In_    LPCSTR lpString1,
_In_    int cchCount1,
_In_    LPCSTR lpString2,
_In_    int cchCount2
)
{
static LPCOMPARESTRINGA BackupCompareStringA;
if (!BackupCompareStringA)
{
BackupCompareStringA = (LPCOMPARESTRINGA) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("CompareStringA"));
}

LPWSTR lpuString1 = AnsiToUnicode(lpString1);
LPWSTR lpuString2 = AnsiToUnicode(lpString2);
//MessageBox(0, lpuString1, lpuString2, 0);
//int Result = BackupCompareStringA(Locale, dwCmpFlags, lpString1, cchCount1, lpString2, cchCount2);
int Result = CompareString(Locale, dwCmpFlags, lpuString1, cchCount1, lpuString2, cchCount2);

MojibakeFree(lpuString1);
MojibakeFree(lpuString2);
return Result;
}
*/

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

LIBRARY_EXPORT BOOL WINAPI DetourIsDBCSLeadByte(
    _In_    BYTE TestChar
    )
{
    return DetourIsDBCSLeadByteEx(stNewEnvir.AnsiCodePage, TestChar);
}

LIBRARY_EXPORT UINT WINAPI DetourGetPrivateProfileIntA(
    _In_    LPCSTR lpAppName,
    _In_    LPCSTR lpKeyName,
    _In_    INT nDefault,
    _In_    LPCSTR lpFileName
    )
{
    static LPGETPRIVATEPROFILEINTA BackupGetPrivateProfileIntA;
    if (!BackupGetPrivateProfileIntA)
    {
        BackupGetPrivateProfileIntA = (LPGETPRIVATEPROFILEINTA) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("GetPrivateProfileIntA"));
    }

    LPSTR lpszFileName = MBCSToMBCS(stNewEnvir.AnsiCodePage, stOldEnvir.AnsiCodePage, lpFileName);

    UINT Result = BackupGetPrivateProfileIntA(lpAppName, lpKeyName, nDefault, lpszFileName);

    MojibakeFree(lpszFileName);
    return Result;
}

LIBRARY_EXPORT DWORD WINAPI DetourGetPrivateProfileStringA(
    _In_    LPCSTR lpAppName,
    _In_    LPCSTR lpKeyName,
    _In_    LPCSTR lpDefault,
    _Out_   LPSTR lpReturnedString,
    _In_    DWORD nSize,
    _In_    LPCSTR lpFileName
    )
{
    static LPGETPRIVATEPROFILESTRINGA BackupGetPrivateProfileStringA;
    if (!BackupGetPrivateProfileStringA)
    {
        BackupGetPrivateProfileStringA = (LPGETPRIVATEPROFILESTRINGA) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("GetPrivateProfileStringA"));
    }

    LPSTR lpszFileName = MBCSToMBCS(stNewEnvir.AnsiCodePage, stOldEnvir.AnsiCodePage, lpFileName);

    DWORD Result = BackupGetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpszFileName);

    MojibakeFree(lpszFileName);
    return Result;
}

LIBRARY_EXPORT LONG WINAPI DetourRegQueryValueExA(
    _In_        HKEY hKey,
    _In_opt_    LPCSTR lpValueName,
    _Reserved_  LPDWORD lpReserved,
    _Out_opt_   LPDWORD lpType,
    _Out_opt_   LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
    )
{
    LPWSTR lpuszValueName = AnsiToUnicode(lpValueName);
    DWORD cbData = 0;
    LPBYTE lpuData = NULL;
    LPBYTE lpcData = NULL;
    if (lpcbData)
    {
        cbData = *lpcbData;
        *lpcbData = *lpcbData  * sizeof(WCHAR);
        lpuData = (LPBYTE) WaffleAlloc((cbData + 1) * sizeof(lpuData[0]));
        if (!lpuData)
        {
            return ERROR_NOT_ENOUGH_MEMORY;
        }
        lpcData = (LPBYTE) WaffleAlloc((cbData + 1) * sizeof(lpcData[0]));
        if (!lpcData)
        {
            MojibakeFree(lpuData);
            return ERROR_NOT_ENOUGH_MEMORY;
        }
    }

    LONG Result = RegQueryValueEx(hKey, lpuszValueName, lpReserved, lpType, lpuData, lpcbData);

    DWORD LastError = GetLastError();
    if (lpcbData)
    {
        *lpcbData = WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, (LPWSTR) lpuData, -1, (LPSTR) lpcData, cbData * sizeof(CHAR), NULL, NULL);
    }
    if (lpData)
    {
        RtlMoveMemory(lpData, lpcData, cbData*sizeof(lpData[0]));
    }

    MojibakeFree(lpuszValueName);
    MojibakeFree(lpuData);
    MojibakeFree(lpcData);
    SetLastError(LastError);
    if (Result != ERROR_SUCCESS)
    {
        *lpcbData = cbData;
    }
    return Result;
}

LIBRARY_EXPORT BOOL WINAPI DetourGetThreadPreferredUILanguages(
    _In_        DWORD dwFlags,
    _Out_       PULONG pulNumLanguages,
    _Out_opt_   PZZWSTR pwszLanguagesBuffer,
    _Inout_     PULONG pcchLanguagesBuffer
    )
{
    static LPGETTHREADPREFERREDUILANGUAGES BackupGetThreadPreferredUILanguages;
    if (!BackupGetThreadPreferredUILanguages)
    {
        BackupGetThreadPreferredUILanguages = (LPGETTHREADPREFERREDUILANGUAGES) WaffleGetBackupAddress(TEXT("kernel32.dll"), TEXT("GetThreadPreferredUILanguages"));
    }

    BOOL Result = BackupGetThreadPreferredUILanguages(dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
    if (pwszLanguagesBuffer)
    {
        lstrcpyW(&pwszLanguagesBuffer[1 + *pcchLanguagesBuffer], L"ja-JP\0ja\0");
    }
    *pulNumLanguages += 2;
    *pcchLanguagesBuffer += 9;
    if (pwszLanguagesBuffer)
    {
        pwszLanguagesBuffer[1 + *pcchLanguagesBuffer] = L'\0';
    }
    return Result;
}

LIBRARY_EXPORT LANGID WINAPI DetourGetUserDefaultUILanguage(void)
{
    return LANGIDFROMLCID(stNewEnvir.ThreadLocale);
}