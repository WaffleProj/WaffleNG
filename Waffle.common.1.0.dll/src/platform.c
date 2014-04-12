#include "..\common.h"

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
        return WAFFLE_PORT_MACHINE_ERROR;
    }

    //Mapping it
    HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!hMapFile)
    {
        CloseHandle(hFile);
        return WAFFLE_PORT_MACHINE_ERROR;
    }
    LPVOID lpFile = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
    if (!lpFile)
    {
        CloseHandle(hMapFile);
        CloseHandle(hFile);
        return WAFFLE_PORT_MACHINE_ERROR;
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
                Machine = WAFFLE_PORT_MACHINE_UNKNOWN;
            }
            else
            {
                Machine = lpNtHeader->FileHeader.Machine;
            }
        }
        else    //DOS or OS/2 File
        {
            Machine = WAFFLE_PORT_MACHINE_UNKNOWN;
        }
    }
    else    //Unknown File Type
    {
        Machine = WAFFLE_PORT_MACHINE_UNKNOWN;
    }

    //Clean up
    UnmapViewOfFile(lpFile);
    CloseHandle(hMapFile);
    CloseHandle(hFile);         //Has to close this handle for CreateProcess

    return Machine;
}