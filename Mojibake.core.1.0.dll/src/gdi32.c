#include "..\mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

int CALLBACK EnumFontsFilterA(
    _In_  const LOGFONTA *lplf,
    _In_  const TEXTMETRICA *lptm,
    _In_  DWORD dwType,
    _In_  LPARAM lpData
    )
{
    // Cast lpData
    LPENUM_FONTS_FILTER_DATA lpFilter = (LPENUM_FONTS_FILTER_DATA) lpData;

    // Copy the data
    LOGFONTA stLogFont;
    RtlMoveMemory(&stLogFont, lplf, sizeof(*lplf));
    TEXTMETRICA stTextMetric;
    RtlMoveMemory(&stTextMetric, lptm, sizeof(*lptm));

    // Get the current locale
    TCHAR szThreadLocale[16];
    wsprintf(szThreadLocale, TEXT("%i"), stNewEnvir.ThreadLocale);

    // Get the new font name
    LPSTR lpszFaceName = NULL;
    LPWSTR lpuszFaceName = AnsiToUnicode(lplf->lfFaceName);
    TCHAR szFaceName[64];
    WaffleGetOptionString(TEXT("FontRemap.ini"), szThreadLocale, lpuszFaceName, szFaceName, lengthof(szFaceName), NULL);
    if (Wafflelstrcmp(szFaceName, TEXT("")))
    {
        lpszFaceName = UnicodeToAnsi(szFaceName);
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

    ENUM_FONTS_FILTER_DATA stFilter;
    stFilter.lpFontFunc = lpFontFunc;
    stFilter.lpData = lParam;
    return BackupEnumFontsA(hdc, lpFaceName, EnumFontsFilterA, (LPARAM) &stFilter);
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

    ENUM_FONTS_FILTER_DATA stFilter;
    stFilter.lpFontFunc = lpEnumFontFamProc;
    stFilter.lpData = lParam;
    return BackupEnumFontFamiliesA(hdc, lpszFamily, EnumFontsFilterA, (LPARAM) &stFilter);
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

    ENUM_FONTS_FILTER_DATA stFilter;
    stFilter.lpFontFunc = lpEnumFontFamExProc;
    stFilter.lpData = lParam;
    return BackupEnumFontFamiliesExA(hdc, lpLogfont, EnumFontsFilterA, (LPARAM) &stFilter, dwFlags);
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