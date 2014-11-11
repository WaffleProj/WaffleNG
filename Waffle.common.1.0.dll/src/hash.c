#include "..\common.h"
#include <Wincrypt.h>

typedef struct
{
    HMODULE hModule;

    LPCRYPTACQUIRECONTEXTW CryptAcquireContextW;
    LPCRYPTCREATEHASH CryptCreateHash;
    LPCRYPTDESTROYHASH CryptDestroyHash;
    LPCRYPTRELEASECONTEXT CryptReleaseContext;
    LPCRYPTHASHDATA CryptHashData;
    LPCRYPTGETHASHPARAM CryptGetHashParam;

    HCRYPTPROV hCryptProv;

    HCRYPTHASH hCryptHash;
} WAFFLE_HASH, *LPWAFFLE_HASH;

LIBRARY_EXPORT VOID WINAPI WaffleDestroyHash(
    _In_    LPWAFFLE_HASH hHash
    )
{
    if (hHash)
    {
        if (hHash->hModule)
        {
            if (hHash->CryptDestroyHash && hHash->hCryptHash)
            {
                hHash->CryptDestroyHash(hHash->hCryptHash);
            }

            if (hHash->CryptReleaseContext && hHash->hCryptProv)
            {
                hHash->CryptReleaseContext(hHash->hCryptProv, 0);
            }

            FreeLibrary(hHash->hModule);
        }

        WaffleFree(hHash);
    }
}

LIBRARY_EXPORT LPWAFFLE_HASH WINAPI WaffleCreateHash(void)
{
    LPWAFFLE_HASH hHash = WaffleAlloc(sizeof(*hHash));
    if (!hHash)
    {
        WaffleDestroyHash(hHash);
        return NULL;
    }

    hHash->hModule = LoadLibrary(TEXT("advapi32.dll"));
    if (!hHash->hModule)
    {
        WaffleDestroyHash(hHash);
        return NULL;
    }
    hHash->CryptAcquireContextW = (LPCRYPTACQUIRECONTEXTW) WaffleGetProcAddress(hHash->hModule, TEXT("CryptAcquireContextW"));
    hHash->CryptCreateHash = (LPCRYPTCREATEHASH) WaffleGetProcAddress(hHash->hModule, TEXT("CryptCreateHash"));
    hHash->CryptDestroyHash = (LPCRYPTDESTROYHASH) WaffleGetProcAddress(hHash->hModule, TEXT("CryptDestroyHash"));
    hHash->CryptReleaseContext = (LPCRYPTRELEASECONTEXT) WaffleGetProcAddress(hHash->hModule, TEXT("CryptReleaseContext"));
    hHash->CryptHashData = (LPCRYPTHASHDATA) WaffleGetProcAddress(hHash->hModule, TEXT("CryptHashData"));
    hHash->CryptGetHashParam = (LPCRYPTGETHASHPARAM) WaffleGetProcAddress(hHash->hModule, TEXT("CryptGetHashParam"));

    if (!hHash->CryptAcquireContextW(&hHash->hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        WaffleDestroyHash(hHash);
        return NULL;
    }

    if (!hHash->CryptCreateHash(hHash->hCryptProv, CALG_SHA1, 0, 0, &hHash->hCryptHash))
    {
        WaffleDestroyHash(hHash);
        return NULL;
    }

    return hHash;
}

LIBRARY_EXPORT BOOL WINAPI WaffleFeedHash(
    _In_    LPWAFFLE_HASH hHash,
    _In_    LPBYTE lpBuffer,
    _In_    DWORD nNumberOfBytesRead
    )
{
    return hHash->CryptHashData(hHash->hCryptHash, lpBuffer, nNumberOfBytesRead, 0);
}

LIBRARY_EXPORT BOOL WINAPI WaffleGetHashString(
    _In_    LPWAFFLE_HASH hHash,
    _Out_   LPTSTR lpszResult
    )
{
    lpszResult[0] = TEXT('\0');

    BYTE rgbHash[WAFFLE_HASH_LENGTH];      //SHA1LEN == 20
    DWORD cbHash = WAFFLE_HASH_LENGTH;     //MD5LEN == 16
    if (hHash->CryptGetHashParam(hHash->hCryptHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {
        TCHAR rgbDigits[] = TEXT("0123456789ABCDEF");
        DWORD i, n;
        for (i = 0, n = 0; i < cbHash; i++)
        {
            lpszResult[n++] = rgbDigits[rgbHash[i] >> 4];
            lpszResult[n++] = rgbDigits[rgbHash[i] & 0xf];
        }
        lpszResult[n++] = 0;
        return TRUE;
    }
    return FALSE;
}

LIBRARY_EXPORT VOID WINAPI WaffleGetFileHash(
    _In_    LPCTSTR lpszFile,
    _Out_   LPTSTR lpszResult
    )
{
    LPWAFFLE_HASH hHash = WaffleCreateHash();

    if (!hHash)
    {
        return;
    }
#ifdef WAFFLE_FILE_HASH
    PVOID OldValue = 0;
    WaffleDisableWow64FsRedirection(&OldValue);
    HANDLE hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    WaffleRevertWow64FsRedirection(OldValue);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        WaffleDestroyHash(hHash);
        return;
    }

    LPBYTE lpBuffer = WaffleAlloc(1024);   //BUFSIZE == 1024
    if (!lpBuffer)
    {
        CloseHandle(hFile);
        WaffleDestroyHash(hHash);
        return;
    }

    DWORD nNumberOfBytesRead;
    while (ReadFile(hFile, lpBuffer, 1024, &nNumberOfBytesRead, NULL))   //BUFSIZE == 1024
    {
        if (nNumberOfBytesRead == 0)
        {
            break;
        }

        if (!WaffleFeedHash(hHash, lpBuffer, nNumberOfBytesRead))
        {
            WaffleFree(lpBuffer);
            CloseHandle(hFile);
            WaffleDestroyHash(hHash);
            return;
        }
    }

    WaffleFree(lpBuffer);
    CloseHandle(hFile);
#else
    if (!WaffleFeedHash(hHash, (LPBYTE) lpszFile, Wafflelstrlen(lpszFile) * sizeof(lpszFile[0])))
    {
        WaffleDestroyHash(hHash);
        return;
    }
#endif
    WaffleGetHashString(hHash, lpszResult);

    WaffleDestroyHash(hHash);
}