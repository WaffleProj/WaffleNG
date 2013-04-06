#define UNICODE
#include <windows.h>
#include <WinNT.h>
#include <psapi.h>
#include "..\core.h"
#include "membp.h"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

HMODULE WINAPI CopyLibrary(
  _In_  LPSTR lpszDllName
){
    if  (!lpszDllName)
        return NULL;

    //Get dll base address and size
    HANDLE hProcess = GetCurrentProcess();
    HMODULE hModule = LoadLibraryA(lpszDllName);
    if  (!hModule)
        return NULL;
    MODULEINFO  stModuleInfo;
    GetModuleInformation(hProcess,hModule,&stModuleInfo,sizeof(stModuleInfo));

    //Reserve memory address
    HMODULE hDll = (HMODULE)VirtualAlloc(NULL,stModuleInfo.SizeOfImage,MEM_RESERVE,PAGE_NOACCESS);

    LPVOID addrPointer = stModuleInfo.lpBaseOfDll;
    LPVOID addrEnd = (LPVOID)((SIZE_T)stModuleInfo.lpBaseOfDll + stModuleInfo.SizeOfImage);
    while (addrPointer < addrEnd)
    {
        MEMORY_BASIC_INFORMATION stMemInfo;
        VirtualQuery(addrPointer,&stMemInfo,sizeof(stMemInfo));
        if  (stMemInfo.State == MEM_COMMIT)
        {
            //Get offset of the memory
            LPVOID addrNew = (LPVOID)((SIZE_T)stMemInfo.BaseAddress - (SIZE_T)stMemInfo.AllocationBase + (SIZE_T)hDll);

            VirtualAlloc(addrNew,stMemInfo.RegionSize,MEM_COMMIT,PAGE_READWRITE);
            RtlMoveMemory(addrNew,stMemInfo.BaseAddress,stMemInfo.RegionSize);
            VirtualProtect(addrNew,stMemInfo.RegionSize,stMemInfo.Protect,&stMemInfo.AllocationProtect);
        }
        addrPointer = (PBYTE)stMemInfo.BaseAddress + stMemInfo.RegionSize;
    }
    
    FreeLibrary(hModule);

    return hDll;
}

LPVOID WINAPI GetProcAddr(
  _In_  HMODULE hDll,
  _In_  LPSTR lpszFuncName
){
    if (!hDll)
        return NULL;

    if (((PIMAGE_DOS_HEADER)hDll)->e_magic == IMAGE_DOS_SIGNATURE)
    {
        PIMAGE_DATA_DIRECTORY lpDataDirectory;
        PIMAGE_EXPORT_DIRECTORY lpExportTable;
        
        DWORD *lpName;
        WORD *lpOrdinal;
        DWORD *lpFunction;

        DWORD i;
    
        WORD Magic = ((PIMAGE_NT_HEADERS)((SIZE_T)hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew))->OptionalHeader.Magic;
        if (Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        {
            PIMAGE_NT_HEADERS lpNtHeader = (PIMAGE_NT_HEADERS)((SIZE_T)hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            lpDataDirectory = (PIMAGE_DATA_DIRECTORY)(lpOptionalHeader->DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
        }
        else if (Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
            PIMAGE_NT_HEADERS64 lpNtHeader = (PIMAGE_NT_HEADERS64)((SIZE_T)hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew);
            PIMAGE_OPTIONAL_HEADER64 lpOptionalHeader = &(lpNtHeader->OptionalHeader);
            lpDataDirectory = (PIMAGE_DATA_DIRECTORY)(lpOptionalHeader->DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
        }
        else
            return NULL;

        lpExportTable = (PIMAGE_EXPORT_DIRECTORY)((SIZE_T)hDll + lpDataDirectory->VirtualAddress);
        
        lpName = (LPVOID)((SIZE_T)hDll + lpExportTable->AddressOfNames);
        lpOrdinal = (LPVOID)((SIZE_T)hDll + lpExportTable->AddressOfNameOrdinals);
        lpFunction = (LPVOID)((SIZE_T)hDll + lpExportTable->AddressOfFunctions);

        if  ((SIZE_T)lpszFuncName >= 0x00010000)
        {
            for (i = 0; i < lpExportTable->NumberOfFunctions; i++)
            {
                if (!lstrcmpA((LPVOID)((SIZE_T)hDll + lpName[i]),lpszFuncName))
                    return (LPVOID)((SIZE_T)hDll + lpFunction[lpOrdinal[i]]);
            }
        }
        else
        {
            if  ((SIZE_T)lpszFuncName <= lpExportTable->NumberOfFunctions)
                return (LPVOID)((SIZE_T)hDll + lpFunction[(SIZE_T)lpszFuncName - lpExportTable->Base]);
        }
    }
    return NULL;
}

LONG CALLBACK BreakpointHandler(
  _In_  PEXCEPTION_POINTERS ExceptionInfo
){
    CHAR szExceptionRecord[512];

    lpwsprintfA(szExceptionRecord,"ExceptionRecord->ExceptionCode = %08x\nExceptionRecord->ExceptionFlags = %08x\nExceptionRecord->ExceptionRecord = %016I64X\nExceptionRecord->ExceptionAddress = %016I64X\nExceptionRecord->NumberParameters = %08x",ExceptionInfo->ExceptionRecord->ExceptionCode,ExceptionInfo->ExceptionRecord->ExceptionFlags,(UINT64)(ExceptionInfo->ExceptionRecord->ExceptionRecord),(UINT64)(ExceptionInfo->ExceptionRecord->ExceptionAddress),ExceptionInfo->ExceptionRecord->NumberParameters);

    int i;
    for (i = 0; i < ExceptionInfo->ExceptionRecord->NumberParameters; i++)
    {
        CHAR szBuf[256];
        lpwsprintfA(szBuf,"\nExceptionRecord->ExceptionInformation[%u] = %016I64X",i,(UINT64)(ExceptionInfo->ExceptionRecord->ExceptionInformation[i]));
        lstrcatA(szExceptionRecord,szBuf);
    }
    lpMessageBoxA(0,szExceptionRecord,"BreakpointHandler",0);

    if  (stMessageBoxA.lpOldFunction == ExceptionInfo->ExceptionRecord->ExceptionAddress)
    {
        #if defined(_WIN64)
        ExceptionInfo->ContextRecord->Rip = (SIZE_T)HookedMessageBoxA;
        #else
        ExceptionInfo->ContextRecord->Eip = (SIZE_T)HookedMessageBoxA;
        #endif  // defined(_WIN64)
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}