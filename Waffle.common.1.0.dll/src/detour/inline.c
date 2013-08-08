#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\..\common.h"

#if (WAFFLE_PORT_MACHINE == WAFFLE_PORT_MACHINE_I386)
LIBRARY_EXPORT BOOL WINAPI WaffleInlineDetour(
    _In_    LPBYTE  lpFunction
    )
{
    LPBYTE lpHotpatch = lpFunction - 5;
    LPBYTE Signature = (LPBYTE)"\x90\x90\x90\x90\x90\x8B\xFF";
    int i;
    for (i = 0; i < 7; i++)
    {
        if (lpHotpatch[i] != Signature[i])
        {
            return FALSE;
        }
    }

    DWORD flOldProtect;
    VirtualProtect(lpHotpatch, 7, PAGE_EXECUTE_READWRITE, &flOldProtect);

    lpHotpatch[0] = 0xE8;    //call
    *(LPDWORD) &lpHotpatch[1] = (DWORD) WaffleInlineHandler - (DWORD) lpFunction;
    *(LPWORD) &lpHotpatch[5] = 0xF9EB;    //jmp $-7

    FlushInstructionCache(GetCurrentProcess(), lpHotpatch, 7);
    VirtualProtect(lpHotpatch, 7, flOldProtect, &flOldProtect);

    return TRUE;
}

#pragma GCC push_options
#pragma GCC optimize ("0")
#pragma optimize("", off)
//Using cdecl so there is no code for cleaning the stack
LIBRARY_EXPORT VOID WaffleInlineHandler(
    _In_    SIZE_T *lpReserved
    )
{
    // ebp-c Argc0
    // ebp-8 ReturnAddress <== (&lpReserved)[0]
    // ebp-4 HotpatchReturnAddress <== (&lpReserved)[-1]
    // ebp   EBP <== (&lpReserved)[-2]
    // ebp+4 Local variable <== (&lpReserved)[-3] IT WON'T LAST LONG, BE CAREFUL!

    (&lpReserved)[-3] = (SIZE_T *) WaffleFindDetourAddress((SIZE_T *) (&lpReserved)[-1], (SIZE_T *) (&lpReserved)[0]);
    if ((&lpReserved)[-3])
    {
        /*
        __asm
        {
        mov     esp, ebp;
        pop     ebp;
        add     esp, 4; //hot patch call return address
        jmp     dword ptr [esp - 12]; //Local variable
        }
        */
        (&lpReserved)[-1] = (&lpReserved)[-3];
        return;
    }
    else
    {
        (&lpReserved)[-1] = (SIZE_T *) ((SIZE_T) (&lpReserved)[-1] + 2); //HotpatchReturnAddress += 2
        return;
    }
}
#pragma optimize("", on)
#pragma GCC pop_options
#else
LIBRARY_EXPORT BOOL WINAPI WaffleInlineDetour(
    _In_    LPBYTE  lpFunction
    )
{
    return FALSE;
}

LIBRARY_EXPORT VOID WaffleInlineHandler(
    _In_    SIZE_T *lpReserved
    )
{
    return;
}
#endif