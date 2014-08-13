//=================================================================
//
// Project: Store an object oriented storage manager
// Version: Development ver for os2 Warp
// This file: pmstore.cpp master file for the graphical user intfc
// The project must be linked with store.lib which is the oodatabase
// library file. All other files are included from this one
//
//=================================================================
#define INCL_PM                         // Define to "switch on" PM includes
                                        //   in "os2.h"
#include <os2.h>                        // PM includes
#include <string.h>                     // String functions
#include <stdio.h>
#include <fstream.h>
#undef NULL
#define NULL 0L
#include "..\store.h"
#include "pmstore.h"                    // Resource file symbolics
#include "menuact.cpp"
#include "validation.cpp"   //datafield validation procedures

#include "pmmainw.cpp" //mainwindow code
#include "dboutdlg.cpp" //dbout dialog code
#include "dbcreate.cpp" //database creation dialog
#include "cretype.cpp" //type creation dialog
#include "creprop.cpp" //property creation dialog
#include "creinst.cpp" //instance creation dialog
#include "inspectdb.cpp" //inspect database
#include "inspectp.cpp" //inspect type
#include "inspinst.cpp" //inspect instances
#include "inspdel.cpp" //update or delete stuff
//--------------------------WinCallBack()----------------------------------

MRESULT EXPENTRY _export WinCallBack(HM12)
{
    MRESULT retVal = (MRESULT)FALSE;
    // Pointer to the c++ window object
    Window *pWindow = (Window *)WinQueryWindowULong(hwnd,0);

    // The pointer pWindow will have an invalid value if the WM_CREATE
    // message has not yet been processed (we respond to the WM_CREATE
    // message by setting the extra bytes to be a pointer to the
    // (C++) object corresponding to the Window identified
    // by hwnd).  The messages that
    // precede WM_CREATE must be processed without using pWindow so we
    // pass them to WinDefWindowProc.

    if (pWindow == 0) {
	    if (msg == WM_CREATE) {
		pWindow = CallBack::winThis;
		WinSetWindowULong(hwnd,0,(ULONG)pWindow);
		retVal = pWindow->WndProc(hwnd,msg,mp1,mp2);
	    }
       else
		retVal = WinDefWindowProc(hwnd,msg,mp1,mp2);
    }
    else
	    retVal =  pWindow->WndProc(hwnd,msg,mp1,mp2);

    return(retVal);
}

//--------------------------DlgCallBack()---------------------------------
// N.B: If this function is going to be used by more than 1 dialog box at
// the same time, you must save the CallBack::dlgThis for different hwnd in
//  a local linked list, and free it when the WM_CLOSE(or whatever
// terminating message) is issued.

MRESULT EXPENTRY _export DlgCallBack(HM12)
{
    MRESULT retVal = (MRESULT)FALSE;
    // Pointer to the c++ window object
    Dialog *pDialog;

    pDialog = CallBack::dlgThis;
    if(pDialog)
    {
	retVal = pDialog->DlgProc(hwnd,msg,mp1,mp2);
    }
    else
	retVal = WinDefDlgProc(hwnd,msg,mp1,mp2);
    return(retVal);
}


//----------------------------------------------------------
// Name         : SetSysMenu
//
// Description  : Procedure to remove unselectable items from
//                the window system menu.
//
// Concepts     : This routine determines the number of items
//                in the system menu, loops through those
//                items removing disabled menu items and menu
//                seperators.
//
// API's        : WinMessageBox
//
// Parameters   : None
//
// Return       : None
//
//-------------------------------------------------------------

static VOID SetSysMenu(HWND hwndDlg)
{
  SHORT sMenuItems;
  USHORT usItemid;
  MENUITEM menuItem;
  HWND  hwndSubMenu;

  // Determine the definition of the system menu
  WinSendDlgItemMsg(hwndDlg,(SHORT)FID_SYSMENU, MM_QUERYITEM,
		    MPFROM2SHORT((SHORT)SC_SYSMENU,FALSE),
		    MPFROMP(&menuItem));

  hwndSubMenu = menuItem.hwndSubMenu;

  // Find the number of items in the in the submenu
  sMenuItems = (SHORT)WinSendMsg(hwndSubMenu,
				 MM_QUERYITEMCOUNT,NULL,NULL);

   // Loop through the submenu items and remove disabled
   // menu items and menu separators.

  for (sMenuItems-- ; sMenuItems >= 0 ;sMenuItems-- )
  {
    // Find the item ID for the current position.
    usItemid = (USHORT)WinSendMsg(hwndSubMenu,MM_ITEMIDFROMPOSITION,
			 MPFROMSHORT(sMenuItems), NULL);

    // Query the definition of the current item
    WinSendMsg(hwndSubMenu, MM_QUERYITEM,
	       MPFROM2SHORT(usItemid,FALSE), MPFROMP(&menuItem));

   /*
    * If the menu item is disabled or the item has a style
    * of MIS_SEPARATOR - delete it.
    */
    if ((BOOL)(WinSendMsg(hwndSubMenu, MM_QUERYITEMATTR,
	  MPFROM2SHORT(usItemid,FALSE), (MPARAM)MIA_DISABLED)) ||
	  (menuItem.afStyle & MIS_SEPARATOR))
    {
      WinSendMsg(hwndSubMenu, MM_DELETEITEM,
		MPFROM2SHORT(usItemid,FALSE),NULL);
    }
  } // endfor
}

//-----------------------------CenterDialog()-----------------------------
// This function places a dialog in the center of the Desktop

void CenterDialog(HWND hDlg)
{
  RECTL r;

  WinQueryWindowRect(hDlg, &r);
  WinSetWindowPos
  (
    hDlg,
    0,
    (WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN) - (r.xRight-r.xLeft))/2,
    (WinQuerySysValue( HWND_DESKTOP, SV_CYSCREEN) - (r.yTop-r.yBottom))/2,
    r.xRight - r.xLeft,
    r.yTop - r.yBottom,
    SWP_MOVE
  );
}

// main entry point
void cdecl main()
{
    // Initialize PM first
    if (Main::Initialize() == FALSE)
        AbortHello((HWND)NULL,(HWND)NULL);

    {
        MainWindow MainWnd; // Set up new scope and create an instance
                            // of MainWindow, which causes it to
                            // Register the class and Create the
                            // main window.  When MainWnd goes out
                            // of scope, the destructor will be called
                            // which cleans up with WinDestroyWindow
        Main::MessageLoop();
    }
    Main::Terminate();
}


MRESULT EXPENTRY _export WndProc(HWND hWnd,ULONG iMessage,
    MPARAM mParam1, MPARAM mParam2)
{
    // Pointer to the c++ window object
    Window *pWindow = (Window *)WinQueryWindowULong(hWnd,0);

    // The pointer pWindow will have an invalid value if the WM_CREATE
    // message has not yet been processed (we respond to the WM_CREATE
    // message by setting the extra bytes to be a pointer to the
    // (C++) object corresponding to the Window identified
    // by hWnd).  The messages that
    // precede WM_CREATE must be processed without using pWindow so we
    // pass them to WinDefWindowProc.

    if (pWindow == 0) {
        if (iMessage == WM_CREATE) {
            pWindow = Main::tempthis;
            WinSetWindowULong(hWnd,0,(ULONG)pWindow);
            return pWindow->WndProc(hWnd,iMessage,mParam1,mParam2);
        } else
            return WinDefWindowProc(hWnd,iMessage,mParam1,mParam2);
    } else
        return pWindow->WndProc(hWnd,iMessage,mParam1,mParam2);
}

// AbortHello -- report an error returned from an API service.
VOID AbortHello(HWND hwndFrame,HWND hwndClient)
{
PERRINFO  pErrInfoBlk;
PSZ       pszOffSet;
void      stdprint(void);
PSZ  pszErrMsg;

      DosBeep(100,10);
      if ((pErrInfoBlk = WinGetErrorInfo(Main::hab)) != NULL)
      {
        pszOffSet = ((PSZ)pErrInfoBlk) + pErrInfoBlk->offaoffszMsg;
        pszErrMsg = ((PSZ)pErrInfoBlk) + *((PSHORT)pszOffSet);
        if (hwndFrame && hwndClient)
            WinMessageBox(HWND_DESKTOP,      // Parent window is desk top
                       hwndFrame,            // Owner window is our frame
                       (PSZ)pszErrMsg,       // PMWIN Error message
                       (PSZ)"Error Msg",     // Title bar message
                       MSGBOXID,             // Message identifier
                       MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL ); // Flags
        else
            WinMessageBox(HWND_DESKTOP,      // Parent window is desk top
                       HWND_DESKTOP,         // Owner window is desktop
                       (PSZ)pszErrMsg,       // PMWIN Error message
                       (PSZ)"Error Msg",     // Title bar message
                       MSGBOXID,             // Message identifier
                       MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL ); // Flags
        WinFreeErrorInfo(pErrInfoBlk);
      }
      WinPostMsg(hwndClient, WM_QUIT, (ULONG)0, (ULONG)0);
}
