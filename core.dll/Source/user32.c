#define  UNICODE
#include <windows.h>
#include "..\core.h"

LRESULT WINAPI NewCallWindowProcA(
  _In_  WNDPROC lpPrevWndFunc,
  _In_  HWND hWnd,
  _In_  UINT Msg,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
){
    if  (Msg == WM_SETTEXT)
    {
        LRESULT Result = ((lpCallWindowProcA)stCallWindowProcA.lpOldFunction)(lpPrevWndFunc,hWnd,Msg,wParam,lParam);
    
        DWORD LastError = GetLastError();
        int sizeString = DefWindowProcA(hWnd,WM_GETTEXTLENGTH,0,0);
        sizeString++;
        LPVOID lpszString = HeapAlloc(hHeap,HEAP_ZERO_MEMORY,sizeString);
        DefWindowProcA(hWnd,WM_GETTEXT,sizeString,(LPARAM)lpszString);
        LPVOID lpuszString = AnsiToUnicode(lpszString);
        DefWindowProc(hWnd,WM_SETTEXT,0,(LPARAM)lpuszString);
        HeapFree(hHeap,0,lpuszString);
        HeapFree(hHeap,0,lpszString);
        SetLastError(LastError);
        return Result;
    }
    else
    {
        return ((lpCallWindowProcA)stCallWindowProcA.lpOldFunction)(lpPrevWndFunc,hWnd,Msg,wParam,lParam);
    }    
}

HWND WINAPI NewCreateWindowExA(
  _In_      DWORD dwExStyle,
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
    //if  (!lstrcmpA("TFormView",lpClassName))
        //MessageBox(0,lpuszWindowName,TEXT("NewCreateWindowExA"),0);
    HWND Result = CreateWindowEx(dwExStyle,lpuszClassName,lpuszWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);

    DWORD LastError = GetLastError();
    HeapFree(hHeap,0,lpuszClassName);
    HeapFree(hHeap,0,lpuszWindowName);
    SetLastError(LastError);
    return Result;
}

int WINAPI NewMessageBoxA(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
){
    LPVOID lpuszText = AnsiToUnicode(lpText);
    LPVOID lpuszCaption = AnsiToUnicode(lpCaption);
    int Result = MessageBox(hWnd,lpuszText,lpuszCaption,uType);

    DWORD LastError = GetLastError();
    HeapFree(hHeap,0,lpuszText);
    HeapFree(hHeap,0,lpuszCaption);
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

LRESULT WINAPI NewSendMessageA(
  _In_  HWND hWnd,
  _In_  UINT Msg,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
){
/*
    if  (Msg == WM_SETTEXT)
    {
        LPVOID lpuszString = AnsiToUnicode((LPVOID)lParam);
        //MessageBox(0,lpuszString,TEXT("NewSendMessageA"),0);
        LRESULT Result = SendMessage(hWnd,Msg,wParam,(LPARAM)lpuszString);
    
        KeepLastErrorAndFree(lpuszString);
        return Result;
    }
    else
    {
*/
        return ((lpSendMessageA)stSendMessageA.lpOldFunction)(hWnd,Msg,wParam,lParam);
//    }
}

BOOL WINAPI NewSetWindowTextA(
  _In_      HWND hWnd,
  _In_opt_  LPCSTR lpString
){
    LPVOID lpuszString = AnsiToUnicode(lpString);
    //MessageBox(0,lpuszString,TEXT("NewSetWindowTextA"),0);
    BOOL Result = SetWindowText(hWnd,lpuszString);

    KeepLastErrorAndFree(lpuszString);
    return Result;
}