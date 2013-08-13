#ifndef __SDK_WAFFLE_COMMON_REDEF_H_
#define __SDK_WAFFLE_COMMON_REDEF_H_

#ifdef  UNICODE

#define WaffleCopyLibrary WaffleCopyLibraryW
#define WaffleAddLibrary WaffleAddLibraryW
#define WaffleCreateLibraryArray WaffleCreateLibraryArrayW

#define WaffleAddFunction WaffleAddFunctionW
#define WaffleCreateFunctionArray WaffleCreateFunctionArrayW
#define WaffleGetBackupAddress WaffleGetBackupAddressW

#define WaffleLoadComponent WaffleLoadComponentW
#define WaffleAddComponent WaffleAddComponentW

#define WaffleArgv WaffleArgvW
#define WaffleArgp WaffleArgpW

#define WaffleGetModuleDirectory WaffleGetModuleDirectoryW

#define WaffleGetFileHash WaffleGetFileHashW

#define WaffleCreateProcess WaffleCreateProcessW
#define WaffleInjectDll WaffleInjectDllW
#define WaffleExecute WaffleExecuteW

#define WaffleWriteLogFile WaffleWriteLogFileW

#define WaffleCheckOptionEncoding WaffleCheckOptionEncodingW
#define WaffleGetOptionString WaffleGetOptionStringW
#define WaffleSetOptionString WaffleSetOptionStringW
#define WaffleGetOptionInt WaffleGetOptionIntW
#define WaffleSetOptionInt WaffleSetOptionIntW
#define WaffleGetOptionStringNumber WaffleGetOptionStringNumberW
#define WaffleGetOptionSectionNames WaffleGetOptionSectionNamesW
#define WaffleGetOptionSectionKeys WaffleGetOptionSectionKeysW

#define WaffleGetMachineType WaffleGetMachineTypeW

#define Wafflelstrcmpi WafflelstrcmpiW
#define Wafflelstrcmp WafflelstrcmpW
#define WaffleStrToInt WaffleStrToIntW
#define WaffleGetProcAddress WaffleGetProcAddressW

#define WaffleOpenProcessSetting WaffleOpenProcessSettingW
#define WaffleCreateProcessSetting WaffleCreateProcessSettingW

#else

#define Wafflelstrcmpi WafflelstrcmpiA
#define Wafflelstrcmp WafflelstrcmpA
#define WaffleStrToInt WaffleStrToIntA
#define WaffleGetProcAddress WaffleGetProcAddressA

#endif

#endif /* __SDK_WAFFLE_COMMON_REDEF_H_ */