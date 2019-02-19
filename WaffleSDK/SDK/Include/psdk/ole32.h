#ifndef __SDK_WAFFLE_PSDK_OLE32_H_
#define __SDK_WAFFLE_PSDK_OLE32_H_

typedef HRESULT(WINAPI *LPCOCREATEINSTANCE)(
    _In_        REFCLSID rclsid,
    _In_opt_    LPUNKNOWN pUnkOuter,
    _In_        DWORD dwClsContext,
    _In_        REFIID riid,
    _Out_       LPVOID *ppv
    );

#endif /* __SDK_WAFFLE_PSDK_OLE32_H_ */