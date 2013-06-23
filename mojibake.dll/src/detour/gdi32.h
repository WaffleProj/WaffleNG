#ifndef __GDI32_H_
#define __GDI32_H_

#include <windows.h>
#include "..\..\mojibake.h"

typedef HFONT (WINAPI *LPCREATEFONTA)(
  _In_  int nHeight,
  _In_  int nWidth,
  _In_  int nEscapement,
  _In_  int nOrientation,
  _In_  int fnWeight,
  _In_  DWORD fdwItalic,
  _In_  DWORD fdwUnderline,
  _In_  DWORD fdwStrikeOut,
  _In_  DWORD fdwCharSet,
  _In_  DWORD fdwOutputPrecision,
  _In_  DWORD fdwClipPrecision,
  _In_  DWORD fdwQuality,
  _In_  DWORD fdwPitchAndFamily,
  _In_  LPCSTR lpszFace
);

typedef BOOL (WINAPI *LPTEXTOUTA)(
  _In_  HDC hdc,
  _In_  int nXStart,
  _In_  int nYStart,
  _In_  LPCSTR lpString,
  _In_  int cchString
);

#endif /* __GDI32_H_ */