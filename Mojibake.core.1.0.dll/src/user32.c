#include "..\mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifdef __cplusplus
extern "C" {
#endif
    /*
    LIBRARY_EXPORT ATOM WINAPI DetourRegisterClassA(
    _In_    const WNDCLASSA *lpWndClass
    )
    {
    WNDCLASSW WndClass;
    RtlMoveMemory(&WndClass, lpWndClass, sizeof(WndClass));

    LPWSTR lpszMenuName = AnsiToUnicode(lpWndClass->lpszMenuName);
    LPWSTR lpszClassName = AnsiToUnicode(lpWndClass->lpszClassName);
    WndClass.lpszMenuName = lpszMenuName;
    WndClass.lpszClassName = lpszClassName;

    ATOM Result = RegisterClass(&WndClass);

    DWORD LastError = GetLastError();
    WaffleFree(lpszMenuName);
    WaffleFree(lpszClassName);
    SetLastError(LastError);
    return Result;
    }

    LIBRARY_EXPORT ATOM WINAPI DetourRegisterClassExA(
    _In_    const WNDCLASSEXA *lpWndClass
    )
    {
    WNDCLASSEXW WndClass;
    RtlMoveMemory(&WndClass, lpWndClass, sizeof(WndClass));

    LPWSTR lpszMenuName = AnsiToUnicode(lpWndClass->lpszMenuName);
    LPWSTR lpszClassName = AnsiToUnicode(lpWndClass->lpszClassName);
    WndClass.lpszMenuName = lpszMenuName;
    WndClass.lpszClassName = lpszClassName;

    ATOM Result = RegisterClassEx(&WndClass);

    DWORD LastError = GetLastError();
    WaffleFree(lpszMenuName);
    WaffleFree(lpszClassName);
    SetLastError(LastError);
    return Result;
    }
    */

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
        static LPCREATEWINDOWEXA BackupCreateWindowExA;
        if (!BackupCreateWindowExA)
        {
            BackupCreateWindowExA = (LPCREATEWINDOWEXA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("CreateWindowExA"));
        }

        HWND Result = BackupCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

        DWORD LastError = GetLastError();
        LPWSTR lpuszWindowName = AnsiToUnicode(lpWindowName);
        SetWindowText(Result, lpuszWindowName);
        WaffleFree(lpuszWindowName);
        SetLastError(LastError);
        return Result;
    }

    LIBRARY_EXPORT HWND WINAPI DetourCreateWindowA(
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
        return DetourCreateWindowExA(0, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
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
                WaffleFree(lpuszString);
            }
        default:
            {
                return BackupDefWindowProcA(hWnd, Msg, wParam, lParam);
            }
        }
    }

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
                LPSTR lpszString = (LPSTR) WaffleAlloc(sizeString);
                DefWindowProcA(hWnd, WM_GETTEXT, sizeString, (LPARAM) lpszString);
                LPWSTR lpuszString = AnsiToUnicode(lpszString);
                DefWindowProc(hWnd, WM_SETTEXT, 0, (LPARAM) lpuszString);
                WaffleFree(lpuszString);
                WaffleFree(lpszString);
                SetLastError(LastError);
                return Result;
            }
        default:
            {
                return BackupCallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
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
        LPWSTR lpuTemplateName = (LPWSTR) lpTemplateName;
        if ((SIZE_T) lpTemplateName > 0xFFFF)
        {
            lpuTemplateName = AnsiToUnicode(lpTemplateName);
        }
        LRESULT Result = DialogBoxParam(hInstance, lpuTemplateName, hWndParent, lpDialogFunc, dwInitParam);

        if ((SIZE_T) lpTemplateName > 0xFFFF)
        {
            KeepLastErrorAndFree(lpuTemplateName);
        }
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
        WaffleFree(lpuszText);
        WaffleFree(lpuszCaption);
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
        return DetourMessageBoxExA(hWnd, lpText, lpCaption, uType, LANG_NEUTRAL);
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
                MessageBox(0, TEXT("WM_GETTEXT"), TEXT("DetourSendMessageA"), 0);
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

    LIBRARY_EXPORT BOOL WINAPI DetourExitWindowsEx(
        _In_    UINT uFlags,
        _In_    DWORD dwReason
        )
    {
        static LPEXITWINDOWSEX BackupExitWindowsEx;
        if (!BackupExitWindowsEx)
        {
            BackupExitWindowsEx = (LPEXITWINDOWSEX) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("ExitWindowsEx"));
        }

        int Result = MessageBox(0, TEXT("FIXME:This program called ExitWindowsEx, which may not be what you want it to do.\nAre you sure to execute this function?"), 0, MB_YESNO | MB_ICONWARNING);
        if (Result == IDYES)
        {
            return BackupExitWindowsEx(uFlags, dwReason);
        }
        else
        {
            return FALSE;
        }
    }
#ifdef __cplusplus
};
#endif