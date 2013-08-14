#ifndef __SUBCLASS_H_
#define __SUBCLASS_H_

typedef struct
{
    WNDPROC lpComboBoxA;
    WNDPROC lpComboBoxW;
} SUBCLASS_PROC, *LPSUBCLASS_PROC;

#ifdef __cplusplus
extern "C" {
#endif

    VOID WINAPI MojibakeSubclass(VOID);

#ifdef __cplusplus
};
#endif

#endif /* __SUBCLASS_H_ */