#include "../shell.h"

HRESULT STDMETHODCALLTYPE IWaffleShellPropSheetExt_QueryInterface(
    _In_    IWaffleShellPropSheetExt *this,
    _In_    REFIID riid,
    _In_    void **ppvObject
)
{
    if (!IsEqualIID(riid, &IID_IUnknown) && !IsEqualIID(riid, &IID_IShellPropSheetExt) && !IsEqualIID(riid, &IID_IWaffleShellPropSheetExt))
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    *ppvObject = this;
    this->lpVtbl->AddRef(this);
    return S_OK;
}

ULONG STDMETHODCALLTYPE IWaffleShellPropSheetExt_AddRef(
    _In_    IWaffleShellPropSheetExt *this
)
{
    InterlockedIncrement(&((_IWaffleShellPropSheetExt *) this)->nRef);

    return ((_IWaffleShellPropSheetExt *) this)->nRef;
}

ULONG STDMETHODCALLTYPE IWaffleShellPropSheetExt_Release(
    _In_    IWaffleShellPropSheetExt *this
)
{
    InterlockedDecrement(&((_IWaffleShellPropSheetExt *) this)->nRef);

    return ((_IWaffleShellPropSheetExt *) this)->nRef;
}