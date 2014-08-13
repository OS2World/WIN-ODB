//prodefures for pulldownmeny actions

#include "..\store.h"

//Forwarddcl
MRESULT APIENTRY OpenSaveFilterDlgProc(HWND hwnd, USHORT msg,
				       MPARAM mp1, MPARAM mp2);
VOID FileOpen( MPARAM mp2)
{
FILEDLG fileDialog;
CHAR szTitle[MESSAGELEN],
szButton[MESSAGELEN];


   fileDialog.cbSize = sizeof(FILEDLG);

   if (!WinLoadString(Main::hab, (HMODULE)0, IDS_OPENDLGTITLE, MESSAGELEN, szTitle))
   {
	WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Failed to Load String",
	(PCH)"Database",0,MB_NOICON|MB_OK);
      return;
   }

   if (!WinLoadString(Main::hab,(HMODULE)0, IDS_OPENDLGBUTTON, MESSAGELEN, szButton))
   {
      WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Failed to Load String",
	(PCH)"Database",0,MB_NOICON|MB_OK);      return;
   }

   fileDialog.pszTitle = szTitle;
   fileDialog.pszOKButton = szButton;
   fileDialog.ulUser = 0;
   fileDialog.fl = FDS_HELPBUTTON | FDS_CENTER | FDS_OPEN_DIALOG;
   fileDialog.pfnDlgProc = (PFNWP)OpenSaveFilterDlgProc;
   fileDialog.lReturn = 0;
   fileDialog.lSRC = 0;
   fileDialog.hMod = (HMODULE)0;
   fileDialog.usDlgId = IDD_FILEOPEN;
   fileDialog.x = 0;
   fileDialog.y = 0;

   if (!WinLoadString(Main::hab, (HMODULE)0, IDS_FILEOPENEXT, CCHMAXPATH,
                     fileDialog.szFullFile))
   {
      WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Failed to Load String",
	(PCH)"Database",0,MB_NOICON|MB_OK);      return;
   }

   fileDialog.pszIType = NULL;
   fileDialog.papszITypeList = NULL;
   fileDialog.pszIDrive = NULL;
   fileDialog.papszIDriveList = NULL;
   fileDialog.sEAType = 0;
   fileDialog.papszFQFilename = NULL;
   fileDialog.ulFQFCount = 0;

   /*
    *Call the standard file dialog to get the file
    */
   if (!WinFileDlg(HWND_DESKTOP, HWND_DESKTOP, (PFILEDLG)&fileDialog))
       return;

   /*
    *  Upon sucessful return of a file, open it for reading
    */

   if (fileDialog.lReturn == ID_OK) {
      strcpy(szFullPath, fileDialog.szFullFile);
      if (odb.load_database(szFullPath)==0)
	WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Could not load databasefile. The system may be unstable due to erroneous data file",
		(PCH)"Database",0,MB_NOICON|MB_OK|MB_MOVEABLE);

      /*else
	WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Database loded",
		(PCH)"Database",0,MB_NOICON|MB_OK|MB_MOVEABLE);
       */
	return;
      }
   }

VOID FileSave( MPARAM mp2)
{
FILEDLG fileDialog;
CHAR szTitle[MESSAGELEN],szButton[MESSAGELEN];


   fileDialog.cbSize = sizeof(FILEDLG);

   if (!WinLoadString(Main::hab, (HMODULE)0, IDS_SAVEDLGTITLE, MESSAGELEN, szTitle))
   {
	WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Failed to Load String",
	(PCH)"Database",0,MB_NOICON|MB_OK);
      return;
   }

   if (!WinLoadString(Main::hab,(HMODULE)0, IDS_SAVEDLGBUTTON, MESSAGELEN, szButton))
   {
      WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Failed to Load String",
	(PCH)"Database",0,MB_NOICON|MB_OK);      return;
   }

   fileDialog.pszTitle = szTitle;
   fileDialog.pszOKButton = szButton;
   fileDialog.ulUser = 0;
   fileDialog.fl = FDS_HELPBUTTON | FDS_CENTER | FDS_OPEN_DIALOG;
   fileDialog.pfnDlgProc = (PFNWP)OpenSaveFilterDlgProc;
   fileDialog.lReturn = 0;
   fileDialog.lSRC = 0;
   fileDialog.hMod = (HMODULE)0;
   fileDialog.usDlgId = IDD_FILEOPEN;
   fileDialog.x = 0;
   fileDialog.y = 0;

   if (!WinLoadString(Main::hab, (HMODULE)0, IDS_FILEOPENEXT, CCHMAXPATH,
		     fileDialog.szFullFile))
   {
      WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Failed to Load String",
	(PCH)"Database",0,MB_NOICON|MB_OK);      return;
   }

   fileDialog.pszIType = NULL;
   fileDialog.papszITypeList = NULL;
   fileDialog.pszIDrive = NULL;
   fileDialog.papszIDriveList = NULL;
   fileDialog.sEAType = 0;
   fileDialog.papszFQFilename = NULL;
   fileDialog.ulFQFCount = 0;

   /*
    *Call the standard file dialog to get the file
    */
   if (!WinFileDlg(HWND_DESKTOP, HWND_DESKTOP, (PFILEDLG)&fileDialog))
       return;

   /*
    *  Upon sucessful return of a file, open it for reading
    */

   if (fileDialog.lReturn == ID_OK) {
      strcpy(szFullPath, fileDialog.szFullFile);
      if (odb.save_database(szFullPath)==0)
	WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Could not Save databasefile",
		(PCH)"Database",0,MB_NOICON|MB_OK|MB_MOVEABLE);

      else
	WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Database Saved",
		(PCH)"Database",0,MB_NOICON|MB_OK|MB_MOVEABLE);

	return;
      }
   }

/***********************************************************
 * Name         : OpenSaveFilterProc
 *
 * Description  : Procedure to handle wm_help messages for
 *                the file open/save dialog.
 *
 * Concepts     : This routine handles the WM_HELP messages for
 *                the dialog boxs created with the WinFileDlg
 *                Checks the flags used on the call to determine
 *                the correct help panel to display.
 *
 * API's        : WinQueryWIndowULong
 *                WinSendMessage
 *
 * Parameters   : hwnd - Window handle to which message is addressed
 *                msg - Message type
 *                mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns : Dependent upon message sent
 **************************************************************/

MRESULT APIENTRY OpenSaveFilterDlgProc(HWND hwnd, USHORT msg,
				       MPARAM mp1, MPARAM mp2)
{
   if(msg == WM_HELP)
   {
      PFILEDLG pOpenSaveFileDlgStruct;
      /*
       * Get a pointer to the file dialog structure.
       */
      pOpenSaveFileDlgStruct = (PFILEDLG) WinQueryWindowULong(hwnd, QWL_USER);

      /*
       * If this is an the File Open dialog, display the file open help
       * panel.
       */
      if (pOpenSaveFileDlgStruct->fl & FDS_OPEN_DIALOG)
      {
	 //DisplayHelpPanel(PANEL_FILEOPEN);
         return (MRESULT)FALSE ;
      }

      /*
       * If this is an the File Save or Save As dialog, display the file
       * Save As help panel.
       */

      if (pOpenSaveFileDlgStruct->fl & FDS_SAVEAS_DIALOG)
      {
	 //DisplayHelpPanel(PANEL_FILESAVEAS);
         return (MRESULT)FALSE ;
      }
   }
   return WinDefFileDlgProc( hwnd, msg, mp1, mp2 );
}                                       /* End of OpenSaveFilterDlgProc  */


