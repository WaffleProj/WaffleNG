#ifndef __SDK_WAFFLE_PORT_ARMNT_H_
#define __SDK_WAFFLE_PORT_ARMNT_H_


#define WAFFLE_PORT_EXCEPTION_INSTRUCTION                   0xBE00      //bkpt 00h   Thumb-16
                                                            //0xE1200070  bkpt 0000h ARM
#define WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA              WORD
#define WAFFLE_PORT_EXCEPTION_CODE                          EXCEPTION_BREAKPOINT
#define WAFFLE_PORT_WRITE_EXCEPTION_INSTRUCTION(lpAddress)  *(WAFFLE_PORT_EXCEPTION_INSTRUCTION_DATA *)(lpAddress) = WAFFLE_PORT_EXCEPTION_INSTRUCTION

#define	WAFFLE_PORT_STACK_POINTER                           Sp
#define	WAFFLE_PORT_FRAME_POINTER                           R11
#define	WAFFLE_PORT_PROGRAM_POINTER                         Pc
#define	WAFFLE_PORT_FASTCALL_ARGUMENT                       R0

#define WAFFLE_PORT_MACHINE             WAFFLE_PORT_MACHINE_ARMNT
#define WAFFLE_PORT_MACHINE_STRING      WAFFLE_PORT_MACHINE_STRING_ARMNT

#endif /* __SDK_WAFFLE_PORT_ARMNT_H_ */