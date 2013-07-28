#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"
#include <psapi.h>
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wparentheses"

LPLIBRARY_TABLE_OBJECT stLibraryTable;

LIBRARY_EXPORT void WINAPI WaffleSetLibraryTable(
    _In_    LPLIBRARY_TABLE_OBJECT lpstLibraryTable
    )
{
    stLibraryTable = lpstLibraryTable;
}

LIBRARY_EXPORT int WINAPI _lstrcmpiW(
    _In_    LPCWSTR lpString1,
    _In_    LPCWSTR lpString2
    )
{
    if (lpString1 && lpString2)
    {
        int i;
        for (i = 0;; i++)
        {
            if (((lpString1[i] | 0x20) != (lpString2[i] | 0x20)) || (lpString1[i] == 0) || (lpString2[i] == 0))
            {
                break;
            }
        }
        return lpString1[i] - lpString2[i];
    }
    else
    {
        return (int) (lpString1 - lpString2);
    }
}

LIBRARY_EXPORT HMODULE WINAPI WaffleGetModuleAddressW(
    _In_    LPCWSTR lpszModule
    )
{
    if (!lpszModule)
    {
        return 0;
    }
    PPEB lpPeb = WAFFLE_PORT_PEB_ADDRESS;

    PLIST_ENTRY lpListEntry = lpPeb->Ldr->InMemoryOrderModuleList.Flink;
    for (; ((PLDR_DATA_TABLE_ENTRY) lpListEntry)->FullDllName.Buffer;)
    {
        if (_lstrcmpiW(lpszModule, ((PLDR_DATA_TABLE_ENTRY) lpListEntry)->FullDllName.Buffer) == 0)
        {
            return (HMODULE) ((PLDR_DATA_TABLE_ENTRY) ((SIZE_T) lpListEntry - FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks)))->DllBase;
        }
        else
        {
            lpListEntry = lpListEntry->Flink;
        }
    }
    return 0;
}

LIBRARY_EXPORT LPVOID WINAPI WaffleGetFunctionAddressA(
    _In_    HMODULE hDll,
    _In_    LPCSTR lpszFuncName
    )
{
    if (!hDll)
    {
        return NULL;
    }

    if (((PIMAGE_DOS_HEADER) hDll)->e_magic == IMAGE_DOS_SIGNATURE)
    {
        PIMAGE_DATA_DIRECTORY lpDataDirectory;
        WORD Magic = ((PIMAGE_NT_HEADERS) ((SIZE_T) hDll + ((PIMAGE_DOS_HEADER) hDll)->e_lfanew))->OptionalHeader.Magic;
        if (Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        {
            PIMAGE_NT_HEADERS lpNtHeader = (PIMAGE_NT_HEADERS) ((SIZE_T) hDll + ((PIMAGE_DOS_HEADER) hDll)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            lpDataDirectory = (PIMAGE_DATA_DIRECTORY) (lpOptionalHeader->DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
        }
        else if (Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
            PIMAGE_NT_HEADERS64 lpNtHeader = (PIMAGE_NT_HEADERS64) ((SIZE_T) hDll + ((PIMAGE_DOS_HEADER) hDll)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER64 lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            lpDataDirectory = (PIMAGE_DATA_DIRECTORY) (lpOptionalHeader->DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
        }
        else
        {
            return NULL;
        }

        PIMAGE_EXPORT_DIRECTORY lpExportTable = (PIMAGE_EXPORT_DIRECTORY) ((SIZE_T) hDll + lpDataDirectory->VirtualAddress);

        DWORD *lpName = (DWORD *) ((SIZE_T) hDll + lpExportTable->AddressOfNames);
        WORD *lpOrdinal = (WORD *) ((SIZE_T) hDll + lpExportTable->AddressOfNameOrdinals);
        DWORD *lpFunction = (DWORD *) ((SIZE_T) hDll + lpExportTable->AddressOfFunctions);

        if ((SIZE_T) lpszFuncName >= 0x00010000)
        {
            DWORD i;
            for (i = 0; i < lpExportTable->NumberOfFunctions; i++)
            {
                if (!lstrcmpA((LPSTR) ((SIZE_T) hDll + lpName[i]), lpszFuncName))
                {
                    return (LPVOID) ((SIZE_T) hDll + lpFunction[lpOrdinal[i]]);
                }
            }
        }
        else
        {
            if ((SIZE_T) lpszFuncName <= lpExportTable->NumberOfFunctions)
            {
                return (LPVOID) ((SIZE_T) hDll + lpFunction[(SIZE_T) lpszFuncName - lpExportTable->Base]);
            }
        }
    }
    return NULL;
}

LIBRARY_EXPORT HMODULE WINAPI WaffleCopyLibrary(
    _In_    HMODULE hModule
    )
{
    if (!hModule)
    {
        return NULL;
    }

    //Get dll base address and size
    HANDLE hProcess = GetCurrentProcess();
    MODULEINFO  stModuleInfo;
    GetModuleInformation(hProcess, hModule, &stModuleInfo, sizeof(stModuleInfo));

    //Reserve memory address
    HMODULE hDll = (HMODULE) VirtualAlloc(NULL, stModuleInfo.SizeOfImage, MEM_RESERVE, PAGE_NOACCESS);

    LPVOID addrPointer = stModuleInfo.lpBaseOfDll;
    LPVOID addrEnd = (LPVOID) ((SIZE_T) stModuleInfo.lpBaseOfDll + stModuleInfo.SizeOfImage);
    while (addrPointer < addrEnd)
    {
        MEMORY_BASIC_INFORMATION stMemInfo;
        VirtualQuery(addrPointer, &stMemInfo, sizeof(stMemInfo));
        if (stMemInfo.State == MEM_COMMIT)
        {
            //Get offset of the memory
            LPVOID addrNew = (LPVOID) ((SIZE_T) stMemInfo.BaseAddress - (SIZE_T) stMemInfo.AllocationBase + (SIZE_T) hDll);

            VirtualAlloc(addrNew, stMemInfo.RegionSize, MEM_COMMIT, PAGE_READWRITE);
            RtlMoveMemory(addrNew, stMemInfo.BaseAddress, stMemInfo.RegionSize);
            VirtualProtect(addrNew, stMemInfo.RegionSize, stMemInfo.Protect, &stMemInfo.AllocationProtect);
        }
        addrPointer = (PBYTE) stMemInfo.BaseAddress + stMemInfo.RegionSize;
    }

    return hDll;
}

LIBRARY_EXPORT HMODULE WINAPI WaffleCopyLibraryEx(
    _In_    LPLIBRARY_TABLE_OBJECT stLibrary
    )
{
    stLibrary->hModule = WaffleGetModuleAddressW(stLibrary->lpszLibrary);
    if (!stLibrary->hModule)
    {
        return NULL;
    }

    //Get dll base address and size
    HANDLE hProcess = GetCurrentProcess();
    MODULEINFO  stModuleInfo;
    GetModuleInformation(hProcess, stLibrary->hModule, &stModuleInfo, sizeof(stModuleInfo));
    stLibrary->lpEndOfModule = (LPVOID) ((SIZE_T) stLibrary->hModule + stModuleInfo.SizeOfImage);

    //Reserve memory address
    stLibrary->lpLibrary = (HMODULE) VirtualAlloc(NULL, stModuleInfo.SizeOfImage, MEM_RESERVE, PAGE_NOACCESS);

    LPVOID addrPointer = stModuleInfo.lpBaseOfDll;
    LPVOID addrEnd = (LPVOID) ((SIZE_T) stModuleInfo.lpBaseOfDll + stModuleInfo.SizeOfImage);
    while (addrPointer < addrEnd)
    {
        MEMORY_BASIC_INFORMATION stMemInfo;
        VirtualQuery(addrPointer, &stMemInfo, sizeof(stMemInfo));
        if (stMemInfo.State == MEM_COMMIT)
        {
            //Get offset of the memory
            LPVOID addrNew = (LPVOID) ((SIZE_T) stMemInfo.BaseAddress - (SIZE_T) stMemInfo.AllocationBase + (SIZE_T) stLibrary->lpLibrary);

            VirtualAlloc(addrNew, stMemInfo.RegionSize, MEM_COMMIT, PAGE_READWRITE);
            RtlMoveMemory(addrNew, stMemInfo.BaseAddress, stMemInfo.RegionSize);
            VirtualProtect(addrNew, stMemInfo.RegionSize, stMemInfo.Protect, &stMemInfo.AllocationProtect);
        }
        addrPointer = (PBYTE) stMemInfo.BaseAddress + stMemInfo.RegionSize;
    }

    LPHOOK_TABLE_OBJECT stFunction = stLibrary->lpHookTable;
    int i;
    for (i = 0; stFunction[i].lpszFunction; i++)
    {
        stFunction[i].lpNewFunction = WaffleGetFunctionAddressA(stLibrary->lpLibrary, stFunction[i].lpszFunction);
        stFunction[i].lpOriginalFunction = WaffleGetFunctionAddressA(stLibrary->hModule, stFunction[i].lpszFunction);
    }

    return stLibrary->lpLibrary;
}

LIBRARY_EXPORT LONG CALLBACK WaffleBreakpointHandler(
    _In_    PEXCEPTION_POINTERS ExceptionInfo
    )
{
    if ((ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_PRIV_INSTRUCTION) && (*(WAFFLE_PORT_PRIVILEGED_INSTRUCTION_DATA *) (ExceptionInfo->ExceptionRecord->ExceptionAddress) == (WAFFLE_PORT_PRIVILEGED_INSTRUCTION_DATA) WAFFLE_PORT_PRIVILEGED_INSTRUCTION))
    {
        int i;
        for (i = 0; stLibraryTable[i].lpszLibrary; i++)
        {
            LPHOOK_TABLE_OBJECT lpHookTable = stLibraryTable[i].lpHookTable;    //kernel32中的api可能是ntdll的存根
            if (((SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress >= (SIZE_T) stLibraryTable[i].hModule) && ((SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress <= (SIZE_T) stLibraryTable[i].lpEndOfModule))
            {
                int j;
                for (j = 0; lpHookTable[j].lpszFunction; j++)
                {
                    if (lpHookTable[j].lpOriginalFunction == ExceptionInfo->ExceptionRecord->ExceptionAddress)
                    {
                        //((LPMESSAGEBOXA)stUser32Table[MESSAGEBOXA].lpNewFunction)(0,lpHookTable[j].lpszFunction,"BreakpointHandler",0);
                        WAFFLE_PORT_PROGRAM_COUNTER(ExceptionInfo) = (SIZE_T) lpHookTable[j].lpDetourFunction;
                        return EXCEPTION_CONTINUE_EXECUTION;
                    }
                }
                WAFFLE_PORT_PROGRAM_COUNTER(ExceptionInfo) = (SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress - (SIZE_T) stLibraryTable[i].hModule + (SIZE_T) stLibraryTable[i].lpLibrary;
                return EXCEPTION_CONTINUE_EXECUTION;
            }
        }
    }
    /*
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == 0x0EEDFADE)
    {
    CHAR szExceptionRecord[2048];

    _wsprintfA(szExceptionRecord, "ExceptionRecord->ExceptionCode = %08x\nExceptionRecord->ExceptionFlags = %08x\nExceptionRecord->ExceptionRecord = %016I64X\nExceptionRecord->ExceptionAddress = %016I64X\nExceptionRecord->NumberParameters = %08x", ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ExceptionRecord->ExceptionFlags, (UINT64) (ExceptionInfo->ExceptionRecord->ExceptionRecord), (UINT64) (ExceptionInfo->ExceptionRecord->ExceptionAddress), ExceptionInfo->ExceptionRecord->NumberParameters);

    DWORD i;
    for (i = 0; i < ExceptionInfo->ExceptionRecord->NumberParameters; i++)
    {
    CHAR szBuf[256];
    _wsprintfA(szBuf, "\nExceptionRecord->ExceptionInformation[%u] = %016I64X", i, (UINT64) (ExceptionInfo->ExceptionRecord->ExceptionInformation[i]));
    lstrcatA(szExceptionRecord, szBuf);
    }
    ((LPMESSAGEBOXA) stUser32Table[MESSAGEBOXA].lpNewFunction)(0, szExceptionRecord, "BreakpointHandler", 0);
    }
    */
    return EXCEPTION_CONTINUE_SEARCH;
}