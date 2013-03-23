#define  UNICODE
#include <windows.h>
#include "..\core.h"

HWND WINAPI NewCreateWindowA(
  _In_opt_  LPCSTR lpClassName,
  _In_opt_  LPCSTR lpWindowName,
  _In_      DWORD dwStyle,
  _In_      int x,
  _In_      int y,
  _In_      int nWidth,
  _In_      int nHeight,
  _In_opt_  HWND hWndParent,
  _In_opt_  HMENU hMenu,
  _In_opt_  HINSTANCE hInstance,
  _In_opt_  LPVOID lpParam
){
    LPVOID lpuszClassName = AnsiToUnicode(lpClassName);
    LPVOID lpuszWindowName = AnsiToUnicode(lpWindowName);
    HWND Result = CreateWindow(lpuszClassName,lpuszWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);

    DWORD LastError = GetLastError();
    HeapFree(hHeap,0,lpuszClassName);
    HeapFree(hHeap,0,lpuszWindowName);
    SetLastError(LastError);
    return Result;
}

int WINAPI NewMessageBoxExA(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType,
  _In_      WORD wLanguageId
){
    LPVOID lpuszText = AnsiToUnicode(lpText);
    LPVOID lpuszCaption = AnsiToUnicode(lpCaption);
    int Result = MessageBoxEx(hWnd,lpuszText,lpuszCaption,uType,wLanguageId);

    DWORD LastError = GetLastError();
    HeapFree(hHeap,0,lpuszText);
    HeapFree(hHeap,0,lpuszCaption);
    SetLastError(LastError);
    return Result;
}


BOOL WINAPI NewSetWindowTextA(
  _In_      HWND hWnd,
  _In_opt_  LPCSTR lpString
){
    LPVOID lpuszString = AnsiToUnicode(lpString);
    BOOL Result = SetWindowText(hWnd,lpuszString);

    KeepLastErrorAndFree(lpuszString);
    return Result;
}