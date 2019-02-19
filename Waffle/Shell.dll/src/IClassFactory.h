#ifndef _WAFFLE_SHELL_ICLASSFACTORY_H_
#define _WAFFLE_SHELL_ICLASSFACTORY_H_

// {70325977-3B4F-42DF-9A25-9C246070A8CF}
DEFINE_GUID(CLSID_IWaffleClassFactory,
            0x70325977, 0x3b4f, 0x42df, 0x9a, 0x25, 0x9c, 0x24, 0x60, 0x70, 0xa8, 0xcf);
// {3D3EBB0E-5905-495F-9AE4-11684214B875}
DEFINE_GUID(IID_IWaffleClassFactory,
            0x3d3ebb0e, 0x5905, 0x495f, 0x9a, 0xe4, 0x11, 0x68, 0x42, 0x14, 0xb8, 0x75);

#undef  INTERFACE
#define INTERFACE IWaffleClassFactory
DECLARE_INTERFACE_(INTERFACE, IClassFactory)
{
    STDMETHOD(QueryInterface) (THIS_ REFIID, void **) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)PURE;
    STDMETHOD_(ULONG, Release) (THIS)PURE;
    STDMETHOD(CreateInstance) (THIS_ IUnknown *, REFIID, void **) PURE;
    STDMETHOD(LockServer) (THIS_ BOOL) PURE;
};

typedef struct __IWaffleClassFactory
{
    IWaffleClassFactoryVtbl *lpVtbl;
    volatile LONG refClassFactory;
    volatile LONG refContextMenu;
    volatile LONG nLock;
} _IWaffleClassFactory;

#endif // _WAFFLE_SHELL_ICLASSFACTORY_H_