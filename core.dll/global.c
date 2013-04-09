#define  UNICODE
#include <windows.h>
#include "core.h"

HINSTANCE   hDLL;
HANDLE      hHeap;

ENVIRONMENT_BLOCK   stOldEnvir;
ENVIRONMENT_BLOCK   stNewEnvir;

HGLOBAL lpszCommandLineA;

LPVOID  stHookTable[] = {
//lpNewFunction lpOldFunction lpszModule lpszFunction
0,0,0,0,
};

HOOK_TABLE_OBJECT   stGetCPInfo;
HOOK_TABLE_OBJECT   stMultiByteToWideChar;
HOOK_TABLE_OBJECT   stWideCharToMultiByte;
HOOK_TABLE_OBJECT   stSendMessageA;
HOOK_TABLE_OBJECT   stCallWindowProcA;
HOOK_TABLE_OBJECT   stMessageBoxA = {NULL,NULL,"MessageBoxA","User32.dll"};
LPMESSAGEBOXA       _MessageBoxA;
LPWSPRINTFA         _wsprintfA;