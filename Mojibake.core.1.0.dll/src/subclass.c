#include "..\mojibake.h"

static SUBCLASS_PROC stSubclass;

LRESULT CALLBACK MojibakeSubclassComboBox(
    _In_    HWND hwnd,
    _In_    UINT uMsg,
    _In_    WPARAM wParam,
    _In_    LPARAM lParam
    )
{
    LRESULT Result = 0;
    switch (uMsg)
    {
    case CB_ADDSTRING:
    {
        LPWSTR lpuszText = (LPWSTR) AnsiToUnicode((LPCSTR) lParam);
        Result = CallWindowProc(stSubclass.lpComboBoxW, hwnd, uMsg, wParam, (LPARAM) lpuszText);
        MojibakeFree(lpuszText);
        break;
    }
    case CB_GETLBTEXTLEN:
    {
        LRESULT nText = CallWindowProc(stSubclass.lpComboBoxW, hwnd, uMsg, wParam, lParam) + 1;
        DWORD LastError = GetLastError();

        LPWSTR lpuszText = (LPWSTR) WaffleAlloc(nText * sizeof(WCHAR));
        CallWindowProcA(stSubclass.lpComboBoxA, hwnd, CB_GETLBTEXT, wParam, (LPARAM) lpuszText);
        Result = (WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszText, -1, NULL, 0, NULL, NULL) - 1) * 2;
        MojibakeFree(lpuszText);
        SetLastError(LastError);
        break;
    }
    case CB_GETLBTEXT:
    {
        LRESULT nText = CallWindowProc(stSubclass.lpComboBoxW, hwnd, CB_GETLBTEXTLEN, wParam, lParam) + 1;
        LPWSTR lpuszText = (LPWSTR) WaffleAlloc(nText * sizeof(WCHAR));
        CallWindowProc(stSubclass.lpComboBoxW, hwnd, uMsg, wParam, (LPARAM) lpuszText);
        DWORD LastError = GetLastError();
        Result = WideCharToMultiByte(stNewEnvir.AnsiCodePage, 0, lpuszText, -1, (LPSTR) lParam, 0x7FFFFFFF, NULL, NULL) - 1;
        MojibakeFree(lpuszText);
        SetLastError(LastError);
        break;
    }
    default:
    {
        Result = CallWindowProcA(stSubclass.lpComboBoxA, hwnd, uMsg, wParam, lParam);
    }
    }

    return Result;
}

VOID WINAPI MojibakeSubclass(VOID)
{
    HWND hComboBox = CreateWindowEx(0, TEXT("ComboBox"), TEXT("ComboBox"), 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
    stSubclass.lpComboBoxA = (WNDPROC) GetClassLongPtrA(hComboBox, GCLP_WNDPROC);
    stSubclass.lpComboBoxW = (WNDPROC) GetClassLongPtrW(hComboBox, GCLP_WNDPROC);
    SetClassLongPtrA(hComboBox, GCLP_WNDPROC, (LONG_PTR) MojibakeSubclassComboBox);
    DestroyWindow(hComboBox);
}