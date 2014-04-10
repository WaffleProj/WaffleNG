#include "..\common.h"

LIBRARY_EXPORT VOID WINAPI WaffleCheckOptionEncoding(
    _In_    LPCTSTR lpFileName
    )
{
    HANDLE hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile)
    {
        DWORD dwFile;
        WORD wBOM = 0xFEFF;
        switch (GetLastError())
        {
            case ERROR_ALREADY_EXISTS:
                if (!ReadFile(hFile, &wBOM, 2, &dwFile, NULL))
                {
                    MessageBox(0, TEXT("FIXME:Unable to read the option file."), 0, 0);
                    break;
                }
                if (wBOM != 0xFEFF && wBOM != 0xFFFE)
                {
                    MessageBox(0, TEXT("FIXME:At least one of your config file is in the multibyte character set. Please use UTF-16."), 0, 0);
                    break;
                }
                break;
            case ERROR_SUCCESS:
                WriteFile(hFile, &wBOM, 2, &dwFile, NULL);
                break;
        }
        CloseHandle(hFile);
    }
}

LIBRARY_EXPORT VOID WINAPI WaffleGetOptionString(
    _In_opt_    LPCTSTR lpszFileName,
    _In_opt_    LPCTSTR lpszSectionName,
    _In_        LPCTSTR lpszKeyName,
    _Out_       LPTSTR lpszValue,
    _In_        DWORD nSize,
    _In_opt_    LPCTSTR lpszDefaultValue
    )
{
    if (!lpszSectionName)
    {
        lpszSectionName = TEXT("Default");
    }
    if (!lpszDefaultValue)
    {
        lpszDefaultValue = TEXT("");
    }
    TCHAR szConfigPath[MAX_PATH];
    wsprintf(szConfigPath, TEXT("%s\\%s\\Config"), lpstProcessSetting->szComponentDirectory, lpstProcessSetting->szComponent);

    if (lpszFileName)
    {
        TCHAR szFileName[MAX_PATH];
        wsprintf(szFileName, TEXT("%s\\%s"), szConfigPath, lpszFileName);
        WaffleCheckOptionEncoding(szFileName);
        GetPrivateProfileString(lpszSectionName, lpszKeyName, lpszDefaultValue, lpszValue, nSize, szFileName);
    }
    else
    {
        //1.Check Hash.ini
        TCHAR szProcessHash[MAX_PATH];
        wsprintf(szProcessHash, TEXT("%s\\Hash.ini"), szConfigPath);
        WaffleCheckOptionEncoding(szProcessHash);
        GetPrivateProfileString(lpstProcessSetting->szProcessHash, lpszKeyName, lpszDefaultValue, lpszValue, nSize, szProcessHash);
        if (Wafflelstrcmp(lpszValue, lpszDefaultValue))
        {
            return;
        }

        //2.Check Option.ini
        TCHAR szOption[MAX_PATH];
        wsprintf(szOption, TEXT("%s\\Option.ini"), szConfigPath);
        WaffleCheckOptionEncoding(szOption);
        GetPrivateProfileString(lpszSectionName, lpszKeyName, lpszDefaultValue, lpszValue, nSize, szOption);
    }

    return;
}

LIBRARY_EXPORT VOID WINAPI WaffleSetOptionString(
    _In_opt_    LPCTSTR lpszFileName,
    _In_opt_    LPCTSTR lpszSectionName,
    _In_        LPCTSTR lpszKeyName,
    _In_        LPCTSTR lpszValue,
    _In_        BOOL bGlobal
    )
{
    if (!lpszSectionName)
    {
        lpszSectionName = TEXT("Default");
    }
    TCHAR szConfigPath[MAX_PATH];
    wsprintf(szConfigPath, TEXT("%s\\%s\\Config"), lpstProcessSetting->szComponentDirectory, lpstProcessSetting->szComponent);

    if (lpszFileName)
    {
        TCHAR szFileName[MAX_PATH];
        wsprintf(szFileName, TEXT("%s\\%s"), szConfigPath, lpszFileName);
        WaffleCheckOptionEncoding(szFileName);
        WritePrivateProfileString(lpszSectionName, lpszKeyName, lpszValue, szFileName);
    }
    else
    {
        if (bGlobal)
        {
            TCHAR szOption[MAX_PATH];
            wsprintf(szOption, TEXT("%s\\Option.ini"), szConfigPath);
            WaffleCheckOptionEncoding(szOption);
            WritePrivateProfileString(lpszSectionName, lpszKeyName, lpszValue, szOption);
        }
        else
        {
            TCHAR szProcessHash[MAX_PATH];
            wsprintf(szProcessHash, TEXT("%s\\Hash.ini"), szConfigPath);
            WaffleCheckOptionEncoding(szProcessHash);
            WritePrivateProfileString(lpstProcessSetting->szProcessHash, lpszKeyName, lpszValue, szProcessHash);
        }
    }

    return;
}

LIBRARY_EXPORT int WINAPI WaffleGetOptionInt(
    _In_opt_    LPCTSTR lpszFileName,
    _In_opt_    LPCTSTR lpszSectionName,
    _In_        LPCTSTR lpszKeyName,
    _In_opt_    int nDefaultValue
    )
{
    TCHAR szValue[256];
    WaffleGetOptionString(lpszFileName, lpszSectionName, lpszKeyName, szValue, lengthof(szValue), NULL);
    return WaffleStrToInt(szValue, nDefaultValue);
}

LIBRARY_EXPORT VOID WINAPI WaffleSetOptionInt(
    _In_opt_    LPCTSTR lpszFileName,
    _In_opt_    LPCTSTR lpszSectionName,
    _In_        LPCTSTR lpszKeyName,
    _In_        int nValue,
    _In_        BOOL bGlobal
    )
{
    TCHAR szValue[256];
    wsprintf(szValue, TEXT("%i"), nValue);
    WaffleSetOptionString(lpszFileName, lpszSectionName, lpszKeyName, szValue, bGlobal);
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

    WaffleCheckOptionEncoding(szOption);
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

    WaffleCheckOptionEncoding(szOption);
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