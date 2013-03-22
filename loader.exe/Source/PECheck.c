#define  UNICODE
#include <windows.h>
#include "..\loader.h"

#include <stdio.h>

WORD WINAPI GetPEMagic(LPVOID lpFile)
{
    WORD Magic = 0;
    if (((PIMAGE_DOS_HEADER)lpFile)->e_magic == IMAGE_DOS_SIGNATURE)                            //首先出现的是DOS文件标志
    {
        PIMAGE_NT_HEADERS lpNtHeader = lpFile + ((PIMAGE_DOS_HEADER)lpFile)->e_lfanew;
        if (lpNtHeader->Signature == IMAGE_NT_SIGNATURE)                                        //由DOS头部决定PE文件头部的位置
        {
            if (lpNtHeader->FileHeader.Characteristics & IMAGE_FILE_DLL)
                printf("[0004]This file is a dll.\n");
                //DLL文件
            else
                Magic = lpNtHeader->OptionalHeader.Magic;
        }
        else
            printf("[0003]This PE file is targeting another platform.\n");
            //DOS或OS/2的PE文件
    }
    else
        printf("[0002]This is not a legal PE file.\n");
        //不明文件种类
    
    return Magic;
}