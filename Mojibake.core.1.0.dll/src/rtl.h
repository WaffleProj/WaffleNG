#ifndef __RTL_H_
#define __RTL_H_

#ifdef __cplusplus
extern "C" {
#endif

    LPWSTR WINAPI AnsiToUnicode(
        _In_    LPCSTR lpszText
        );

    LPSTR WINAPI ProgramCPToWindowsCP(
        _In_    LPCSTR lpszText
        );

    VOID WINAPI KeepLastErrorAndFree(
        _In_    LPVOID lpMem
        );

#ifdef __cplusplus
};
#endif

#endif /* __RTL_H_ */