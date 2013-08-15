#ifndef __RTL_H_
#define __RTL_H_

#ifdef __cplusplus
extern "C" {
#endif

    LPWSTR WINAPI MBCSToUnicode(
        _In_    UINT AnsiCodePage,
        _In_    LPCSTR lpszText
        );

    LPSTR WINAPI MBCSToMBCS(
        _In_    UINT FromCodePage,
        _In_    UINT ToCodePage,
        _In_    LPCSTR lpszText
        );

    LPWSTR WINAPI AnsiToUnicode(
        _In_    LPCSTR lpszText
        );

    LPSTR WINAPI ProgramCPToWindowsCP(
        _In_    LPCSTR lpszText
        );

    VOID WINAPI MojibakeFree(
        _In_    LPVOID lpMem
        );

#ifdef __cplusplus
};
#endif

#endif /* __RTL_H_ */