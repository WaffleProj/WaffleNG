#ifndef __RTL_H_
#define __RTL_H_

LPWSTR WINAPI MBCSToUnicode(
    _In_        UINT AnsiCodePage,
    _In_opt_    LPCSTR lpszText
    );

LPSTR WINAPI MBCSToMBCS(
    _In_        UINT FromCodePage,
    _In_        UINT ToCodePage,
    _In_opt_    LPCSTR lpszText
    );

LPWSTR WINAPI AnsiToUnicode(
    _In_opt_    LPCSTR lpszText
    );

LPSTR WINAPI UnicodeToAnsi(
    _In_opt_    LPCWSTR lpuszText
    );

LPSTR WINAPI ProgramCPToWindowsCP(
    _In_opt_    LPCSTR lpszText
    );

_Ret_maybenull_
_Success_(return == 0)
LPVOID WINAPI MojibakeFree(
_In_opt_    LPVOID lpMem
);

#endif /* __RTL_H_ */