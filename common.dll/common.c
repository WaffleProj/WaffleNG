#define  UNICODE
#define _UNICODE
#include "common.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

LIBRARY_EXPORT BOOL WINAPI WaffleCreateProcess(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
){
    if (fdwReason == DLL_PROCESS_ATTACH) 
    {
        DisableThreadLibraryCalls(hinstDLL);
    }
    return TRUE;
}

BOOL WINAPI DllMain(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
){
    if (fdwReason == DLL_PROCESS_ATTACH) 
    {
        DisableThreadLibraryCalls(hinstDLL);
    }
    return TRUE;
}