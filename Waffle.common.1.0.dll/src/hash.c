#define  UNICODE
#define  _UNICODE
#include "..\common.h"
#include <Wincrypt.h>

LIBRARY_EXPORT void WINAPI WaffleGetFileHash(
    _In_    LPTSTR lpszFile,
    _In_    LPTSTR lpszResult
    )
{
    lpszResult[0] = 0;

    HANDLE hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }

    HCRYPTPROV hProv;
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        CloseHandle(hFile);
        return;
    }

    HCRYPTHASH hHash;
    if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
    {
        CloseHandle(hFile);
        CryptReleaseContext(hProv, 0);
        return;
    }

    LPVOID lpBuffer = GlobalAlloc(GPTR, 1024);   //BUFSIZE == 1024
    if (!lpBuffer)
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
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

        if (!CryptHashData(hHash, lpBuffer, nNumberOfBytesRead, 0))
        {
            CryptReleaseContext(hProv, 0);
            CryptDestroyHash(hHash);
            CloseHandle(hFile);
            return;
        }
    }

    BYTE rgbHash[20];      //SHA1LEN == 20
    DWORD cbHash = 20;     //MD5LEN == 16
    if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {
        CHAR rgbDigitsU [] = "0123456789ABCDEF";
        DWORD i, n;
        for (i = 0, n = 0; i < cbHash; i++)
        {
            lpszResult[n++] = (TCHAR) (rgbDigitsU[rgbHash[i] >> 4]);
            lpszResult[n++] = (TCHAR) (rgbDigitsU[rgbHash[i] & 0xf]);
        }
        lpszResult[n++] = 0;
    }

    GlobalFree(lpBuffer);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);
}