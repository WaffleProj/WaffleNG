#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\..\common.h"

LIBRARY_EXPORT BOOL WINAPI WaffleExceptionDetour(
    _In_    LPBYTE  lpFunction
    )
{
    DWORD flOldProtect;
    VirtualProtect(lpFunction, sizeof(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA), PAGE_EXECUTE_READWRITE, &flOldProtect);

    WAFFLE_PORT_WRITE_EXCEPTION_INSTRUCTION(lpFunction);

    FlushInstructionCache(GetCurrentProcess(), lpFunction, sizeof(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA));
    VirtualProtect(lpFunction, sizeof(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA), flOldProtect, &flOldProtect);
    return TRUE;
}

LIBRARY_EXPORT LONG CALLBACK WaffleExceptionHandler(
    _In_    PEXCEPTION_POINTERS ExceptionInfo
    )
{
    switch (ExceptionInfo->ExceptionRecord->ExceptionCode)
    {
    case WAFFLE_PORT_EXCEPTION_CODE:
        if (*(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA *) (ExceptionInfo->ExceptionRecord->ExceptionAddress) == (WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA) WAFFLE_PORT_EXCEPTION_INSTRUCTION)
        {
            SIZE_T lpCaller = *(SIZE_T *) (ExceptionInfo->ContextRecord->WAFFLE_PORT_STACK_POINTER);
            SIZE_T lpDetour = WaffleFindDetourAddress(ExceptionInfo->ExceptionRecord->ExceptionAddress, (PVOID) lpCaller);
            if (lpDetour)
            {
                ExceptionInfo->ContextRecord->WAFFLE_PORT_PROGRAM_POINTER = lpDetour;
                return EXCEPTION_CONTINUE_EXECUTION;
            }
        }
        break;
    case 0x0EEDFADE:
        {
            TCHAR szExceptionRecord[2048];

            wsprintf(szExceptionRecord, TEXT("ExceptionRecord->ExceptionCode = %08x\nExceptionRecord->ExceptionFlags = %08x\nExceptionRecord->ExceptionRecord = %016I64X\nExceptionRecord->ExceptionAddress = %016I64X\nExceptionRecord->NumberParameters = %08x"), ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ExceptionRecord->ExceptionFlags, (UINT64) (ExceptionInfo->ExceptionRecord->ExceptionRecord), (UINT64) (ExceptionInfo->ExceptionRecord->ExceptionAddress), ExceptionInfo->ExceptionRecord->NumberParameters);

            TCHAR szBuf[256];
            wsprintf(szBuf, TEXT("\nExceptionInfo->ContextRecord->WAFFLE_PORT_STACK_POINTER = %016I64X"), (UINT64) (ExceptionInfo->ContextRecord->WAFFLE_PORT_STACK_POINTER));
            lstrcat(szExceptionRecord, szBuf);

            DWORD i;
            for (i = 0; i < ExceptionInfo->ExceptionRecord->NumberParameters; i++)
            {
                TCHAR szBuf[256];
                wsprintf(szBuf, TEXT("\nExceptionRecord->ExceptionInformation[%u] = %016I64X"), i, (UINT64) (ExceptionInfo->ExceptionRecord->ExceptionInformation[i]));
                lstrcat(szExceptionRecord, szBuf);
            }
            //MessageBox(0, szExceptionRecord, 0, 0);
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}