#define  UNICODE
#include <windows.h>
#include "core.h"
#include "Source\membp.h"

HINSTANCE   hDLL;
HANDLE      hHeap;

ENVIRONMENT_BLOCK   stOldEnvir;
ENVIRONMENT_BLOCK   stNewEnvir;

HGLOBAL lpszCommandLineA;
UINT    ParentTid;

LPVOID  stHookTable[] = {
//lpNewFunction lpOldFunction lpszModule lpszFunction
0,0,0,0,
};

HOOK_TABLE_OBJECT		stGetCPInfo;
HOOK_TABLE_OBJECT		stMultiByteToWideChar;
HOOK_TABLE_OBJECT		stWideCharToMultiByte;
HOOK_TABLE_OBJECT		stSendMessageA;
HOOK_TABLE_OBJECT		stCallWindowProcA;

LPMESSAGEBOXA lpMessageBoxA;
LPWSPRINTFA lpwsprintfA;
HMODULE hDll;
HOOK_TABLE_OBJECT stMessageBoxA = {HookedMessageBoxA,NULL,"MessageBoxA","User32.dll"};

int WINAPI HookedMessageBoxA(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
){

    return lpMessageBoxA(hWnd,"MessageBoxA has been hooked","Dll Copy Example",uType);
}

int WINAPI SetHook(LPVOID stHookTable[])
{
    DWORD flOldProtect;
    PVOID hBreakpoint;
    DWORD DividedZero = 0;

    hDll = CopyLibrary("User32.dll");
    lpMessageBoxA = (LPMESSAGEBOXA)GetProcAddr(hDll,"MessageBoxA");
    lpwsprintfA = (LPWSPRINTFA)GetProcAddr(hDll,"wsprintfA");

    stMessageBoxA.lpOldFunction = GetProcAddress(GetModuleHandle(TEXT("User32.dll")),"MessageBoxA");
    VirtualProtect(stMessageBoxA.lpOldFunction,1,PAGE_READONLY,&flOldProtect);
    hBreakpoint = AddVectoredExceptionHandler(TRUE,(PVECTORED_EXCEPTION_HANDLER)BreakpointHandler);

    lpMessageBoxA(0,"lpMessageBoxA","Dll Copy Example",0);
    MessageBoxA(0,"MessageBoxA","Dll Copy Example",0);

    VirtualProtect(stMessageBoxA.lpOldFunction,1,flOldProtect,&flOldProtect);
    RemoveVectoredExceptionHandler(hBreakpoint);
    return 0;
}