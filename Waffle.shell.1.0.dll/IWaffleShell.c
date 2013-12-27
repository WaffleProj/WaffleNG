#include "shell.h"

HRESULT STDAPICALLTYPE IWaffleShell_QueryInterface(
    _In_    IWaffleShell *this,
    _In_    REFIID riid,
    _In_    void **ppvObject
    )
{
    if (!IsEqualIID(riid, &IID_IUnknown) && !IsEqualIID(riid, &IID_IWaffleShell))
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    *ppvObject = this;
    this->lpVtbl->AddRef(this);
    return S_OK;
}

ULONG STDAPICALLTYPE IWaffleShell_AddRef(
    _In_    IWaffleShell *this
    )
{
    ((_IWaffleShell *)this)->count++;

    return ((_IWaffleShell *)this)->count;
}

ULONG STDAPICALLTYPE IWaffleShell_Release(
    _In_    IWaffleShell *this
    )
{
    ((_IWaffleShell *)this)->count--;

    if (((_IWaffleShell *)this)->count == 0)
    {
        GlobalFree(this);
        InterlockedDecrement(&nIWaffleShell);
        return(0);
    }
    return ((_IWaffleShell *)this)->count;
}

IWaffleShellVtbl IWaffleShell_Vtbl = {
    IWaffleShell_QueryInterface,
    IWaffleShell_AddRef,
    IWaffleShell_Release,
};

IWaffleShell IWaffleShellObject = { &IWaffleShell_Vtbl };