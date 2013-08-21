/*------------------------------------------------------------------------
Module:        MAINPROPSHEET.C
Author:        Daniel Guerrero Miralles
Project:
State:
Creation Date: 6/11/98
Description:   Template for a Property Sheet as the
application main window. Includes
WinMain.
------------------------------------------------------------------------*/
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
// (c) Copyright 1998 Daniel Guerrero Miralles. All Rights Reserved.

#include "PropSheetPage.h"
/* --- Constants --- */

#define MAINPROPSHEET_TITLE TEXT("Property Sheet")
#define MAINPROPSHEET_NUMPAGES 2
#define MAINPROPSHEET_STARTPAGE 0


/* --- Prototypes --- */

static BOOL CALLBACK MainPropSheetProc(HWND, UINT, WPARAM, LPARAM);


/* --- Globals --- */

DLGPROC lpfnMainPropSheet_DefProc = NULL;


/* --- Private Functions --- */

static void MainPropSheet_PreCreate(LPDLGTEMPLATE lpDlgTemplate)
{
    // TODO: add pre-creation code here
}

static void MainPropSheet_Initialize(HWND hwnd)
{
    lpfnMainPropSheet_DefProc = (DLGPROC) SubclassDialog(hwnd, MainPropSheetProc);
    // TODO: add initialization code here
}

static void MainPropSheet_OnDestroy(HWND hwnd)
{
    // TODO : add de-initialization code here
}


/* --- Callbacks --- */

static BOOL CALLBACK MainPropSheetProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    BOOL fDefResult;

    fDefResult = (BOOL) CallWindowProc((WNDPROC) lpfnMainPropSheet_DefProc, hwnd, msg, wParam, lParam);
    switch (msg)
    {
    case WM_DESTROY:
        HANDLE_WM_DESTROY(hwnd, wParam, lParam, MainPropSheet_OnDestroy);
        break;
        // TODO: add handlers to new messages here
    }
    return fDefResult;
}

static int CALLBACK MainPropSheet_InitCallback(HWND hwnd, UINT uMsg, LPARAM lParam)
{
    switch (uMsg)
    {
    case PSCB_INITIALIZED:
        MainPropSheet_Initialize(hwnd);
        break;
    case PSCB_PRECREATE:
        MainPropSheet_PreCreate((LPDLGTEMPLATE) lParam);
        break;
    }
    return 0;
}


/* --- Public functions --- */

int DoMainPropSheet(HINSTANCE hInstance)
{
    PROPSHEETPAGE aPsp[MAINPROPSHEET_NUMPAGES];
    PROPSHEETHEADER Psh;

    // TODO: fill the PROPSHEETHEADER structures for each page
    PropSheetPage_FillPropPage(&aPsp[0], hInstance); // <- example
    PropSheetPage_FillPropPage2(&aPsp[1], hInstance); // <- example

    Psh.dwSize = sizeof (PROPSHEETHEADER);
    Psh.dwFlags = PSH_PROPSHEETPAGE | PSH_USECALLBACK;
    Psh.hwndParent = NULL;
    Psh.pszCaption = MAINPROPSHEET_TITLE;
    Psh.nPages = MAINPROPSHEET_NUMPAGES;
    Psh.nStartPage = MAINPROPSHEET_STARTPAGE;
    Psh.ppsp = (LPCPROPSHEETPAGE) &aPsp[0];
    Psh.pfnCallback = (PFNPROPSHEETCALLBACK) MainPropSheet_InitCallback;

    return PropertySheet(&Psh);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    DoMainPropSheet(hInst);
    return 0;
}
