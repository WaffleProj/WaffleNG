#ifndef  UNICODE
#define  UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "..\common.h"

LIBRARY_EXPORT BOOL WINAPI WaffleSetDetour(
    _In_    DWORD dwLibrary,
    _In_    DWORD dwFunction
    )
{
    if (!lpstProcessSetting->lpstLibrary)
    {
        return FALSE;
    }
    else if (dwLibrary > lpstProcessSetting->lpstLibrary[0].dwBehind)
    {
        return FALSE;
    }
    else if (dwFunction > lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[0].dwBehind)
    {
        return FALSE;
    }
    LPWAFFLE_FUNCTION_ARRAY lpstFunction = &lpstProcessSetting->lpstLibrary[dwLibrary].lpstFunction[dwFunction];
    DWORD flOldProtect;
    if (lpstFunction->lpDetour)
    {
        VirtualProtect(lpstFunction->lpSource, sizeof(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA), PAGE_EXECUTE_READWRITE, &flOldProtect);
        WAFFLE_PORT_WRITE_EXCEPTION_INSTRUCTION(lpstFunction->lpSource);
        FlushInstructionCache(GetCurrentProcess(), lpstFunction->lpSource, sizeof(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA));
        VirtualProtect(lpstFunction->lpSource, sizeof(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA), flOldProtect, &flOldProtect);
    }
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
            int i;
            for (i = 0; lpstProcessSetting->lpstLibrary[i].lpszLibrary; i++)
            {
                LPWAFFLE_FUNCTION_ARRAY lpstFunction = lpstProcessSetting->lpstLibrary[i].lpstFunction;    //kernel32中的api可能是ntdll的存根
                if (((SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress >= (SIZE_T) lpstProcessSetting->lpstLibrary[i].hSource) && ((SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress <= (SIZE_T) lpstProcessSetting->lpstLibrary[i].hSourceEnd))
                {
                    int j;
                    for (j = 0; lpstFunction[j].lpszFunction; j++)
                    {
                        if (lpstFunction[j].lpSource == ExceptionInfo->ExceptionRecord->ExceptionAddress)
                        {
                            if (lpstFunction[j].lpDetour)
                            {
                                ExceptionInfo->ContextRecord->WAFFLE_PORT_PROGRAM_POINTER = (SIZE_T) lpstFunction[j].lpDetour;
                                return EXCEPTION_CONTINUE_EXECUTION;
                            }
                            break;
                        }
                    }
                    ExceptionInfo->ContextRecord->WAFFLE_PORT_PROGRAM_POINTER = (SIZE_T) ExceptionInfo->ExceptionRecord->ExceptionAddress - (SIZE_T) lpstProcessSetting->lpstLibrary[i].hSource + (SIZE_T) lpstProcessSetting->lpstLibrary[i].hBackup;
                    return EXCEPTION_CONTINUE_EXECUTION;
                }
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
            MessageBox(0, szExceptionRecord, 0, 0);
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}