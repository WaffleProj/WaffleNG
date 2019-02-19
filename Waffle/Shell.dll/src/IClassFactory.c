#include "../shell.h"

HRESULT STDMETHODCALLTYPE IWaffleClassFactory_QueryInterface(
    _In_    IWaffleClassFactory *this,
    _In_    REFIID riid,
    _In_    void **ppvObject
    )
{
    if (!IsEqualIID(riid, &IID_IUnknown) && !IsEqualIID(riid, &IID_IClassFactory) && !IsEqualIID(riid, &IID_IWaffleClassFactory))
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
    
    *ppvObject = this;
    this->lpVtbl->AddRef(this);
    return S_OK;
}

ULONG STDMETHODCALLTYPE IWaffleClassFactory_AddRef(
    _In_    IWaffleClassFactory *this
    )
{
    InterlockedIncrement(&((_IWaffleClassFactory *)this)->refClassFactory);

    return ((_IWaffleClassFactory *)this)->refClassFactory;
}

ULONG STDMETHODCALLTYPE IWaffleClassFactory_Release(
    _In_    IWaffleClassFactory *this
    )
{
    InterlockedDecrement(&((_IWaffleClassFactory *)this)->refClassFactory);

    return ((_IWaffleClassFactory *)this)->refClassFactory;
}

HRESULT STDMETHODCALLTYPE IWaffleClassFactory_CreateInstance(
    _In_    IWaffleClassFactory *this,
    _In_    IUnknown *pUnkOuter,
    _In_    REFIID riid,
    _In_    void **ppvObject
    )
{
    HRESULT hr;

    *ppvObject = NULL;

    // We don't support aggregation in this example
    if (pUnkOuter)
    {
        hr = CLASS_E_NOAGGREGATION;
    }
    else
    {
        IWaffleContextMenu * IWaffleContextMenuPointer = (IWaffleContextMenu *)GlobalAlloc(GPTR, sizeof(_IWaffleContextMenu));
        if (!IWaffleContextMenuPointer)
        {
            hr = E_OUTOFMEMORY;
        }
        else
        {
            IWaffleContextMenuPointer->lpVtbl = IWaffleContextMenuObject.lpVtbl;
            ((_IWaffleContextMenu *)IWaffleContextMenuPointer)->nRef = 1;
            ((_IWaffleContextMenu *)IWaffleContextMenuPointer)->IShellExtInit.lpVtbl = IWaffleShellExtInitObject.lpVtbl;
            ((_IWaffleContextMenu *)IWaffleContextMenuPointer)->IShellExtInit.IContextMenu = (_IWaffleContextMenu *) IWaffleContextMenuPointer;

            hr = IWaffleContextMenuPointer->lpVtbl->QueryInterface(IWaffleContextMenuPointer, riid, ppvObject);

            IWaffleContextMenuPointer->lpVtbl->Release(IWaffleContextMenuPointer);

            if (SUCCEEDED(hr))
            {
                InterlockedIncrement(&((_IWaffleClassFactory *)this)->refContextMenu);
                return(hr);
            }
        }
    }

    return(hr);
}

// IWaffleClassFactory's LockServer(). It is called by someone
// who wants to lock this DLL in memory
HRESULT STDMETHODCALLTYPE IWaffleClassFactory_LockServer(
    _In_    IWaffleClassFactory *this,
    _In_    BOOL fLock
    )
{
    if (fLock)
    {
        InterlockedIncrement(&((_IWaffleClassFactory *)this)->nLock);
    }
    else
    {
        InterlockedDecrement(&((_IWaffleClassFactory *)this)->nLock);
    }

    return S_OK;
}

IWaffleClassFactoryVtbl IWaffleClassFactory_Vtbl = {
    IWaffleClassFactory_QueryInterface,
    IWaffleClassFactory_AddRef,
    IWaffleClassFactory_Release,
    IWaffleClassFactory_CreateInstance,
    IWaffleClassFactory_LockServer,
};

_IWaffleClassFactory IWaffleClassFactoryObject = { &IWaffleClassFactory_Vtbl };