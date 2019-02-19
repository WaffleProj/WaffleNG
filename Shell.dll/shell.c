#include "shell.h"

HINSTANCE hInstance;

VOID PrintGuid(
	_In_        REFIID riid,
	_In_opt_    LPCTSTR lpszTitle
	)
{
	TCHAR szBuf[128];
	wsprintf(szBuf, TEXT("Guid = {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		riid->Data1, riid->Data2, riid->Data3,
		riid->Data4[0], riid->Data4[1], riid->Data4[2], riid->Data4[3],
		riid->Data4[4], riid->Data4[5], riid->Data4[6], riid->Data4[7]);
	MessageBox(0, szBuf, lpszTitle, 0);
}

_Check_return_
STDAPI DllGetClassObject(
	_In_ REFCLSID rclsid,
	_In_ REFIID riid,
	_Outptr_ LPVOID FAR* ppv
	)
{
	if (IsEqualCLSID(rclsid, &CLSID_IWaffleContextMenu))
	{
		return IWaffleClassFactoryObject.lpVtbl->QueryInterface((IWaffleClassFactory *)&IWaffleClassFactoryObject, riid, ppv);
	}
	else
	{
		*ppv = NULL;
		return CLASS_E_CLASSNOTAVAILABLE;
	}
}

STDAPI DllCanUnloadNow(void)
{

	if (!IWaffleClassFactoryObject.nLock && !IWaffleClassFactoryObject.refClassFactory && !IWaffleClassFactoryObject.refContextMenu)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDAPI DllRegisterServer(void)
{
	HKEY hKey = NULL;
	HKEY hSubKey = NULL;

	if (RegCreateKey(HKEY_CLASSES_ROOT, TEXT("*\\shellex\\ContextMenuHandlers\\Waffle"), &hKey) == ERROR_SUCCESS)
	{
		RegSetValue(hKey, NULL, REG_SZ, TEXT("{C3B16F86-68FD-4F32-BF53-975180752E04}"), 0);
		RegCloseKey(hKey);
	}

	if (RegCreateKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{C3B16F86-68FD-4F32-BF53-975180752E04}"), &hKey) == ERROR_SUCCESS)
	{
		RegSetValue(hKey, NULL, REG_SZ, TEXT("Waffle Shell Extension"), 0);
		if (RegCreateKey(hKey, TEXT("InprocServer32"), &hSubKey) == ERROR_SUCCESS)
		{
			RegSetValue(hSubKey, NULL, REG_SZ, TEXT("D:\\Project\\WaffleProject\\WaffleNightly\\Component\\Waffle\\AMD64\\Waffle.shell.1.0.dll"), 0);
			RegSetKeyValue(hSubKey, NULL, TEXT("ThreadingModel"), REG_SZ, TEXT("Both"), 8);
			RegCloseKey(hSubKey);
		}
		RegCloseKey(hKey);
	}

	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, TEXT("*\\shellex\\ContextMenuHandlers"), 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		RegDeleteKey(hKey, TEXT("Waffle"));
		RegCloseKey(hKey);
	}
	else
	{
		return E_UNEXPECTED;
	}

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, TEXT("CLSID\\{C3B16F86-68FD-4F32-BF53-975180752E04}"), 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		RegDeleteKey(hKey, TEXT("InprocServer32"));
		RegCloseKey(hKey);
	}
	else
	{
		return E_UNEXPECTED;
	}

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, TEXT("CLSID"), 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		RegDeleteKey(hKey, TEXT("{C3B16F86-68FD-4F32-BF53-975180752E04}"));
		RegCloseKey(hKey);
	}
	else
	{
		return E_UNEXPECTED;
	}

	return S_OK;
}

BOOL WINAPI DllMain(
	_In_    HINSTANCE hinstDLL,
	_In_    DWORD fdwReason,
	_In_    LPVOID lpvReserved
	)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		hInstance = hinstDLL;
		DisableThreadLibraryCalls(hinstDLL);
	}
	}

	return TRUE;
}
