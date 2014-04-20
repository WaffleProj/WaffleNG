#ifndef __MOJIBAKE_MAIN_MOJIBAKE_H_
#define __MOJIBAKE_MAIN_MOJIBAKE_H_

#define WAFFLE_COMPONENT_BUILD
#include <waffle.h>
#include "src\rtl.h"
#include "src\ttf.h"
#include "src\subclass.h"

typedef struct
{
    UINT    AnsiCodePage;
    UINT    OemCodePage;
    WORD    NlsAnsiCodePage;
    LCID    ThreadLocale;
    BYTE    DefaultCharSet;
} ENVIRONMENT_SETTING, *LPENVIRONMENT_SETTING;

typedef struct _ENUM_FONTS_FILTER_DATAA
{
    FONTENUMPROCA   lpFontFunc;
    LPARAM          lpData;
    HDC             hDC;
} ENUM_FONTS_FILTER_DATAA, *LPENUM_FONTS_FILTER_DATAA;

typedef struct _ENUM_FONTS_FILTER_DATAW
{
    FONTENUMPROCW   lpFontFunc;
    LPARAM          lpData;
    HDC             hDC;
} ENUM_FONTS_FILTER_DATAW, *LPENUM_FONTS_FILTER_DATAW;

extern ENVIRONMENT_SETTING  stOldEnvir;
extern ENVIRONMENT_SETTING  stNewEnvir;

#endif /* __MOJIBAKE_MAIN_MOJIBAKE_H_ */