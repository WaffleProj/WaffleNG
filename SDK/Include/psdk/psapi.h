#ifndef __SDK_WAFFLE_PSDK_PSAPI_H_
#define __SDK_WAFFLE_PSDK_PSAPI_H_

#include <psapi.h>

typedef DWORD(WINAPI *LPGETMODULEFILENAMEEXA)(
    _In_        HANDLE hProcess,
    _In_opt_    HMODULE hModule,
    _Out_       LPSTR lpFilename,
    _In_        DWORD nSize
    );

#endif /* __SDK_WAFFLE_PSDK_PSAPI_H_ */