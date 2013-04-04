#define UNICODE
#include <windows.h>
#include <psapi.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "psapi.lib")

typedef int (WINAPI *LPMESSAGEBOXA)(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCSTR lpText,
  _In_opt_  LPCSTR lpCaption,
  _In_      UINT uType
);

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
){
    HANDLE hProcess;
    HMODULE hModule;
    LPVOID lpNewDll;
    SIZE_T offsetMessageBoxA;

    MODULEINFO               stModuleInfo;
    MEMORY_BASIC_INFORMATION stMemInfo;
    LPVOID                   addrPointer,addrEnd;
    LPMESSAGEBOXA            lpMessageBoxA;

    //Get dll base address and size
    hProcess = GetCurrentProcess();
    hModule = GetModuleHandle(TEXT("User32.dll"));
    if  (!hModule)
    {
        hModule = LoadLibrary(TEXT("User32.dll"));
    }
    GetModuleInformation(hProcess,hModule,&stModuleInfo,sizeof(stModuleInfo));

    //Reserve memory address
    lpNewDll = VirtualAlloc(NULL,stModuleInfo.SizeOfImage,MEM_RESERVE,PAGE_NOACCESS);

    addrPointer = stModuleInfo.lpBaseOfDll;
    addrEnd = (LPVOID)((SIZE_T)stModuleInfo.lpBaseOfDll + stModuleInfo.SizeOfImage);
    while (addrPointer < addrEnd)
    {
        VirtualQuery(addrPointer,&stMemInfo,sizeof(stMemInfo));
        if  (stMemInfo.State == MEM_COMMIT)
        {
            //Get offset of the memory
            LPVOID addrNew = (LPVOID)((SIZE_T)stMemInfo.BaseAddress - (SIZE_T)stMemInfo.AllocationBase + (SIZE_T)lpNewDll);

            VirtualAlloc(addrNew,stMemInfo.RegionSize,MEM_COMMIT,PAGE_READWRITE);
            RtlMoveMemory(addrNew,stMemInfo.BaseAddress,stMemInfo.RegionSize);
            VirtualProtect(addrNew,stMemInfo.RegionSize,stMemInfo.Protect,&stMemInfo.AllocationProtect);
        }
        addrPointer = (PBYTE)stMemInfo.BaseAddress + stMemInfo.RegionSize;
    }
    
    //Get offset of MessageBoxA
    offsetMessageBoxA = (SIZE_T)GetProcAddress(hModule,"MessageBoxA") - (SIZE_T)stModuleInfo.lpBaseOfDll;

    //Call new MessageBoxA
    lpMessageBoxA = (LPMESSAGEBOXA)((SIZE_T)lpNewDll + offsetMessageBoxA);
    lpMessageBoxA(0,"Test Message","Dll Copy Example",0);

    return 0;
}