#ifndef __SHELL32_H_
#define __SHELL32_H_

#include <windows.h>
#include <shellapi.h>
#include "..\..\waffle.h"

typedef int (WINAPI *LPSHELLABOUTW)(
  _In_opt_  HWND hWnd,
  _In_      LPCTSTR szApp,
  _In_opt_  LPCTSTR szOtherStuff,
  _In_opt_  HICON hIcon
);

#endif /* __SHELL32_H_ */