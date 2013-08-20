#include "..\..\common.h"

LIBRARY_EXPORT int WINAPI WafflelstrlenW(
    _In_    LPCWSTR lpString
    )
{
    if (lpString)
    {
        int i;
        for (i = 0; !lpString[i]; i++);
        return i;
    }
    else
    {
        return 0;
    }
}

LIBRARY_EXPORT int WINAPI WafflelstrlenA(
    _In_    LPCSTR lpString
    )
{
    if (lpString)
    {
        int i;
        for (i = 0; !lpString[i]; i++);
        return i;
    }
    else
    {
        return 0;
    }
}

LIBRARY_EXPORT LPWSTR WINAPI WafflelstrcatW(
    _In_    LPWSTR lpString1,
    _In_    LPCWSTR lpString2
    )
{
    if (!lpString1)
    {
        return NULL;
    }

    if (lpString2)
    {
        int i, j;
        for (i = WafflelstrlenW(lpString1), j = 0; !lpString2[j]; lpString1[i] = lpString2[j], i++, j++);
    }
    return lpString1;
}

LIBRARY_EXPORT LPSTR WINAPI WafflelstrcatA(
    _In_    LPSTR lpString1,
    _In_    LPCSTR lpString2
    )
{
    if (!lpString1)
    {
        return NULL;
    }

    if (lpString2)
    {
        int i, j;
        for (i = WafflelstrlenA(lpString1), j = 0; !lpString2[j]; lpString1[i] = lpString2[j], i++, j++);
    }
    return lpString1;
}

LIBRARY_EXPORT int WINAPI WafflelstrcmpiW(
    _In_    LPCWSTR lpString1,
    _In_    LPCWSTR lpString2
    )
{
    if (lpString1 && lpString2)
    {
        int i;
        for (i = 0; !(((lpString1[i] | 0x20) != (lpString2[i] | 0x20)) || (lpString1[i] == 0) || (lpString2[i] == 0)); i++);
        return lpString1[i] - lpString2[i];
    }
    else
    {
        return (int) (lpString1 - lpString2);
    }
}

LIBRARY_EXPORT int WINAPI WafflelstrcmpiA(
    _In_    LPCSTR lpString1,
    _In_    LPCSTR lpString2
    )
{
    if (lpString1 && lpString2)
    {
        int i;
        for (i = 0; !(((lpString1[i] | 0x20) != (lpString2[i] | 0x20)) || (lpString1[i] == 0) || (lpString2[i] == 0)); i++);
        return lpString1[i] - lpString2[i];
    }
    else
    {
        return (int) (lpString1 - lpString2);
    }
}

LIBRARY_EXPORT int WINAPI WafflelstrcmpW(
    _In_    LPCWSTR lpString1,
    _In_    LPCWSTR lpString2
    )
{
    if (lpString1 && lpString2)
    {
        int i;
        for (i = 0; !((lpString1[i] != lpString2[i]) || (lpString1[i] == 0) || (lpString2[i] == 0)); i++);
        return lpString1[i] - lpString2[i];
    }
    else
    {
        return (int) (lpString1 - lpString2);
    }
}

LIBRARY_EXPORT int WINAPI WafflelstrcmpA(
    _In_    LPCSTR lpString1,
    _In_    LPCSTR lpString2
    )
{
    if (lpString1 && lpString2)
    {
        int i;
        for (i = 0; !((lpString1[i] != lpString2[i]) || (lpString1[i] == 0) || (lpString2[i] == 0)); i++);
        return lpString1[i] - lpString2[i];
    }
    else
    {
        return (int) (lpString1 - lpString2);
    }
}
LIBRARY_EXPORT int WINAPI WaffleStrToIntW(
    _In_    LPCWSTR lpString,
    _In_    int nDefault
    )
{
    int i = 0;
    BOOL bHex = FALSE;
    int nResult = 0;
    if (lpString[0] == L'0' && (lpString[1] | 0x20) == L'x')
    {
        bHex = TRUE;
        i = 2;
    }
    else if (lpString[0] == L'-')
    {
        i = 1;
    }
    while (lpString[i])
    {
        int Digital;
        if ((lpString[i] >= L'0') && (lpString[i] <= L'9'))
        {
            Digital = lpString[i] - L'0';
        }
        else if (((lpString[i] | 0x20) >= L'a') && ((lpString[i] | 0x20) <= L'f'))
        {
            Digital = (lpString[i] | 0x20) - L'a' + 10;
        }
        else
        {
            break;
        }

        if (!bHex)
        {
            nResult = nResult * 10 + Digital;
        }
        else
        {
            nResult = nResult * 16 + Digital;
        }

        i++;
    }
    if ((i == 0) || (i == 2 && bHex) || (i == 1 && lpString[0] == L'-'))
    {
        return nDefault;
    }
    else if (lpString[0] == L'-')
    {
        return -nResult;
    }
    else
    {
        return nResult;
    }
}

LIBRARY_EXPORT int WINAPI WaffleStrToIntA(
    _In_    LPCSTR lpString,
    _In_    int nDefault
    )
{
    int i = 0;
    BOOL bHex = FALSE;
    int nResult = 0;
    if (lpString[0] == '0' && (lpString[1] | 0x20) == 'x')
    {
        bHex = TRUE;
        i = 2;
    }
    else if (lpString[0] == '-')
    {
        i = 1;
    }
    while (lpString[i])
    {
        int Digital;
        if ((lpString[i] >= '0') && (lpString[i] <= '9'))
        {
            Digital = lpString[i] - '0';
        }
        else if (((lpString[i] | 0x20) >= 'a') && ((lpString[i] | 0x20) <= 'f'))
        {
            Digital = (lpString[i] | 0x20) - 'a' + 10;
        }
        else
        {
            break;
        }

        if (!bHex)
        {
            nResult = nResult * 10 + Digital;
        }
        else
        {
            nResult = nResult * 16 + Digital;
        }

        i++;
    }
    if ((i == 0) || (i == 2 && bHex) || (i == 1 && lpString[0] == '-'))
    {
        return nDefault;
    }
    else if (lpString[0] == '-')
    {
        return -nResult;
    }
    else
    {
        return nResult;
    }
}

LIBRARY_EXPORT VOID WINAPI WaffleCreateRWLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    if (!lpstRWLock->hReader)
    {
        InitializeCriticalSectionAndSpinCount(&lpstRWLock->csRead, 5120);
        InitializeCriticalSectionAndSpinCount(&lpstRWLock->csWrite, 3072);
        lpstRWLock->hReader = CreateEvent(NULL, TRUE, FALSE, NULL);
        lpstRWLock->dwReader = 0;
    }
}

LIBRARY_EXPORT VOID WINAPI WaffleReleaseRWLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    if (lpstRWLock->hReader)
    {
        WaitForSingleObject(lpstRWLock->hReader, INFINITE);
        CloseHandle(lpstRWLock->hReader);
        lpstRWLock->hReader = NULL;
        DeleteCriticalSection(&lpstRWLock->csWrite);
        DeleteCriticalSection(&lpstRWLock->csRead);
    }
}

LIBRARY_EXPORT VOID WINAPI WaffleEnterWriterLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    EnterCriticalSection(&lpstRWLock->csRead);
    for (EnterCriticalSection(&lpstRWLock->csWrite); lpstRWLock->dwReader; EnterCriticalSection(&lpstRWLock->csWrite))
    {
        LeaveCriticalSection(&lpstRWLock->csWrite);
        WaitForSingleObject(lpstRWLock->hReader, INFINITE);
    }
    LeaveCriticalSection(&lpstRWLock->csRead);
}

LIBRARY_EXPORT VOID WINAPI WaffleLeaveWriterLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    LeaveCriticalSection(&lpstRWLock->csWrite);
}

LIBRARY_EXPORT VOID WINAPI WaffleEnterReaderLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    EnterCriticalSection(&lpstRWLock->csRead);
    EnterCriticalSection(&lpstRWLock->csWrite);
    lpstRWLock->dwReader++;
    if (lpstRWLock->dwReader == 1)
    {
        ResetEvent(lpstRWLock->hReader);    //nonsignaled, need to wait
    }
    LeaveCriticalSection(&lpstRWLock->csWrite);
    LeaveCriticalSection(&lpstRWLock->csRead);
}

LIBRARY_EXPORT VOID WINAPI WaffleLeaveReaderLock(
    LPWAFFLE_RWLOCK lpstRWLock
    )
{
    EnterCriticalSection(&lpstRWLock->csWrite);
    lpstRWLock->dwReader--;
    if (lpstRWLock->dwReader == 0)
    {
        SetEvent(lpstRWLock->hReader);      //signaled
    }
    LeaveCriticalSection(&lpstRWLock->csWrite);
}

LIBRARY_EXPORT LPBYTE WINAPI WaffleGetProcAddressW(
    _In_    HMODULE hModule,
    _In_    LPCWSTR lpszFuncName
    )
{
    DWORD nSize = WideCharToMultiByte(CP_ACP, 0, lpszFuncName, -1, NULL, 0, NULL, NULL);
    LPSTR lpszFunction = (LPSTR) WaffleAlloc(nSize*sizeof(CHAR));
    WideCharToMultiByte(CP_ACP, 0, lpszFuncName, -1, lpszFunction, nSize, NULL, NULL);
    LPBYTE lpFunction = WaffleGetProcAddressA(hModule, lpszFunction);
    WaffleFree(lpszFunction);
    return lpFunction;
}

LIBRARY_EXPORT LPBYTE WINAPI WaffleGetProcAddressA(
    _In_    HMODULE hModule,
    _In_    LPCSTR lpszFuncName
    )
{
    LPBYTE lpAddress = NULL;

    //GetProcAddress won't work if hModule is invalid
    if (!hModule)
    {
        return NULL;
    }

    SIZE_T lpExportTableBegin = 0;
    SIZE_T lpExportTableEnd = 0;
    if (((PIMAGE_DOS_HEADER) hModule)->e_magic == IMAGE_DOS_SIGNATURE)
    {
        PIMAGE_DATA_DIRECTORY lpDataDirectory;
        WORD Magic = ((PIMAGE_NT_HEADERS) ((SIZE_T) hModule + ((PIMAGE_DOS_HEADER) hModule)->e_lfanew))->OptionalHeader.Magic;
        if (Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        {
            PIMAGE_NT_HEADERS lpNtHeader = (PIMAGE_NT_HEADERS) ((SIZE_T) hModule + ((PIMAGE_DOS_HEADER) hModule)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            lpDataDirectory = &lpOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
        }
        else if (Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
            PIMAGE_NT_HEADERS64 lpNtHeader = (PIMAGE_NT_HEADERS64) ((SIZE_T) hModule + ((PIMAGE_DOS_HEADER) hModule)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER64 lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            lpDataDirectory = &lpOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
        }
        else
        {
            return NULL;
        }

        lpExportTableBegin = (SIZE_T) hModule + lpDataDirectory->VirtualAddress;
        PIMAGE_EXPORT_DIRECTORY lpExportTable = (PIMAGE_EXPORT_DIRECTORY) lpExportTableBegin;
        lpExportTableEnd = (SIZE_T) lpExportTable + lpDataDirectory->Size;

        DWORD nName = lpExportTable->NumberOfNames;
        DWORD nFunction = lpExportTable->NumberOfFunctions;
        DWORD *lpName = (DWORD *) ((SIZE_T) hModule + lpExportTable->AddressOfNames);
        WORD *lpOrdinal = (WORD *) ((SIZE_T) hModule + lpExportTable->AddressOfNameOrdinals);
        DWORD *lpFunction = (DWORD *) ((SIZE_T) hModule + lpExportTable->AddressOfFunctions);

        if ((SIZE_T) lpszFuncName > 0xFFFF)
        {
            int Direction = 1;
            DWORD Begin = 0;
            DWORD End = nName;
            DWORD i;
            while (Direction)
            {
                if (Begin > End)
                {
                    return NULL;
                }
                i = (Begin + End) / 2;
                Direction = WafflelstrcmpA(lpszFuncName, (LPSTR) ((SIZE_T) hModule + lpName[i]));
                if (Direction > 0)
                {
                    Begin = i + 1;
                }
                else if (Direction < 0)
                {
                    End = i - 1;
                }
            }
            lpAddress = (LPBYTE) ((SIZE_T) hModule + lpFunction[lpOrdinal[i]]);
        }
        else
        {
            if ((SIZE_T) lpszFuncName <= nFunction)
            {
                lpAddress = (LPBYTE) ((SIZE_T) hModule + lpFunction[(SIZE_T) lpszFuncName - lpExportTable->Base]);
            }
        }
    }

    if (lpAddress) //check if this address is pointing to another library
    {
        if (lpExportTableBegin < (SIZE_T) lpAddress && (SIZE_T) lpAddress < lpExportTableEnd)
        {
            LPCSTR lpszExport = (LPCSTR) lpAddress;
            CHAR szLibrary[1024];   //H·A·R·D·C·O·D·E·D～
            int i;
            for (i = 0; lpszExport[i] != '.'; szLibrary[i] = lpszExport[i], i++);       //maybe the name of this library contains dot?
            szLibrary[i] = '\0';
            i++;
            WafflelstrcatA(szLibrary, ".dll");
            HMODULE hExport = GetModuleHandleA(szLibrary);
            lpAddress = WaffleGetProcAddressA(hExport, &lpszExport[i]);
        }
    }

    return lpAddress;
}

LIBRARY_EXPORT VOID WINAPI WaffleIntBox(
    int i
    )
{
    TCHAR szBuf[32];
    wsprintf(szBuf, TEXT("%i"), i);
    MessageBox(0, szBuf, 0, 0);
}

LIBRARY_EXPORT VOID WINAPI WaffleHexBox(
    DWORD i
    )
{
    TCHAR szBuf[32];
    wsprintf(szBuf, TEXT("0x%08X"), i);
    MessageBox(0, szBuf, 0, 0);
}

LIBRARY_EXPORT DWORD WINAPI WaffleGetImageSize(
    HMODULE hModule
    )
{
    if (!hModule)
    {
        return 0;
    }

    if (((PIMAGE_DOS_HEADER) hModule)->e_magic == IMAGE_DOS_SIGNATURE)
    {
        WORD Magic = ((PIMAGE_NT_HEADERS) ((SIZE_T) hModule + ((PIMAGE_DOS_HEADER) hModule)->e_lfanew))->OptionalHeader.Magic;
        if (Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        {
            PIMAGE_NT_HEADERS lpNtHeader = (PIMAGE_NT_HEADERS) ((SIZE_T) hModule + ((PIMAGE_DOS_HEADER) hModule)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            return lpOptionalHeader->SizeOfImage;
        }
        else if (Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
            PIMAGE_NT_HEADERS64 lpNtHeader = (PIMAGE_NT_HEADERS64) ((SIZE_T) hModule + ((PIMAGE_DOS_HEADER) hModule)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER64 lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            return lpOptionalHeader->SizeOfImage;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

LIBRARY_EXPORT LPVOID WINAPI WaffleAlloc(
    _In_    SIZE_T dwBytes
    )
{
    if (!lpstProcessSetting)
    {
        return (LPVOID) GlobalAlloc(GPTR, dwBytes);
    }

    int i = WaffleFindComponent(WAFFLE_PORT_RETURN_ADDRESS);
    if (i >= 0)
    {
        if (!lpstProcessSetting->lpstComponent[i].hHeap)
        {
            WaitForSingleObject(lpstProcessSetting->hGlobalMutex, INFINITE);
            if (!lpstProcessSetting->lpstComponent[i].hHeap)  //make sure we need to do so
            {
                lpstProcessSetting->lpstComponent[i].hHeap = HeapCreate(0, 0, 0);
                if (!lpstProcessSetting->lpstComponent[i].hHeap)
                {
                    MessageBox(0, TEXT("FIXME:Unable to create component heap"), 0, 0);
                    ExitProcess(0);
                }
            }
            ReleaseMutex(lpstProcessSetting->hGlobalMutex);
        }

        return (LPVOID) HeapAlloc(lpstProcessSetting->lpstComponent[i].hHeap, HEAP_ZERO_MEMORY, dwBytes);
    }
    else
    {
        return (LPVOID) GlobalAlloc(GPTR, dwBytes);
    }
}

LIBRARY_EXPORT LPVOID WINAPI WaffleReAlloc(
    _In_    LPVOID lpMemory,
    _In_    SIZE_T dwBytes
    )
{
    if (!lpstProcessSetting)
    {
        return (LPVOID) GlobalReAlloc(lpMemory, dwBytes, GHND);
    }

    int i = WaffleFindComponent(WAFFLE_PORT_RETURN_ADDRESS);

    if (i >= 0)
    {
        return (LPVOID) HeapReAlloc(lpstProcessSetting->lpstComponent[i].hHeap, HEAP_ZERO_MEMORY, lpMemory, dwBytes);
    }
    else
    {
        return (LPVOID) GlobalReAlloc(lpMemory, dwBytes, GHND);
    }
}

LIBRARY_EXPORT LPVOID WINAPI WaffleFree(
    _In_    LPVOID lpMemory
    )
{
    if (!lpstProcessSetting)
    {
        return (LPVOID) GlobalFree(lpMemory);
    }

    int i = WaffleFindComponent(WAFFLE_PORT_RETURN_ADDRESS);

    if (i >= 0)
    {
        if (HeapFree(lpstProcessSetting->lpstComponent[i].hHeap, 0, lpMemory))
        {
            return NULL;
        }
        else
        {
            return lpMemory;
        }
    }
    else
    {
        return (LPVOID) GlobalFree(lpMemory);
    }
}