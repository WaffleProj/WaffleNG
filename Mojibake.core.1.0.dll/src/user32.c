#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\mojibake.h"

#ifdef __cplusplus
extern "C" {
#endif
LIBRARY_EXPORT LRESULT WINAPI DetourCallWindowProcA(
    _In_    WNDPROC lpPrevWndFunc,
    _In_    HWND hWnd,
    _In_    UINT Msg,
    _In_    WPARAM wParam,
    _In_    LPARAM lParam
    )
{
    static LPCALLWINDOWPROCA BackupCallWindowProcA;
    if (!BackupCallWindowProcA)
    {
        BackupCallWindowProcA = (LPCALLWINDOWPROCA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("CallWindowProcA"));
    }

    if (Msg == WM_SETTEXT)
    {
        LRESULT Result = BackupCallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam);

        DWORD LastError = GetLastError();
        LRESULT sizeString = DefWindowProcA(hWnd, WM_GETTEXTLENGTH, 0, 0);
        sizeString++;
        LPSTR lpszString = (LPSTR) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeString);
        DefWindowProcA(hWnd, WM_GETTEXT, sizeString, (LPARAM) lpszString);
        LPWSTR lpuszString = AnsiToUnicode(lpszString);
        DefWindowProc(hWnd, WM_SETTEXT, 0, (LPARAM) lpuszString);
        HeapFree(hHeap, 0, lpuszString);
        HeapFree(hHeap, 0, lpszString);
        SetLastError(LastError);
        return Result;
    }
    else
    {
        return BackupCallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
    }
}

LIBRARY_EXPORT HWND WINAPI DetourCreateWindowExA(
    _In_        DWORD dwExStyle,
    _In_opt_    LPCSTR lpClassName,
    _In_opt_    LPCSTR lpWindowName,
    _In_        DWORD dwStyle,
    _In_        int x,
    _In_        int y,
    _In_        int nWidth,
    _In_        int nHeight,
    _In_opt_    HWND hWndParent,
    _In_opt_    HMENU hMenu,
    _In_opt_    HINSTANCE hInstance,
    _In_opt_    LPVOID lpParam
    )
{
    LPWSTR lpuszClassName = AnsiToUnicode(lpClassName);
    LPWSTR lpuszWindowName = AnsiToUnicode(lpWindowName);
    //if  (!lstrcmpA("TFormView",lpClassName))
    //MessageBox(0,lpuszWindowName,TEXT("DetourCreateWindowExA"),0);
    HWND Result = CreateWindowEx(dwExStyle, lpuszClassName, lpuszWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

    DWORD LastError = GetLastError();
    HeapFree(hHeap, 0, lpuszClassName);
    HeapFree(hHeap, 0, lpuszWindowName);
    SetLastError(LastError);
    return Result;
}

LIBRARY_EXPORT int WINAPI DetourMessageBoxA(
    _In_opt_    HWND hWnd,
    _In_opt_    LPCSTR lpText,
    _In_opt_    LPCSTR lpCaption,
    _In_        UINT uType
    )
{
    LPWSTR lpuszText = AnsiToUnicode(lpText);
    LPWSTR lpuszCaption = AnsiToUnicode(lpCaption);
    int Result = MessageBox(hWnd, lpuszText, lpuszCaption, uType);

    DWORD LastError = GetLastError();
    HeapFree(hHeap, 0, lpuszText);
    HeapFree(hHeap, 0, lpuszCaption);
    SetLastError(LastError);
    return Result;
}

LIBRARY_EXPORT int WINAPI DetourMessageBoxExA(
    _In_opt_    HWND hWnd,
    _In_opt_    LPCSTR lpText,
    _In_opt_    LPCSTR lpCaption,
    _In_        UINT uType,
    _In_        WORD wLanguageId
    )
{
    LPWSTR lpuszText = AnsiToUnicode(lpText);
    LPWSTR lpuszCaption = AnsiToUnicode(lpCaption);
    int Result = MessageBoxEx(hWnd, lpuszText, lpuszCaption, uType, wLanguageId);

    DWORD LastError = GetLastError();
    HeapFree(hHeap, 0, lpuszText);
    HeapFree(hHeap, 0, lpuszCaption);
    SetLastError(LastError);
    return Result;
}

LIBRARY_EXPORT LRESULT WINAPI DetourSendMessageA(
    _In_    HWND hWnd,
    _In_    UINT Msg,
    _In_    WPARAM wParam,
    _In_    LPARAM lParam
    )
{
    /*
    if  (Msg == WM_SETTEXT)
    {
    LPVOID lpuszString = AnsiToUnicode((LPVOID)lParam);
    //MessageBox(0,lpuszString,TEXT("DetourSendMessageA"),0);
    LRESULT Result = SendMessage(hWnd,Msg,wParam,(LPARAM)lpuszString);

    KeepLastErrorAndFree(lpuszString);
    return Result;
    }
    else
    {
    */
    static LPSENDMESSAGEA BackupSendMessageA;
    if (!BackupSendMessageA)
    {
        BackupSendMessageA = (LPSENDMESSAGEA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("SendMessageA"));
    }

    return BackupSendMessageA(hWnd, Msg, wParam, lParam);
    //    }
}

LIBRARY_EXPORT BOOL WINAPI DetourSetWindowTextA(
    _In_        HWND hWnd,
    _In_opt_    LPCSTR lpString
    )
{
    LPWSTR lpuszString = AnsiToUnicode(lpString);
    //MessageBox(0,lpuszString,TEXT("DetourSetWindowTextA"),0);
    BOOL Result = SetWindowText(hWnd, lpuszString);

    KeepLastErrorAndFree(lpuszString);
    return Result;
}
#ifdef __cplusplus
};
#endif