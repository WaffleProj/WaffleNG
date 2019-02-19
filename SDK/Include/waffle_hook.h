#ifndef __SDK_WAFFLE_HOOK_H_
#define __SDK_WAFFLE_HOOK_H_

#ifdef  _BUILD_WAFFLE_HOOK_DLL_
#ifdef _MSC_VER
#define WAFFLE_HOOK_DLL_FUNCTION
#else
#define WAFFLE_HOOK_DLL_FUNCTION LIBRARY_EXPORT
#endif
#else
#define WAFFLE_HOOK_DLL_FUNCTION LIBRARY_IMPORT
#endif

typedef INT(WINAPI *LPCOMPARE) (LPVOID Element1, LPVOID Element2);

typedef enum
{
    SEARCH_SUCCESS = 0,
    SEARCH_BETWEEN,
    SEARCH_MISSING
} WAFFLE_SMART_ARRAY_SEARCH_RESULT_ENUM, *LPWAFFLE_SMART_ARRAY_SEARCH_RESULT_ENUM;

typedef struct
{
    WAFFLE_SMART_ARRAY_SEARCH_RESULT_ENUM   Status;
    SIZE_T  Param1;
    SIZE_T  Param2;
} WAFFLE_SMART_ARRAY_SEARCH_RESULT, *LPWAFFLE_SMART_ARRAY_SEARCH_RESULT;

typedef struct
{
    HANDLE      hHeap;
    SRWLOCK     hLock;
    SIZE_T      nSize;
    SIZE_T      nCount;
    LPVOID      lpArray;
} WAFFLE_SMART_ARRAY, *LPWAFFLE_SMART_ARRAY;

typedef struct
{
    LPTSTR lpszModule;
    HMODULE hModule;
    DWORD dwSizeOfImage;
    SIZE_T ModuleStart;
    SIZE_T ModuleEnd;
    SIZE_T BackupStart;
    SIZE_T BackupEnd;
    SIZE_T Offset;
    LPWAFFLE_SMART_ARRAY    lpstFunction;
} WAFFLE_MODULE_NODE, *LPWAFFLE_MODULE_NODE;

typedef struct
{
    SIZE_T lpOldFunction;
    SIZE_T lpBackupFunction;
    SIZE_T lpNewFunction;
    LPWAFFLE_MODULE_NODE lpstModule;
} WAFFLE_FUNCTION_NODE, *LPWAFFLE_FUNCTION_NODE;

WAFFLE_HOOK_DLL_FUNCTION FARPROC WINAPI WaffleRegisterHookOnAddress(
    _In_    FARPROC lpOldFunction,
    _In_    FARPROC lpNewFunction
    );

WAFFLE_HOOK_DLL_FUNCTION FARPROC WINAPI WaffleRegisterHookOnAPI(
    _In_opt_    LPCWSTR lpModuleName,
    _In_        LPCSTR lpProcName,
    _In_        FARPROC lpNewFunction
    );

#define InterfaceOffset(Interface, Method) (((SIZE_T) &((struct Interface##Vtbl *)NULL)->Method) / sizeof(FARPROC))
#define WaffleRegisterHookOnCOMMethod(CLSID, Interface, Method, lpNewFunction) WaffleRegisterHookOnCOMOffset(&CLSID, &IID_##Interface, InterfaceOffset(Interface, Method), lpNewFunction)

WAFFLE_HOOK_DLL_FUNCTION FARPROC WINAPI WaffleRegisterHookOnCOMOffset(
    _In_    REFCLSID rclsid,
    _In_    REFIID riid,
    _In_    SIZE_T vtableOffset,
    _In_    FARPROC lpNewFunction
    );

WAFFLE_HOOK_DLL_FUNCTION FARPROC WINAPI WaffleUnregisterHook(
    _In_    FARPROC lpOldFunction
    );

WAFFLE_HOOK_DLL_FUNCTION WAFFLE_SMART_ARRAY_SEARCH_RESULT WINAPI WaffleSmartArraySearch(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader,
    _In_    LPVOID lpElement,
    _In_    LPCOMPARE pCompare
    );

WAFFLE_HOOK_DLL_FUNCTION LPVOID WINAPI WaffleSmartArrayAdd(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader,
    _In_    LPVOID lpElement
    );

WAFFLE_HOOK_DLL_FUNCTION LPWAFFLE_SMART_ARRAY WINAPI WaffleSmartArrayCreate(
    _In_    SIZE_T nSize
    );

WAFFLE_HOOK_DLL_FUNCTION LPWAFFLE_FUNCTION_NODE WINAPI WaffleHookDBAddFunction(
    _In_    FARPROC lpOldFunction,
    _In_    FARPROC lpNewFunction
    );

WAFFLE_HOOK_DLL_FUNCTION LPVOID WINAPI WaffleSmartArrayIndexToAddress(
    _In_    LPWAFFLE_SMART_ARRAY lpHeader,
    _In_    SIZE_T Index
    );

WAFFLE_HOOK_DLL_FUNCTION SIZE_T WINAPI WaffleHookDBLookup(
    _In_    SIZE_T ExceptionAddress,
    _In_    SIZE_T CallerAddress
    );

WAFFLE_HOOK_DLL_FUNCTION LPWAFFLE_MODULE_NODE WINAPI WaffleHookDBAddSkipModule(
    _In_    HMODULE hModule
    );

WAFFLE_HOOK_DLL_FUNCTION LPWAFFLE_MODULE_NODE WINAPI WaffleHookDBAddModule(
    _In_    HMODULE hModule
    );

#endif /* __SDK_WAFFLE_HOOK_H_ */