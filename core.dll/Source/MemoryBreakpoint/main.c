#define UNICODE
#include <windows.h>
#include <WinNT.h>
#include "membp.h"

#pragma comment(lib, "user32.lib")

LPMESSAGEBOXA lpMessageBoxA;
LPWSPRINTFA lpwsprintfA;
HMODULE hDll;
HOOK_TABLE_OBJECT stMessageBoxA = {NewMessageBoxA,NULL,"MessageBoxA","User32.dll"};

int WINAPI NewMessageBoxA(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
){

    return lpMessageBoxA(hWnd,"MessageBoxA has been hooked","Dll Copy Example",uType);
}

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
){
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