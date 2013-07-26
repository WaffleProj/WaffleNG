#define  UNICODE
#define  _UNICODE
#include "..\common.h"
#include <Wincrypt.h>

#define CHECK_NULL_RET(bCondition) if (!bCondition) goto Exit0
#define BUFSIZE 1024
#define MD5LEN  16

LIBRARY_EXPORT void WINAPI WaffleGetFileHash(LPTSTR lpszFile, LPTSTR lpszResult)
{
    BOOL bResult = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HANDLE hFile = NULL;
    BYTE rgbFile[BUFSIZE];
    DWORD cbRead = 0;
    BYTE rgbHash[MD5LEN];
    DWORD cbHash = 0;
    CHAR rgbDigitsU[] = "0123456789ABCDEF";
    TCHAR szResult[MD5LEN*2+1] = {0};
 
    bResult = CryptAcquireContext(&hProv,
        NULL,
        NULL,
        PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT);
    CHECK_NULL_RET(bResult);
 
    bResult = CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);
    CHECK_NULL_RET(bResult);

    hFile = CreateFile(lpszFile,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);
    CHECK_NULL_RET(!(INVALID_HANDLE_VALUE == hFile));

    while (ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL))
    {
        if (0 == cbRead)
        {
        break;
        }

        bResult = CryptHashData(hHash, rgbFile, cbRead, 0);
        CHECK_NULL_RET(bResult);
    }
 
    cbHash = MD5LEN;
    if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {
        TCHAR szTmpBuff[3] = {0};
        DWORD i;
        for (i = 0; i < cbHash; i++)
        {
            szTmpBuff[0] = (TCHAR)(rgbDigitsU[rgbHash[i] >> 4]);
            szTmpBuff[1] = (TCHAR)(rgbDigitsU[rgbHash[i] & 0xf]);
            lstrcat(szResult, szTmpBuff);
        }
        bResult = TRUE;
    }
 
Exit0:
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);
 
    lstrcpy(lpszResult, szResult);
}