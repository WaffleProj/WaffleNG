#include "../shell.h"

HRESULT STDMETHODCALLTYPE IWaffleContextMenu_QueryInterface(
    _In_    IWaffleContextMenu *this,
    _In_    REFIID riid,
    _In_    void **ppvObject
)
{
    HRESULT hr = ((_IWaffleContextMenu *) this)->IShellExtInit.lpVtbl->QueryInterface((IWaffleShellExtInit *)&((_IWaffleContextMenu *) this)->IShellExtInit, riid, ppvObject);
    if (SUCCEEDED(hr))
    {
        return hr;
    }
    else if (!IsEqualIID(riid, &IID_IUnknown) && !IsEqualIID(riid, &IID_IContextMenu) && !IsEqualIID(riid, &IID_IWaffleContextMenu))
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    *ppvObject = this;
    this->lpVtbl->AddRef(this);
    return S_OK;
}

ULONG STDMETHODCALLTYPE IWaffleContextMenu_AddRef(
    _In_    IWaffleContextMenu *this
)
{
    InterlockedIncrement(&((_IWaffleContextMenu *) this)->nRef);

    return ((_IWaffleContextMenu *) this)->nRef;
}

ULONG STDMETHODCALLTYPE IWaffleContextMenu_Release(
    _In_    IWaffleContextMenu *this
)
{
    InterlockedDecrement(&((_IWaffleContextMenu *) this)->nRef);

    if (!((_IWaffleContextMenu *) this)->nRef)
    {
        GlobalFree(this);
        InterlockedDecrement(&IWaffleClassFactoryObject.refContextMenu);
        return(0);
    }
    return ((_IWaffleContextMenu *) this)->nRef;
}

HRESULT STDMETHODCALLTYPE IWaffleContextMenu_GetCommandString(
    _In_    IWaffleContextMenu *this,
    _In_    UINT_PTR idCmd,
    _In_    UINT uFlags,
    _In_    UINT *pwReserved,
    _In_    LPSTR pszName,
    _In_    UINT cchMax
)
{
    HRESULT hr = E_INVALIDARG;

    if (idCmd == IDM_WAFFLE_OPEN)
    {
        switch (uFlags)
        {
        case GCS_HELPTEXTW:
        {
            hr = StringCchCopyW((LPWSTR) pszName, cchMax, L"Open this file with Waffle");
            break;
        }
        case GCS_VERBW:
        {
            hr = StringCchCopyW((LPWSTR) pszName, cchMax, L"waffle");
            break;
        }
        }
    }
    return hr;
}

HRESULT STDMETHODCALLTYPE IWaffleContextMenu_InvokeCommand(
    _In_    IWaffleContextMenu *this,
    _In_    LPCMINVOKECOMMANDINFO pici
)
{
    switch (LOWORD(pici->lpVerb))
    {
    case IDM_WAFFLE_OPEN:
    {
        LPTSTR lpszFileName = InterlockedExchangePointer(&((_IWaffleContextMenu *) this)->IShellExtInit.lpszFileName, NULL);
        MessageBox(pici->hwnd, lpszFileName, TEXT("Waffle"), MB_OK | MB_ICONINFORMATION);
        GlobalFree(lpszFileName);
        break;
    }
    case IDM_WAFFLE_SETTING:
    {
        MessageBox(pici->hwnd, TEXT("Setting"), TEXT("Waffle"), MB_OK | MB_ICONINFORMATION);
        break;
    }
    default:
    {
        return E_FAIL;
    }
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE IWaffleContextMenu_QueryContextMenu(
    _In_    IWaffleContextMenu *this,
    _In_    HMENU hmenu,
    _In_    UINT indexMenu,
    _In_    UINT idCmdFirst,
    _In_    UINT idCmdLast,
    _In_    UINT uFlags
)
{
    if (!(uFlags & CMF_DEFAULTONLY) && ((_IWaffleContextMenu *) this)->IShellExtInit.lpszFileName)
    {
        MENUITEMINFO mii;
        mii.cbSize = sizeof(mii);
        mii.fMask = MIIM_SUBMENU | MIIM_TYPE;
        mii.fType = MIIM_STRING;
        mii.dwTypeData = TEXT("&Waffle");
        mii.hSubMenu = CreatePopupMenu();

        AppendMenu(mii.hSubMenu, MF_STRING, idCmdFirst + IDM_WAFFLE_OPEN, TEXT("&Open"));
        AppendMenu(mii.hSubMenu, MF_STRING, idCmdFirst + IDM_WAFFLE_SETTING, TEXT("&Setting"));
        //InsertMenuItem(hmenu, 0, TRUE, &mii);
        InsertMenu(hmenu, indexMenu, MF_POPUP | MF_BYPOSITION, (UINT_PTR) mii.hSubMenu, TEXT("&Waffle"));

        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, IDM_WAFFLE_LAST + 1);
    }

    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, 0);
}

IWaffleContextMenuVtbl IWaffleContextMenu_Vtbl = {
    IWaffleContextMenu_QueryInterface,
    IWaffleContextMenu_AddRef,
    IWaffleContextMenu_Release,
    IWaffleContextMenu_QueryContextMenu,
    IWaffleContextMenu_InvokeCommand,
    IWaffleContextMenu_GetCommandString,
};

_IWaffleContextMenu IWaffleContextMenuObject = { &IWaffleContextMenu_Vtbl };