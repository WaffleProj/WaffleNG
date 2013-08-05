#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"

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
    if (!lpstProcessSetting->lpstLibrary)
    {
        lpstProcessSetting->lpstLibrary = (LPWAFFLE_LIBRARY_ARRAY) GlobalAlloc(GPTR, sizeof(WAFFLE_LIBRARY_ARRAY));
        if (!lpstProcessSetting->lpstLibrary)
        {
            MessageBox(0, TEXT("FIXME:Unablt to allocate memory for library array"), 0, 0);
            ExitProcess(0);
        }
        lpstNewLibrary->dwBehind = 0;
    }
    else
    {
        int i;
        for (i = lpstProcessSetting->lpstLibrary[0].dwBehind; i >= 0; i--)
        {
            lpstProcessSetting->lpstLibrary[i].dwBehind++;
        }
        lpstProcessSetting->lpstLibrary = (LPWAFFLE_LIBRARY_ARRAY) GlobalReAlloc(lpstProcessSetting->lpstLibrary, sizeof(WAFFLE_LIBRARY_ARRAY)*(lpstProcessSetting->lpstLibrary[0].dwBehind + 1), GHND);
        if (!lpstProcessSetting->lpstLibrary)
        {
            MessageBox(0, TEXT("FIXME:Unablt to add elements in library array"), 0, 0);
            ExitProcess(0);
        }
    }
    RtlMoveMemory(&lpstProcessSetting->lpstLibrary[lpstProcessSetting->lpstLibrary[0].dwBehind], lpstNewLibrary, sizeof(WAFFLE_LIBRARY_ARRAY));
    lpstProcessSetting->lpstLibrary[lpstProcessSetting->lpstLibrary[0].dwBehind].dwBehind = 0;
}

LIBRARY_EXPORT int WINAPI WaffleCreateLibraryArray(VOID)
{
    LPTSTR lpszSection = WaffleGetOptionSectionNames(TEXT("Detour.ini"));
    if (!lpszSection)
    {
        MessageBox(0, TEXT("FIXME:Unable to allocate more memory"), 0, 0);
        return 0;
    }

    int nLibrary = 0;
    {
        LPTSTR lpszNextSection = lpszSection;
        DWORD nSizeOfSection = lstrlen(lpszNextSection);
        while (nSizeOfSection)
        {
            //Check if we already loaded this library
            //for now we just load everything because we don't have a function to work with LoadLibrary
            //HMODULE hLibrary = GetModuleHandle(lpszNext);
            HMODULE hLibrary = LoadLibrary(lpszNextSection);
            if (hLibrary)
            {
                nLibrary++;
                WAFFLE_LIBRARY_ARRAY stLibrary;
                RtlZeroMemory(&stLibrary, sizeof(stLibrary));
                stLibrary.lpszLibrary = lpszNextSection;
                WaffleCopyLibrary(&stLibrary);
                if (WaffleCreateFunctionArray(&stLibrary))
                {
                    WaffleAddLibrary(&stLibrary);
                }
            }

            lpszNextSection = lpszNextSection + nSizeOfSection + 1;
            nSizeOfSection = lstrlen(lpszNextSection);
        }
    }
    return nLibrary;
}