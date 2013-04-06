#ifndef __MOJIBAKE_LOADER_H_
#define __MOJIBAKE_LOADER_H_
#include "..\mojibake.h"

extern HANDLE hStdOut;

#ifdef __cplusplus
extern "C" {
#endif

int WINAPI argc();
LPCTSTR WINAPI argv(int intPosition, LPTSTR lpString, int intSize);
LPCTSTR WINAPI argp(int intPosition);
WORD WINAPI GetPEMagic(LPVOID lpFile);
PROCESS_INFORMATION WINAPI InjectDll(LPCTSTR lpszTarget, LPTSTR lpszArgument, LPCTSTR lpszDirectory, LPCTSTR lpszDllFull);

#ifdef __cplusplus
};
#endif

#endif /* __MOJIBAKE_LOADER_H_ */