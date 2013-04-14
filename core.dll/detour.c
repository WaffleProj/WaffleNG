#define  UNICODE
#include <windows.h>
#include "core.h"
#include "src\detour\kernel32.c"
#include "src\detour\user32.c"
#include "src\detour\gdi32.c"
#include "src\detour\psapi.c"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

HINSTANCE   hDLL;
HANDLE      hHeap;

ENVIRONMENT_BLOCK   stOldEnvir;
ENVIRONMENT_BLOCK   stNewEnvir;

HGLOBAL lpszCommandLineA;

HOOK_TABLE_OBJECT stKernel32Table[] = {
//lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
#define CREATEDIRECTORYA 0
{"CreateDirectoryA",DetourCreateDirectoryA},
#define CREATEFILEA 1
{"CreateFileA",DetourCreateFileA},
#define DELETEFILEA 2
{"DeleteFileA",DetourDeleteFileA},
#define FINDFIRSTFILEA 3
{"FindFirstFileA",DetourFindFirstFileA},
#define FINDNEXTFILEA 4
{"FindNextFileA",DetourFindNextFileA},
#define GETACP 5
{"GetACP",DetourGetACP},
#define GETCOMMANDLINEA 6
{"GetCommandLineA",DetourGetCommandLineA},
#define GETCPINFO 7
{"GetCPInfo",DetourGetCPInfo},
#define GETFILEATTRIBUTESA 8
{"GetFileAttributesA",DetourGetFileAttributesA},
#define GETMODULEFILENAMEA 9
{"GetModuleFileNameA",DetourGetModuleFileNameA},
#define GETMODULEHANDLEA 10
{"GetModuleHandleA",DetourGetModuleHandleA},
#define LOADLIBRARYA 11
{"LoadLibraryA",DetourLoadLibraryA},
#define LOADLIBRARYEXA 12
{"LoadLibraryExA",DetourLoadLibraryExA},
#define MULTIBYTETOWIDECHAR 13
{"MultiByteToWideChar",DetourMultiByteToWideChar},
#define SETCURRENTDIRECTORYA 14
{"SetCurrentDirectoryA",DetourSetCurrentDirectoryA},
#define SETFILEATTRIBUTESA 15
{"SetFileAttributesA",DetourSetFileAttributesA},
#define WIDECHARTOMULTIBYTE 16
{"WideCharToMultiByte",DetourWideCharToMultiByte},
{NULL},
};
HOOK_TABLE_OBJECT stUser32Table[] = {
//lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
#define CALLWINDOWPROCA 0
{"CallWindowProcA",DetourCallWindowProcA},
#define CREATEWINDOWEXA 0
{"CreateWindowExA",DetourCreateWindowExA},
#define MESSAGEBOXA 0
{"MessageBoxA",DetourMessageBoxA},
#define MESSAGEBOXEXA 0
{"MessageBoxExA",DetourMessageBoxExA},
#define SENDMESSAGEA 0
{"SendMessageA",DetourSendMessageA},
#define SETWINDOWTEXTA 0
{"SetWindowTextA",DetourSetWindowTextA},
{NULL},
};
HOOK_TABLE_OBJECT stGdi32Table[] = {
//lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
#define CREATEFONTA 0
{"CreateFontA",DetourCreateFontA},
#define TEXTOUTA 0
{"TextOutA",DetourTextOutA},
{NULL},
};
HOOK_TABLE_OBJECT stPsapiTable[] = {
//lpszFunction lpDetourFunction lpNewFunction lpOriginalFunction
#define GETMODULEFILENAMEEXA 0
{"GetModuleFileNameExA",DetourGetModuleFileNameExA},
{NULL},
};

LIBRARY_TABLE_OBJECT stHookTable[] = {
//lpLibrary hModule lpEndOfModule lpHookTable lpszLibrary
#define KERNEL32 0
{L"kernel32.dll",stKernel32Table},
#define USER32 1
{L"user32.dll",stUser32Table},
#define GDI32 2
{L"gdi32.dll",stGdi32Table},
#define PSAPI 3
{L"psapi.dll",stPsapiTable},
{NULL},
};


HOOK_TABLE_OBJECT   stGetCPInfo;
HOOK_TABLE_OBJECT   stMultiByteToWideChar;
HOOK_TABLE_OBJECT   stWideCharToMultiByte;
HOOK_TABLE_OBJECT   stSendMessageA;
HOOK_TABLE_OBJECT   stCallWindowProcA;
HOOK_TABLE_OBJECT   stMessageBoxA = {"MessageBoxA"};
LPMESSAGEBOXA       _MessageBoxA;
LPWSPRINTFA         _wsprintfA;