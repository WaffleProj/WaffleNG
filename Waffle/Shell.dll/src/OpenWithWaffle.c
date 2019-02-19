#include "../shell.h"

#define DEBUG_PRINTF(fmt, ...) \
        { \
    TCHAR szMessage[256]; \
    wsprintf(szMessage, TEXT("[%s@%s] ") ## TEXT(fmt), TEXT(__FILE__), TEXT(__TIMESTAMP__), __VA_ARGS__); \
    MessageBox(0, szMessage, TEXT("Waffle"), 0); \
        }

int WINAPI OpenWithWaffle(
	_In_    LPCWSTR lpszComponent,
	_In_    LPCWSTR lpszTarget,
	_In_    LPCWSTR lpszArgument
	)
{
	// The path of Waffle
	TCHAR szPath[MAX_PATH];
	szPath[0] = TEXT('\0');
	WaffleGetModuleDirectory(hInstance, szPath, lengthof(szPath));

    // The folder that contains the target
    TCHAR szDirectory[MAX_PATH];
    szDirectory[0] = TEXT('\0');

    Wafflelstrcpy(szDirectory, lpszTarget);
    int len = lstrlen(lpszTarget);
    for (; szDirectory[len] != TEXT('\\'); len--);
    szDirectory[len] = TEXT('\0');

	// We should always have a target
	if (Wafflelstrcmpi(lpszTarget, TEXT("")))
	{
		int i, j;

		// Check for file association
		for (i = Wafflelstrlen(lpszTarget); lpszTarget[i] != TEXT('.'); i--);

		TCHAR szExtName[32];
		szExtName[0] = TEXT('\0');
		Wafflelstrcpy(szExtName, &lpszTarget[i]);

		// Try new fancy AssocQueryString 
		DWORD cchOut = 0;
		AssocQueryString(ASSOCF_NONE, ASSOCSTR_COMMAND, szExtName, NULL, NULL, &cchOut);
		LPTSTR lpszShellOpen = (LPTSTR)WaffleAlloc(cchOut * sizeof(TCHAR));
		AssocQueryString(ASSOCF_NONE, ASSOCSTR_COMMAND, szExtName, NULL, lpszShellOpen, &cchOut);

		// Fill in value
		SIZE_T nSize = Wafflelstrlen(lpszShellOpen) + 1;
		LPTSTR lpszCmdLine = (LPTSTR)WaffleAlloc(nSize * sizeof(TCHAR));
		for (i = 0, j = 0; (lpszCmdLine[j] = lpszShellOpen[i]) != 0; i++, j++)
		{
			if (lpszShellOpen[i] == TEXT('%'))
			{
				switch (lpszShellOpen[++i])
				{
				case TEXT('0'):
				case TEXT('1'):
				case TEXT('L'):
				{
					nSize += Wafflelstrlen(lpszTarget);
					if (!(lpszCmdLine = (LPTSTR)WaffleReAlloc(lpszCmdLine, nSize * sizeof(TCHAR))))
					{
						//DEBUG_PRINTF("Out of memory.");
						ExitProcess(0);
					}
					Wafflelstrcpy(&lpszCmdLine[j], lpszTarget);
					j = Wafflelstrlen(lpszCmdLine) - 1;
					break;
				}
				case TEXT('*'):
				{
					nSize += Wafflelstrlen(lpszArgument);
					if (!(lpszCmdLine = (LPTSTR)WaffleReAlloc(lpszCmdLine, nSize * sizeof(TCHAR))))
					{
						//DEBUG_PRINTF("Out of memory.");
						ExitProcess(0);
					}
					Wafflelstrcpy(&lpszCmdLine[j], lpszArgument);
					j = Wafflelstrlen(lpszCmdLine) - 1;
					break;
				}
				default:
				{
					//DEBUG_PRINTF("Unknown argument:%s.", lpszShellOpen);
					ExitProcess(0);
					break;
				}
				}
			}
		}

		WaffleFree(lpszShellOpen);

		// Overwrite
		WaffleArgv(lpszCmdLine, 1, lpszTarget, lstrlenW(lpszTarget));

		lpszArgument = (LPTSTR)GlobalAlloc(GPTR, (lstrlen(WaffleArgp(lpszCmdLine, 2)) + 1) * sizeof(TCHAR));
		if (lpszArgument)
		{
			Wafflelstrcpy(lpszArgument, WaffleArgp(lpszCmdLine, 2));
		}
		else
		{
			//DEBUG_PRINTF("Out of memory.");
			ExitProcess(0);
		}

		// Check if we need to call another version of Waffle
		WORD MachineType = WaffleGetMachineType(lpszTarget);
		// Check UAC here also
		STARTUPINFO stStartUp;
		PROCESS_INFORMATION stProcessInfo;
		stStartUp.cb = sizeof(stStartUp);
		GetStartupInfo(&stStartUp);
		BOOL UACEnabled = FALSE;
		if (CreateProcess(lpszTarget, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &stStartUp, &stProcessInfo))
		{
			CloseHandle(stProcessInfo.hThread);
			CloseHandle(stProcessInfo.hProcess);
			TerminateProcess(stProcessInfo.hProcess, 0);
		}
		else
		{
            UACEnabled = GetLastError() == ERROR_ELEVATION_REQUIRED;
		}

        LPWAFFLE_PROCESS_SETTING lpstProcessSetting = WaffleOpenProcessSetting();

		if ((UACEnabled) || (MachineType != WAFFLE_PORT_MACHINE))
		{
			TCHAR szLoader[MAX_PATH];
			Wafflelstrcpy(szLoader, szPath);
			int i = lstrlen(szLoader);
			for (i--; szLoader[i] != TEXT('\\'); i--); szLoader[i] = TEXT('\0');

			switch (MachineType)
			{
			case WAFFLE_PORT_MACHINE_I386:
			{
				lstrcat(szLoader, TEXT("\\I386\\Waffle.exe"));
				break;
			}
			case WAFFLE_PORT_MACHINE_AMD64:
			{
				lstrcat(szLoader, TEXT("\\AMD64\\Waffle.exe"));
				break;
			}
			case WAFFLE_PORT_MACHINE_ARMNT:
			{
				lstrcat(szLoader, TEXT("\\ARMNT\\Waffle.exe"));
				break;
			}
			case WAFFLE_PORT_MACHINE_IA64:
			{
				lstrcat(szLoader, TEXT("\\IA64\\Waffle.exe"));
				break;
			}
			case WAFFLE_PORT_MACHINE_ERROR:
			{
				//DEBUG_PRINTF("Unable to open the target.");
				ExitProcess(0);
				break;
			}
			default:
			{
				//DEBUG_PRINTF("Unsupported file.");
				ExitProcess(0);
				break;
			}
			}

			LPTSTR lpszCommandLine;
			lpszCommandLine = (LPTSTR)GlobalAlloc(GPTR, (lstrlen(szLoader) + lstrlen(lpszComponent) + lstrlenW(lpszTarget) + lstrlen(lpszArgument) + 3 + 5 + 1 + 3 + 1) * sizeof(TCHAR));
			if (lpszCommandLine)
			{
				wsprintf(lpszCommandLine, TEXT("\"%s\" open %s \"%s\" %s"), szLoader, lpszComponent, lpszTarget, lpszArgument);
			}
			else
			{
				//DEBUG_PRINTF("Out of memory.");
				ExitProcess(0);
			}
			LPCTSTR lpVerb = UACEnabled ? TEXT("runas") : NULL;
			ShellExecute(NULL, lpVerb, szLoader, lpszCommandLine, szDirectory, SW_SHOW);
			GlobalFree(lpszArgument);
		}

		// Once we have the hash, we can save the name
		WaffleSetOptionString(NULL, NULL, TEXT("ProgramName"), lpszTarget, FALSE);
	}
	else
	{
		//DEBUG_PRINTF("Please enter a target.");
		ExitProcess(0);
	}

	// If szComponent == "default", read from the config file
	if (!Wafflelstrcmpi(lpszComponent, TEXT("default")))
	{
		WaffleGetOptionString(NULL, NULL, TEXT("DefaultPlugin"), lpszComponent, lstrlenW(lpszComponent), NULL);
		if (!Wafflelstrcmpi(lpszComponent, TEXT("")))
		{
			DEBUG_PRINTF("DefaultPlugin has no value.");
			ExitProcess(0);
		}

		// Write back to hash.ini
		WaffleSetOptionString(NULL, NULL, TEXT("DefaultPlugin"), lpszComponent, FALSE);
	}
	// Create shared process setting
	LPWAFFLE_PROCESS_SETTING lpstProcessSetting = WaffleShareProcessSetting();

	Wafflelstrcpy(lpstProcessSetting->szComponent, lpszComponent);
	Wafflelstrcpy(lpstProcessSetting->szComponentDirectory, szPath);
	len = lstrlen(lpstProcessSetting->szComponentDirectory);
	for (len--; lpstProcessSetting->szComponentDirectory[len] != TEXT('\\'); len--); lpstProcessSetting->szComponentDirectory[len] = TEXT('\0');
	for (len--; lpstProcessSetting->szComponentDirectory[len] != TEXT('\\'); len--); lpstProcessSetting->szComponentDirectory[len] = TEXT('\0');

	LPTSTR lpszCommandLine = (LPTSTR)GlobalAlloc(GPTR, (lstrlen(lpszTarget) + lstrlen(lpszArgument) + 3 + 1) * sizeof(TCHAR));
	if (lpszCommandLine)
	{
		wsprintf(lpszCommandLine, TEXT("\"%s\" %s"), lpszTarget, lpszArgument);
		WaffleExecute(lpstProcessSetting, lpszTarget, lpszCommandLine, szDirectory);
		GlobalFree(lpszCommandLine);
	}
	else
	{
		//DEBUG_PRINTF("Out of memory.");
		ExitProcess(0);
	}
}