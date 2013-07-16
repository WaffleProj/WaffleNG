#define  UNICODE
#define  _UNICODE
#include "..\..\mojibake.h"

BOOL WINAPI DetourCreateDirectoryA(
  _In_      LPCSTR lpPathName,
  _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes
){
	LPVOID lpuszPathName = AnsiToUnicode(lpPathName);
	BOOL Result = CreateDirectory(lpuszPathName,lpSecurityAttributes);

	KeepLastErrorAndFree(lpuszPathName);
	return Result;
}

HANDLE WINAPI DetourCreateFileA(
  _In_      LPCSTR lpFileName,
  _In_      DWORD dwDesiredAccess,
  _In_      DWORD dwShareMode,
  _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  _In_      DWORD dwCreationDisposition,
  _In_      DWORD dwFlagsAndAttributes,
  _In_opt_  HANDLE hTemplateFile
){
	LPVOID lpuszFileName = AnsiToUnicode(lpFileName);
	//MessageBox(0,lpuszFileName,TEXT("DetourCreateFileA"),0);
	HANDLE Result = CreateFile(lpuszFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);

	KeepLastErrorAndFree(lpuszFileName);
	return Result;
}

BOOL WINAPI DetourDeleteFileA(
  _In_  LPCSTR lpFileName
){
	LPVOID lpuszFileName = AnsiToUnicode(lpFileName);
	BOOL Result = DeleteFile(lpuszFileName);

	KeepLastErrorAndFree(lpuszFileName);
	return Result;
}

HANDLE WINAPI DetourFindFirstFileA(
  _In_   LPCSTR lpFileName,
  _Out_  LPWIN32_FIND_DATAA lpFindFileData
){
	LPVOID lpuszFileName = AnsiToUnicode(lpFileName);
	WIN32_FIND_DATA FindFileData;
	HANDLE Result = FindFirstFile(lpuszFileName,&FindFileData);

	DWORD LastError = GetLastError();

	lpFindFileData->dwFileAttributes = FindFileData.dwFileAttributes;
	lpFindFileData->ftCreationTime = FindFileData.ftCreationTime;
	lpFindFileData->ftLastAccessTime = FindFileData.ftLastAccessTime;
	lpFindFileData->ftLastWriteTime = FindFileData.ftLastWriteTime;
	lpFindFileData->nFileSizeHigh = FindFileData.nFileSizeHigh;
	lpFindFileData->nFileSizeLow = FindFileData.nFileSizeLow;
	lpFindFileData->dwReserved0 = FindFileData.dwReserved0;
	lpFindFileData->dwReserved1 = FindFileData.dwReserved1;
	WideCharToMultiByte(stNewEnvir.ACP,0,FindFileData.cFileName,-1,lpFindFileData->cFileName,sizeof(lpFindFileData->cFileName),NULL,NULL);
	WideCharToMultiByte(stNewEnvir.ACP,0,FindFileData.cAlternateFileName,-1,lpFindFileData->cAlternateFileName,sizeof(lpFindFileData->cAlternateFileName),NULL,NULL);

	HeapFree(hHeap,0,lpuszFileName);
	SetLastError(LastError);
	return Result;
}

BOOL WINAPI DetourFindNextFileA(
  _In_   HANDLE hFindFile,
  _Out_  LPWIN32_FIND_DATAA lpFindFileData
){
	WIN32_FIND_DATA FindFileData;
	BOOL Result = FindNextFile(hFindFile,&FindFileData);

	DWORD LastError = GetLastError();

	lpFindFileData->dwFileAttributes = FindFileData.dwFileAttributes;
	lpFindFileData->ftCreationTime = FindFileData.ftCreationTime;
	lpFindFileData->ftLastAccessTime = FindFileData.ftLastAccessTime;
	lpFindFileData->ftLastWriteTime = FindFileData.ftLastWriteTime;
	lpFindFileData->nFileSizeHigh = FindFileData.nFileSizeHigh;
	lpFindFileData->nFileSizeLow = FindFileData.nFileSizeLow;
	lpFindFileData->dwReserved0 = FindFileData.dwReserved0;
	lpFindFileData->dwReserved1 = FindFileData.dwReserved1;
	WideCharToMultiByte(stNewEnvir.ACP,0,FindFileData.cFileName,-1,lpFindFileData->cFileName,sizeof(lpFindFileData->cFileName),NULL,NULL);
	WideCharToMultiByte(stNewEnvir.ACP,0,FindFileData.cAlternateFileName,-1,lpFindFileData->cAlternateFileName,sizeof(lpFindFileData->cAlternateFileName),NULL,NULL);

	SetLastError(LastError);
	return Result;
}

UINT WINAPI DetourGetACP(void)
{
	return stNewEnvir.ACP;
}

LPSTR WINAPI DetourGetCommandLineA(void)   //由于这个函数直接返回指针无需释放,所以只能这么做
{
	return lpszCommandLineA;
}

BOOL WINAPI DetourGetCPInfo(
  _In_   UINT CodePage,
  _Out_  LPCPINFO lpCPInfo
){
	if  (!CodePage)
		CodePage = stNewEnvir.ACP;

	return ((LPGETCPINFO)stKernel32Table[GETCPINFO].lpNewFunction)(CodePage,lpCPInfo);
}

DWORD WINAPI DetourGetFileAttributesA(
  _In_  LPCSTR lpFileName
){
	LPVOID lpuszFileName = AnsiToUnicode(lpFileName);
	DWORD Result = GetFileAttributes(lpuszFileName);

	KeepLastErrorAndFree(lpuszFileName);
	return Result;
}

DWORD WINAPI DetourGetModuleFileNameA(
  _In_opt_  HMODULE hModule,
  _Out_     LPSTR lpFilename,
  _In_      DWORD nSize
){
	LPVOID lpuszFilename = 0;
	if  (lpFilename)
		lpuszFilename = HeapAlloc(hHeap,HEAP_ZERO_MEMORY,4*nSize);

	GetModuleFileName(hModule,lpuszFilename,nSize);

	DWORD LastError = GetLastError();
	WideCharToMultiByte(stNewEnvir.ACP,0,lpuszFilename,-1,lpFilename,nSize,NULL,NULL);
	HeapFree(hHeap,0,lpuszFilename);
	SetLastError(LastError);
	return lstrlenA(lpFilename);
}

HMODULE WINAPI DetourGetModuleHandleA(
  _In_opt_  LPCSTR lpModuleName
){
	LPVOID lpuszModuleName = AnsiToUnicode(lpModuleName);
	HMODULE Result = GetModuleHandle(lpuszModuleName);

	KeepLastErrorAndFree(lpuszModuleName);
	return Result;
}

HMODULE WINAPI DetourLoadLibraryA(
  _In_  LPCSTR lpFileName
){
	LPVOID lpuszFileName = AnsiToUnicode(lpFileName);
	HMODULE Result = LoadLibrary(lpuszFileName);

	KeepLastErrorAndFree(lpuszFileName);
	return Result;
}

HMODULE WINAPI DetourLoadLibraryExA(
  _In_        LPCSTR lpFileName,
  _Reserved_  HANDLE hFile,
  _In_        DWORD dwFlags
){
	LPVOID lpuszFileName = AnsiToUnicode(lpFileName);
	HMODULE Result = LoadLibraryEx(lpuszFileName,hFile,dwFlags);

	KeepLastErrorAndFree(lpuszFileName);
	return Result;
}

int WINAPI DetourMultiByteToWideChar(
  _In_       UINT CodePage,
  _In_       DWORD dwFlags,
  _In_       LPCSTR lpMultiByteStr,
  _In_       int cbMultiByte,
  _Out_opt_  LPWSTR lpWideCharStr,
  _In_       int cchWideChar
){
	if  (!CodePage)
		CodePage = stNewEnvir.ACP;

	return ((LPMULTIBYTETOWIDECHAR)stKernel32Table[MULTIBYTETOWIDECHAR].lpNewFunction)(CodePage,dwFlags,lpMultiByteStr,cbMultiByte,lpWideCharStr,cchWideChar);
}

BOOL WINAPI DetourSetCurrentDirectoryA(
  _In_  LPCSTR lpPathName
){
	LPVOID lpuszPathName = AnsiToUnicode(lpPathName);
	BOOL Result = SetCurrentDirectory(lpuszPathName);

	KeepLastErrorAndFree(lpuszPathName);
	return Result;
}

BOOL WINAPI DetourSetFileAttributesA(
  _In_  LPCSTR lpFileName,
  _In_  DWORD dwFileAttributes
){
	LPVOID lpuszFileName = AnsiToUnicode(lpFileName);
	BOOL Result = SetFileAttributes(lpuszFileName,dwFileAttributes);

	KeepLastErrorAndFree(lpuszFileName);
	return Result;
}

int WINAPI DetourWideCharToMultiByte(
  _In_       UINT CodePage,
  _In_       DWORD dwFlags,
  _In_       LPCWSTR lpWideCharStr,
  _In_       int cchWideChar,
  _Out_opt_  LPSTR lpMultiByteStr,
  _In_       int cbMultiByte,
  _In_opt_   LPCSTR lpDefaultChar,
  _Out_opt_  LPBOOL lpUsedDefaultChar
){
	if  (!CodePage)
		CodePage = stNewEnvir.ACP;

	return ((LPWIDECHARTOMULTIBYTE)stKernel32Table[WIDECHARTOMULTIBYTE].lpNewFunction)(CodePage,dwFlags,lpWideCharStr,cchWideChar,lpMultiByteStr,cbMultiByte,lpDefaultChar,lpUsedDefaultChar);
}