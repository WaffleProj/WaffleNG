#ifndef __SDK_WAFFLE_PSDK_GDI32_H_
#define __SDK_WAFFLE_PSDK_GDI32_H_

typedef int (WINAPI *LPENUMFONTSA)(
    _In_    HDC hdc,
    _In_    LPCSTR lpFaceName,
    _In_    FONTENUMPROCA lpFontFunc,
    _In_    LPARAM lParam
    );

typedef int (WINAPI *LPENUMFONTFAMILIESA)(
    _In_    HDC hdc,
    _In_    LPCSTR lpszFamily,
    _In_    FONTENUMPROCA lpEnumFontFamProc,
    _In_    LPARAM lParam
    );

typedef int (WINAPI *LPENUMFONTFAMILIESEXA)(
    _In_    HDC hdc,
    _In_    LPLOGFONTA lpLogfont,
    _In_    FONTENUMPROCA lpEnumFontFamExProc,
    _In_    LPARAM lParam,
    _In_    DWORD dwFlags
    );

typedef int (WINAPI *LPENUMFONTFAMILIESEXW)(
    _In_    HDC hdc,
    _In_    LPLOGFONTW lpLogfont,
    _In_    FONTENUMPROCW lpEnumFontFamExProc,
    _In_    LPARAM lParam,
    _In_    DWORD dwFlags
    );

typedef int (WINAPI *LPENUMFONTSW)(
    _In_    HDC hdc,
    _In_    LPCWSTR lpFaceName,
    _In_    FONTENUMPROCW lpFontFunc,
    _In_    LPARAM lParam
    );

typedef HFONT(WINAPI *LPCREATEFONTA)(
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
    );

typedef HFONT(WINAPI *LPCREATEFONTW)(
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
    );

typedef HFONT(WINAPI *LPCREATEFONTINDIRECTA)(
    _In_    const LOGFONTA *lplf
    );

typedef HFONT(WINAPI *LPCREATEFONTINDIRECTW)(
    _In_    const LOGFONTW *lplf
    );

typedef HFONT(WINAPI *LPCREATEFONTINDIRECTEXA)(
    _In_    const ENUMLOGFONTEXDVA *penumlfex
    );

typedef HFONT(WINAPI *LPCREATEFONTINDIRECTEXW)(
    _In_    const ENUMLOGFONTEXDVW *penumlfex
    );

typedef BOOL(WINAPI *LPTEXTOUTA)(
    _In_    HDC hdc,
    _In_    int nXStart,
    _In_    int nYStart,
    _In_    LPCSTR lpString,
    _In_    int cchString
    );

#endif /* __SDK_WAFFLE_PSDK_GDI32_H_ */