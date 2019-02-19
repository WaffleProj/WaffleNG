#ifndef _WAFFLE_SHELL_ISHELLPROPSHEETEXT_H_
#define _WAFFLE_SHELL_ISHELLPROPSHEETEXT_H_

// {5E941F61-5199-4821-920B-E3457F8C3043}
DEFINE_GUID(CLSID_IWaffleShellPropSheetExt,
            0x5e941f61, 0x5199, 0x4821, 0x92, 0xb, 0xe3, 0x45, 0x7f, 0x8c, 0x30, 0x43);
// {D76BE977-7BC3-4730-9521-AA604D3D522B}
DEFINE_GUID(IID_IWaffleShellPropSheetExt,
            0xd76be977, 0x7bc3, 0x4730, 0x95, 0x21, 0xaa, 0x60, 0x4d, 0x3d, 0x52, 0x2b);

#undef  INTERFACE
#define INTERFACE IWaffleShellPropSheetExt
DECLARE_INTERFACE_(INTERFACE, IShellPropSheetExt)
{
    STDMETHOD(QueryInterface) (THIS, REFIID, void **) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)PURE;
    STDMETHOD_(ULONG, Release) (THIS)PURE;
    STDMETHOD(AddPages) (THIS, LPFNSVADDPROPSHEETPAGE, LPARAM) PURE;
    STDMETHOD(ReplacePage) (THIS, EXPPS, LPFNSVADDPROPSHEETPAGE, LPARAM) PURE;
};

typedef struct __IWaffleShellPropSheetExt _IWaffleShellPropSheetExt;
typedef struct __IWaffleShellPropSheetExt
{
    IWaffleShellPropSheetExtVtbl *lpVtbl;
    volatile LONG nRef;
} _IWaffleShellPropSheetExt;

#endif // _WAFFLE_SHELL_ISHELLPROPSHEETEXT_H_