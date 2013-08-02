#ifndef __MOJIBAKE_MAIN_MOJIBAKE_H_
#define __MOJIBAKE_MAIN_MOJIBAKE_H_

#define WAFFLE_COMPONENT_BUILD
#include <waffle.h>
#include "src\rtl.h"

extern HANDLE       hHeap;

typedef struct
{
    UINT    ANSICodePage;
    UINT    OEMCodePage;
    LCID    ThreadLocale;
    BYTE    DefaultCharSet;
} ENVIRONMENT_SETTING, *LPENVIRONMENT_SETTING;

extern ENVIRONMENT_SETTING  stOldEnvir;
extern ENVIRONMENT_SETTING  stNewEnvir;

#endif /* __MOJIBAKE_MAIN_MOJIBAKE_H_ */