#ifndef __KERNEL32_H_
#define __KERNEL32_H_

#include "..\..\mojibake.h"

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

typedef UINT(WINAPI *LPGETACP)(void) ;

typedef LPSTR(WINAPI *LPGETCOMMANDLINEA)(void) ;

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

#endif /* __KERNEL32_H_ */