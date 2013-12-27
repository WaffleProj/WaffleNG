#include "shell.h"

HRESULT STDAPICALLTYPE IClassFactory_QueryInterface(
    _In_    IClassFactory *this,
    _In_    REFIID riid,
    _In_    void **ppvObject
    )
{
    if (!IsEqualIID(riid, &IID_IUnknown) && !IsEqualIID(riid, &IID_IClassFactory))
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
    
    *ppvObject = this;
    this->lpVtbl->AddRef(this);
    return S_OK;
}

ULONG STDAPICALLTYPE IClassFactory_AddRef(
    _In_    IClassFactory *this
    )
{
    InterlockedIncrement(&nIClassFactory);

    return nIClassFactory;
}

ULONG STDAPICALLTYPE IClassFactory_Release(
    _In_    IClassFactory *this
    )
{
    InterlockedDecrement(&nIClassFactory);

    return nIClassFactory;
}

HRESULT STDAPICALLTYPE IClassFactory_CreateInstance(
    _In_    IClassFactory *this,
    _In_    IUnknown *pUnkOuter,
    _In_    REFIID riid,
    _In_    void **ppvObject
    )
{
    HRESULT hr;
    IWaffleShell *IWaffleShellPointer;

    *ppvObject = NULL;

    // We don't support aggregation in this example
    if (pUnkOuter)
    {
        hr = CLASS_E_NOAGGREGATION;
    }
    else
    {
        IWaffleShellPointer = (IWaffleShell *)GlobalAlloc(GPTR, sizeof(_IWaffleShell));
        if (!IWaffleShellPointer)
        {
            hr = E_OUTOFMEMORY;
        }
        else
        {
            IWaffleShellPointer->lpVtbl = IWaffleShellObject.lpVtbl;
            ((_IWaffleShell *)IWaffleShellPointer)->count = 1;

            // Fill in the caller's handle with a pointer to the IWaffleShell we just
            // allocated above. We'll let IWaffleShell's QueryInterface do that, because
            // it also checks the GUID the caller passed, and also increments the
            // reference count (to 2) if all goes well
            hr = IWaffleShellPointer->lpVtbl->QueryInterface(IWaffleShellPointer, riid, ppvObject);

            // Decrement reference count. NOTE: If there was an error in QueryInterface()
            // then Release() will be decrementing the count back to 0 and will free the
            // IWaffleShell for us. One error that may occur is that the caller is asking for
            // some sort of object that we don't support (ie, it's a GUID we don't recognize)
            IWaffleShellPointer->lpVtbl->Release(IWaffleShellPointer);

            // If success, inc static object count to keep this DLL loaded
            if (SUCCEEDED(hr))
            {
                InterlockedIncrement(&nIWaffleShell);
            }
        }
    }

    return(hr);
}

// IClassFactory's LockServer(). It is called by someone
// who wants to lock this DLL in memory
HRESULT STDAPICALLTYPE IClassFactory_LockServer(
    _In_    IClassFactory *this,
    _In_    BOOL fLock
    )
{
    if (fLock)
    {
        InterlockedIncrement(&nIClassFactoryLock);
    }
    else
    {
        InterlockedDecrement(&nIClassFactoryLock);
    }

    return S_OK;
}

IClassFactoryVtbl IClassFactory_Vtbl = {
    IClassFactory_QueryInterface,
    IClassFactory_AddRef,
    IClassFactory_Release,
    IClassFactory_CreateInstance,
    IClassFactory_LockServer
};

IClassFactory IClassFactoryObject = { &IClassFactory_Vtbl };