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

    MojibakeFree(lpszMenuName);
    MojibakeFree(lpszClassName);
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

    MojibakeFree(lpszMenuName);
    MojibakeFree(lpszClassName);
    return Result;
    }
    */

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

        LRESULT Result = 0;
        switch (Msg)
        {
            /*
            case WM_SETTEXT:
            {
            LPWSTR lpuszString = AnsiToUnicode((LPCSTR) lParam);
            Result = DefWindowProc(hWnd, Msg, wParam, (LPARAM) lpuszString);
            MojibakeFree(lpuszString);
            break;
            }
            */
        case WM_GETTEXTLENGTH:
        {
            LRESULT nText = DefWindowProc(hWnd, Msg, wParam, lParam) + 1;
            DWORD LastError = GetLastError();

            LPWSTR lpuszText = (LPWSTR) WaffleAlloc(nText * sizeof(WCHAR));
            DefWindowProc(hWnd, WM_GETTEXT, nText, (LPARAM) lpuszText);
            Result = WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszText, -1, NULL, 0, NULL, NULL) - 1;
            MojibakeFree(lpuszText);
            SetLastError(LastError);
            break;
        }
        case WM_GETTEXT:
        {
            LPWSTR lpuszText = (LPWSTR) WaffleAlloc(wParam * sizeof(WCHAR));
            DefWindowProc(hWnd, Msg, wParam, (LPARAM) lpuszText);
            DWORD LastError = GetLastError();
            Result = WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszText, -1, (LPSTR) lParam, (unsigned int) wParam * sizeof(CHAR), NULL, NULL) - 1;
            MojibakeFree(lpuszText);
            SetLastError(LastError);
            break;
        }
        default:
        {
            return BackupDefWindowProcA(hWnd, Msg, wParam, lParam);
        }
        }

        return Result;
    }

    LIBRARY_EXPORT LRESULT WINAPI DetourDefDlgProcA(
        _In_    HWND hDlg,
        _In_    UINT Msg,
        _In_    WPARAM wParam,
        _In_    LPARAM lParam
        )
    {
        static LPDEFDLGPROCA BackupDefDlgProcA;
        if (!BackupDefDlgProcA)
        {
            BackupDefDlgProcA = (LPDEFDLGPROCA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("DefDlgProcA"));
        }

        LRESULT Result = 0;
        switch (Msg)
        {
            /*
            case WM_SETTEXT:
            {
            LPWSTR lpuszString = AnsiToUnicode((LPCSTR) lParam);
            Result = DefDlgProc(hDlg, Msg, wParam, (LPARAM) lpuszString);
            MojibakeFree(lpuszString);
            break;
            }
            */
        case WM_GETTEXTLENGTH:
        {
            LRESULT nText = DefWindowProc(hDlg, Msg, wParam, lParam) + 1;
            DWORD LastError = GetLastError();

            LPWSTR lpuszText = (LPWSTR) WaffleAlloc(nText * sizeof(WCHAR));
            DefDlgProc(hDlg, WM_GETTEXT, nText, (LPARAM) lpuszText);
            Result = WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszText, -1, NULL, 0, NULL, NULL) - 1;
            MojibakeFree(lpuszText);
            SetLastError(LastError);
            break;
        }
        case WM_GETTEXT:
        {
            LPWSTR lpuszText = (LPWSTR) WaffleAlloc(wParam * sizeof(WCHAR));
            DefDlgProc(hDlg, Msg, wParam, (LPARAM) lpuszText);
            DWORD LastError = GetLastError();
            Result = WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszText, -1, (LPSTR) lParam, (unsigned int) wParam * sizeof(CHAR), NULL, NULL) - 1;
            MojibakeFree(lpuszText);
            SetLastError(LastError);
            break;
        }
        default:
        {
            return BackupDefDlgProcA(hDlg, Msg, wParam, lParam);
        }
        }

        return Result;
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
        static LPCREATEWINDOWEXA BackupCreateWindowExA;
        if (!BackupCreateWindowExA)
        {
            BackupCreateWindowExA = (LPCREATEWINDOWEXA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("CreateWindowExA"));
        }

        HWND Result = BackupCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

        DWORD LastError = GetLastError();
        LPWSTR lpuszWindowName = AnsiToUnicode(lpWindowName);
        DefWindowProc(Result, WM_SETTEXT, 0, (LPARAM) lpuszWindowName);
        MojibakeFree(lpuszWindowName);
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

    LIBRARY_EXPORT HWND WINAPI DetourCreateDialogA(
        _In_opt_    HINSTANCE hInstance,
        _In_        LPCSTR lpTemplate,
        _In_opt_    HWND hWndParent,
        _In_opt_    DLGPROC lpDialogFunc
        )
    {
        LPWSTR lpuTemplate = (LPWSTR) lpTemplate;
        if ((SIZE_T) lpTemplate > 0xFFFF)
        {
            lpuTemplate = AnsiToUnicode(lpTemplate);
        }
        HWND Result = CreateDialog(hInstance, lpuTemplate, hWndParent, lpDialogFunc);

        if ((SIZE_T) lpTemplate > 0xFFFF)
        {
            MojibakeFree(lpuTemplate);
        }
        return Result;
    }

    LIBRARY_EXPORT HWND WINAPI DetourCreateDialogIndirectA(
        _In_opt_    HINSTANCE hInstance,
        _In_        LPCDLGTEMPLATE lpTemplate,
        _In_opt_    HWND hWndParent,
        _In_opt_    DLGPROC lpDialogFunc
        )
    {
        return CreateDialogIndirect(hInstance, lpTemplate, hWndParent, lpDialogFunc);
    }

    LIBRARY_EXPORT HWND WINAPI DetourCreateDialogIndirectParamA(
        _In_opt_    HINSTANCE hInstance,
        _In_        LPCDLGTEMPLATE lpTemplate,
        _In_opt_    HWND hWndParent,
        _In_opt_    DLGPROC lpDialogFunc,
        _In_        LPARAM lParamInit
        )
    {
        return CreateDialogIndirectParam(hInstance, lpTemplate, hWndParent, lpDialogFunc, lParamInit);
    }

    LIBRARY_EXPORT HWND WINAPI DetourCreateDialogParamA(
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
        HWND Result = CreateDialogParam(hInstance, lpuTemplateName, hWndParent, lpDialogFunc, dwInitParam);

        if ((SIZE_T) lpTemplateName > 0xFFFF)
        {
            MojibakeFree(lpuTemplateName);
        }
        return Result;
    }

    LIBRARY_EXPORT LRESULT WINAPI DetourDialogBoxA(
        _In_opt_    HINSTANCE hInstance,
        _In_        LPCSTR lpTemplate,
        _In_opt_    HWND hWndParent,
        _In_opt_    DLGPROC lpDialogFunc
        )
    {
        LPWSTR lpuTemplate = (LPWSTR) lpTemplate;
        if ((SIZE_T) lpTemplate > 0xFFFF)
        {
            lpuTemplate = AnsiToUnicode(lpTemplate);
        }
        LRESULT Result = DialogBox(hInstance, lpuTemplate, hWndParent, lpDialogFunc);

        if ((SIZE_T) lpTemplate > 0xFFFF)
        {
            MojibakeFree(lpuTemplate);
        }
        return Result;
    }

    LIBRARY_EXPORT INT_PTR WINAPI DetourDialogBoxIndirectA(
        _In_opt_    HINSTANCE hInstance,
        _In_        LPCDLGTEMPLATE lpTemplate,
        _In_opt_    HWND hWndParent,
        _In_opt_    DLGPROC lpDialogFunc
        )
    {
        return DialogBoxIndirect(hInstance, lpTemplate, hWndParent, lpDialogFunc);
    }

    LIBRARY_EXPORT INT_PTR WINAPI DetourDialogBoxIndirectParamA(
        _In_opt_    HINSTANCE hInstance,
        _In_        LPCDLGTEMPLATE lpTemplate,
        _In_opt_    HWND hWndParent,
        _In_opt_    DLGPROC lpDialogFunc,
        _In_        LPARAM lParamInit
        )
    {
        return DialogBoxIndirectParam(hInstance, lpTemplate, hWndParent, lpDialogFunc, lParamInit);
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
            MojibakeFree(lpuTemplateName);
        }
        return Result;
    }

    LIBRARY_EXPORT UINT WINAPI DetourGetDlgItemTextA(
        _In_    HWND hDlg,
        _In_    int nIDDlgItem,
        _Out_   LPSTR lpString,
        _In_    int nMaxCount
        )
    {
        LPWSTR lpuszString = (LPWSTR) WaffleAlloc(nMaxCount * sizeof(WCHAR));
        UINT Result = GetDlgItemText(hDlg, nIDDlgItem, lpuszString, nMaxCount);

        DWORD LastError = GetLastError();
        WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszString, -1, lpString, nMaxCount, NULL, FALSE);
        MojibakeFree(lpuszString);
        SetLastError(LastError);
        return Result;
    }

    LIBRARY_EXPORT BOOL WINAPI DetourSetDlgItemTextA(
        _In_    HWND hDlg,
        _In_    int nIDDlgItem,
        _Out_   LPSTR lpString
        )
    {
        LPWSTR lpuszString = AnsiToUnicode(lpString);
        BOOL Result = SetDlgItemText(hDlg, nIDDlgItem, lpuszString);

        MojibakeFree(lpuszString);
        return Result;
    }

    LIBRARY_EXPORT HMENU WINAPI DetourLoadMenuA(
        _In_opt_    HINSTANCE hInstance,
        _In_        LPCSTR lpMenuName
        )
    {
        LPWSTR lpuMenuName = (LPWSTR) lpMenuName;
        if ((SIZE_T) lpMenuName > 0xFFFF)
        {
            lpuMenuName = AnsiToUnicode(lpMenuName);
        }

        HMENU Result = LoadMenu(hInstance, lpuMenuName);

        if ((SIZE_T) lpMenuName > 0xFFFF)
        {
            MojibakeFree(lpuMenuName);
        }
        return Result;
    }

    LIBRARY_EXPORT BOOL WINAPI DetourAppendMenuA(
        _In_        HMENU hMenu,
        _In_        UINT uFlags,
        _In_        UINT_PTR uIDNewItem,
        _In_opt_    LPCSTR lpNewItem
        )
    {
        LPWSTR lpuszNewItem = AnsiToUnicode(lpNewItem);
        BOOL Result = AppendMenu(hMenu, uFlags, uIDNewItem, lpuszNewItem);

        MojibakeFree(lpuszNewItem);
        return Result;
    }

    LIBRARY_EXPORT BOOL WINAPI DetourInsertMenuA(
        _In_        HMENU hMenu,
        _In_        UINT uPosition,
        _In_        UINT uFlags,
        _In_        UINT_PTR uIDNewItem,
        _In_opt_    LPCSTR lpNewItem
        )
    {
        LPWSTR lpuszNewItem = AnsiToUnicode(lpNewItem);
        BOOL Result = InsertMenu(hMenu, uPosition, uFlags, uIDNewItem, lpuszNewItem);

        MojibakeFree(lpuszNewItem);
        return Result;
    }

    LIBRARY_EXPORT BOOL WINAPI DetourInsertMenuItemA(
        _In_    HMENU hMenu,
        _In_    UINT uItem,
        _In_    BOOL fByPosition,
        _In_    LPCMENUITEMINFOA lpmii
        )
    {
        MENUITEMINFOW mii;
        RtlMoveMemory(&mii, lpmii, sizeof(mii));
        mii.dwTypeData = AnsiToUnicode(lpmii->dwTypeData);
        mii.cch = lstrlen(mii.dwTypeData);
        BOOL Result = InsertMenuItem(hMenu, uItem, fByPosition, &mii);

        MojibakeFree(mii.dwTypeData);
        return Result;
    }

    LIBRARY_EXPORT BOOL WINAPI DetourGetMenuItemInfoA(
        _In_        HMENU hMenu,
        _In_        UINT uItem,
        _In_        BOOL fByPosition,
        _Inout_     LPMENUITEMINFOA lpmii
        )
    {
        MENUITEMINFOW mii;
        RtlMoveMemory(&mii, lpmii, sizeof(MENUITEMINFOW));
        if (lpmii->dwTypeData)
        {
            mii.dwTypeData = (LPWSTR) WaffleAlloc(lpmii->cch * sizeof(WCHAR));
        }

        BOOL Result = GetMenuItemInfo(hMenu, uItem, fByPosition, &mii);

        DWORD LastError = GetLastError();
        LPSTR lpTypeData = NULL;
        if (lpmii->dwTypeData)
        {
            lpTypeData = lpmii->dwTypeData;
            WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, mii.dwTypeData, -1, lpTypeData, lpmii->cch * sizeof(CHAR), NULL, FALSE);
            MojibakeFree(mii.dwTypeData);
        }
        RtlMoveMemory(lpmii, &mii, sizeof(MENUITEMINFOA));
        lpmii->dwTypeData = lpTypeData;
        SetLastError(LastError);
        return Result;
    }

    LIBRARY_EXPORT BOOL WINAPI DetourSetMenuItemInfoA(
        _In_        HMENU hMenu,
        _In_        UINT uItem,
        _In_        BOOL fByPosition,
        _Inout_     LPMENUITEMINFOA lpmii
        )
    {
        MENUITEMINFOW mii;
        RtlMoveMemory(&mii, lpmii, sizeof(mii));
        mii.dwTypeData = AnsiToUnicode(lpmii->dwTypeData);
        mii.cch = lstrlen(mii.dwTypeData);
        BOOL Result = SetMenuItemInfo(hMenu, uItem, fByPosition, &mii);

        MojibakeFree(mii.dwTypeData);
        return Result;
    }

    LIBRARY_EXPORT int WINAPI DetourLoadStringA(
        _In_opt_    HINSTANCE hInstance,
        _In_        UINT uID,
        _Out_       LPSTR lpBuffer,
        _In_        int nBufferMax
        )
    {
        static LPLOADSTRINGA BackupLoadStringA;
        if (!BackupLoadStringA)
        {
            BackupLoadStringA = (LPLOADSTRINGA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("LoadStringA"));
        }

        if (!nBufferMax)
        {
            MessageBox(0, TEXT("DetourLoadStringA with zero nBufferMax"), 0, 0);
            return BackupLoadStringA(hInstance, uID, lpBuffer, nBufferMax);
        }

        LPWSTR lpuBuffer = (LPWSTR) WaffleAlloc(nBufferMax * sizeof(WCHAR));
        int Result = LoadString(hInstance, uID, lpuBuffer, nBufferMax);
        DWORD LastError = GetLastError();
        WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuBuffer, -1, lpBuffer, nBufferMax * sizeof(CHAR), NULL, FALSE);
        MojibakeFree(lpuBuffer);
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

        MojibakeFree(lpuszText);
        MojibakeFree(lpuszCaption);
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

        LRESULT Result = 0;
        switch (Msg)
        {
        case WM_SETTEXT:
        {
            /*
            Result = BackupCallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
            LPWSTR lpuszString = AnsiToUnicode((LPCSTR) lParam);
            Result = CallWindowProc(lpPrevWndFunc, hWnd, Msg, wParam, (LPARAM) lpuszString);
            MojibakeFree(lpuszString);
            */

            Result = BackupCallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam);

            DWORD LastError = GetLastError();
            LRESULT sizeString = DefWindowProcA(hWnd, WM_GETTEXTLENGTH, 0, 0);
            sizeString++;
            LPSTR lpszString = (LPSTR) WaffleAlloc(sizeString);
            DefWindowProcA(hWnd, WM_GETTEXT, sizeString, (LPARAM) lpszString);
            LPWSTR lpuszString = AnsiToUnicode(lpszString);
            DefWindowProc(hWnd, WM_SETTEXT, 0, (LPARAM) lpuszString);
            MojibakeFree(lpuszString);
            MojibakeFree(lpszString);
            SetLastError(LastError);

            break;
        }
        default:
        {
            return BackupCallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
        }
        }

        return Result;
    }

    LIBRARY_EXPORT LRESULT WINAPI DetourSendMessageA(
        _In_    HWND hWnd,
        _In_    UINT Msg,
        _In_    WPARAM wParam,
        _In_    LPARAM lParam
        )
    {
        static LPSENDMESSAGEA BackupSendMessageA;
        if (!BackupSendMessageA)
        {
            BackupSendMessageA = (LPSENDMESSAGEA) WaffleGetBackupAddress(TEXT("user32.dll"), TEXT("SendMessageA"));
        }

        LRESULT Result;
        switch (Msg)
        {
        case WM_SETTEXT:
        {
            LPWSTR lpuszString = AnsiToUnicode((LPCSTR) lParam);
            Result = SendMessage(hWnd, Msg, wParam, (LPARAM) lpuszString);
            MojibakeFree(lpuszString);
            break;
        }
        default:
        {
            return BackupSendMessageA(hWnd, Msg, wParam, lParam);
        }
        }

        return Result;
    }

    LIBRARY_EXPORT BOOL WINAPI DetourSetWindowTextA(
        _In_        HWND hWnd,
        _In_opt_    LPCSTR lpString
        )
    {
        LPWSTR lpuszString = AnsiToUnicode(lpString);
        //MessageBox(0, lpuszString, TEXT("DetourSetWindowTextA"), 0);
        BOOL Result = SetWindowText(hWnd, lpuszString);

        MojibakeFree(lpuszString);
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