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
    lpstNewLibrary->lpSource = GetModuleHandle(lpstNewLibrary->lpszLibrary);
    if (!lpstNewLibrary->lpSource)
    {
        return;
    }

    //Get dll base address and size
    MODULEINFO  stModuleInfo;
    GetModuleInformation(GetCurrentProcess(), (HMODULE) lpstNewLibrary->lpSource, &stModuleInfo, sizeof(stModuleInfo));
    lpstNewLibrary->lpSource = stModuleInfo.lpBaseOfDll;
    lpstNewLibrary->lpSourceEnd = (LPVOID) ((SIZE_T) lpstNewLibrary->lpSource + stModuleInfo.SizeOfImage);

    //Reserve memory address
    lpstNewLibrary->lpBackup = (LPVOID) VirtualAlloc(NULL, stModuleInfo.SizeOfImage, MEM_RESERVE, PAGE_NOACCESS);
    lpstNewLibrary->lpBackupEnd = (LPVOID) ((SIZE_T) lpstNewLibrary->lpBackup + stModuleInfo.SizeOfImage);

    LPVOID addrPointer = lpstNewLibrary->lpSource;
    LPVOID addrEnd = lpstNewLibrary->lpSourceEnd;
    while (addrPointer < addrEnd)
    {
        MEMORY_BASIC_INFORMATION stMemInfo;
        VirtualQuery(addrPointer, &stMemInfo, sizeof(stMemInfo));
        if (stMemInfo.State == MEM_COMMIT)
        {
            //Get offset of the memory
            LPVOID addrNew = (LPVOID) ((SIZE_T) stMemInfo.BaseAddress - (SIZE_T) stMemInfo.AllocationBase + (SIZE_T) lpstNewLibrary->lpBackup);

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
    }
    else
    {
        int i;
        for (i = lpstLibrary[0].dwBehind; i; i--)
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
