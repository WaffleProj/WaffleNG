#ifndef __MOJIBAKE_MAIN_MOJIBAKE_H_
#define __MOJIBAKE_MAIN_MOJIBAKE_H_

#define WAFFLE_COMPONENT_BUILD
#include <waffle.h>
#include "src\rtl.h"

typedef struct
{
    UINT    AnsiCodePage;
    UINT    OemCodePage;
    WORD    NlsAnsiCodePage;
    LCID    ThreadLocale;
    BYTE    DefaultCharSet;
} ENVIRONMENT_SETTING, *LPENVIRONMENT_SETTING;

extern ENVIRONMENT_SETTING  stOldEnvir;
extern ENVIRONMENT_SETTING  stNewEnvir;

extern LOGFONTA lfGothic;
extern TEXTMETRICA tmGothic;
extern DWORD dwGothic;

#endif /* __MOJIBAKE_MAIN_MOJIBAKE_H_ */