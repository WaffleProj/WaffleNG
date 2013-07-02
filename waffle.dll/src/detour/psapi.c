#define  UNICODE
#define  _UNICODE
#include "..\..\waffle.h"
#include <psapi.h>

LIBRARY_API DWORD WINAPI DetourGetModuleFileNameExA(
  _In_      HANDLE hProcess,
  _In_opt_  HMODULE hModule,
  _Out_     LPSTR lpFilename,
  _In_      DWORD nSize
){
	LPVOID lpuszFilename = 0;
	if  (lpFilename)
		lpuszFilename = HeapAlloc(hHeap,HEAP_ZERO_MEMORY,4*nSize);

	GetModuleFileNameEx(hProcess,hModule,lpuszFilename,nSize);

	DWORD LastError = GetLastError();
	WideCharToMultiByte(stNewEnvir.ACP,0,lpuszFilename,-1,lpFilename,nSize,NULL,NULL);
	HeapFree(hHeap,0,lpuszFilename);
	SetLastError(LastError);
	return lstrlenA(lpFilename);
}