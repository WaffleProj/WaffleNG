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

LIBRARY_EXPORT LPVOID WINAPI WaffleGetProcAddress(
    _In_    HMODULE hModule,
    _In_    LPCSTR lpszFuncName
    )
{
    //GetProcAddress won't work if hModule is invalid
    if (!hModule)
    {
        return NULL;
    }

    if (((PIMAGE_DOS_HEADER) hModule)->e_magic == IMAGE_DOS_SIGNATURE)
    {
        PIMAGE_DATA_DIRECTORY lpDataDirectory;
        WORD Magic = ((PIMAGE_NT_HEADERS) ((SIZE_T) hModule + ((PIMAGE_DOS_HEADER) hModule)->e_lfanew))->OptionalHeader.Magic;
        if (Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        {
            PIMAGE_NT_HEADERS lpNtHeader = (PIMAGE_NT_HEADERS) ((SIZE_T) hModule + ((PIMAGE_DOS_HEADER) hModule)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            lpDataDirectory = (PIMAGE_DATA_DIRECTORY) (lpOptionalHeader->DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
        }
        else if (Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
            PIMAGE_NT_HEADERS64 lpNtHeader = (PIMAGE_NT_HEADERS64) ((SIZE_T) hModule + ((PIMAGE_DOS_HEADER) hModule)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER64 lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            lpDataDirectory = (PIMAGE_DATA_DIRECTORY) (lpOptionalHeader->DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
        }
        else
        {
            return NULL;
        }

        PIMAGE_EXPORT_DIRECTORY lpExportTable = (PIMAGE_EXPORT_DIRECTORY) ((SIZE_T) hModule + lpDataDirectory->VirtualAddress);

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
                if (Begin == End)
                {
                    return NULL;
                }
                i = (Begin + End) / 2;
                Direction = WafflelstrcmpA(lpszFuncName, (LPSTR) ((SIZE_T) hModule + lpName[i]));
                if (Direction > 0)
                {
                    Begin = i;
                }
                else if (Direction < 0)
                {
                    End = i;
                }
            }
            return (LPVOID) ((SIZE_T) hModule + lpFunction[lpOrdinal[i]]);
        }
        else
        {
            if ((SIZE_T) lpszFuncName <= nFunction)
            {
                return (LPVOID) ((SIZE_T) hModule + lpFunction[(SIZE_T) lpszFuncName - lpExportTable->Base]);
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
    stLibrary->hModule = GetModuleHandle(stLibrary->lpszLibrary);
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
        stFunction[i].lpNewFunction = WaffleGetProcAddress(stLibrary->lpLibrary, stFunction[i].lpszFunction);
        stFunction[i].lpOriginalFunction = WaffleGetProcAddress(stLibrary->hModule, stFunction[i].lpszFunction);
    }

    return stLibrary->lpLibrary;
}

LIBRARY_EXPORT void WINAPI WaffleSetBreakpoint(void)
{
    //MessageBox(0,TEXT("Start Hooking"),TEXT("Debugger"),0);
    int i;
    for (i = 0; stLibraryTable[i].lpszLibrary; i++)
    {
        WaffleCopyLibraryEx(&stLibraryTable[i]);
    }
    //_wsprintfA = (LPWSPRINTFA) WaffleGetProcAddress(stLibraryTable[USER32].lpLibrary, "wsprintfA");
    //_VirtualProtect = (LPVIRTUALPROTECT) WaffleGetProcAddress(stLibraryTable[KERNEL32].lpLibrary, "VirtualProtect");

    //stUser32Table[MESSAGEBOXA].lpDetourFunction = HookedMessageBoxA;
    //stUser32Table[MESSAGEBOXA].lpOriginalFunction = GetProcAddress(GetModuleHandle(TEXT("User32.dll")),"MessageBoxA");

    AddVectoredExceptionHandler(TRUE, WaffleExceptionHandler);
    for (i = 0; stLibraryTable[i].lpszLibrary; i++)
    {
        LPHOOK_TABLE_OBJECT lpHookTable = stLibraryTable[i].lpHookTable;
        int j;
        for (j = 0; lpHookTable[j].lpszFunction; j++)
        {
            DWORD flOldProtect;
            //if  (_VirtualProtect(lpHookTable[j].lpOriginalFunction,1,PAGE_EXECUTE_READWRITE,&flOldProtect))
            //{
            if (lpHookTable[j].lpDetourFunction)
            {
                VirtualProtect(lpHookTable[j].lpOriginalFunction, 1, PAGE_EXECUTE_READWRITE, &flOldProtect);
                WAFFLE_PORT_WRITE_EXCEPTION_INSTRUCTION(lpHookTable[j].lpOriginalFunction);
                VirtualProtect(lpHookTable[j].lpOriginalFunction, 1, flOldProtect, &flOldProtect);
            }
            //}
            //else
            //{
            //((LPMESSAGEBOXA)stUser32Table[MESSAGEBOXA].lpNewFunction)(0,lpHookTable[j].lpszFunction,"Unable to hook",0);
            //}
        }
    }

    //MessageBoxA(0,"MessageBoxA","Dll Copy Example",0);
    //((LPMESSAGEBOXA)stUser32Table[MESSAGEBOXA].lpNewFunction)(0,"lpMessageBoxA","Dll Copy Example",0);

    //VirtualProtect(stUser32Table[MESSAGEBOXA].lpOriginalFunction,1,flOldProtect,&flOldProtect);
    //RemoveVectoredExceptionHandler(hBreakpoint);

    //((LPMESSAGEBOXA)stUser32Table[MESSAGEBOXA].lpNewFunction)(0,"Finish Hooking","Debugger",0);
    return;
}

LIBRARY_EXPORT LONG CALLBACK WaffleExceptionHandler(
    _In_    PEXCEPTION_POINTERS ExceptionInfo
    )
{
    if ((ExceptionInfo->ExceptionRecord->ExceptionCode == WAFFLE_PORT_EXCEPTION_CODE) && (*(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA *) (ExceptionInfo->ExceptionRecord->ExceptionAddress) == (WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA) WAFFLE_PORT_EXCEPTION_INSTRUCTION))
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
                        ExceptionInfo->ContextRecord->WAFFLE_PORT_PROGRAM_POINTER = (SIZE_T) lpHookTable[j].lpDetourFunction;
                        return EXCEPTION_CONTINUE_EXECUTION;
                    }
                }
                ExceptionInfo->ContextRecord->WAFFLE_PORT_PROGRAM_POINTER = (SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress - (SIZE_T) stLibraryTable[i].hModule + (SIZE_T) stLibraryTable[i].lpLibrary;
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