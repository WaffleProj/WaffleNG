#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\mojibake.h"
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)
#endif

#ifdef __cplusplus
extern "C" {
#endif
    LIBRARY_EXPORT NTSTATUS WINAPI DetourRtlMultiByteToUnicodeN(
        _Out_       PWCH UnicodeString,
        _In_        ULONG MaxBytesInUnicodeString,
        _Out_opt_   PULONG BytesInUnicodeString,
        _In_        const CHAR *MultiByteString,
        _In_        ULONG BytesInMultiByteString
        )
    {
        MultiByteToWideChar(stNewEnvir.ANSICodePage, 0, MultiByteString, BytesInMultiByteString, UnicodeString, MaxBytesInUnicodeString);
        if (BytesInUnicodeString)
        {
            *BytesInUnicodeString = lstrlenW(UnicodeString);
        }
        return STATUS_SUCCESS;
    }

    LIBRARY_EXPORT NTSTATUS WINAPI DetourRtlUnicodeToMultiByteN(
        _Out_       PCHAR MultiByteString,
        _In_        ULONG MaxBytesInMultiByteString,
        _Out_opt_   PULONG BytesInMultiByteString,
        _In_        PCWCH UnicodeString,
        _In_        ULONG BytesInUnicodeString
        )
    {
        WideCharToMultiByte(stNewEnvir.ANSICodePage, 0, UnicodeString, BytesInUnicodeString, MultiByteString, MaxBytesInMultiByteString, NULL, FALSE);
        if (BytesInMultiByteString)
        {
            *BytesInMultiByteString = lstrlenA(MultiByteString);
        }
        return STATUS_SUCCESS;
    }
#ifdef __cplusplus
};
#endif