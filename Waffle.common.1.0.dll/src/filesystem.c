#define  UNICODE
#define  _UNICODE
#include "..\common.h"

LIBRARY_EXPORT void WINAPI WaffleGetModuleDirectory(
    _In_opt_    HMODULE hModule,
    _Out_       LPTSTR lpFilename,
    _In_        DWORD nSize
    )
{
    GetModuleFileName(hModule, lpFilename, nSize);
    int i;
    for (i = lstrlen(lpFilename); lpFilename[i] != '\\'; i--);
    lpFilename[i] = '\0';
}