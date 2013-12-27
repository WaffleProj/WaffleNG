#ifndef _WAFFLE_SHELL_H_
#define _WAFFLE_SHELL_H_

#include <windows.h>
#include <objbase.h>
#include <initguid.h>

//IWaffleShell object's GUID
// {C3B16F86-68FD-4F32-BF53-975180752E04}
DEFINE_GUID(CLSID_IWaffleShell,
    0xc3b16f86, 0x68fd, 0x4f32, 0xbf, 0x53, 0x97, 0x51, 0x80, 0x75, 0x2e, 0x4);

// IWaffleShell VTable's GUID
// {D1CCA9BB-3C00-4EB7-A8F9-1F822CDE30A9}
DEFINE_GUID(IID_IWaffleShell,
    0xd1cca9bb, 0x3c00, 0x4eb7, 0xa8, 0xf9, 0x1f, 0x82, 0x2c, 0xde, 0x30, 0xa9);

#undef  INTERFACE
#define INTERFACE IWaffleShell
DECLARE_INTERFACE_(INTERFACE, IUnknown)
{
    STDMETHOD(QueryInterface) (THIS_ REFIID, void **) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)PURE;
    STDMETHOD_(ULONG, Release) (THIS)PURE;
};

typedef struct
{
    IWaffleShellVtbl *lpVtbl;
    DWORD count;
    TCHAR buffer[80];
    volatile LONG nIWaffleShell;
} _IWaffleShell;

extern volatile LONG nIClassFactory;
extern volatile LONG nIClassFactoryLock;
extern volatile LONG nIWaffleShell;

extern IClassFactory IClassFactoryObject;
extern IWaffleShell IWaffleShellObject;
HRESULT STDAPICALLTYPE IClassFactory_QueryInterface(
    _In_    IClassFactory *this,
    _In_    REFIID riid,
    _In_    void **ppvObject
    );

#endif // _WAFFLE_SHELL_H_
