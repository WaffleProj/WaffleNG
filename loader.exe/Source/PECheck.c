#define  UNICODE
#include <windows.h>
#include "..\loader.h"

WORD WINAPI GetPEMagic(LPVOID lpFile)
{
    WORD Magic = 0;
    if (((PIMAGE_DOS_HEADER)lpFile)->e_magic == IMAGE_DOS_SIGNATURE)                            //首先出现的是DOS文件标志
    {
        PIMAGE_NT_HEADERS lpNtHeader = lpFile + ((PIMAGE_DOS_HEADER)lpFile)->e_lfanew;
        if (lpNtHeader->Signature == IMAGE_NT_SIGNATURE)                                        //由DOS头部决定PE文件头部的位置
        {
            if (lpNtHeader->FileHeader.Characteristics & IMAGE_FILE_DLL)
                MessageBox(0,TEXT("[0004]This file is a dll."),0,0);                            //DLL文件
            else
                Magic = lpNtHeader->OptionalHeader.Magic;
        }
        else
            MessageBox(0,TEXT("[0003]This PE file is targeting another platform."),0,0);        //DOS或OS/2的PE文件
    }
    else
        MessageBox(0,TEXT("[0002]This is not a legal PE file."),0,0);                           //不明文件种类
    
    return Magic;
}