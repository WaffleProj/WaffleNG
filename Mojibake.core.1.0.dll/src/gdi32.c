#include "..\mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

int CALLBACK EnumFontsFilterA(
    _In_    const LOGFONTA *lplf,
    _In_    const TEXTMETRICA *lptm,
    _In_    DWORD dwType,
    _In_    LPARAM lpData
    )
{
    // Cast lpData
    LPENUM_FONTS_FILTER_DATAA lpFilter = (LPENUM_FONTS_FILTER_DATAA) lpData;

    // Copy the data
    LOGFONTA stLogFont;
    RtlMoveMemory(&stLogFont, lplf, sizeof(*lplf));
    TEXTMETRICA stTextMetric;
    RtlMoveMemory(&stTextMetric, lptm, sizeof(*lptm));

    // Get the current locale
    TCHAR szThreadLocale[16];
    wsprintf(szThreadLocale, TEXT("%i"), stNewEnvir.ThreadLocale);

    // Get the localized font name from ttf file
    LPWSTR lpuszFaceName = MBCSToUnicode(stOldEnvir.AnsiCodePage, lplf->lfFaceName);
    int Remap = WaffleGetOptionInt(TEXT("FontRemap.ini"), lpuszFaceName, TEXT("Remap"), -1);
    if (Remap == -1 && (lptm->tmPitchAndFamily & TMPF_TRUETYPE))
    {
        // Create Section
        WaffleSetOptionInt(TEXT("FontRemap.ini"), lpuszFaceName, TEXT("Remap"), -1, 0);
        Remap = FALSE;

        // Create Font
        HFONT hFont = CreateFontIndirectA(lplf);
        SelectObject(lpFilter->hDC, hFont);

        DWORD nSize = GetFontData(lpFilter->hDC, TT_TABLE_NAME, 0, NULL, 0);
        if (nSize != GDI_ERROR)
        {
            LPVOID lpBuffer = WaffleAlloc(nSize);
            if (lpBuffer)
            {
                if (GetFontData(lpFilter->hDC, TT_TABLE_NAME, 0, lpBuffer, nSize) != GDI_ERROR)
                {
                    LPTT_NAME_TABLE_HEADER lpTable = (LPTT_NAME_TABLE_HEADER) lpBuffer;
                    LPTT_NAME_RECORD lpRecord = (LPTT_NAME_RECORD) (lpTable + 1);
                    USHORT nRecord = TTF_WORD(lpTable->nNameRecord);
                    for (int i = 0; i < nRecord; i++)
                    {
                        if (TTF_WORD(lpRecord[i].languageID) == TT_LANG_ENGLISH) continue;
                        if (TTF_WORD(lpRecord[i].languageID) == TT_LANG_NEUTRAL) continue;
                        if (TTF_WORD(lpRecord[i].platformID) != TT_PLATFORM_ID_MICROSOFT) continue;
                        if (TTF_WORD(lpRecord[i].nameID) != TT_NAME_ID_FULL_NAME) continue;
                        //if (!Wafflelstrcmpi(lpuszFaceName, TEXT("MS Gothic")))
                        {
                            nSize = TTF_WORD(lpRecord[i].nString);
                            LPWSTR szFullName = (LPWSTR) WaffleAlloc(nSize + sizeof(WCHAR));
                            if (szFullName)
                            {
                                LPWSTR lpFullName = (LPWSTR) (((LPBYTE) lpBuffer) + TTF_WORD(lpTable->offset) + TTF_WORD(lpRecord[i].stringOffset));
                                for (UINT j = 0; (j < (nSize / sizeof(szFullName[0]))) || (szFullName[j] = L'\0'); j++)
                                {
                                    szFullName[j] = TTF_WORD(lpFullName[j]);
                                }

                                TCHAR szLanguageID[16];
                                wsprintf(szLanguageID, TEXT("%hu"), TTF_WORD(lpRecord[i].languageID));
                                WaffleSetOptionString(TEXT("FontRemap.ini"), lpuszFaceName, szLanguageID, szFullName, 0);
                                Remap = TRUE;
                                WaffleFree(szFullName);
                            }
                        }
                    }
                    WaffleSetOptionInt(TEXT("FontRemap.ini"), lpuszFaceName, TEXT("Remap"), Remap, 0);
                }

                WaffleFree(lpBuffer);
            }
        }

        DeleteObject(hFont);
    }

    // Return the localized font name
    if (Remap == TRUE)
    {
        TCHAR szFaceName[64];
        WaffleGetOptionString(TEXT("FontRemap.ini"), lpuszFaceName, szThreadLocale, szFaceName, lengthof(szFaceName), lpuszFaceName);
        LPSTR lpszFaceName = UnicodeToAnsi(szFaceName);
        if (lpszFaceName)
        {
            lstrcpyA(stLogFont.lfFaceName, lpszFaceName);
            MojibakeFree(lpszFaceName);
        }

        stLogFont.lfCharSet = stNewEnvir.DefaultCharSet;
        stTextMetric.tmCharSet = stNewEnvir.DefaultCharSet;
    }
    MojibakeFree(lpuszFaceName);

    return lpFilter->lpFontFunc(&stLogFont, &stTextMetric, dwType, lpFilter->lpData);
}

LIBRARY_EXPORT int WINAPI DetourEnumFontsA(
    _In_  HDC hdc,
    _In_  LPCSTR lpFaceName,
    _In_  FONTENUMPROCA lpFontFunc,
    _In_  LPARAM lParam
    )
{
    static LPENUMFONTSA BackupEnumFontsA;
    if (!BackupEnumFontsA)
    {
        BackupEnumFontsA = (LPENUMFONTSA) WaffleGetBackupAddress(TEXT("gdi32.dll"), TEXT("EnumFontsA"));
    }

    ENUM_FONTS_FILTER_DATAA stFilter;
    stFilter.lpFontFunc = lpFontFunc;
    stFilter.lpData = lParam;
    stFilter.hDC = CreateCompatibleDC(NULL);

    int Result = BackupEnumFontsA(hdc, lpFaceName, EnumFontsFilterA, (LPARAM) &stFilter);

    DeleteDC(stFilter.hDC);
    return Result;
}

LIBRARY_EXPORT int WINAPI DetourEnumFontFamiliesA(
    _In_    HDC hdc,
    _In_    LPCSTR lpszFamily,
    _In_    FONTENUMPROCA lpEnumFontFamProc,
    _In_    LPARAM lParam
    )
{
    static LPENUMFONTFAMILIESA BackupEnumFontFamiliesA;
    if (!BackupEnumFontFamiliesA)
    {
        BackupEnumFontFamiliesA = (LPENUMFONTFAMILIESA) WaffleGetBackupAddress(TEXT("gdi32.dll"), TEXT("EnumFontFamiliesA"));
    }

    ENUM_FONTS_FILTER_DATAA stFilter;
    stFilter.lpFontFunc = lpEnumFontFamProc;
    stFilter.lpData = lParam;
    stFilter.hDC = CreateCompatibleDC(NULL);

    int Result = BackupEnumFontFamiliesA(hdc, lpszFamily, EnumFontsFilterA, (LPARAM) &stFilter);

    DeleteDC(stFilter.hDC);
    return Result;
}

LIBRARY_EXPORT int WINAPI DetourEnumFontFamiliesExA(
    _In_    HDC hdc,
    _In_    LPLOGFONTA lpLogfont,
    _In_    FONTENUMPROCA lpEnumFontFamExProc,
    _In_    LPARAM lParam,
    _In_    DWORD dwFlags
    )
{
    static LPENUMFONTFAMILIESEXA BackupEnumFontFamiliesExA;
    if (!BackupEnumFontFamiliesExA)
    {
        BackupEnumFontFamiliesExA = (LPENUMFONTFAMILIESEXA) WaffleGetBackupAddress(TEXT("gdi32.dll"), TEXT("EnumFontFamiliesExA"));
    }

    ENUM_FONTS_FILTER_DATAA stFilter;
    stFilter.lpFontFunc = lpEnumFontFamExProc;
    stFilter.lpData = lParam;
    stFilter.hDC = CreateCompatibleDC(NULL);

    int Result = BackupEnumFontFamiliesExA(hdc, lpLogfont, EnumFontsFilterA, (LPARAM) &stFilter, dwFlags);

    DeleteDC(stFilter.hDC);
    return Result;
}

LIBRARY_EXPORT HFONT WINAPI DetourCreateFontW(
    _In_    int nHeight,
    _In_    int nWidth,
    _In_    int nEscapement,
    _In_    int nOrientation,
    _In_    int fnWeight,
    _In_    DWORD fdwItalic,
    _In_    DWORD fdwUnderline,
    _In_    DWORD fdwStrikeOut,
    _In_    DWORD fdwCharSet,
    _In_    DWORD fdwOutputPrecision,
    _In_    DWORD fdwClipPrecision,
    _In_    DWORD fdwQuality,
    _In_    DWORD fdwPitchAndFamily,
    _In_    LPCWSTR lpszFace
    )
{
    static LPCREATEFONTW BackupCreateFontW;
    if (!BackupCreateFontW)
    {
        BackupCreateFontW = (LPCREATEFONTW) WaffleGetBackupAddress(TEXT("gdi32.dll"), TEXT("CreateFontW"));
    }

    if (fdwCharSet == DEFAULT_CHARSET)
    {
        fdwCharSet = stNewEnvir.DefaultCharSet;
    }

    return BackupCreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
}

LIBRARY_EXPORT HFONT WINAPI DetourCreateFontA(
    _In_    int nHeight,
    _In_    int nWidth,
    _In_    int nEscapement,
    _In_    int nOrientation,
    _In_    int fnWeight,
    _In_    DWORD fdwItalic,
    _In_    DWORD fdwUnderline,
    _In_    DWORD fdwStrikeOut,
    _In_    DWORD fdwCharSet,
    _In_    DWORD fdwOutputPrecision,
    _In_    DWORD fdwClipPrecision,
    _In_    DWORD fdwQuality,
    _In_    DWORD fdwPitchAndFamily,
    _In_    LPCSTR lpszFace
    )
{
    LPWSTR lpuszFace = AnsiToUnicode(lpszFace);
    if (fdwCharSet == DEFAULT_CHARSET)
    {
        fdwCharSet = stNewEnvir.DefaultCharSet;
    }
    HFONT Result = DetourCreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpuszFace);

    MojibakeFree(lpuszFace);
    return Result;
}

LIBRARY_EXPORT HFONT WINAPI DetourCreateFontIndirectW(
    _In_    const LOGFONTW *lplf
    )
{
    static LPCREATEFONTINDIRECTW BackupCreateFontIndirectW;
    if (!BackupCreateFontIndirectW)
    {
        BackupCreateFontIndirectW = (LPCREATEFONTINDIRECTW) WaffleGetBackupAddress(TEXT("gdi32.dll"), TEXT("CreateFontIndirectW"));
    }

    LOGFONTW lf;
    RtlMoveMemory(&lf, lplf, sizeof(lf));

    if (lf.lfCharSet == DEFAULT_CHARSET)
    {
        lf.lfCharSet = stNewEnvir.DefaultCharSet;
    }

    return BackupCreateFontIndirectW(&lf);
}

LIBRARY_EXPORT HFONT WINAPI DetourCreateFontIndirectA(
    _In_    const LOGFONTA *lplf
    )
{
    LOGFONTW lf;
    RtlMoveMemory(&lf, lplf, sizeof(lf));

    if (lf.lfCharSet == DEFAULT_CHARSET)
    {
        lf.lfCharSet = stNewEnvir.DefaultCharSet;
    }

    LPWSTR lfuFaceName = AnsiToUnicode(lplf->lfFaceName);
    lstrcpy(lf.lfFaceName, lfuFaceName);
    MojibakeFree(lfuFaceName);

    return DetourCreateFontIndirectW(&lf);
}

LIBRARY_EXPORT HFONT WINAPI DetourCreateFontIndirectExW(
    _In_    const ENUMLOGFONTEXDVW *penumlfex
    )
{
    static LPCREATEFONTINDIRECTEXW BackupCreateFontIndirectExW;
    if (!BackupCreateFontIndirectExW)
    {
        BackupCreateFontIndirectExW = (LPCREATEFONTINDIRECTEXW) WaffleGetBackupAddress(TEXT("gdi32.dll"), TEXT("CreateFontIndirectExW"));
    }

    ENUMLOGFONTEXDVW enumlfex;
    RtlMoveMemory(&enumlfex, penumlfex, sizeof(enumlfex));

    if (enumlfex.elfEnumLogfontEx.elfLogFont.lfCharSet == DEFAULT_CHARSET)
    {
        enumlfex.elfEnumLogfontEx.elfLogFont.lfCharSet = stNewEnvir.DefaultCharSet;
    }

    return BackupCreateFontIndirectExW(&enumlfex);
}

LIBRARY_EXPORT HFONT WINAPI DetourCreateFontIndirectExA(
    _In_    const ENUMLOGFONTEXDVA *penumlfex
    )
{
    ENUMLOGFONTEXDVW enumlfex;
    RtlMoveMemory(&enumlfex, penumlfex, sizeof(enumlfex));

    if (enumlfex.elfEnumLogfontEx.elfLogFont.lfCharSet == DEFAULT_CHARSET)
    {
        enumlfex.elfEnumLogfontEx.elfLogFont.lfCharSet = stNewEnvir.DefaultCharSet;
    }

    LPWSTR lfuFaceName = AnsiToUnicode(penumlfex->elfEnumLogfontEx.elfLogFont.lfFaceName);
    LPWSTR elfuFullName = AnsiToUnicode((LPCSTR) penumlfex->elfEnumLogfontEx.elfFullName);
    LPWSTR elfuStyle = AnsiToUnicode((LPCSTR) penumlfex->elfEnumLogfontEx.elfStyle);
    LPWSTR elfuScript = AnsiToUnicode((LPCSTR) penumlfex->elfEnumLogfontEx.elfScript);
    lstrcpy(enumlfex.elfEnumLogfontEx.elfLogFont.lfFaceName, lfuFaceName);
    lstrcpy(enumlfex.elfEnumLogfontEx.elfFullName, elfuFullName);
    lstrcpy(enumlfex.elfEnumLogfontEx.elfStyle, elfuStyle);
    lstrcpy(enumlfex.elfEnumLogfontEx.elfScript, elfuScript);
    MojibakeFree(lfuFaceName);
    MojibakeFree(elfuFullName);
    MojibakeFree(elfuStyle);
    MojibakeFree(elfuScript);

    return DetourCreateFontIndirectExW(&enumlfex);
}

LIBRARY_EXPORT BOOL WINAPI DetourTextOutA(
    _In_    HDC hdc,
    _In_    int nXStart,
    _In_    int nYStart,
    _In_    LPCSTR lpString,
    _In_    int cchString
    )
{
    LPWSTR lpuszString = AnsiToUnicode(lpString);
    BOOL Result = TextOut(hdc, nXStart, nYStart, lpuszString, cchString);

    MojibakeFree(lpuszString);
    return Result;
}

LIBRARY_EXPORT DWORD WINAPI DetourGdiGetCodePage(
    _In_    HDC hdc
    )
{
    return stNewEnvir.AnsiCodePage;
}