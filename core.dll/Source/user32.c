#define  UNICODE
#include <windows.h>
#include "..\core.h"

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