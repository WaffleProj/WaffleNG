/*------------------------------------------------------------------------
Module:        PROPSHEETPAGE.C
Author:        Daniel Guerrero Miralles
Project:
State:
Creation Date: 19/11/98
Description:   Template for a Property Sheet page
dialog.
------------------------------------------------------------------------*/
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
// (c) Copyright 1998 Daniel Guerrero Miralles. All Rights Reserved.

#include "PropSheetPage.h"
#include <CommCtrl.h>
#include "resource.h"

#pragma comment(lib, "comctl32.lib")

// Enable Visual Style
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

/* --- Constants --- */
// TODO: Add your constants here


/* --- Macros --- */
#define PropSheetPage_SetResult(h,f) SetWindowLong(h,DWL_MSGRESULT,f)
// TODO: add your macros here


/* --- Globals --- */
// TODO: Add your global variables here


/* --- Private functions --- */
// Functions that will be called only by this module
/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnInitDialog ID:1
Purpose:       Do dialog initialization.
Input:         hwnd : handle of the page dialog window.
hwndFocus : handle of the first dialog item that can receive
the input focus.
lParam : pointer to the PROPSHEETPAGE structure used to
create the dialog.
Output:        FALSE if the function has set the input focus to an item,
TRUE if Windows has to do it.
Errors:        None.
------------------------------------------------------------------------*/
static BOOL PropSheetPage_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    // TODO : add code to init the dialog here
    return TRUE;
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnDestroy ID:1
Purpose:       Perform dialog de-initialization.
Input:         hwnd : handle of the page dialog window.
Output:        None.
Errors:        None.
------------------------------------------------------------------------*/
static void PropSheetPage_OnDestroy(HWND hwnd)
{
    // TODO : add code to de-init the dialog here
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnReset ID:1
Purpose:       This function is called for all initialized pages when the
user clicks the "Cancel" button, prior to call to
PropSheetPage_Destroy. Use it to do "on cancellation"
specific de-initialization.
Input:         hwnd : handle of the page dialog window.
hwPropSheet : handle of the parent Property Sheet window.
fOkClose : According to Microsoft, TRUE if the user hit the
"OK" or "Cancel" button, or FALSE if the "Close" button, but
in fact, it is always FALSE and this function is only called
when the user clicks the "Cancel" button.
Output:        None.
Errors:        None.
------------------------------------------------------------------------*/
static void PropSheetPage_OnReset(HWND hwnd, HWND hwPropSheet, BOOL fOkClose)
{
    // TODO : add code to be executed when the user cancels.
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnSetActive ID:1
Purpose:       This function is called when a page is about to gain
activation. Use it to initialize the page controls.
Input:         hwnd : handle of the page dialog window.
hwPropSheet : handle of the parent Property Sheet window.
Output:        return TRUE if tha page agrees to gain activation. To skip
this page return FALSE. To jump to another page instead of
activating this, return the page resource identifier.
Errors:        None.
------------------------------------------------------------------------*/
static BOOL PropSheetPage_OnSetActive(HWND hwnd, HWND hwPropSheet)
{
    // TODO: add code to initialize the page controls here
    return TRUE;
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnKillActive ID:1
Purpose:       This function is called when the page is about to lose
activation. It should validate the user input.
Input:         hwnd : handle of the page dialog window.
hwPropSheet : handle of the parent Property Sheet window.
Output:        return TRUE to avoid page deactivation, or FALSE to allow
it.
Errors:        if the function returns TRUE, i may display a message box
explaining the error to the user. ??
------------------------------------------------------------------------*/
static BOOL PropSheetPage_OnKillActive(HWND hwnd, HWND hwPropSheet)
{
    // TODO: validate the page controls.
    return FALSE;
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnQueryCancel ID:1
Purpose:       This funtion is called when the user clicks the "Cancel"
button on the wizard. Use it to query the user if agrees to
cancel.
Input:         hwnd : handle of the page dialog window.
hwPropSheet : handle of the parent Property Sheet window.
Output:        return FALSE to prevent cancelation, or TRUE to allow it.
Errors:        If the function returns TRUE it should inform the user about
the reason.
------------------------------------------------------------------------*/
static BOOL PropSheetPage_OnQueryCancel(HWND hwnd, HWND hwPropSheet)
{
    return FALSE;
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnApply ID:1
Purpose:       This function is called when the user selects the "OK" or
"Apply Now" button in the Property Sheet.
Input:         hwnd : handle of the page dialog window.
hwPropSheet : handle of the parent Property Sheet window.
fClosing : TRUE if the user hit the "OK" or "Close" buton,
FALSE if the "Apply" button was hit.
Output:        The function should return PSNRET_NOERROR if all was OK, or
PSNRET_INVALID_NOPAGECHANGE if there was an error, to avoid
Property Sheet closing.
Errors:        On error, the page should notify the user about the cause.
------------------------------------------------------------------------*/
static int PropSheetPage_OnApply(HWND hwnd, HWND hwPropSheet, BOOL fClosing)
{
    //TODO : add code to apply changes here
    return PSNRET_NOERROR;
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnHelp ID:1
Purpose:       This function is called when the user clicks the "Help"
button on the Property Sheet, if this page has the
PSP_HASHELP style.
Input:         hwnd : handle of the page dialog window.
hwPropSheet : handle of the parent Property Sheet window.
Output:        None.
Errors:        None.
------------------------------------------------------------------------*/
static void PropSheetPage_OnHelp(HWND hwnd, HWND hwPropSheet)
{
    // TODO: add code to show help here
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnNotify ID:1
Purpose:       Dispatch notification messages to the proper handler
funtion.
Input:         Standard.
Output:        TRUE if the notification was handled.
Errors:        None.
------------------------------------------------------------------------*/
static BOOL PropSheetPage_OnNotify(HWND hwnd, int idCtrl, LPNMHDR lpNmh)
{
    LPPSHNOTIFY lpPshn = (LPPSHNOTIFY) lpNmh;

    switch (lpPshn->hdr.code)
    {
    case PSN_SETACTIVE:
        PropSheetPage_SetResult(hwnd, PropSheetPage_OnSetActive(hwnd, lpPshn->hdr.hwndFrom));
        return TRUE;
    case PSN_KILLACTIVE:
        PropSheetPage_SetResult(hwnd, PropSheetPage_OnKillActive(hwnd, lpPshn->hdr.hwndFrom));
        return TRUE;
    case PSN_RESET:
        PropSheetPage_OnReset(hwnd, lpPshn->hdr.hwndFrom, lpPshn->lParam);
        return TRUE;
    case PSN_QUERYCANCEL:
        PropSheetPage_SetResult(hwnd, PropSheetPage_OnQueryCancel(hwnd, lpPshn->hdr.hwndFrom));
        return TRUE;
    case PSN_HELP:
        PropSheetPage_OnHelp(hwnd, lpPshn->hdr.hwndFrom);
        return TRUE;
    case PSN_APPLY:
        PropSheetPage_SetResult(hwnd, PropSheetPage_OnApply(hwnd, lpPshn->hdr.hwndFrom, lpPshn->lParam));
        return TRUE;
        // TODO : add code to handle other notifications here
    }

    return FALSE;
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_OnCommand ID:1
Purpose:       Handle conmmands sent by page controls.
Input:         hwnd : handle of the page dialog window.
id : identifier of the control that sent the command.
hwndCtl : handle of the control window.
codeNotify : command code.
Output:        None.
Errors:        None.
------------------------------------------------------------------------*/
static void PropSheetPage_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    //TODO: add code to handle commands sent by page controls here
}

/* --- Callbacks --- */
// Functions that will be called by Windows

/*------------------------------------------------------------------------
Procedure:     PropSheetPageProc ID:1
Purpose:       Page dialog message handler function.
Input:         Standard.
Output:        TRUE if the message was processed, FALSE to allow DefDlgProc
to handle it.
Errors:        None.
------------------------------------------------------------------------*/
static BOOL CALLBACK PropSheetPageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, PropSheetPage_OnInitDialog);
        HANDLE_MSG(hwnd, WM_DESTROY, PropSheetPage_OnDestroy);
        HANDLE_MSG(hwnd, WM_COMMAND, PropSheetPage_OnCommand);
        HANDLE_MSG(hwnd, WM_NOTIFY, PropSheetPage_OnNotify);
    }
    // let DefDlgProc handle it...
    return FALSE;
}


/* --- Public Functions --- */
// Function that will be called by other modules

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_FillPropPage ID:1
Purpose:       Fills the PROPSHEETPAGE structure for this page.
Input:         lpPsp : pointer to the PROPSHEETPAGE structure.
hInst : handle of the instance with the dialog resource for
this page.
Output:        None.
Errors:        None.
------------------------------------------------------------------------*/
void PropSheetPage_FillPropPage(LPPROPSHEETPAGE lpPsp, HINSTANCE hInst)
{
    lpPsp->dwSize = sizeof(PROPSHEETPAGE);
    lpPsp->dwFlags = PSP_DEFAULT | PSP_HASHELP;
    lpPsp->hInstance = hInst;
    lpPsp->pszTemplate = MAKEINTRESOURCE(IDD_DIALOG1);
    lpPsp->pfnDlgProc = PropSheetPageProc;
    lpPsp->lParam = 0;
}

/*------------------------------------------------------------------------
Procedure:     PropSheetPage_FillPropPage ID:2
Purpose:       Fills the PROPSHEETPAGE structure for this page.
Input:         lpPsp : pointer to the PROPSHEETPAGE structure.
hInst : handle of the instance with the dialog resource for
this page.
Output:        None.
Errors:        None.
------------------------------------------------------------------------*/
void PropSheetPage_FillPropPage2(LPPROPSHEETPAGE lpPsp, HINSTANCE hInst)
{
    lpPsp->dwSize = sizeof(PROPSHEETPAGE);
    lpPsp->dwFlags = PSP_DEFAULT | PSP_HASHELP;
    lpPsp->hInstance = hInst;
    lpPsp->pszTemplate = MAKEINTRESOURCE(IDD_TABCONTROLDIALOG);
    lpPsp->pfnDlgProc = PropSheetPageProc;
    lpPsp->lParam = 0;
}

