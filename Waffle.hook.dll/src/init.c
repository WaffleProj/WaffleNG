#include "..\main.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

LIBRARY_EXPORT SIZE_T WINAPI WaffleInit(
    _In_    LPVOID lpReserved
    )
{
    if (lpstLibrary)
    {
        return FALSE;
    }

    return 0;
}