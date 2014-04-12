#include <waffle.h>
#include "resource.h"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#define DEBUG_PRINTF(fmt, ...) \
    { \
    TCHAR szMessage[256]; \
    wsprintf(szMessage, TEXT("[%s@%s] ") ## TEXT(fmt), TEXT(__FILE__), TEXT(__TIMESTAMP__), __VA_ARGS__); \
    MessageBox(0, szMessage, TEXT("Waffle"), 0); \
    }

int CALLBACK PropSheetProc(
    _In_        HWND hwndDlg,
    _In_        UINT uMsg,
    _In_opt_    LPARAM lParam
    )
{
    switch (uMsg)
    {
        case PSCB_BUTTONPRESSED:
        {
            switch (lParam)
            {
                case PSBTN_OK:
                case PSBTN_CANCEL:
                case PSBTN_APPLYNOW:
                case PSBTN_FINISH:
                    break;
            }
            break;
        }
        case PSCB_INITIALIZED:
        case PSCB_PRECREATE:
            break;
    }

    return 0;
}

INT_PTR CALLBACK DialogProc(
    _In_    HWND hwndDlg,
    _In_    UINT uMsg,
    _In_    WPARAM wParam,
    _In_    LPARAM lParam
    )
{
    return FALSE;
}

int WINAPI Main(void)
{
#ifdef _DEBUG
    MessageBox(0, TEXT("Please attach the debugger."), TEXT("Waffle.exe"), 0);
    if (IsDebuggerPresent())
    {
        DebugBreak();
    }
#endif // _DEBUG

    // The path of Waffle
    TCHAR szPath[MAX_PATH];
    szPath[0] = TEXT('\0');
    // The command we are going to perform
    TCHAR szCommand[MAX_PATH];
    szCommand[0] = TEXT('\0');
    // The name of the plugin
    TCHAR szComponent[MAX_PATH];
    szComponent[0] = TEXT('\0');
    // The file we are going to deal with
    TCHAR szTarget[MAX_PATH];
    szTarget[0] = TEXT('\0');
    // The folder that contains the target
    TCHAR szDirectory[MAX_PATH];
    szDirectory[0] = TEXT('\0');

    WaffleGetModuleDirectory(NULL, szPath, lengthof(szPath));

    // The number of command line arguments
    int nArg = WaffleArgc();
    switch (nArg)
    {
        case 0:
        {
            // Won't happen, error
            DEBUG_PRINTF("Cannot get the command line.");
            ExitProcess(0);
            break;
        }
        case 1:
        {
            // Execute with no command line, assume using default plugin to open
            Wafflelstrcpy(szCommand, TEXT("open"));
            Wafflelstrcpy(szComponent, TEXT("default"));

            OPENFILENAME stOpenFile;
            RtlZeroMemory(&stOpenFile, sizeof(stOpenFile));

            stOpenFile.lStructSize = sizeof(stOpenFile);
            stOpenFile.lpstrFile = szTarget;
            stOpenFile.nMaxFile = lengthof(szTarget);
            stOpenFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // GetOpenFileName will create a bunch of worker threads, so return 0 won't close our process
            if (!GetOpenFileName(&stOpenFile) || !lstrlen(szTarget))
            {
                ExitProcess(0);
            }
            break;
        }
        default:
        {
            // 1.Path 2. Command
            WaffleArgv(2, szCommand, lengthof(szCommand));

            if (!Wafflelstrcmpi(szCommand, TEXT("open")))
            {
                // 3. Plugin 4. Target
                WaffleArgv(3, szComponent, lengthof(szComponent));
                WaffleArgv(4, szTarget, lengthof(szTarget));

                if (WaffleGetMachineType(szTarget) != WAFFLE_PORT_MACHINE)
                {
                    // We need to call another version of Waffle
                    Wafflelstrcpy(szCommand, TEXT("redirect"));
                }
            }
            else if (!Wafflelstrcmpi(szCommand, TEXT("option")))
            {
                // 3. Target
                Wafflelstrcpy(szComponent, TEXT(""));
                WaffleArgv(3, szTarget, lengthof(szTarget));
            }
            else if (!Wafflelstrcmpi(szCommand, TEXT("help")))
            {
                MessageBox(0, TEXT("You really need help?"), TEXT("Waffle"), 0);
                ExitProcess(0);
            }
            else
            {
                MessageBox(0, TEXT("Waffle help\n1. open\nWaffle.vbs open [plugin/default] [target]\n2. option\nWaffle.vbs option [target]\n3. help\nWaffle.vbs help"), TEXT("Waffle"), 0);
                ExitProcess(0);
            }
            break;
        }
    }

    // We should always have a target
    if (Wafflelstrcmpi(szTarget, TEXT("")))
    {
        LPWAFFLE_PROCESS_SETTING lpstProcessSetting = WaffleOpenProcessSetting();
        WaffleGetFileHash(szTarget, lpstProcessSetting->szProcessHash);

        lstrcpy(szDirectory, szTarget);
        int i = lstrlen(szTarget);
        for (; szDirectory[i] != TEXT('\\'); i--);
        szDirectory[i] = TEXT('\0');

        // Once we have the hash, we can save the name
        WaffleSetOptionString(NULL, NULL, TEXT("ProgramName"), szTarget, FALSE);
    }
    else
    {
        DEBUG_PRINTF("Please enter a target.");
        ExitProcess(0);
    }

    // If szComponent == "default", read from the config file
    if (!Wafflelstrcmpi(szComponent, TEXT("default")))
    {
        WaffleGetOptionString(NULL, NULL, TEXT("DefaultPlugin"), szComponent, lengthof(szComponent), NULL);
        if (!Wafflelstrcmpi(szComponent, TEXT("")))
        {
            DEBUG_PRINTF("DefaultPlugin has no value.");
            ExitProcess(0);
        }

        // Write back to hash.ini
        WaffleSetOptionString(NULL, NULL, TEXT("DefaultPlugin"), szComponent, FALSE);
    }

    if (!Wafflelstrcmpi(szCommand, TEXT("open")))
    {
        // Create shared process setting
        LPWAFFLE_PROCESS_SETTING lpstProcessSetting = WaffleShareProcessSetting();

        Wafflelstrcpy(lpstProcessSetting->szComponent, szComponent);
        Wafflelstrcpy(lpstProcessSetting->szComponentDirectory, szPath);
        int i = lstrlen(lpstProcessSetting->szComponentDirectory);
        for (i--; lpstProcessSetting->szComponentDirectory[i] != TEXT('\\'); i--); lpstProcessSetting->szComponentDirectory[i] = TEXT('\0');
        for (i--; lpstProcessSetting->szComponentDirectory[i] != TEXT('\\'); i--); lpstProcessSetting->szComponentDirectory[i] = TEXT('\0');

        LPTSTR lpszArgument = (LPTSTR) GlobalAlloc(GPTR, (lstrlen(szTarget) + lstrlen(WaffleArgp(5)) + 3 + 1) * sizeof(TCHAR));
        if (lpszArgument)
        {
            wsprintf(lpszArgument, TEXT("\"%s\" %s"), szTarget, WaffleArgp(5));
            WaffleExecute(lpstProcessSetting, szTarget, lpszArgument, szDirectory);
            GlobalFree(lpszArgument);
        }
        else
        {
            DEBUG_PRINTF("Out of memory.");
            ExitProcess(0);
        }
    }
    else if (!Wafflelstrcmpi(szCommand, TEXT("redirect")))
    {
        TCHAR szLoader[MAX_PATH];
        Wafflelstrcpy(szLoader, szPath);
        int i = lstrlen(szLoader);
        for (i--; szLoader[i] != TEXT('\\'); i--); szLoader[i] = TEXT('\0');

        WORD MachineType = WaffleGetMachineType(szTarget);
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
            case 0xFFFF:
            {
                DEBUG_PRINTF("Unable to open the target.");
                ExitProcess(0);
                break;
            }
            default:
            {
                DEBUG_PRINTF("Unsupported file.");
                ExitProcess(0);
                break;
            }
        }

        LPTSTR lpszArgument = (LPTSTR) GlobalAlloc(GPTR, (lstrlen(szLoader) + lstrlen(WaffleArgp(3)) + 3 + 5 + 1) * sizeof(TCHAR));
        if (lpszArgument)
        {
            wsprintf(lpszArgument, TEXT("\"%s\" open %s"), szLoader, WaffleArgp(3));
        }
        else
        {
            DEBUG_PRINTF("Out of memory.");
            ExitProcess(0);
        }

        STARTUPINFO stStartUp;
        PROCESS_INFORMATION stProcessInfo;

        stStartUp.cb = sizeof(stStartUp);
        GetStartupInfo(&stStartUp);

        CreateProcess(szLoader, lpszArgument, NULL, NULL, TRUE, 0, 0, szDirectory, &stStartUp, &stProcessInfo);
        CloseHandle(stProcessInfo.hThread);
        CloseHandle(stProcessInfo.hProcess);
        GlobalFree(lpszArgument);
    }
    else if (!Wafflelstrcmpi(szCommand, TEXT("option")))
    {
        PROPSHEETPAGE stPage[1];
        PROPSHEETHEADER stHeader;

        stPage[0].dwSize = sizeof(stPage);
        stPage[0].dwFlags = PSP_DEFAULT;
        stPage[0].hInstance = GetModuleHandle(NULL);
        stPage[0].pszTemplate = MAKEINTRESOURCE(IDD_PLUGIN);
        stPage[0].pfnDlgProc = DialogProc;
        stPage[0].lParam = 0;

        stHeader.dwSize = sizeof(stHeader);
        stHeader.dwFlags = PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW | PSH_NOCONTEXTHELP | PSH_USECALLBACK;
        stHeader.hwndParent = NULL;
        stHeader.pszCaption = TEXT("Waffle Option");
        stHeader.nPages = 1;
        stHeader.nStartPage = 0;
        stHeader.ppsp = (LPCPROPSHEETPAGE) &stPage[0];
        stHeader.pfnCallback = (PFNPROPSHEETCALLBACK) PropSheetProc;

        if (PropertySheet(&stHeader) == -1)
        {
            DEBUG_PRINTF("Unable to show option.");
            ExitProcess(0);
        }
    }

    ExitProcess(0);
}