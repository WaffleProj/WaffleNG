#ifndef __MOJIBAKE_MAIN_MOJIBAKE_H_
#define __MOJIBAKE_MAIN_MOJIBAKE_H_

#define WAFFLE_COMPONENT_BUILD
#include <waffle.h>
#include "src\rtl.h"
#include "src\subclass.h"

typedef struct
{
    UINT    AnsiCodePage;
    UINT    OemCodePage;
    WORD    NlsAnsiCodePage;
    LCID    ThreadLocale;
    BYTE    DefaultCharSet;
} ENVIRONMENT_SETTING, *LPENVIRONMENT_SETTING;

typedef struct
{
    FONTENUMPROCA   lpFontFunc;
    LPARAM          lpData;
} ENUM_FONTS_FILTER_DATA, *LPENUM_FONTS_FILTER_DATA;

extern ENVIRONMENT_SETTING  stOldEnvir;
extern ENVIRONMENT_SETTING  stNewEnvir;

#endif /* __MOJIBAKE_MAIN_MOJIBAKE_H_ */