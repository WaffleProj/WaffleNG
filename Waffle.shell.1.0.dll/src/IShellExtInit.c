#include "../shell.h"

HRESULT STDMETHODCALLTYPE IWaffleShellExtInit_QueryInterface(
    _In_    IWaffleShellExtInit *this,
    _In_    REFIID riid,
    _In_    void **ppvObject
    )
{
    if (!IsEqualIID(riid, &IID_IUnknown) && !IsEqualIID(riid, &IID_IShellExtInit) && !IsEqualIID(riid, &IID_IWaffleShellExtInit))
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    *ppvObject = this;
    this->lpVtbl->AddRef(this);
    return S_OK;
}

ULONG STDMETHODCALLTYPE IWaffleShellExtInit_AddRef(
    _In_    IWaffleShellExtInit *this
    )
{
    return ((_IWaffleShellExtInit *)this)->IContextMenu->lpVtbl->AddRef((IWaffleContextMenu *)((_IWaffleShellExtInit *)this)->IContextMenu);
}

ULONG STDMETHODCALLTYPE IWaffleShellExtInit_Release(
    _In_    IWaffleShellExtInit *this
    )
{
    return ((_IWaffleShellExtInit *)this)->IContextMenu->lpVtbl->Release((IWaffleContextMenu *)((_IWaffleShellExtInit *)this)->IContextMenu);
}

HRESULT STDMETHODCALLTYPE IWaffleShellExtInit_Initialize(
    _In_    IWaffleShellExtInit *this,
    _In_    PCIDLIST_ABSOLUTE pidlFolder,
    _In_    IDataObject *pdtobj,
    _In_    HKEY hkeyProgID
    )
{
    FORMATETC formatetcIn = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM medium;

    pdtobj->lpVtbl->AddRef(pdtobj);

    if (SUCCEEDED(pdtobj->lpVtbl->GetData(pdtobj, &formatetcIn, &medium)))
    {
        LPTSTR lpszFileName;
        if (DragQueryFile((HDROP)medium.hGlobal, (UINT)-1, NULL, 0) == 1)
        {
            UINT cch = DragQueryFile((HDROP) medium.hGlobal, 0, NULL, 0) + 1;
            lpszFileName = GlobalAlloc(GPTR, cch * sizeof(lpszFileName[0]));
            if (lpszFileName)
            {
                DragQueryFile((HDROP) medium.hGlobal, 0, lpszFileName, cch);
            }
            DragFinish((HDROP) medium.hGlobal);
        }
        else
        {
            lpszFileName = NULL;
        }

        lpszFileName = InterlockedExchangePointer(&((_IWaffleShellExtInit *)this)->lpszFileName, lpszFileName);
        if (lpszFileName)
        {
            GlobalFree(lpszFileName);
        }

        ReleaseStgMedium(&medium);
    }

    pdtobj->lpVtbl->Release(pdtobj);

    return S_OK;
}

IWaffleShellExtInitVtbl IWaffleShellExtInit_Vtbl = {
    IWaffleShellExtInit_QueryInterface,
    IWaffleShellExtInit_AddRef,
    IWaffleShellExtInit_Release,
    IWaffleShellExtInit_Initialize,
};

_IWaffleShellExtInit IWaffleShellExtInitObject = { &IWaffleShellExtInit_Vtbl };