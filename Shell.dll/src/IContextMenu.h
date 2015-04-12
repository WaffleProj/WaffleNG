#ifndef _WAFFLE_SHELL_ICONTEXTMENU_H_
#define _WAFFLE_SHELL_ICONTEXTMENU_H_

#include "IShellExtInit.h"

// {C3B16F86-68FD-4F32-BF53-975180752E04}
DEFINE_GUID(CLSID_IWaffleContextMenu,
    0xc3b16f86, 0x68fd, 0x4f32, 0xbf, 0x53, 0x97, 0x51, 0x80, 0x75, 0x2e, 0x4);
// {D1CCA9BB-3C00-4EB7-A8F9-1F822CDE30A9}
DEFINE_GUID(IID_IWaffleContextMenu,
    0xd1cca9bb, 0x3c00, 0x4eb7, 0xa8, 0xf9, 0x1f, 0x82, 0x2c, 0xde, 0x30, 0xa9);

#undef  INTERFACE
#define INTERFACE IWaffleContextMenu
DECLARE_INTERFACE_(INTERFACE, IContextMenu)
{
    STDMETHOD(QueryInterface) (THIS_ REFIID, void **) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)PURE;
    STDMETHOD_(ULONG, Release) (THIS)PURE;
    STDMETHOD(QueryContextMenu) (THIS_ HMENU, UINT, UINT, UINT, UINT) PURE;
    STDMETHOD(InvokeCommand) (THIS_ LPCMINVOKECOMMANDINFO) PURE;
    STDMETHOD(GetCommandString) (THIS_ UINT_PTR, UINT, UINT *, LPSTR, UINT) PURE;
};

typedef struct __IWaffleShellExtInit _IWaffleShellExtInit;
typedef struct __IWaffleContextMenu
{
    IWaffleContextMenuVtbl *lpVtbl;
    volatile LONG nRef;
    _IWaffleShellExtInit IShellExtInit;
} _IWaffleContextMenu;

#define IDM_WAFFLE_OPEN     0x0000
#define IDM_WAFFLE_SETTING  0x0001
#define IDM_WAFFLE_LAST     IDM_WAFFLE_SETTING

#endif // _WAFFLE_SHELL_ICONTEXTMENU_H_