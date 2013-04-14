#ifndef __INIT_H_
#define __INIT_H_

typedef struct {
    HANDLE   hThread;
    PCONTEXT lpstContext;
} THREAD_CONTEXT,*LPTHREAD_CONTEXT;

#ifdef __cplusplus
extern "C" {
#endif

VOID WINAPI InitLibrary(
  _In_  DWORD  dwThreadId
);

#ifdef __cplusplus
};
#endif

#endif /* __INIT_H_ */