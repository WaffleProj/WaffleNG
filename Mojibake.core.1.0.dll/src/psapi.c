#include "..\mojibake.h"
#include <psapi.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifdef __cplusplus
extern "C" {
#endif
    LIBRARY_EXPORT DWORD WINAPI DetourGetModuleFileNameExA(
        _In_        HANDLE hProcess,
        _In_opt_    HMODULE hModule,
        _Out_       LPSTR lpFilename,
        _In_        DWORD nSize
        )
    {
        LPWSTR lpuszFilename = 0;
        if (lpFilename)
            lpuszFilename = (LPWSTR) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 4 * nSize);

        GetModuleFileNameEx(hProcess, hModule, lpuszFilename, nSize);

        DWORD LastError = GetLastError();
        WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszFilename, -1, lpFilename, nSize, NULL, NULL);
        HeapFree(hHeap, 0, lpuszFilename);
        SetLastError(LastError);
        return lstrlenA(lpFilename);
    }
#ifdef __cplusplus
};
#endif