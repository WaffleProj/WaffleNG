#ifndef __SDK_WAFFLE_PSDK_PSDK_H_
#define __SDK_WAFFLE_PSDK_PSDK_H_

#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <intrin.h>
#include <winternl.h>

#ifndef  _In_
#define  _In_
#endif
#ifndef  _In_opt_
#define  _In_opt_
#endif
#ifndef  _Out_
#define  _Out_
#endif
#ifndef  _Outptr_
#define  _Outptr_
#endif
#ifndef  _Outptr_
#define  _Outptr_
#endif
#ifndef  _Out_writes_
#define  _Out_writes_(x)
#endif
#ifndef  _Out_writes_to_opt_
#define  _Out_writes_to_opt_(x)
#endif
#ifndef  _Out_writes_bytes_all_
#define  _Out_writes_bytes_all_(x)
#endif
#ifndef  _Inout_
#define  _Inout_
#endif
#ifndef  _Inout_opt_
#define  _Inout_opt_
#endif
#ifndef  _Reserved_
#define  _Reserved_
#endif
#ifndef  _Frees_ptr_opt_
#define  _Frees_ptr_opt_
#endif

#ifdef __cplusplus
#define LIBRARY_EXPORT      extern "C" __declspec(dllexport)
#define LIBRARY_IMPORT      extern "C" __declspec(dllimport)
#else
#define LIBRARY_EXPORT      __declspec(dllexport)
#define LIBRARY_IMPORT      __declspec(dllimport)
#endif

#define INLINE              inline
#if     defined(__GNUC__)
#define NOINLINE            __attribute__((noinline))
#elif   defined(_MSC_VER)
#define NOINLINE            __declspec(noinline)
#endif

#define lengthof(x)     (sizeof((x))/sizeof((x)[0]))

#undef  RtlMoveMemory
LIBRARY_IMPORT VOID WINAPI RtlMoveMemory(
    _Out_writes_bytes_all_(Length)  PVOID Destination,
    _In_    const VOID *Source,
    _In_    SIZE_T Length
    );

#undef  RtlCopyMemory
LIBRARY_IMPORT VOID WINAPI RtlCopyMemory(
    _Out_writes_bytes_all_(Length)  PVOID Destination,
    _In_    const VOID *Source,
    _In_    SIZE_T Length
    );

#undef  RtlFillMemory
LIBRARY_IMPORT VOID WINAPI RtlFillMemory(
    _Out_writes_bytes_all_(Length)  PVOID Destination,
    _In_    SIZE_T Length,
    _In_    BYTE Fill
    );

#undef  RtlZeroMemory
LIBRARY_IMPORT VOID WINAPI RtlZeroMemory(
    _Out_writes_bytes_all_(Length)  PVOID Destination,
    _In_    SIZE_T Length
    );

#include "kernel32.h"
#include "user32.h"
#include "gdi32.h"
#include "advapi32.h"
#include "shell32.h"
#include "psapi.h"
#include "comctl32.h"
#include "ole32.h"

#ifdef WAFFLE_DISABLE_CODE_ANALYSIS
#ifdef _When_
#undef _When_
#endif
#define _When_(x,y)

#ifdef _Success_
#undef _Success_
#endif
#define _Success_(x)

#ifdef _Check_return_
#undef _Check_return_
#endif
#define _Check_return_

#ifdef _Ret_maybenull_
#undef _Ret_maybenull_
#endif
#define _Ret_maybenull_
#endif

#endif /* __SDK_WAFFLE_PSDK_PSDK_H_ */