#ifndef __SUBCLASS_H_
#define __SUBCLASS_H_

typedef struct
{
    WNDPROC lpComboBoxA;
    WNDPROC lpComboBoxW;
} SUBCLASS_PROC, *LPSUBCLASS_PROC;

VOID WINAPI MojibakeSubclass(void);

#endif /* __SUBCLASS_H_ */