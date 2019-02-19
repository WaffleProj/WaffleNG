#include "..\common.h"

WAFFLE_COMMON_DLL_FUNCTION int WINAPI WaffleFindLibrary(
    _In_    LPVOID lpMemory
    )
{
    if (!lpstProcessSetting->lpstLibrary)
    {
        return 0;
    }

    int i;
    for (i = lpstProcessSetting->lpstLibrary[0].dwBehind; i >= 0; i--)
    {
        if (((SIZE_T) lpMemory >= (SIZE_T) lpstProcessSetting->lpstLibrary[i].hSource) && ((SIZE_T) lpMemory <= lpstProcessSetting->lpstLibrary[i].hSourceEnd))
        {
            return i;
        }
    }
    return -1;
}

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleCopyLibrary(
    _In_    LPWAFFLE_LIBRARY_ARRAY lpstNewLibrary
    )
{
    if (!lpstNewLibrary)
    {
        return;
    }

    if (lpstProcessSetting->lpstLibrary)
    {
        int i;
        for (i = lpstProcessSetting->lpstLibrary[0].dwBehind; i >= 0; i--)
        {
            if (lpstNewLibrary->hSource == lpstProcessSetting->lpstLibrary[i].hSource)
            {
                lpstNewLibrary->hSourceEnd = lpstProcessSetting->lpstLibrary[i].hSourceEnd;
                lpstNewLibrary->hBackup = lpstProcessSetting->lpstLibrary[i].hBackup;
                lpstNewLibrary->hBackupEnd = lpstProcessSetting->lpstLibrary[i].hBackupEnd;
                return;
            }
        }
    }

    //Get dll base address and size
    DWORD dwSizeOfImage = WaffleGetImageSize(lpstNewLibrary->hSource);
    lpstNewLibrary->hSourceEnd = (SIZE_T) lpstNewLibrary->hSource + dwSizeOfImage;

    //Reserve memory address
    lpstNewLibrary->hBackup = (HMODULE) VirtualAlloc(NULL, dwSizeOfImage, MEM_RESERVE, PAGE_NOACCESS);
    lpstNewLibrary->hBackupEnd = (SIZE_T) lpstNewLibrary->hBackup + dwSizeOfImage;

    LPVOID addrPointer = lpstNewLibrary->hSource;
    LPVOID addrEnd = (LPVOID) lpstNewLibrary->hSourceEnd;
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

    return;
}

WAFFLE_COMMON_DLL_FUNCTION VOID WINAPI WaffleAddLibrary(
    _In_    HMODULE hModule
    )
{
    //Check argument
    if (!hModule)
    {
        return;
    }

    //Do nothing if we have one
    if (lpstProcessSetting->lpstLibrary)
    {
        int i;
        for (i = lpstProcessSetting->lpstLibrary[0].dwBehind; i >= 0; i--)
        {
            if (hModule == lpstProcessSetting->lpstLibrary[i].hSource)
            {
                return;
            }
        }
    }

    LPWAFFLE_LIBRARY_ARRAY lpstLibrary = lpstProcessSetting->lpstLibrary;

    //Allocate the memory for new library
    if (!lpstLibrary)
    {
        lpstLibrary = (LPWAFFLE_LIBRARY_ARRAY) WaffleAlloc(sizeof(WAFFLE_LIBRARY_ARRAY));
        if (!lpstLibrary)
        {
            MessageBox(0, TEXT("FIXME:Unable to allocate memory for library array"), 0, 0);
            ExitProcess(0);
        }
    }
    else
    {
        int i;
        for (i = lpstLibrary[0].dwBehind; i >= 0; i--)
        {
            lpstLibrary[i].dwBehind++;
        }
        lpstLibrary = (LPWAFFLE_LIBRARY_ARRAY) WaffleReAlloc(lpstLibrary, sizeof(WAFFLE_LIBRARY_ARRAY) * (lpstLibrary[0].dwBehind + 1));
        if (!lpstLibrary)
        {
            MessageBox(0, TEXT("FIXME:Unable to add elements in library array"), 0, 0);
            ExitProcess(0);
        }
    }
    lpstProcessSetting->lpstLibrary = lpstLibrary;

    //get the information
    WAFFLE_LIBRARY_ARRAY stNewLibrary;
    stNewLibrary.dwBehind = 0;
    stNewLibrary.lpszLibrary = (LPTSTR) WaffleAlloc(MAX_PATH * sizeof(TCHAR));
    GetModuleFileName(hModule, stNewLibrary.lpszLibrary, MAX_PATH);
    stNewLibrary.hSource = hModule;
    stNewLibrary.lpstFunction = NULL;
    WaffleCopyLibrary(&stNewLibrary);
    RtlMoveMemory(&lpstLibrary[lpstLibrary[0].dwBehind], &stNewLibrary, sizeof(WAFFLE_LIBRARY_ARRAY));
}