#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"

LIBRARY_EXPORT void WINAPI WaffleGetOptionString(
    _In_        LPWAFFLE_PROCESS_SETTING lpstProcessSetting,
    _In_        LPCTSTR lpszKeyName,
    _Inout_     LPTSTR lpszValue,
    _In_        DWORD nSize,
    _In_opt_    LPTSTR lpszDefaultValue
    )
{
    if (!lpszDefaultValue)
    {
        lpszDefaultValue = TEXT("");
    }
    TCHAR szConfigPath[MAX_PATH];
    wsprintf(szConfigPath, TEXT("%s\\%s\\Config"), szComponentPath, lpstProcessSetting->szPlugin);

    //1.Check Hash.ini
    TCHAR szHash[MAX_PATH];
    wsprintf(szHash, TEXT("%s\\Hash.ini"), szConfigPath);
    GetPrivateProfileString(lpstProcessSetting->szHash, lpszKeyName, lpszDefaultValue, lpszValue, nSize, szHash);
    if (Wafflelstrcmp(lpszValue, lpszDefaultValue))
    {
        return;
    }

    //2.Check Option.ini
    TCHAR szOption[MAX_PATH];
    wsprintf(szOption, TEXT("%s\\Option.ini"), szConfigPath);
    GetPrivateProfileString(TEXT("Default"), lpszKeyName, lpszDefaultValue, lpszValue, nSize, szOption);
    return;
}

LIBRARY_EXPORT void WINAPI WaffleSetOptionString(
    _In_    LPWAFFLE_PROCESS_SETTING lpstProcessSetting,
    _In_    LPCTSTR lpszKeyName,
    _In_    LPCTSTR lpszValue,
    _In_    BOOL bGlobal
    )
{
    TCHAR szConfigPath[MAX_PATH];
    wsprintf(szConfigPath, TEXT("%s\\%s\\Config"), szComponentPath, lpstProcessSetting->szPlugin);

    TCHAR szOption[MAX_PATH];
    if (bGlobal)
    {
        wsprintf(szOption, TEXT("%s\\Option.ini"), szConfigPath);
        WritePrivateProfileString(TEXT("Default"), lpszKeyName, lpszValue, szOption);
    }
    else
    {
        wsprintf(szOption, TEXT("%s\\Hash.ini"), szConfigPath);
        WritePrivateProfileString(lpstProcessSetting->szHash, lpszKeyName, lpszValue, szOption);
    }

    return;
}

LIBRARY_EXPORT int WINAPI WaffleGetOptionInt(
    _In_        LPWAFFLE_PROCESS_SETTING lpstProcessSetting,
    _In_        LPCTSTR lpszKeyName,
    _In_opt_    int nDefaultValue
    )
{
    TCHAR szValue[256];
    WaffleGetOptionString(lpstProcessSetting, lpszKeyName, szValue, sizeof(szValue) / sizeof(szValue[0]), NULL);
    return WaffleStrToInt(szValue, nDefaultValue);
}

LIBRARY_EXPORT void WINAPI WaffleSetOptionInt(
    _In_    LPWAFFLE_PROCESS_SETTING lpstProcessSetting,
    _In_    LPCTSTR lpszKeyName,
    _In_    int nValue,
    _In_    BOOL bGlobal
    )
{
    TCHAR szValue[256];
    wsprintf(szValue, TEXT("0x%X"), nValue);
    WaffleSetOptionString(lpstProcessSetting, lpszKeyName, szValue, bGlobal);
    return;
}