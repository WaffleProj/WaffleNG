#define  UNICODE
#include <windows.h>
#include <shlwapi.h>
#include "core.h"

HINSTANCE	hDLL;

HANDLE		hHeap;
UINT		NewACP;
UINT		OldACP;
UINT		NewOEMCP;
UINT		OldOEMCP;
LCID		NewLocale;
LCID		OldLocale;

HGLOBAL		lpszCommandLineA;
UINT            ParentTid;

LPVOID WINAPI AnsiToUnicode(
  _In_  LPCSTR lpszText
){
    if  (lpszText)
    {
        int intText = lstrlenA(lpszText) + 1;
        LPVOID lpuszText = HeapAlloc(hHeap,HEAP_ZERO_MEMORY,2*intText);
        MultiByteToWideChar(NewACP,0,lpszText,-1,lpuszText,intText);
        return lpuszText;
    }
    else
       return 0;
}

VOID WINAPI KeepLastErrorAndFree(
  _In_  LPVOID lpMem
){
    DWORD LastError = GetLastError();
    HeapFree(hHeap,0,lpMem);
    SetLastError(LastError);
   return;
}

DWORD WINAPI SetThreadEnvironment()
{
    SetThreadLocale(NewLocale);
    MessageBox(0,0,0,0);
    PostThreadMessage(ParentTid,TM_RESUMETMAINIP,0,0);
    while (TRUE);
    return 0;
}

DWORD WINAPI InitLibrary(
  _In_  LPVOID lpParameter
){
    //∂¡»°≥Ã–Ú≈‰÷√
    //CryptCATAdminCalcHashFromFileHandle
    OldACP = GetACP();
    NewACP = CP_SHIFT_JIS;
    OldOEMCP = GetOEMCP();
    NewOEMCP = CP_SHIFT_JIS;
    OldLocale = GetThreadLocale();
    NewLocale = LOCALE_JA_JP;
    hHeap = HeapCreate(0,0,0);
    
    TCHAR szEnvirVar[32];
    GetEnvironmentVariable(TEXT("ParentTID"),szEnvirVar,sizeof(szEnvirVar));
    SetEnvironmentVariable(TEXT("ParentTID"),NULL);
    StrToIntEx(szEnvirVar,STIF_SUPPORT_HEX,&ParentTid);

    LPTSTR lpszCommandLineW = GetCommandLineW();
    int intSize = 4*lstrlenW(lpszCommandLineW);
    lpszCommandLineA = GlobalAlloc(GPTR,intSize);
    WideCharToMultiByte(NewACP,0,lpszCommandLineW,-1,lpszCommandLineA,intSize,NULL,NULL);

    _SetHook(&stHookTable);

    PostThreadMessage(ParentTid,TM_SETMAINIP,0,(LPARAM)SetThreadEnvironment);
    return 0;
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