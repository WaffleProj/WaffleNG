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

        switch (Msg)
        {
        case WM_SETTEXT:
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
        default:
            {
                return BackupCallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
            }
        }
    }

    HWND WINAPI DetourCreateWindowA(
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
        //There doesn't exist a function called CreateWindowA
        //See http://msdn.microsoft.com/en-us/library/windows/desktop/ms632679.aspx
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
        HWND Result = CreateWindowEx(dwExStyle, lpuszClassName, lpuszWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

        DWORD LastError = GetLastError();
        HeapFree(hHeap, 0, lpuszClassName);
        HeapFree(hHeap, 0, lpuszWindowName);
        SetLastError(LastError);
        return Result;
    }

    LIBRARY_EXPORT LRESULT WINAPI DetourDefWindowProcA(
        _In_    HWND hWnd,
        _In_    UINT Msg,
        _In_    WPARAM wParam,
        _In_    LPARAM lParam
        )
    {
        static LPDEFWINDOWPROCA BackupDefWindowProcA;
        if (!BackupDefWindowProcA)
        {
            BackupDefWindowProcA = (LPDEFWINDOWPROCA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("DefWindowProcA"));
        }

        switch (Msg)
        {
        case WM_SETTEXT:
            {
                LPWSTR lpuszString = AnsiToUnicode((LPCSTR) lParam);
                MessageBox(0, lpuszString, L"DetourDefWindowProcA", 0);
                HeapFree(hHeap, 0, lpuszString);
            }
        default:
            {
                return BackupDefWindowProcA(hWnd, Msg, wParam, lParam);
            }
        }
    }

    LIBRARY_EXPORT LRESULT WINAPI DetourDialogBoxParamA(
        _In_opt_    HINSTANCE hInstance,
        _In_        LPCSTR lpTemplateName,
        _In_opt_    HWND hWndParent,
        _In_opt_    DLGPROC lpDialogFunc,
        _In_        LPARAM dwInitParam
        )
    {
        LPWSTR lpuTemplateName = AnsiToUnicode(lpTemplateName);
        LRESULT Result = DialogBoxParam(hInstance, lpuTemplateName, hWndParent, lpDialogFunc, dwInitParam);

        KeepLastErrorAndFree(lpuTemplateName);
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

    LIBRARY_EXPORT ATOM WINAPI DetourRegisterClassA(
        _In_    const WNDCLASSA *lpWndClass
        )
    {
        static LPREGISTERCLASSA BackupRegisterClassA;
        if (!BackupRegisterClassA)
        {
            MessageBox(0, L"RegisterClassA is incomplete.", 0, 0);
            BackupRegisterClassA = (LPREGISTERCLASSA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("RegisterClassA"));
        }
        return BackupRegisterClassA(lpWndClass);
    }

    LIBRARY_EXPORT ATOM WINAPI DetourRegisterClassExA(
        _In_    const WNDCLASSEXA *lpWndClass
        )
    {
        //FIXME: ugly ugly ugly
        static LPREGISTERCLASSEXA BackupRegisterClassExA;
        if (!BackupRegisterClassExA)
        {
            BackupRegisterClassExA = (LPREGISTERCLASSEXA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("RegisterClassExA"));
        }

        WNDCLASSEXA WndClass;
        RtlMoveMemory(&WndClass, lpWndClass, sizeof(WNDCLASSEX));

        LPWSTR lpszMenuName = AnsiToUnicode(lpWndClass->lpszMenuName);
        LPSTR szMenuName = (LPSTR) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 2 * lstrlen(lpszMenuName));
        WideCharToMultiByte(stOldEnvir.ACP, 0, lpszMenuName, -1, szMenuName, 2 * lstrlen(lpszMenuName), NULL, FALSE);
        WndClass.lpszMenuName = szMenuName;

        LPWSTR lpszClassName = AnsiToUnicode(lpWndClass->lpszClassName);
        LPSTR szClassName = (LPSTR) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 2 * lstrlen(lpszClassName));
        WideCharToMultiByte(stOldEnvir.ACP, 0, lpszClassName, -1, szClassName, 2 * lstrlen(lpszClassName), NULL, FALSE);
        WndClass.lpszClassName = szClassName;

        ATOM Result = BackupRegisterClassExA(&WndClass);

        DWORD LastError = GetLastError();
        HeapFree(hHeap, 0, (LPVOID) lpszMenuName);
        HeapFree(hHeap, 0, (LPVOID) lpszClassName);
        HeapFree(hHeap, 0, (LPVOID) szMenuName);
        HeapFree(hHeap, 0, (LPVOID) szClassName);
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
        LPWSTR lpuszString;
        LRESULT Result;
        switch (Msg)
        {
        case WM_SETTEXT:
            {
                lpuszString = AnsiToUnicode((LPCSTR) lParam);
                Result = SendMessage(hWnd, Msg, wParam, (LPARAM) lpuszString);
                break;
            }
        case WM_GETTEXT:
            {
                MessageBoxW(0, L"WM_GETTEXT", L"DetourSendMessageA", 0);
            }
        default:
            {
                static LPSENDMESSAGEA BackupSendMessageA;
                if (!BackupSendMessageA)
                {
                    BackupSendMessageA = (LPSENDMESSAGEA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("SendMessageA"));
                }

                return BackupSendMessageA(hWnd, Msg, wParam, lParam);
            }
        }

        KeepLastErrorAndFree(lpuszString);
        return Result;
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