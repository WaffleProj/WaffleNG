#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\mojibake.h"

#ifdef __cplusplus
extern "C" {
#endif
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
    HFONT Result = CreateFont(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpuszFace);

    KeepLastErrorAndFree(lpuszFace);
    return Result;
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
#ifdef __cplusplus
};
#endif