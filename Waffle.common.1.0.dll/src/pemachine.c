#define  UNICODE
#define  _UNICODE
#include "..\common.h"

LIBRARY_EXPORT WORD WINAPI WaffleGetMachineType(LPCTSTR lpszFile)
{
    //Disable redirection if the target is in the "system32"
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    PVOID OldValue;

    //Open the target
    LPWOW64DISABLEWOW64FSREDIRECTION lpWow64DisableWow64FsRedirection = (LPVOID)GetProcAddress(hKernel32,"Wow64DisableWow64FsRedirection");
    LPWOW64REVERTWOW64FSREDIRECTION lpWow64RevertWow64FsRedirection = (LPVOID)GetProcAddress(hKernel32,"Wow64RevertWow64FsRedirection");

    if (lpWow64DisableWow64FsRedirection)
    {
        lpWow64DisableWow64FsRedirection(&OldValue);
    }
    HANDLE hFile = CreateFile(lpszFile,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if (lpWow64RevertWow64FsRedirection)
    {
        lpWow64RevertWow64FsRedirection(OldValue);
    }

    if  (hFile == INVALID_HANDLE_VALUE)
    {
        return 0xFFFF;
    }

    //Mapping it
    HANDLE hMapFile = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
    LPVOID lpFile = MapViewOfFile(hMapFile,FILE_MAP_READ,0,0,0);
    if (!lpFile)
    {
        CloseHandle(hMapFile);
        CloseHandle(hFile);
        return 0xFFFF;
    }

    //Get the machine type
    WORD Machine;
    if (((PIMAGE_DOS_HEADER)lpFile)->e_magic == IMAGE_DOS_SIGNATURE)
    {
        PIMAGE_NT_HEADERS lpNtHeader = lpFile + ((PIMAGE_DOS_HEADER)lpFile)->e_lfanew;
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