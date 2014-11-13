#include "..\main.h"

INT WINAPI CompareModule(
    LPWAFFLE_MODULE_NODE Element1,
    LPWAFFLE_MODULE_NODE Element2
    )
{
    if (Element1->hModule == Element1->hModule)
    {
        return 0;
    }
    else
    {
        return (INT) Element1->hModule - (INT) Element1->hModule;
    }
}

WAFFLE_HOOK_DLL_FUNCTION DWORD WINAPI WaffleGetModuleSize(
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

WAFFLE_HOOK_DLL_FUNCTION BOOL WINAPI WaffleGetModuleInfo(
    _In_    LPWAFFLE_MODULE_NODE lpstModule
    )
{
    if (!lpstModule || !lpstModule->hModule)
    {
        return FALSE;
    }

    //Get dll base address and size
    DWORD dwSizeOfImage = WaffleGetModuleSize(lpstModule->hModule);
    if (!dwSizeOfImage)
    {
        return FALSE;

    }

    lpstModule->dwSizeOfImage = dwSizeOfImage;
    lpstModule->ModuleStart = (SIZE_T) lpstModule->hModule;
    lpstModule->ModuleEnd = lpstModule->ModuleStart + lpstModule->dwSizeOfImage;


    // MEMORY ALLOC!
    lpstModule->lpszModule = (LPTSTR) GlobalAlloc(GPTR, MAX_PATH * sizeof(TCHAR));
    if (!lpstModule->lpszModule)
    {
        return FALSE;
    }
    GetModuleFileName(lpstModule->hModule, lpstModule->lpszModule, MAX_PATH);

    return TRUE;
}

WAFFLE_HOOK_DLL_FUNCTION BOOL WINAPI WaffleCopyModule(
    _In_    LPWAFFLE_MODULE_NODE lpstModule
    )
{
    if (!WaffleGetModuleInfo(lpstModule))
    {
        return FALSE;
    }

    //Reserve memory address
    lpstModule->BackupStart = (SIZE_T) VirtualAlloc(NULL, lpstModule->dwSizeOfImage, MEM_RESERVE, PAGE_NOACCESS);
    lpstModule->BackupEnd = lpstModule->BackupStart + lpstModule->dwSizeOfImage;
    lpstModule->Offset = lpstModule->ModuleStart - lpstModule->BackupStart;

    LPVOID addrPointer = (LPVOID) lpstModule->ModuleStart;
    LPVOID addrEnd = (LPVOID) lpstModule->ModuleEnd;
    while (addrPointer < addrEnd)
    {
        MEMORY_BASIC_INFORMATION stMemInfo;
        VirtualQuery(addrPointer, &stMemInfo, sizeof(stMemInfo));
        // We should have a unmapping function on failure
        if (stMemInfo.State == MEM_COMMIT)
        {
            //Get offset of the memory
            LPVOID addrNew = (LPVOID) ((SIZE_T) stMemInfo.BaseAddress - (SIZE_T) stMemInfo.AllocationBase + lpstModule->BackupStart);

            VirtualAlloc(addrNew, stMemInfo.RegionSize, MEM_COMMIT, PAGE_READWRITE);
            RtlMoveMemory(addrNew, stMemInfo.BaseAddress, stMemInfo.RegionSize);
            VirtualProtect(addrNew, stMemInfo.RegionSize, stMemInfo.Protect, &stMemInfo.AllocationProtect);
        }
        addrPointer = (PBYTE) stMemInfo.BaseAddress + stMemInfo.RegionSize;
    }

    return TRUE;
}

WAFFLE_HOOK_DLL_FUNCTION LPWAFFLE_MODULE_NODE WINAPI WaffleHookDBAddModule(
    _In_    HMODULE hModule
    )
{
    WAFFLE_MODULE_NODE stModule;
    stModule.hModule = hModule;
    WAFFLE_SMART_ARRAY_SEARCH_RESULT stResult = WaffleSmartArraySearch(lpstLibrary, &stModule, (LPCOMPARE) CompareModule);

    LPWAFFLE_MODULE_NODE lpstModule = NULL;

    if (stResult.Status != SEARCH_SUCCESS)
    {
        if (!WaffleCopyModule(&stModule))
        {
            return NULL;
        }

        stModule.lpstFunction = WaffleSmartArrayCreate(sizeof(WAFFLE_FUNCTION_NODE));

        lpstModule = (LPWAFFLE_MODULE_NODE) WaffleSmartArrayAdd(lpstLibrary, (LPVOID) &stModule);
        if (!lpstModule)
        {
            // Undo the copy here
            // And release stModule.lpszModule
            return NULL;
        }
    }

    return lpstModule;
}

WAFFLE_HOOK_DLL_FUNCTION LPWAFFLE_MODULE_NODE WINAPI WaffleHookDBAddSkipModule(
    _In_    HMODULE hModule
    )
{
    WAFFLE_MODULE_NODE stModule;
    stModule.hModule = hModule;
    WAFFLE_SMART_ARRAY_SEARCH_RESULT stResult = WaffleSmartArraySearch(lpstSkipLibrary, &stModule, (LPCOMPARE) CompareModule);

    LPWAFFLE_MODULE_NODE lpstModule = NULL;

    if (stResult.Status != SEARCH_SUCCESS)
    {
        WaffleGetModuleInfo(&stModule);
        lpstModule = (LPWAFFLE_MODULE_NODE) WaffleSmartArrayAdd(lpstLibrary, (LPVOID) &stModule);
        if (!lpstModule)
        {
            // Undo the copy here
            // And release stModule.lpszModule
            return NULL;
        }
    }

    return lpstModule;
}

INT WINAPI CompareFunction(
    LPWAFFLE_FUNCTION_NODE Element1,
    LPWAFFLE_FUNCTION_NODE Element2
    )
{
    if (Element1->lpOldFunction == Element1->lpOldFunction)
    {
        return 0;
    }
    else
    {
        return (INT) Element1->lpOldFunction - (INT) Element1->lpOldFunction;
    }
}

WAFFLE_HOOK_DLL_FUNCTION LPWAFFLE_FUNCTION_NODE WINAPI WaffleHookDBAddFunction(
    _In_    FARPROC lpOldFunction,
    _In_    FARPROC lpNewFunction
    )
{
    if (!lpOldFunction)
    {
        return NULL;
    }

    HMODULE hModule = NULL;
    if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR) (SIZE_T) lpOldFunction, &hModule))
    {
        return NULL;
    }
    
    LPWAFFLE_MODULE_NODE lpstModule = WaffleHookDBAddModule(hModule);
    if (!lpstModule)
    {
        return NULL;
    }

    WAFFLE_FUNCTION_NODE stFunction;
    stFunction.lpstModule = lpstModule;
    stFunction.lpOldFunction = (SIZE_T) lpOldFunction;
    stFunction.lpNewFunction = (SIZE_T) lpNewFunction;
    stFunction.lpBackupFunction = (SIZE_T) lpOldFunction + lpstModule->Offset;
    
    WAFFLE_SMART_ARRAY_SEARCH_RESULT stResult = WaffleSmartArraySearch(lpstModule->lpstFunction, &stFunction, (LPCOMPARE) CompareFunction);

    LPWAFFLE_FUNCTION_NODE lpstFunction = NULL;

    if (stResult.Status != SEARCH_SUCCESS)
    {
        lpstFunction = WaffleSmartArrayAdd(lpstLibrary, (LPVOID) &stFunction);
        if (!lpstFunction)
        {
            // Undo the copy here
            return NULL;
        }
    }

    return lpstFunction;
}

INT WINAPI CompareFunctionToModule(
    LPWAFFLE_MODULE_NODE Element1,
    LPWAFFLE_MODULE_NODE Element2
    )
{
    if (Element1->ModuleStart >= Element1->ModuleStart && Element1->ModuleStart <= Element1->ModuleEnd)
    {
        return 0;
    }
    else
    {
        return (INT) Element1->ModuleStart - (INT) Element1->ModuleStart;
    }
}

WAFFLE_HOOK_DLL_FUNCTION SIZE_T WINAPI WaffleHookDBLookup(
    _In_    SIZE_T ExceptionAddress,
    _In_    SIZE_T CallerAddress
    )
{
    WAFFLE_MODULE_NODE stModule;
    WAFFLE_FUNCTION_NODE stFunction;
    WAFFLE_SMART_ARRAY_SEARCH_RESULT stResult;

    LPWAFFLE_MODULE_NODE lpstModule = NULL;
    LPWAFFLE_FUNCTION_NODE lpstFunction = NULL;
    
    // Check if the function is belong to a module we know (and thus we have a backup)
    stModule.ModuleStart = ExceptionAddress;
    stResult = WaffleSmartArraySearch(lpstLibrary, &stModule, (LPCOMPARE) CompareFunctionToModule);
    if (stResult.Status != SEARCH_SUCCESS)
    {
        // No backup and no detour, CRAAAAAASH!
        return 0;
    }
    else
    {
        // We have a backup. We are safe
        lpstModule = (LPWAFFLE_MODULE_NODE) WaffleSmartArrayIndexToAddress(lpstLibrary, stResult.Param1);
    }

    // Check if the function is hooked
    stFunction.lpOldFunction = ExceptionAddress;
    stResult = WaffleSmartArraySearch(lpstModule->lpstFunction, &stFunction, (LPCOMPARE) CompareFunction);
    if (stResult.Status != SEARCH_SUCCESS)
    {
        // No, so go to the backup
        return (SIZE_T) ExceptionAddress + lpstModule->Offset;
    }
    else
    {
        // Yes, check if this is called by our modules
        lpstFunction = (LPWAFFLE_FUNCTION_NODE) WaffleSmartArrayIndexToAddress(lpstModule->lpstFunction, stResult.Param1);
    }

    stModule.ModuleStart = CallerAddress;
    stResult = WaffleSmartArraySearch(lpstSkipLibrary, &stModule, (LPCOMPARE) CompareFunctionToModule);
    if (stResult.Status != SEARCH_SUCCESS)
    {
        // Called by other modules, go to detoured version
        return lpstFunction->lpNewFunction;
    }
    else
    {
        // Called by us, go to normal version
        return lpstFunction->lpBackupFunction;
    }
}