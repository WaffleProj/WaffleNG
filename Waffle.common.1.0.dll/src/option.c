#include "..\common.h"

LIBRARY_EXPORT VOID WINAPI WaffleGetOptionString(
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
    wsprintf(szConfigPath, TEXT("%s\\%s\\Config"), lpstProcessSetting->szComponentDirectory, lpstProcessSetting->szComponent);

    //1.Check Hash.ini
    TCHAR szProcessHash[MAX_PATH];
    wsprintf(szProcessHash, TEXT("%s\\Hash.ini"), szConfigPath);
    GetPrivateProfileString(lpstProcessSetting->szProcessHash, lpszKeyName, lpszDefaultValue, lpszValue, nSize, szProcessHash);
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

LIBRARY_EXPORT VOID WINAPI WaffleSetOptionString(
    _In_    LPCTSTR lpszKeyName,
    _In_    LPCTSTR lpszValue,
    _In_    BOOL bGlobal
    )
{
    TCHAR szConfigPath[MAX_PATH];
    wsprintf(szConfigPath, TEXT("%s\\%s\\Config"), lpstProcessSetting->szComponentDirectory, lpstProcessSetting->szComponent);

    TCHAR szOption[MAX_PATH];
    if (bGlobal)
    {
        wsprintf(szOption, TEXT("%s\\Option.ini"), szConfigPath);
        WritePrivateProfileString(TEXT("Default"), lpszKeyName, lpszValue, szOption);
    }
    else
    {
        wsprintf(szOption, TEXT("%s\\Hash.ini"), szConfigPath);
        WritePrivateProfileString(lpstProcessSetting->szProcessHash, lpszKeyName, lpszValue, szOption);
    }

    return;
}

LIBRARY_EXPORT int WINAPI WaffleGetOptionInt(
    _In_        LPCTSTR lpszKeyName,
    _In_opt_    int nDefaultValue
    )
{
    TCHAR szValue[256];
    WaffleGetOptionString(lpszKeyName, szValue, sizeof(szValue) / sizeof(szValue[0]), NULL);
    return WaffleStrToInt(szValue, nDefaultValue);
}

LIBRARY_EXPORT VOID WINAPI WaffleSetOptionInt(
    _In_    LPCTSTR lpszKeyName,
    _In_    int nValue,
    _In_    BOOL bGlobal
    )
{
    TCHAR szValue[256];
    wsprintf(szValue, TEXT("%i"), nValue);
    WaffleSetOptionString(lpszKeyName, szValue, bGlobal);
    return;
}

LIBRARY_EXPORT int WINAPI WaffleGetOptionStringNumber(
    _In_    LPCTSTR lpszString
    )
{
    int nString = 0;
    DWORD nSizeOfString = lstrlen(lpszString);
    while (nSizeOfString)
    {
        nString++;
        lpszString = lpszString + nSizeOfString + 1;
        nSizeOfString = lstrlen(lpszString);
    }
    return nString;
}

LIBRARY_EXPORT LPTSTR WINAPI WaffleGetOptionSectionNames(
    _In_    LPCTSTR lpszOption
    )
{
    TCHAR szOption[MAX_PATH];
    wsprintf(szOption, TEXT("%s\\%s\\Config\\%s"), lpstProcessSetting->szComponentDirectory, lpstProcessSetting->szComponent, lpszOption);

    LPTSTR lpszSection;
    DWORD nSizeOfSection = 4;
    lpszSection = (LPTSTR) WaffleAlloc(nSizeOfSection*sizeof(TCHAR));
    if (!lpszSection)
    {
        return NULL;
    }
    while (GetPrivateProfileSectionNames(lpszSection, nSizeOfSection, szOption) == nSizeOfSection - 2)
    {
        nSizeOfSection = nSizeOfSection * 2;
        HGLOBAL lpszSectionRealloc = WaffleReAlloc(lpszSection, nSizeOfSection*sizeof(TCHAR));
        if (!lpszSectionRealloc)
        {
            WaffleFree(lpszSection);
            return NULL;
        }
        else
        {
            lpszSection = (LPTSTR) lpszSectionRealloc;
        }
    }
    return lpszSection;
}

LIBRARY_EXPORT LPTSTR WINAPI WaffleGetOptionSectionKeys(
    _In_    LPCTSTR lpszOption,
    _In_    LPCTSTR lpszSection
    )
{
    TCHAR szOption[MAX_PATH];
    wsprintf(szOption, TEXT("%s\\%s\\Config\\%s"), lpstProcessSetting->szComponentDirectory, lpstProcessSetting->szComponent, lpszOption);

    LPTSTR lpszKey;
    DWORD nSizeOfKey = 4;
    lpszKey = (LPTSTR) WaffleAlloc(nSizeOfKey*sizeof(TCHAR));
    if (!lpszKey)
    {
        return NULL;
    }
    while (GetPrivateProfileSection(lpszSection, lpszKey, nSizeOfKey, szOption) == nSizeOfKey - 2)
    {
        nSizeOfKey = nSizeOfKey * 2;
        if (nSizeOfKey > 32767)
        {
            WaffleFree(lpszKey);
            return NULL;
        }
        HGLOBAL lpszKeyRealloc = WaffleReAlloc(lpszKey, nSizeOfKey*sizeof(TCHAR));
        if (!lpszKeyRealloc)
        {
            WaffleFree(lpszKey);
            return NULL;
        }
        else
        {
            lpszKey = (LPTSTR) lpszKeyRealloc;
        }
    }
    return lpszKey;
}