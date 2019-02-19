#ifndef __WAFFLE_HOOK_DLL_H_
#define __WAFFLE_HOOK_DLL_H_

#define _BUILD_WAFFLE_HOOK_DLL_
#define _BUILD_WAFFLE_COMMON_DLL_ // Because we have WaffleInline/ExceptionHandler, just a workaround
#include <waffle.h>

extern LPWAFFLE_SMART_ARRAY lpstLibrary;
extern LPWAFFLE_SMART_ARRAY lpstSkipLibrary;

#endif /* __WAFFLE_HOOK_DLL_H_ */