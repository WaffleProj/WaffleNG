#ifndef __WAFFLE_WAFFLE_EXE_H_
#define __WAFFLE_WAFFLE_EXE_H_
#include <waffle.h>

typedef BOOL (WINAPI *LPWOW64DISABLEWOW64FSREDIRECTION)(
  _Out_  PVOID *OldValue
);

typedef BOOL (WINAPI *LPWOW64REVERTWOW64FSREDIRECTION)(
  _In_  PVOID OldValue
);

#ifdef __cplusplus
extern "C" {
#endif

int WINAPI argc();
LPCTSTR WINAPI argv(int intPosition, LPTSTR lpString, int intSize);
LPCTSTR WINAPI argp(int intPosition);

WORD WINAPI GetMachineType(LPCTSTR lpszFile);

#ifdef __cplusplus
};
#endif

#endif /* __WAFFLE_WAFFLE_EXE_H_ */