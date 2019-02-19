#ifndef __SDK_WAFFLE_PSDK_KERNEL32_H_
#define __SDK_WAFFLE_PSDK_KERNEL32_H_

typedef HRSRC(WINAPI *LPFINDRESOURCEEXA)(
    _In_opt_  HMODULE hModule,
    _In_      LPCSTR lpType,
    _In_      LPCSTR lpName,
    _In_      WORD wLanguage
    );

typedef HRSRC(WINAPI *LPFINDRESOURCEEXW)(
    _In_opt_  HMODULE hModule,
    _In_      LPCWSTR lpType,
    _In_      LPCWSTR lpName,
    _In_      WORD wLanguage
    );

typedef HGLOBAL(WINAPI *LPLOADRESOURCE)(
    _In_opt_  HMODULE hModule,
    _In_      HRSRC hResInfo
    );

typedef DWORD(WINAPI* LPSIZEOFRESOURCE)(
    _In_opt_  HMODULE hModule,
    _In_      HRSRC hResInfo
    );

typedef BOOL(WINAPI *LPCREATEPROCESSA)(
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
    );

typedef BOOL(WINAPI *LPCREATEPROCESSW)(
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
    );

typedef BOOL(WINAPI *LPCREATEDIRECTORYA)(
    _In_        LPCSTR lpPathName,
    _In_opt_    LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );

typedef HANDLE(WINAPI *LPCREATEFILEA)(
    _In_        LPCSTR lpFileName,
    _In_        DWORD dwDesiredAccess,
    _In_        DWORD dwShareMode,
    _In_opt_    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_        DWORD dwCreationDisposition,
    _In_        DWORD dwFlagsAndAttributes,
    _In_opt_    HANDLE hTemplateFile
    );

typedef BOOL(WINAPI *LPDELETEFILEA)(
    _In_    LPCSTR lpFileName
    );

typedef HANDLE(WINAPI *LPFINDFIRSTFILEA)(
    _In_    LPCSTR lpFileName,
    _Out_   LPWIN32_FIND_DATAA lpFindFileData
    );

typedef BOOL(WINAPI *LPFINDNEXTFILEA)(
    _In_    HANDLE hFindFile,
    _Out_   LPWIN32_FIND_DATAA lpFindFileData
    );

typedef UINT(WINAPI *LPGETACP)(void);

typedef LPSTR(WINAPI *LPGETCOMMANDLINEA)(void);

typedef BOOL(WINAPI *LPGETCPINFO)(
    _In_    UINT CodePage,
    _Out_   LPCPINFO lpCPInfo
    );

typedef DWORD(WINAPI *LPGETFILEATTRIBUTESA)(
    _In_    LPCSTR lpFileName
    );

typedef DWORD(WINAPI *LPGETMODULEFILENAMEA)(
    _In_opt_    HMODULE hModule,
    _Out_       LPSTR lpFilename,
    _In_        DWORD nSize
    );

typedef HMODULE(WINAPI *LPGETMODULEHANDLEA)(
    _In_opt_    LPCSTR lpModuleName
    );

typedef UINT(WINAPI *LPGETOEMCP)(void);

typedef HMODULE(WINAPI *LPLOADLIBRARYA)(
    _In_    LPCSTR lpFileName
    );

typedef HMODULE(WINAPI *LPLOADLIBRARYEXA)(
    _In_        LPCSTR lpFileName,
    _Reserved_  HANDLE hFile,
    _In_        DWORD dwFlags
    );

typedef int (WINAPI *LPMULTIBYTETOWIDECHAR)(
    _In_        UINT CodePage,
    _In_        DWORD dwFlags,
    _In_        LPCSTR lpMultiByteStr,
    _In_        int cbMultiByte,
    _Out_opt_   LPWSTR lpWideCharStr,
    _In_        int cchWideChar
    );

typedef BOOL(WINAPI *LPSETCURRENTDIRECTORYA)(
    _In_    LPCSTR lpPathName
    );

typedef BOOL(WINAPI *LPSETFILEATTRIBUTESA)(
    _In_    LPCSTR lpFileName,
    _In_    DWORD dwFileAttributes
    );

typedef int (WINAPI *LPWIDECHARTOMULTIBYTE)(
    _In_        UINT CodePage,
    _In_        DWORD dwFlags,
    _In_        LPCWSTR lpWideCharStr,
    _In_        int cchWideChar,
    _Out_opt_   LPSTR lpMultiByteStr,
    _In_        int cbMultiByte,
    _In_opt_    LPCSTR lpDefaultChar,
    _Out_opt_   LPBOOL lpUsedDefaultChar
    );

typedef BOOL(WINAPI *LPVIRTUALPROTECT)(
    _In_    LPVOID lpAddress,
    _In_    SIZE_T dwSize,
    _In_    DWORD flNewProtect,
    _Out_   PDWORD lpflOldProtect
    );

typedef BOOL(WINAPI *LPWOW64DISABLEWOW64FSREDIRECTION)(
    _Out_   PVOID *OldValue
    );

typedef BOOL(WINAPI *LPWOW64REVERTWOW64FSREDIRECTION)(
    _In_    PVOID OldValue
    );

typedef int (WINAPI *LPCOMPARESTRINGA)(
    _In_    LCID Locale,
    _In_    DWORD dwCmpFlags,
    _In_    LPCSTR lpString1,
    _In_    int cchCount1,
    _In_    LPCSTR lpString2,
    _In_    int cchCount2
    );

typedef int (WINAPI *LPCOMPARESTRINGW)(
    _In_    LCID Locale,
    _In_    DWORD dwCmpFlags,
    _In_    LPCWSTR lpString1,
    _In_    int cchCount1,
    _In_    LPCWSTR lpString2,
    _In_    int cchCount2
    );

typedef BOOL(WINAPI *LPISDBCSLEADBYTEEX)(
    _In_    UINT CodePage,
    _In_    BYTE TestChar
    );

typedef void (WINAPI *LPRAISEEXCEPTION)(
    _In_    DWORD dwExceptionCode,
    _In_    DWORD dwExceptionFlags,
    _In_    DWORD nNumberOfArguments,
    _In_    const ULONG_PTR *lpArguments
    );

typedef UINT(WINAPI *LPGETPRIVATEPROFILEINTA)(
    _In_    LPCSTR lpAppName,
    _In_    LPCSTR lpKeyName,
    _In_    INT nDefault,
    _In_    LPCSTR lpFileName
    );

typedef DWORD(WINAPI *LPGETPRIVATEPROFILESTRINGA)(
    _In_    LPCSTR lpAppName,
    _In_    LPCSTR lpKeyName,
    _In_    LPCSTR lpDefault,
    _Out_   LPSTR lpReturnedString,
    _In_    DWORD nSize,
    _In_    LPCSTR lpFileName
    );
    
typedef BOOL(WINAPI *LPGETTHREADPREFERREDUILANGUAGES)(
    _In_        DWORD dwFlags,
    _Out_       PULONG pulNumLanguages,
    _Out_opt_   PZZWSTR pwszLanguagesBuffer,
    _Inout_     PULONG pcchLanguagesBuffer
    );
    
#endif /* __SDK_WAFFLE_PSDK_KERNEL32_H_ */