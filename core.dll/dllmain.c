#define  UNICODE
#include <windows.h>
#include <shlwapi.h>
#include "core.h"

LPVOID WINAPI AnsiToUnicode(
  _In_  LPCSTR lpszText
){
    if  (lpszText)
    {
        int intText = lstrlenA(lpszText) + 1;
        LPVOID lpuszText = HeapAlloc(hHeap,HEAP_ZERO_MEMORY,2*intText);
        MultiByteToWideChar(NewCodePage,0,lpszText,-1,lpuszText,intText);
        return lpuszText;
    }
    else
       return 0;
}

VOID WINAPI InitLibrary(
  _In_  LPVOID lpParameter
){
    //∂¡»°≥Ã–Ú≈‰÷√
    //CryptCATAdminCalcHashFromFileHandle
    OldCodePage = GetACP();
    NewCodePage = CP_SHIFT_JIS;
    hHeap = HeapCreate(0,0,0);

    LPTSTR lpszCommandLineW = GetCommandLineW();
    int intSize = 4*lstrlenW(lpszCommandLineW);
    lpszCommandLineA = GlobalAlloc(GPTR,intSize);
    WideCharToMultiByte(NewCodePage,0,lpszCommandLineW,-1,lpszCommandLineA,intSize,NULL,NULL);

    _SetHook(&stHookTable);
    
    TCHAR szEnvirVar[32];
    int intParentTID;
    GetEnvironmentVariable(TEXT("ParentTID"),szEnvirVar,sizeof(szEnvirVar));
    SetEnvironmentVariable(TEXT("ParentTID"),NULL);
    StrToIntEx(szEnvirVar,STIF_SUPPORT_HEX,&intParentTID);
    PostThreadMessage(intParentTID,TM_RESUMEMAINTHREAD,0,0);
    return;
}

BOOL WINAPI DllMain(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
){
    switch(fdwReason) 
    { 
        case DLL_PROCESS_ATTACH:
            hDLL = hinstDLL;
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