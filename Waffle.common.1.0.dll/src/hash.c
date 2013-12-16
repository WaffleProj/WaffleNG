#include "..\common.h"
#include <Wincrypt.h>

LIBRARY_EXPORT VOID WINAPI WaffleGetFileHash(
    _In_    LPCTSTR lpszFile,
    _Out_   LPTSTR lpszResult
    )
{
    HMODULE hModule = LoadLibrary(TEXT("advapi32.dll"));
    LPCRYPTACQUIRECONTEXTW lpCryptAcquireContextW = (LPCRYPTACQUIRECONTEXTW) WaffleGetProcAddress(hModule, TEXT("CryptAcquireContextW"));
    LPCRYPTCREATEHASH lpCryptCreateHash = (LPCRYPTCREATEHASH) WaffleGetProcAddress(hModule, TEXT("CryptCreateHash"));
    LPCRYPTDESTROYHASH lpCryptDestroyHash = (LPCRYPTDESTROYHASH) WaffleGetProcAddress(hModule, TEXT("CryptDestroyHash"));
    LPCRYPTRELEASECONTEXT lpCryptReleaseContext = (LPCRYPTRELEASECONTEXT) WaffleGetProcAddress(hModule, TEXT("CryptReleaseContext"));
    LPCRYPTHASHDATA lpCryptHashData = (LPCRYPTHASHDATA) WaffleGetProcAddress(hModule, TEXT("CryptHashData"));
    LPCRYPTGETHASHPARAM lpCryptGetHashParam = (LPCRYPTGETHASHPARAM) WaffleGetProcAddress(hModule, TEXT("CryptGetHashParam"));

    lpszResult[0] = 0;

    PVOID OldValue = 0;
    WaffleDisableWow64FsRedirection(&OldValue);
    HANDLE hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    WaffleRevertWow64FsRedirection(OldValue);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        FreeLibrary(hModule);
        return;
    }

    HCRYPTPROV hProv;
    if (!lpCryptAcquireContextW(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        FreeLibrary(hModule);
        CloseHandle(hFile);
        return;
    }

    HCRYPTHASH hHash;
    if (!lpCryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
    {
        lpCryptReleaseContext(hProv, 0);
        FreeLibrary(hModule);
        CloseHandle(hFile);
        return;
    }

    LPVOID lpBuffer = WaffleAlloc(1024);   //BUFSIZE == 1024
    if (!lpBuffer)
    {
        lpCryptDestroyHash(hHash);
        lpCryptReleaseContext(hProv, 0);
        FreeLibrary(hModule);
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
            FreeLibrary(hModule);
            CloseHandle(hFile);
            return;
        }
    }

    BYTE rgbHash[WAFFLE_HASH_LENGTH];      //SHA1LEN == 20
    DWORD cbHash = WAFFLE_HASH_LENGTH;     //MD5LEN == 16
    if (lpCryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {
        TCHAR rgbDigits[] = TEXT("0123456789ABCDEF");
        DWORD i, n;
        for (i = 0, n = 0; i < cbHash; i++)
        {
            lpszResult[n++] = rgbDigits[rgbHash[i] >> 4];
            lpszResult[n++] = rgbDigits[rgbHash[i] & 0xf];
        }
        lpszResult[n++] = 0;
    }

    WaffleFree(lpBuffer);
    lpCryptDestroyHash(hHash);
    lpCryptReleaseContext(hProv, 0);
    FreeLibrary(hModule);
    CloseHandle(hFile);
}