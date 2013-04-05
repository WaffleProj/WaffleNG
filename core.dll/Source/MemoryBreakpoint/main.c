#define UNICODE
#include <windows.h>
#include <WinNT.h>
#include "membp.h"

#pragma comment(lib, "user32.lib")

typedef int (WINAPI *LPMESSAGEBOXA)(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
);

int WINAPI NewMessageBoxA(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
);

HMODULE hDll;
HOOK_TABLE_OBJECT stMessageBoxA = {NewMessageBoxA,NULL,"MessageBoxA","User32.dll"};;
LPMESSAGEBOXA lpMessageBoxA;

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
    hDll = CopyLibrary("User32.dll");


    //Call new MessageBoxA
    lpMessageBoxA = (LPMESSAGEBOXA)GetProcAddr(hDll,"MessageBoxA");
    lpMessageBoxA(0,"lpMessageBoxA By Name","Dll Copy Example",0);

    #if UINTPTR_MAX == 0x00000000FFFFFFFF
    lpMessageBoxA = (LPMESSAGEBOXA)GetProcAddr(hDll,(LPVOID)0x07F7);    //Win7 SP1 Ultimate 32
    #elif UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF
    lpMessageBoxA = (LPMESSAGEBOXA)GetProcAddr(hDll,(LPVOID)0x07FB);    //Win7 SP1 Ultimate 64
    #el
    #error "Environment is not 32 or 64-bit."
    #endif
    lpMessageBoxA(0,"lpMessageBoxA By Order","Dll Copy Example",0);


    MessageBoxA(0,"MessageBoxA","Dll Copy Example",0);
    NewMessageBoxA(0,"NewMessageBoxA","Dll Copy Example",0);

    return 0;
}