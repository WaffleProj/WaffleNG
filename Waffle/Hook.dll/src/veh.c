#include "..\main.h"

WAFFLE_HOOK_DLL_FUNCTION BOOL WINAPI WaffleExceptionDetour(
    _In_    LPBYTE  lpFunction
)
{
    DWORD flOldProtect;
    lpFunction = (LPBYTE) WAFFLE_PORT_PROGRAM_COUNTER_TO_PHYSICAL_ADDRESS(lpFunction);

    if (!VirtualProtect(lpFunction, sizeof(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA), PAGE_EXECUTE_READWRITE, &flOldProtect))
    {
        return FALSE;
    }

    WAFFLE_PORT_WRITE_EXCEPTION_INSTRUCTION(lpFunction);

    FlushInstructionCache(GetCurrentProcess(), lpFunction, sizeof(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA));
    VirtualProtect(lpFunction, sizeof(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA), flOldProtect, &flOldProtect);
    return TRUE;
}

WAFFLE_HOOK_DLL_FUNCTION LONG CALLBACK WaffleExceptionHandler(
    _In_    PEXCEPTION_POINTERS ExceptionInfo
)
{
    switch (ExceptionInfo->ExceptionRecord->ExceptionCode)
    {
#if (WAFFLE_PORT_EXCEPTION_INSTRUCTION == WAFFLE_PORT_DEBUG_INSTRUCTION)
    case EXCEPTION_BREAKPOINT:
#else
    case WAFFLE_PORT_OPTIONAL_EXCEPTION_CODE:
#endif // WAFFLE_PORT_EXCEPTION_INSTRUCTION
        //if (*(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA *) (ExceptionInfo->ExceptionRecord->ExceptionAddress) == (WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA) WAFFLE_PORT_EXCEPTION_INSTRUCTION)
    {
        SIZE_T lpCaller = (SIZE_T) WAFFLE_PORT_EXCEPTION_GET_CALLER(ExceptionInfo);
        SIZE_T lpDetour = WaffleHookDBLookup((SIZE_T) WAFFLE_PORT_EXCEPTION_ADDRESS_TO_PHYSICAL_ADDRESS(ExceptionInfo), (SIZE_T) lpCaller);
        if (lpDetour)
        {
            ExceptionInfo->ContextRecord->WAFFLE_PORT_PROGRAM_POINTER = lpDetour;
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        break;
    }
#ifdef _DEBUG
    case EXCEPTION_ACCESS_VIOLATION:
    case 0x0EEDFADE:
    case 0xE06D7363:
        break;
    default:
    {
        TCHAR szExceptionRecord[2048];

        wsprintf(szExceptionRecord, TEXT("ExceptionRecord->ExceptionCode = %08x\nExceptionRecord->ExceptionFlags = %08x\nExceptionRecord->ExceptionRecord = %016I64X\nExceptionRecord->ExceptionAddress = %016I64X\nExceptionRecord->NumberParameters = %08x"), ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ExceptionRecord->ExceptionFlags, (UINT64) (SIZE_T) (ExceptionInfo->ExceptionRecord->ExceptionRecord), (UINT64) (SIZE_T) (ExceptionInfo->ExceptionRecord->ExceptionAddress), ExceptionInfo->ExceptionRecord->NumberParameters);

        TCHAR szContextRecord[256];
        wsprintf(szContextRecord, TEXT("\nExceptionInfo->ContextRecord->WAFFLE_PORT_STACK_POINTER = %016I64X"), (UINT64) (ExceptionInfo->ContextRecord->WAFFLE_PORT_STACK_POINTER));
        lstrcat(szExceptionRecord, szContextRecord);

        DWORD i;
        for (i = 0; i < ExceptionInfo->ExceptionRecord->NumberParameters; i++)
        {
            TCHAR szExceptionInfo[256];
            wsprintf(szExceptionInfo, TEXT("\nExceptionRecord->ExceptionInformation[%u] = %016I64X"), i, (UINT64) (ExceptionInfo->ExceptionRecord->ExceptionInformation[i]));
            lstrcat(szExceptionRecord, szExceptionInfo);
        }
        MessageBox(0, szExceptionRecord, 0, 0);
        break;
    }
#endif // _DEBUG
    }
    return EXCEPTION_CONTINUE_SEARCH;
}