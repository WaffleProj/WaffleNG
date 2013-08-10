#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\mojibake.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifdef __cplusplus
extern "C" {
#endif
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

        if (!lpFaceName)
        {
            lpFontFunc(&lfGothic, &tmGothic, dwGothic, lParam);
        }
        return BackupEnumFontsA(hdc, lpFaceName, lpFontFunc, lParam);
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

        KeepLastErrorAndFree(lpuszFace);
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
        if (!Wafflelstrcmp(lfuFaceName, TEXT("ＭＳ ゴシック")))
        {
            lstrcpy(lf.lfFaceName, TEXT("MS Gothic"));
        }
        WaffleFree(lfuFaceName);

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
        WaffleFree(lfuFaceName);
        WaffleFree(elfuFullName);
        WaffleFree(elfuStyle);
        WaffleFree(elfuScript);

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

        KeepLastErrorAndFree(lpuszString);
        return Result;
    }

    LIBRARY_EXPORT DWORD WINAPI DetourGdiGetCodePage(
        _In_    HDC hdc
        )
    {
        return stNewEnvir.AnsiCodePage;
    }
#ifdef __cplusplus
};
#endif