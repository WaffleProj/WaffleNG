#ifndef __SDK_WAFFLE_PSDK_COMCTL32_H_
#define __SDK_WAFFLE_PSDK_COMCTL32_H_

#include <prsht.h>

typedef INT_PTR(WINAPI *LPPROPERTYSHEETW)(
    _In_    LPCPROPSHEETHEADERW lppsph
    );

typedef HPROPSHEETPAGE(WINAPI *LPCREATEPROPERTYSHEETPAGEW)(
    _In_    LPCPROPSHEETPAGEW lppsp
    );

#endif /* __SDK_WAFFLE_PSDK_COMCTL32_H_ */