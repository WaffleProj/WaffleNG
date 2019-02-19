#ifndef __SDK_WAFFLE_PSDK_ADVAPI32_H_
#define __SDK_WAFFLE_PSDK_ADVAPI32_H_

#ifdef  UNICODE
#define LPCRYPTACQUIRECONTEXT LPCRYPTACQUIRECONTEXTW
#else
#define LPCRYPTACQUIRECONTEXT LPCRYPTACQUIRECONTEXTA
#endif

typedef BOOL(WINAPI *LPCRYPTACQUIRECONTEXTW)(
    _Out_       HCRYPTPROV *phProv,
    _In_opt_    LPCWSTR pszContainer,
    _In_opt_    LPCWSTR pszProvider,
    _In_        DWORD dwProvType,
    _In_        DWORD dwFlags
    );

typedef BOOL(WINAPI *LPCRYPTACQUIRECONTEXTA)(
    _Out_   HCRYPTPROV *phProv,
    _In_    LPCSTR pszContainer,
    _In_    LPCSTR pszProvider,
    _In_    DWORD dwProvType,
    _In_    DWORD dwFlags
    );

typedef BOOL(WINAPI *LPCRYPTCREATEHASH)(
    _In_    HCRYPTPROV hProv,
    _In_    ALG_ID Algid,
    _In_    HCRYPTKEY hKey,
    _In_    DWORD dwFlags,
    _Out_   HCRYPTHASH *phHash
    );

typedef BOOL(WINAPI *LPCRYPTDESTROYHASH)(
    _In_  HCRYPTHASH hHash
    );

typedef BOOL(WINAPI *LPCRYPTRELEASECONTEXT)(
    _In_  HCRYPTPROV hProv,
    _In_  DWORD dwFlags
    );

typedef BOOL(WINAPI *LPCRYPTHASHDATA)(
    _In_  HCRYPTHASH hHash,
    _In_  BYTE *pbData,
    _In_  DWORD dwDataLen,
    _In_  DWORD dwFlags
    );

typedef BOOL(WINAPI *LPCRYPTGETHASHPARAM)(
    _In_     HCRYPTHASH hHash,
    _In_     DWORD dwParam,
    _Out_    BYTE *pbData,
    _Inout_  DWORD *pdwDataLen,
    _In_     DWORD dwFlags
    );

#endif /* __SDK_WAFFLE_PSDK_ADVAPI32_H_ */