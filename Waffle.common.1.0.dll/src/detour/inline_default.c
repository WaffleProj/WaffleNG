#include "..\..\common.h"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
    DebugBreak();
    return;
}