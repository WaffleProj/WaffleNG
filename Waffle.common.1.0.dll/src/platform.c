#include "..\common.h"

LIBRARY_EXPORT BOOL WINAPI WaffleIsDotNET(
    _In_    LPCTSTR lpszFile
    )
{
    PVOID OldValue = 0;
    WaffleDisableWow64FsRedirection(&OldValue);
    HANDLE hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    WaffleRevertWow64FsRedirection(OldValue);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    //Mapping it
    HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    LPVOID lpFile = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
    if (!lpFile)
    {
        CloseHandle(hMapFile);
        CloseHandle(hFile);
        return FALSE;
    }

    //Get the pe type
    BOOL bDotNET;
    if (((PIMAGE_DOS_HEADER) lpFile)->e_magic == IMAGE_DOS_SIGNATURE)
    {
        PIMAGE_NT_HEADERS lpNtHeader = (PIMAGE_NT_HEADERS) ((SIZE_T) lpFile + ((PIMAGE_DOS_HEADER) lpFile)->e_lfanew);
        /*
        if (lpNtHeader->Signature == IMAGE_NT_SIGNATURE)
        {
            if (lpNtHeader->FileHeader.Characteristics & IMAGE_FILE_DLL)    //DLL File
            {
                Machine = 0;
            }
            else
                Machine = lpNtHeader->FileHeader.Machine;
        }
        else    //DOS or OS/2 File
        {
            Machine = 0;
        }
        */
    }
    else    //Unknown File Type
    {
        bDotNET = FALSE;
    }

    //Clean up
    UnmapViewOfFile(lpFile);
    CloseHandle(hMapFile);
    CloseHandle(hFile);         //Has to close this handle for CreateProcess

    return  bDotNET;
}

LIBRARY_EXPORT WORD WINAPI WaffleGetMachineType(
    _In_    LPCTSTR lpszFile
    )
{
    PVOID OldValue = 0;
    WaffleDisableWow64FsRedirection(&OldValue);
    HANDLE hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    WaffleRevertWow64FsRedirection(OldValue);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return 0xFFFF;
    }

    //Mapping it
    HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    LPVOID lpFile = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
    if (!lpFile)
    {
        CloseHandle(hMapFile);
        CloseHandle(hFile);
        return 0xFFFF;
    }

    //Get the machine type
    WORD Machine;
    if (((PIMAGE_DOS_HEADER) lpFile)->e_magic == IMAGE_DOS_SIGNATURE)
    {
        PIMAGE_NT_HEADERS lpNtHeader = (PIMAGE_NT_HEADERS) ((SIZE_T) lpFile + ((PIMAGE_DOS_HEADER) lpFile)->e_lfanew);
        if (lpNtHeader->Signature == IMAGE_NT_SIGNATURE)
        {
            if (lpNtHeader->FileHeader.Characteristics & IMAGE_FILE_DLL)    //DLL File
            {
                Machine = 0;
            }
            else
                Machine = lpNtHeader->FileHeader.Machine;
        }
        else    //DOS or OS/2 File
        {
            Machine = 0;
        }
    }
    else    //Unknown File Type
    {
        Machine = 0;
    }

    //Clean up
    UnmapViewOfFile(lpFile);
    CloseHandle(hMapFile);
    CloseHandle(hFile);         //Has to close this handle for CreateProcess

    return  Machine;
}