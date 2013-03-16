#define  UNICODE
#include <windows.h>
#include <shlwapi.h>
#include "core.h"

VOID WINAPI InitLibrary(LPVOID lpParameter)
{
    //∂¡»°≥Ã–Ú≈‰÷√
    //CryptCATAdminCalcHashFromFileHandle

    LPTSTR lpszCommandLineW = GetCommandLineW();
    int intSize = 4*lstrlenW(lpszCommandLineW);
    lpszCommandLineA = GlobalAlloc(GPTR,intSize);
    WideCharToMultiByte(CP_SHIFT_JIS,0,lpszCommandLineW,-1,lpszCommandLineA,intSize,NULL,NULL);

    //SetHook()
    
    TCHAR szEnvirVar[32];
    int intParentTID;
    GetEnvironmentVariable(TEXT("ParentTID"),szEnvirVar,sizeof(szEnvirVar));
    SetEnvironmentVariable(TEXT("ParentTID"),NULL);
    StrToIntEx(szEnvirVar,STIF_SUPPORT_HEX,&intParentTID);
    PostThreadMessage(intParentTID,TM_RESUMEMAINTHREAD,0,0);
    return;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
    switch(fdwReason) 
    { 
        case DLL_PROCESS_ATTACH:
            hInstance = hinstDLL;
            HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)InitLibrary,NULL,0,NULL);
            CloseHandle(hThread);
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}