#define  UNICODE
#define _UNICODE
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "mojibake.h"
#include "src\detour\kernel32.c"
#include "src\detour\user32.c"
#include "src\detour\gdi32.c"
#include "src\detour\psapi.c"
#include "src\detour\shell32.c"

HINSTANCE   hDLL;
HANDLE      hHeap;

ENVIRONMENT_BLOCK   stOldEnvir;
ENVIRONMENT_BLOCK   stNewEnvir;

HGLOBAL lpszCommandLineA;

HOOK_TABLE_OBJECT stKernel32Table [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {"CreateDirectoryA", DetourCreateDirectoryA},
    { "CreateFileA", DetourCreateFileA },
    { "DeleteFileA", DetourDeleteFileA },
    { "FindFirstFileA", DetourFindFirstFileA },
    { "FindNextFileA", DetourFindNextFileA },
    { "GetACP", DetourGetACP },
    { "GetCommandLineA", DetourGetCommandLineA },
    { "GetCPInfo", DetourGetCPInfo },
    { "GetFileAttributesA", DetourGetFileAttributesA },
    { "GetModuleFileNameA", DetourGetModuleFileNameA },
    { "GetModuleHandleA", DetourGetModuleHandleA },
    { "GetOEMCP", DetourGetOEMCP },
    { "LoadLibraryA", DetourLoadLibraryA },
    { "LoadLibraryExA", DetourLoadLibraryExA },
    { "MultiByteToWideChar", DetourMultiByteToWideChar },
    { "SetCurrentDirectoryA", DetourSetCurrentDirectoryA },
    { "SetFileAttributesA", DetourSetFileAttributesA },
    { "WideCharToMultiByte", DetourWideCharToMultiByte },
    { NULL },
};
HOOK_TABLE_OBJECT stUser32Table [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {"CallWindowProcA", DetourCallWindowProcA},
    { "CreateWindowExA", DetourCreateWindowExA },
    { "MessageBoxA", DetourMessageBoxA },
    { "MessageBoxExA", DetourMessageBoxExA },
    { "SendMessageA", DetourSendMessageA },
    { "SetWindowTextA", DetourSetWindowTextA },
    { NULL },
};
HOOK_TABLE_OBJECT stGdi32Table [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {"CreateFontA", DetourCreateFontA},
    { "TextOutA", DetourTextOutA },
    { NULL },
};
HOOK_TABLE_OBJECT stPsapiTable [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {"GetModuleFileNameExA", DetourGetModuleFileNameExA},
    { NULL },
};

HOOK_TABLE_OBJECT stNtdllTable [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {NULL},
};

HOOK_TABLE_OBJECT stShell32Table [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {"ShellAboutW", DetourShellAboutW},
    { NULL },
};

LIBRARY_TABLE_OBJECT stLibraryTable [] =
{
    //lpszLibrary lpHookTable lpLibrary hModule lpEndOfModule
    {L"kernel32.dll", stKernel32Table},
    { L"user32.dll", stUser32Table },
    { L"gdi32.dll", stGdi32Table },
    { L"psapi.dll", stPsapiTable },
    { L"ntdll.dll", stNtdllTable },
    { L"shell32.dll", stShell32Table },
    { NULL },
};

LPWSPRINTFA         _wsprintfA;
LPVIRTUALPROTECT    _VirtualProtect;