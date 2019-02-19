#ifndef _WAFFLE_SHELL_ISHELLEXTINIT_H_
#define _WAFFLE_SHELL_ISHELLEXTINIT_H_

#include "IContextMenu.h"

// {708F82AB-686E-45E3-95F0-67EC36E93BD9}
DEFINE_GUID(CLSID_IWaffleShellExtInit,
            0x708f82ab, 0x686e, 0x45e3, 0x95, 0xf0, 0x67, 0xec, 0x36, 0xe9, 0x3b, 0xd9);
// {87A7047F-7100-47C6-B242-890E2E93804C}
DEFINE_GUID(IID_IWaffleShellExtInit,
            0x87a7047f, 0x7100, 0x47c6, 0xb2, 0x42, 0x89, 0xe, 0x2e, 0x93, 0x80, 0x4c);

#undef  INTERFACE
#define INTERFACE IWaffleShellExtInit
DECLARE_INTERFACE_(INTERFACE, IShellExtInit)
{
    STDMETHOD(QueryInterface) (THIS_ REFIID, void **) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)PURE;
    STDMETHOD_(ULONG, Release) (THIS)PURE;
    STDMETHOD(Initialize) (THIS_ PCIDLIST_ABSOLUTE, IDataObject *, HKEY) PURE;
};

typedef struct __IWaffleContextMenu _IWaffleContextMenu;
typedef struct __IWaffleShellExtInit
{
    IWaffleShellExtInitVtbl *lpVtbl;
    _IWaffleContextMenu * IContextMenu;
    volatile LPTSTR lpszFileName;
} _IWaffleShellExtInit;

#endif // _WAFFLE_SHELL_ISHELLEXTINIT_H_