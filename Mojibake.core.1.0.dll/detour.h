#ifndef __GLOBAL_H_
#define __GLOBAL_H_

typedef struct
{
    UINT ACP;
    UINT OEMCP;
    LCID ThreadLocale;
} ENVIRONMENT_BLOCK, *LPENVIRONMENT_BLOCK;

extern ENVIRONMENT_BLOCK    stOldEnvir;
extern ENVIRONMENT_BLOCK    stNewEnvir;

extern	WAFFLE_FUNCTION_ARRAY   stKernel32Table [];
#define CREATEDIRECTORYA        0
#define CREATEFILEA             1
#define DELETEFILEA             2
#define FINDFIRSTFILEA          3
#define FINDNEXTFILEA           4
#define GETACP                  5
#define GETCOMMANDLINEA         6
#define GETCPINFO               7
#define GETFILEATTRIBUTESA      8
#define GETMODULEFILENAMEA      9
#define GETMODULEHANDLEA        10
#define GETOEMCP                11
#define LOADLIBRARYA            12
#define LOADLIBRARYEXA          13
#define MULTIBYTETOWIDECHAR     14
#define SETCURRENTDIRECTORYA    15
#define SETFILEATTRIBUTESA      16
#define WIDECHARTOMULTIBYTE     17
extern	WAFFLE_FUNCTION_ARRAY   stUser32Table [];
#define CALLWINDOWPROCA         0
#define CREATEWINDOWEXA         1
#define MESSAGEBOXA             2
#define MESSAGEBOXEXA           3
#define SENDMESSAGEA            4
#define SETWINDOWTEXTA          5
extern	WAFFLE_FUNCTION_ARRAY   stGdi32Table [];
#define CREATEFONTA             0
#define TEXTOUTA                1
extern	WAFFLE_FUNCTION_ARRAY   stPsapiTable [];
#define GETMODULEFILENAMEEXA    0
extern	WAFFLE_FUNCTION_ARRAY   stShell32Table [];
#define SHELLABOUTW             0

extern WAFFLE_LIBRARY_ARRAY     stLibraryTable [];
#define KERNEL32                0
#define USER32                  1
#define GDI32                   2
#define PSAPI                   3
#define NTDLL                   4
#define SHELL32                 5

extern LPWSPRINTFA _wsprintfA;

extern LPVIRTUALPROTECT _VirtualProtect;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
};
#endif

#endif /* __GLOBAL_H_ */