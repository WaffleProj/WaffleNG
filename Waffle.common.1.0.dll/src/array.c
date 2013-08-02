#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"

static LPWAFFLE_LIBRARY_ARRAY lpstLibrary;

LIBRARY_EXPORT VOID WINAPI WaffleCopyLibrary(
    _In_    LPWAFFLE_LIBRARY_ARRAY lpstNewLibrary
    )
{
    lpstNewLibrary->hSource = GetModuleHandle(lpstNewLibrary->lpszLibrary);
    if (!lpstNewLibrary->hSource)
    {
        return;
    }

    //Get dll base address and size
    DWORD dwSizeOfImage = WaffleGetImageSize(lpstNewLibrary->hSource);
    lpstNewLibrary->hSourceEnd = (LPVOID) ((SIZE_T) lpstNewLibrary->hSource + dwSizeOfImage);

    //Reserve memory address
    lpstNewLibrary->hBackup = (HMODULE) VirtualAlloc(NULL, dwSizeOfImage, MEM_RESERVE, PAGE_NOACCESS);
    lpstNewLibrary->hBackupEnd = (LPVOID) ((SIZE_T) lpstNewLibrary->hBackup + dwSizeOfImage);

    LPVOID addrPointer = lpstNewLibrary->hSource;
    LPVOID addrEnd = lpstNewLibrary->hSourceEnd;
    while (addrPointer < addrEnd)
    {
        MEMORY_BASIC_INFORMATION stMemInfo;
        VirtualQuery(addrPointer, &stMemInfo, sizeof(stMemInfo));
        if (stMemInfo.State == MEM_COMMIT)
        {
            //Get offset of the memory
            LPVOID addrNew = (LPVOID) ((SIZE_T) stMemInfo.BaseAddress - (SIZE_T) stMemInfo.AllocationBase + (SIZE_T) lpstNewLibrary->hBackup);

            VirtualAlloc(addrNew, stMemInfo.RegionSize, MEM_COMMIT, PAGE_READWRITE);
            RtlMoveMemory(addrNew, stMemInfo.BaseAddress, stMemInfo.RegionSize);
            VirtualProtect(addrNew, stMemInfo.RegionSize, stMemInfo.Protect, &stMemInfo.AllocationProtect);
        }
        addrPointer = (PBYTE) stMemInfo.BaseAddress + stMemInfo.RegionSize;
    }
}

LIBRARY_EXPORT VOID WINAPI WaffleAddLibrary(
    _In_    LPWAFFLE_LIBRARY_ARRAY lpstNewLibrary
    )
{
    if (!lpstLibrary)
    {
        lpstLibrary = (LPWAFFLE_LIBRARY_ARRAY) GlobalAlloc(GPTR, sizeof(WAFFLE_LIBRARY_ARRAY));
        if (!lpstLibrary)
        {
            MessageBox(0, TEXT("FIXME:Unablt to allocate memory for library array"), 0, 0);
            ExitProcess(0);
        }
        lpstNewLibrary->dwBehind = 0;
    }
    else
    {
        int i;
        for (i = lpstLibrary[0].dwBehind; i >= 0; i--)
        {
            lpstLibrary[i].dwBehind++;
        }
        lpstLibrary = (LPWAFFLE_LIBRARY_ARRAY) GlobalReAlloc(lpstLibrary, sizeof(WAFFLE_LIBRARY_ARRAY)*(lpstLibrary[0].dwBehind + 1), GHND);
        if (!lpstLibrary)
        {
            MessageBox(0, TEXT("FIXME:Unablt to add elements in library array"), 0, 0);
            ExitProcess(0);
        }
    }
    RtlMoveMemory(&lpstLibrary[lpstLibrary[0].dwBehind], lpstNewLibrary, sizeof(WAFFLE_LIBRARY_ARRAY));
    lpstLibrary[lpstLibrary[0].dwBehind].dwBehind = 0;
}

LIBRARY_EXPORT VOID WINAPI WaffleAddFunction(
    _In_    LPWAFFLE_LIBRARY_ARRAY lpstNewLibrary,
    _In_    LPCTSTR lpszFunction,
    _In_    HMODULE hDetour,
    _In_    LPCTSTR lpszDetour
    )
{
    LPVOID lpSource = WaffleGetProcAddress(lpstNewLibrary->hSource, lpszFunction);
    if (!lpSource)  //This function doesn't exist
    {
        return;
    }
    if (!lpstNewLibrary->lpstFunction)
    {
        lpstNewLibrary->lpstFunction = (LPWAFFLE_FUNCTION_ARRAY) GlobalAlloc(GPTR, sizeof(WAFFLE_FUNCTION_ARRAY));
        if (!lpstNewLibrary->lpstFunction)
        {
            MessageBox(0, TEXT("FIXME:Unablt to allocate memory for function array"), 0, 0);
            ExitProcess(0);
        }
    }
    else
    {
        int i;
        for (i = lpstNewLibrary->lpstFunction[0].dwBehind; i >= 0; i--)
        {
            lpstNewLibrary->lpstFunction[i].dwBehind++;
        }
        lpstNewLibrary->lpstFunction = (LPWAFFLE_FUNCTION_ARRAY) GlobalReAlloc(lpstNewLibrary->lpstFunction, sizeof(WAFFLE_FUNCTION_ARRAY)*(lpstNewLibrary->lpstFunction[0].dwBehind + 1), GHND);
        if (!lpstNewLibrary->lpstFunction)
        {
            MessageBox(0, TEXT("FIXME:Unablt to add elements in function array"), 0, 0);
            ExitProcess(0);
        }
    }

    lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].dwBehind = 0;
    lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpszFunction = lpszFunction;
    lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpSource = lpSource;
    lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpBackup = WaffleGetProcAddress(lpstNewLibrary->hBackup, lpszFunction);
    if (!hDetour)
    {
        lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpDetour = 0;
    }
    else if (!lpszDetour)
    {
        DWORD nSize = lstrlen(lpszFunction) + sizeof(TEXT("Detour"));
        LPTSTR lpszDefaultDetour = (LPTSTR) GlobalAlloc(GPTR, nSize*sizeof(TCHAR));
        wsprintf(lpszDefaultDetour, TEXT("Detour%s"), lpszFunction);
        lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpDetour = WaffleGetProcAddress(hDetour, lpszDefaultDetour);
        GlobalFree(lpszDefaultDetour);
    }
    else
    {
        lpstNewLibrary->lpstFunction[lpstNewLibrary->lpstFunction[0].dwBehind].lpDetour = WaffleGetProcAddress(hDetour, lpszDetour);
    }
}

LIBRARY_EXPORT VOID WINAPI WaffleCreateFunctionArray(
    _In_    LPWAFFLE_LIBRARY_ARRAY lpstNewLibrary
    )
{
    LPWAFFLE_PROCESS_SETTING lpstProcessSetting = WaffleOpenProcessSetting();

    //GetAllKey
    LPTSTR lpszKey = WaffleGetOptionSectionKeys(lpstProcessSetting, TEXT("Detour.ini"), lpstNewLibrary->lpszLibrary);
    //get the list
    LPTSTR lpszNextKey = lpszKey;
    DWORD nSizeOfKey = lstrlen(lpszNextKey);
    while (nSizeOfKey)
    {
        int i;
        for (i = 0; lpszNextKey[i] != TEXT('='); i++);
        lpszNextKey[i] = TEXT('\0');

        DWORD nSizeOfFunction = lstrlen(lpszNextKey);
        WaffleAddFunction(lpstNewLibrary, lpszNextKey, GetModuleHandle(lpszNextKey + nSizeOfFunction + 1), NULL);

        lpszNextKey = lpszNextKey + nSizeOfKey + 1;
        nSizeOfKey = lstrlen(lpszNextKey);
    }
}

LIBRARY_EXPORT BOOL WINAPI WaffleSetDetour(
    _In_    DWORD dwLibrary,
    _In_    DWORD dwFunction
    )
{
    if (dwLibrary > lpstLibrary[0].dwBehind)
    {
        return FALSE;
    }
    if (dwFunction > lpstLibrary[dwLibrary].lpstFunction[0].dwBehind)
    {
        return FALSE;
    }
    //MessageBox(0, lpstLibrary[dwLibrary].lpstFunction[dwFunction].lpszFunctionT, 0, 0);
    //WaffleIntBox(dwLibrary);
    //WaffleIntBox(dwFunction);
    LPWAFFLE_FUNCTION_ARRAY lpstFunction = &lpstLibrary[dwLibrary].lpstFunction[dwFunction];
    DWORD flOldProtect;
    if (lpstFunction->lpDetour)
    {
        VirtualProtect(lpstFunction->lpSource, 1, PAGE_EXECUTE_READWRITE, &flOldProtect);
        WAFFLE_PORT_WRITE_EXCEPTION_INSTRUCTION(lpstFunction->lpSource);
        VirtualProtect(lpstFunction->lpSource, 1, flOldProtect, &flOldProtect);
    }
    return TRUE;
}

LIBRARY_EXPORT LONG CALLBACK WaffleExceptionHandler(
    _In_    PEXCEPTION_POINTERS ExceptionInfo
    )
{
    if ((ExceptionInfo->ExceptionRecord->ExceptionCode == WAFFLE_PORT_EXCEPTION_CODE) && (*(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA *) (ExceptionInfo->ExceptionRecord->ExceptionAddress) == (WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA) WAFFLE_PORT_EXCEPTION_INSTRUCTION))
    {
        int i;
        for (i = 0; lpstLibrary[i].lpszLibrary; i++)
        {
            LPWAFFLE_FUNCTION_ARRAY lpstFunction = lpstLibrary[i].lpstFunction;    //kernel32中的api可能是ntdll的存根
            if (((SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress >= (SIZE_T) lpstLibrary[i].hSource) && ((SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress <= (SIZE_T) lpstLibrary[i].hSourceEnd))
            {
                int j;
                for (j = 0; lpstFunction[j].lpszFunction; j++)
                {
                    if (lpstFunction[j].lpSource == ExceptionInfo->ExceptionRecord->ExceptionAddress)
                    {
                        if (lpstFunction[j].lpDetour)
                        {
                            ExceptionInfo->ContextRecord->WAFFLE_PORT_PROGRAM_POINTER = (SIZE_T) lpstFunction[j].lpDetour;
                            return EXCEPTION_CONTINUE_EXECUTION;
                        }
                        break;
                    }
                }
                ExceptionInfo->ContextRecord->WAFFLE_PORT_PROGRAM_POINTER = (SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress - (SIZE_T) lpstLibrary[i].hSource + (SIZE_T) lpstLibrary[i].hBackup;
                return EXCEPTION_CONTINUE_EXECUTION;
            }
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

LIBRARY_EXPORT LPVOID WINAPI WaffleGetBackupAddress(
    _In_    LPCTSTR lpszLibrary,
    _In_    LPCTSTR lpszFunction
    )
{
    int i;
    for (i = lpstLibrary[0].dwBehind; i >= 0; i--)
    {
        if (!Wafflelstrcmpi(lpszLibrary, lpstLibrary[i].lpszLibrary))
        {
            //return WaffleGetProcAddress(lpstLibrary[i].hBackup, lpszFunction); uses WideCharToMultiByte
            ///*
            //MessageBox(0, lpszLibrary, lpstLibrary[i].lpszLibrary, 0);
            int j;
            for (j = lpstLibrary[i].lpstFunction[0].dwBehind; j >= 0; j--)
            {
                //MessageBox(0, lpszFunction, lpstLibrary[i].lpstFunction[j].lpszFunction, 0);
                if (!Wafflelstrcmp(lpszFunction, lpstLibrary[i].lpstFunction[j].lpszFunction))
                {
                    return lpstLibrary[i].lpstFunction[j].lpBackup;
                }
            }
            break;
            //*/
        }
    }
    return NULL;
}