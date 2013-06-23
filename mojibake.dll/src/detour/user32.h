#ifndef __USER32_H_
#define __USER32_H_

#include <windows.h>
#include "..\..\mojibake.h"

typedef LRESULT (WINAPI *LPCALLWINDOWPROCA)(
  _In_  WNDPROC lpPrevWndFunc,
  _In_  HWND hWnd,
  _In_  UINT Msg,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
);

typedef HWND (WINAPI *LPCREATEWINDOWEXA)(
  _In_      DWORD dwExStyle,
  _In_opt_  LPCSTR lpClassName,
  _In_opt_  LPCSTR lpWindowName,
  _In_      DWORD dwStyle,
  _In_      int x,
  _In_      int y,
  _In_      int nWidth,
  _In_      int nHeight,
  _In_opt_  HWND hWndParent,
  _In_opt_  HMENU hMenu,
  _In_opt_  HINSTANCE hInstance,
  _In_opt_  LPVOID lpParam
);

typedef int (WINAPI *LPMESSAGEBOXA)(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
);

typedef int (WINAPI *LPMESSAGEBOXEXA)(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType,
  _In_      WORD wLanguageId
);

typedef LRESULT (WINAPI *LPSENDMESSAGEA)(
  _In_  HWND hWnd,
  _In_  UINT Msg,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
);

typedef BOOL (WINAPI *LPSETWINDOWTEXTA)(
  _In_      HWND hWnd,
  _In_opt_  LPCSTR lpString
);

#endif /* __USER32_H_ */