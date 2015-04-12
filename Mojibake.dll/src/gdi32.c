#include "..\mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

int CALLBACK EnumFontFamExFilterW(
    _In_    const LOGFONTW *lpelfe,
    _In_    const TEXTMETRICW *lpntme,
    _In_    DWORD FontType,
    _In_    LPARAM lParam
    )
{
    // Cast lpData
    LPENUM_FONTS_FILTER_DATAW lpFilter = (LPENUM_FONTS_FILTER_DATAW) lParam;

    // Copy the data
    LOGFONTW stLogFont;
    RtlMoveMemory(&stLogFont, lpelfe, sizeof(*lpelfe));
    TEXTMETRICW stTextMetric;
    RtlMoveMemory(&stTextMetric, lpntme, sizeof(*lpntme));

    // Get the localized font name from ttf file
    if (lpntme->tmPitchAndFamily & TMPF_TRUETYPE)
    {
        // Create Font
        HFONT hFont = CreateFontIndirectW(lpelfe);
        SelectObject(lpFilter->hDC, hFont);

        // Read "name" table
        DWORD nSize = GetFontData(lpFilter->hDC, TT_TABLE_NAME, 0, NULL, 0);
        if (nSize != GDI_ERROR)
        {
            LPVOID lpBuffer = WaffleAlloc(nSize);
            if (lpBuffer)
            {
                if (GetFontData(lpFilter->hDC, TT_TABLE_NAME, 0, lpBuffer, nSize) != GDI_ERROR)
                {
                    // Get the full name field
                    LPTT_NAME_TABLE_HEADER lpTable = (LPTT_NAME_TABLE_HEADER) lpBuffer;
                    LPTT_NAME_RECORD lpRecord = (LPTT_NAME_RECORD) (lpTable + 1);
                    USHORT nRecord = TTF_WORD(lpTable->nNameRecord);
                    for (int i = 0; i < nRecord; i++)
                    {
                        if (TTF_WORD(lpRecord[i].nameID) != TT_NAME_ID_FULL_NAME) continue;
                        if (TTF_WORD(lpRecord[i].platformID) != TT_PLATFORM_ID_MICROSOFT) continue;

                        // min(Face name length, lfFaceName size - 1);
                        UINT n = min((TTF_WORD(lpRecord[i].nString) / sizeof(stLogFont.lfFaceName[0])), (sizeof(stLogFont.lfFaceName) / sizeof(stLogFont.lfFaceName[0] - 1)));
                        LPWSTR lpFullName = (LPWSTR) (((LPBYTE) lpBuffer) + TTF_WORD(lpTable->offset) + TTF_WORD(lpRecord[i].stringOffset));

                        // Get the default name for localized font name
                        if (TTF_WORD(lpRecord[i].languageID) == TT_LANG_NEUTRAL)
                        {
                            for (UINT j = 0; (j < n) || (stLogFont.lfFaceName[j] = L'\0'); j++)
                            {
                                stLogFont.lfFaceName[j] = TTF_WORD(lpFullName[j]);
                            }
                        }

                        // Get the localized font name if it has one
                        if (TTF_WORD(lpRecord[i].languageID) == LANGIDFROMLCID(stNewEnvir.ThreadLocale))
                        {
                            for (UINT j = 0; (j < n) || (stLogFont.lfFaceName[j] = L'\0'); j++)
                            {
                                stLogFont.lfFaceName[j] = TTF_WORD(lpFullName[j]);
                            }

                            /*
                            stLogFont.lfCharSet = stNewEnvir.DefaultCharSet;
                            stTextMetric.tmCharSet = stNewEnvir.DefaultCharSet;
                            */
                            break;
                        }
                    }
                }

                WaffleFree(lpBuffer);
            }
        }

        DeleteObject(hFont);
    }

    return lpFilter->lpEnumFontFamExProc(&stLogFont, &stTextMetric, FontType, lpFilter->lParam);
}

int CALLBACK EnumFontFamExFilterA(
    _In_    const LOGFONTW *lpelfe,
    _In_    const TEXTMETRICW *lpntme,
    _In_    DWORD FontType,
    _In_    LPARAM lParam
    )
{
    // ENUMLOGFONTEXDVW stEnumLogFontExDV;
    LPENUM_FONTS_FILTER_DATAA lpFilter = (LPENUM_FONTS_FILTER_DATAA) lParam;

    LOGFONTA stLogFont;
    RtlMoveMemory(&stLogFont, lpelfe, offsetof(LOGFONTW, lfFaceName));
    stLogFont.lfFaceName[0] = '\0';
    LPSTR lpszFaceName = UnicodeToAnsi(lpelfe->lfFaceName);
    if (lpszFaceName)
    {
        WafflelstrcpyA(stLogFont.lfFaceName, lpszFaceName);
        MojibakeFree(lpszFaceName);
    }

    TEXTMETRICA stTextMetric;
    RtlMoveMemory(&stTextMetric, lpntme, offsetof(TEXTMETRICW, tmFirstChar));
    stTextMetric.tmFirstChar = lpntme->tmFirstChar;
    stTextMetric.tmLastChar = lpntme->tmLastChar;
    stTextMetric.tmDefaultChar = lpntme->tmDefaultChar;
    stTextMetric.tmBreakChar = lpntme->tmBreakChar;
    RtlMoveMemory(&(stTextMetric.tmItalic), &(lpntme->tmItalic), sizeof(TEXTMETRICW) - offsetof(TEXTMETRICW, tmItalic));

    return lpFilter->lpEnumFontFamExProc(&stLogFont, &stTextMetric, FontType, lpFilter->lParam);
}

LIBRARY_EXPORT int WINAPI DetourEnumFontFamiliesExW(
    _In_    HDC hdc,
    _In_    LPLOGFONTW lpLogfont,
    _In_    FONTENUMPROCW lpEnumFontFamExProc,
    _In_    LPARAM lParam,
    _In_    DWORD dwFlags
    )
{
    static LPENUMFONTFAMILIESEXW BackupEnumFontFamiliesExW;
    if (!BackupEnumFontFamiliesExW)
    {
        BackupEnumFontFamiliesExW = (LPENUMFONTFAMILIESEXW) WaffleGetBackupAddress(TEXT("gdi32.dll"), TEXT("EnumFontFamiliesExW"));
    }

    ENUM_FONTS_FILTER_DATAW stFilter;
    stFilter.lpEnumFontFamExProc = lpEnumFontFamExProc;
    stFilter.lParam = lParam;
    stFilter.hDC = CreateCompatibleDC(NULL);

    int Result = BackupEnumFontFamiliesExW(hdc, lpLogfont, EnumFontFamExFilterW, (LPARAM) &stFilter, dwFlags);

    DeleteDC(stFilter.hDC);
    return Result;
}

LIBRARY_EXPORT int WINAPI DetourEnumFontFamiliesW(
    _In_    HDC hdc,
    _In_    LPCWSTR lpszFamily,
    _In_    FONTENUMPROCW lpEnumFontFamProc,
    _In_    LPARAM lParam
    )
{
    LOGFONTW stLogFont;

    RtlZeroMemory(&stLogFont, sizeof(stLogFont));
    stLogFont.lfCharSet = DEFAULT_CHARSET;
    stLogFont.lfFaceName[0] = L'\0';
    WafflelstrcpyW(stLogFont.lfFaceName, lpszFamily);

    return DetourEnumFontFamiliesExW(hdc, &stLogFont, lpEnumFontFamProc, lParam, 0);
}

LIBRARY_EXPORT int WINAPI DetourEnumFontsW(
    _In_  HDC hdc,
    _In_  LPCWSTR lpFaceName,
    _In_  FONTENUMPROCW lpFontFunc,
    _In_  LPARAM lParam
    )
{
    LOGFONTW stLogFont;

    RtlZeroMemory(&stLogFont, sizeof(stLogFont));
    stLogFont.lfCharSet = DEFAULT_CHARSET;
    stLogFont.lfFaceName[0] = L'\0';
    WafflelstrcpyW(stLogFont.lfFaceName, lpFaceName);

    return DetourEnumFontFamiliesExW(hdc, &stLogFont, lpFontFunc, lParam, 0);
}

LIBRARY_EXPORT int WINAPI DetourEnumFontFamiliesExA(
    _In_    HDC hdc,
    _In_    LPLOGFONTA lpLogfont,
    _In_    FONTENUMPROCA lpEnumFontFamExProc,
    _In_    LPARAM lParam,
    _In_    DWORD dwFlags
    )
{
    LOGFONTW stLogFont;
    RtlMoveMemory(&stLogFont, lpLogfont, offsetof(LOGFONTA, lfFaceName));
    stLogFont.lfFaceName[0] = L'\0';
    LPWSTR lpuszFaceName = AnsiToUnicode(lpLogfont->lfFaceName);
    if (lpuszFaceName)
    {
        WafflelstrcpyW(stLogFont.lfFaceName, lpuszFaceName);
        MojibakeFree(lpuszFaceName);
    }

    ENUM_FONTS_FILTER_DATAA stFilter;
    stFilter.lpEnumFontFamExProc = lpEnumFontFamExProc;
    stFilter.lParam = lParam;
    stFilter.hDC = NULL;

    return DetourEnumFontFamiliesExW(hdc, &stLogFont, EnumFontFamExFilterA, (LPARAM) &stFilter, 0);
}

LIBRARY_EXPORT int WINAPI DetourEnumFontFamiliesA(
    _In_    HDC hdc,
    _In_    LPCSTR lpszFamily,
    _In_    FONTENUMPROCA lpEnumFontFamProc,
    _In_    LPARAM lParam
    )
{
    LOGFONTA stLogFont;

    RtlZeroMemory(&stLogFont, sizeof(stLogFont));
    stLogFont.lfCharSet = DEFAULT_CHARSET;
    stLogFont.lfFaceName[0] = '\0';
    WafflelstrcpyA(stLogFont.lfFaceName, lpszFamily);

    return DetourEnumFontFamiliesExA(hdc, &stLogFont, lpEnumFontFamProc, lParam, 0);
}

LIBRARY_EXPORT int WINAPI DetourEnumFontsA(
    _In_  HDC hdc,
    _In_  LPCSTR lpFaceName,
    _In_  FONTENUMPROCA lpFontFunc,
    _In_  LPARAM lParam
    )
{
    LOGFONTA stLogFont;

    RtlZeroMemory(&stLogFont, sizeof(stLogFont));
    stLogFont.lfCharSet = DEFAULT_CHARSET;
    stLogFont.lfFaceName[0] = '\0';
    WafflelstrcpyA(stLogFont.lfFaceName, lpFaceName);

    return DetourEnumFontFamiliesExA(hdc, &stLogFont, lpFontFunc, lParam, 0);
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