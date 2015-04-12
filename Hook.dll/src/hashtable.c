#include "..\main.h"

LPVOID WaffleHastTable[0x10000];

WORD WaffleHashTableFindIndex(LPVOID lpAddress)
{
    LPWORD lpData = (LPWORD) &lpAddress;
    WORD Index = 0;
    for (int i = 0; i < sizeof(SIZE_T) / sizeof(WORD); i++)
    {
        Index += lpData[i];
    }
    return Index;
}

