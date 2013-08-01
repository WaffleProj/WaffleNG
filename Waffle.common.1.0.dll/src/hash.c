#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"
#include <Wincrypt.h>

LIBRARY_EXPORT VOID WINAPI WaffleGetFileHash(
    _In_    LPCTSTR lpszFile,
    _Out_   LPTSTR lpszResult
    )
{
    static LPCRYPTACQUIRECONTEXTW lpCryptAcquireContextW;
    static LPCRYPTCREATEHASH lpCryptCreateHash;
    static LPCRYPTDESTROYHASH lpCryptDestroyHash;
    static LPCRYPTRELEASECONTEXT lpCryptReleaseContext;
    static LPCRYPTHASHDATA lpCryptHashData;
    static LPCRYPTGETHASHPARAM lpCryptGetHashParam;
    if (!lpCryptAcquireContextW)
    {
        HMODULE hModule = LoadLibrary(TEXT("advapi32.dll"));
        lpCryptAcquireContextW = (LPCRYPTACQUIRECONTEXTW) WaffleGetProcAddress(hModule, TEXT("CryptAcquireContextW"));
        lpCryptCreateHash = (LPCRYPTCREATEHASH) WaffleGetProcAddress(hModule, TEXT("CryptCreateHash"));
        lpCryptDestroyHash = (LPCRYPTDESTROYHASH) WaffleGetProcAddress(hModule, TEXT("CryptDestroyHash"));
        lpCryptReleaseContext = (LPCRYPTRELEASECONTEXT) WaffleGetProcAddress(hModule, TEXT("CryptReleaseContext"));
        lpCryptHashData = (LPCRYPTHASHDATA) WaffleGetProcAddress(hModule, TEXT("CryptHashData"));
        lpCryptGetHashParam = (LPCRYPTGETHASHPARAM) WaffleGetProcAddress(hModule, TEXT("CryptGetHashParam"));
    }

    lpszResult[0] = 0;

    PVOID OldValue = 0;
    WaffleDisableWow64FsRedirection(&OldValue);
    HANDLE hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    WaffleRevertWow64FsRedirection(OldValue);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }

    HCRYPTPROV hProv;
    if (!lpCryptAcquireContextW(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        CloseHandle(hFile);
        return;
    }

    HCRYPTHASH hHash;
    if (!lpCryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
    {
        CloseHandle(hFile);
        lpCryptReleaseContext(hProv, 0);
        return;
    }

    LPVOID lpBuffer = GlobalAlloc(GPTR, 1024);   //BUFSIZE == 1024
    if (!lpBuffer)
    {
        lpCryptDestroyHash(hHash);
        lpCryptReleaseContext(hProv, 0);
        CloseHandle(hFile);
        return;
    }

    DWORD nNumberOfBytesRead;
    while (ReadFile(hFile, lpBuffer, 1024, &nNumberOfBytesRead, NULL))   //BUFSIZE == 1024
    {
        if (nNumberOfBytesRead == 0)
        {
            break;
        }

        if (!lpCryptHashData(hHash, (BYTE *) lpBuffer, nNumberOfBytesRead, 0))
        {
            lpCryptReleaseContext(hProv, 0);
            lpCryptDestroyHash(hHash);
            CloseHandle(hFile);
            return;
        }
    }

    BYTE rgbHash[WAFFLE_HASH_LENGTH];      //SHA1LEN == 20
    DWORD cbHash = WAFFLE_HASH_LENGTH;     //MD5LEN == 16
    if (lpCryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {
        TCHAR rgbDigits [] = TEXT("0123456789ABCDEF");
        DWORD i, n;
        for (i = 0, n = 0; i < cbHash; i++)
        {
            lpszResult[n++] = rgbDigits[rgbHash[i] >> 4];
            lpszResult[n++] = rgbDigits[rgbHash[i] & 0xf];
        }
        lpszResult[n++] = 0;
    }

    GlobalFree(lpBuffer);
    lpCryptDestroyHash(hHash);
    lpCryptReleaseContext(hProv, 0);
    CloseHandle(hFile);
}