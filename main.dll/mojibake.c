#define  UNICODE
#define _UNICODE
#include "mojibake.h"
#include <shlwapi.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

LPVOID WINAPI AnsiToUnicode(
  _In_  LPCSTR lpszText
){
    if  (lpszText)
    {
        int intText = lstrlenA(lpszText);
        if  (intText)
        {
            intText++;
            LPVOID lpuszText = HeapAlloc(hHeap,HEAP_ZERO_MEMORY,2*intText);
            MultiByteToWideChar(stNewEnvir.ACP,0,lpszText,-1,lpuszText,intText);
            return lpuszText;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

LPVOID WINAPI ProgramCPToWindowsCP(
  _In_  LPCSTR lpszText
){
    if  (lpszText)
    {
        int intText = lstrlenA(lpszText) + 1;
        LPVOID lpuszText = HeapAlloc(hHeap,HEAP_ZERO_MEMORY,2*intText);
        LPVOID lpszNewText = HeapAlloc(hHeap,HEAP_ZERO_MEMORY,2*intText);
        MultiByteToWideChar(stNewEnvir.ACP,0,lpszText,-1,lpuszText,intText);
        WideCharToMultiByte(stOldEnvir.ACP,0,lpuszText,-1,lpszNewText,intText,NULL,FALSE);
        return lpszNewText;
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

LIBRARY_API VOID WINAPI ComponentInit()
{
    TCHAR szEnvirVar[32];
    int ParentTid;
    GetEnvironmentVariable(TEXT("ParentTID"),szEnvirVar,sizeof(szEnvirVar));
    SetEnvironmentVariable(TEXT("ParentTID"),NULL);
    StrToIntEx(szEnvirVar,STIF_SUPPORT_HEX,&ParentTid);
    PostThreadMessage(ParentTid,TM_GETTID,0,(LPARAM)InitLibrary);
}

BOOL WINAPI DllMain(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
){
    if (fdwReason == DLL_PROCESS_ATTACH) 
    {
        DisableThreadLibraryCalls(hinstDLL);
        //Save hinstDLL
        //No. Can not get the correct function address in new library
        //InitLibraryWithTID();
        //ComponentInit();
    }
    return TRUE;
}