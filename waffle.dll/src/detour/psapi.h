#ifndef __PSAPI_H_
#define __PSAPI_H_

#include "..\..\waffle.h"
#include <psapi.h>

typedef DWORD (WINAPI *LPGETMODULEFILENAMEEXA)(
  _In_      HANDLE hProcess,
  _In_opt_  HMODULE hModule,
  _Out_     LPSTR lpFilename,
  _In_      DWORD nSize
);

#endif /* __PSAPI_H_ */