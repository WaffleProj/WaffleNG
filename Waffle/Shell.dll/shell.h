#ifndef _WAFFLE_SHELL_H_
#define _WAFFLE_SHELL_H_

#include <waffle.h>
#include <Shlwapi.h>
#include <strsafe.h>
#include <objbase.h>
#include <initguid.h>
#include <shobjidl.h>
#include "src/IClassFactory.h"
#include "src/IContextMenu.h"
#include "src/IShellExtInit.h"
#include "src/IShellPropSheetExt.h"
#include "src/OpenWithWaffle.h"

extern HINSTANCE hInstance;
extern _IWaffleClassFactory IWaffleClassFactoryObject;
extern _IWaffleContextMenu IWaffleContextMenuObject;
extern _IWaffleShellExtInit IWaffleShellExtInitObject;

VOID PrintGuid(
    _In_        REFIID riid,
    _In_opt_    LPCTSTR lpszTitle
);

#endif // _WAFFLE_SHELL_H_