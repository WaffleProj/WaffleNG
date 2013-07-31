#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "mojibake.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "src\kernel32.c"
#include "src\user32.c"
#include "src\gdi32.c"
#include "src\psapi.c"
#include "src\shell32.c"
#ifdef __cplusplus
};
#endif

ENVIRONMENT_BLOCK   stOldEnvir;
ENVIRONMENT_BLOCK   stNewEnvir;

WAFFLE_FUNCTION_ARRAY stKernel32Table [] =
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
WAFFLE_FUNCTION_ARRAY stUser32Table [] =
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
WAFFLE_FUNCTION_ARRAY stGdi32Table [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {"CreateFontA", DetourCreateFontA},
    { "TextOutA", DetourTextOutA },
    { NULL },
};
WAFFLE_FUNCTION_ARRAY stPsapiTable [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {"GetModuleFileNameExA", DetourGetModuleFileNameExA},
    { NULL },
};

WAFFLE_FUNCTION_ARRAY stNtdllTable [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {NULL},
};

WAFFLE_FUNCTION_ARRAY stShell32Table [] =
{
    //lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
    {"ShellAboutW", DetourShellAboutW},
    { NULL },
};

WAFFLE_LIBRARY_ARRAY stLibraryTable [] =
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