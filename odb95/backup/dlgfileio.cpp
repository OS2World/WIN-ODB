#include <windows.h>    // includes basic windows functionality
#include <commdlg.h>    // includes common dialog functionality
#include <dlgs.h>       // includes common dialog template defines
#include <cderr.h>
#include "d:\users\staffan\odb\odb95\odb95.h"
#include "d:\users\staffan\odb\odb95\dlgfileio.h"

/*
if (odb.load_database(szFullPath)<0)
	WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PCH) "Could not load databasefile. The system may be unstable due to erroneous data file",
		(PCH)"Database",0,MB_NOICON|MB_OK|MB_MOVEABLE);
      */

DlgFileIo::DlgFileIo(){
}

DlgFileIo::DlgFileIo(HINSTANCE hThis,int nm,HWND p,mainWindow *mw){
szFile[0]='\0';
hWndParent=p;
hwndEdit = CreateWindow("EDIT",NULL,WS_CHILD | WS_VISIBLE | WS_VSCROLL |
                    		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
                			0, 0, 0, 0, hWndParent,(HMENU) ID_EDITCHILD,
                			(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),
                			NULL);

	mode=nm;
				
   strcpy( szFile, "");
   OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = p;
    OpenFileName.hInstance         = hThis;
    OpenFileName.lpstrFilter       = "Odb Database files\0*.odb\0ODBQL file\0*.ocr\0\0";
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 1;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = NULL;

    if (nm==LOADDATABASE)
    	OpenFileName.lpstrTitle      = OPENFILE;
    else OpenFileName.lpstrTitle   = "Save Database";
    
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;
	 OpenFileName.lCustData         = (LPARAM)&sMyData;
    OpenFileName.lpfnHook 		   = (LPOFNHOOKPROC) dlgFn;
	 OpenFileName.lpTemplateName    = MAKEINTRESOURCE(IDD_COMDLG32);
    OpenFileName.Flags             = OFN_SHOWHELP | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLETEMPLATE;
}


int DlgFileIo::create(mainWindow *mwh){
database *odb;
	if ((mode==LOADDATABASE)||(mode==LOADODBQL)){
		if (!GetOpenFileName(&OpenFileName))
			ProcessCDError(CommDlgExtendedError(),hWndParent);
   }
   else {
   	if (!GetSaveFileName(&OpenFileName))
			ProcessCDError(CommDlgExtendedError(),hWndParent);
   }
      odb=mwh->getDb();

      if (odb==NULL){
      	warn("Could not get Database Handle!",hWndParent);
         return 0;
      }

      if ((mode==LOADDATABASE)&&(strlen(OpenFileName.lpstrFile)>0)&&
      		(0>((*odb)<<OpenFileName.lpstrFile))) {
      	warn("Database file could not be opened",hWndParent);
         return 0;
      }

      if ((mode==LOADODBQL)&&(strlen(OpenFileName.lpstrFile)>0))
      		if (0==(*odb).parseFile(OpenFileName.lpstrFile)) {
      			warn(odb->getODBQLError(),hWndParent);
         		return 0;
      	}

      if ((mode==SAVEDATABASE)&&(strlen(OpenFileName.lpstrFile)>0)&&
      		(0>((*odb)>>OpenFileName.lpstrFile))) {
      	warn("Database file could not be saved",hWndParent);
         return 0;
      }
      return 1;
}

DlgFileIo::~DlgFileIo(){}




BOOL NEAR PASCAL DlgFileIo::TestNotify(HWND hDlg, LPOFNOTIFY pofn)
{
	switch (pofn->hdr.code)
	{
		// The selection has changed. 
		case CDN_SELCHANGE:
		{
			char szFile[MAX_PATH];

			// Get the file specification from the common dialog.
			if (CommDlg_OpenSave_GetSpec(GetParent(hDlg),
				szFile, sizeof(szFile)) <= sizeof(szFile))
			{
				// Set the dialog item to reflect this.
				SetDlgItemText(hDlg, IDE_SELECTED, szFile);
			}

			// Get the path of the selected file.
			if (CommDlg_OpenSave_GetFilePath(GetParent(hDlg),
				szFile, sizeof(szFile)) <= sizeof(szFile))
			{
				// Display this path in the appropriate box.
				SetDlgItemText(hDlg, IDE_PATH, szFile);
			}
		}
		break;

		// A new folder has been opened.
		case CDN_FOLDERCHANGE:
		{
			char szFile[MAX_PATH];

			if (CommDlg_OpenSave_GetFolderPath(GetParent(hDlg),
				szFile, sizeof(szFile)) <= sizeof(szFile))
			{
				// Display this new path in the appropriate box.
				SetDlgItemText(hDlg, IDE_SELECTED, szFile);
			}
		}
		break;

		// The "Help" pushbutton has been pressed.
		case CDN_HELP:
			MessageBox(hDlg, "Select a file fith .odb extension", MESSAGENAME, MB_OK);
			break;

		// The 'OK' pushbutton has been pressed.
		case CDN_FILEOK:
			// Update the appropriate box.
			SetDlgItemText(hDlg,IDE_SELECTED, pofn->lpOFN->lpstrFile);
			SetWindowLong(hDlg, DWL_MSGRESULT, 1L);
			break;

		// Received a sharing violation.
		case CDN_SHAREVIOLATION:
			// Update the appropriate box.
			SetDlgItemText(hDlg, IDE_SELECTED, pofn->pszFile);
			MessageBox(hDlg, "Got a sharing violation notify.", "ComDlg32 Test", MB_OK);
			break;
	}

	return(TRUE);
}


BOOL DlgFileIo::callbackfn(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		case WM_INITDIALOG:
			// Save off the long pointer to the OPENFILENAME structure.
			SetWindowLong(hDlg, DWL_USER, lParam);
			break;

		case WM_DESTROY:
			{
			LPOPENFILENAME lpOFN = (LPOPENFILENAME)GetWindowLong(hDlg, DWL_USER);
			LPMYDATA psMyData = (LPMYDATA)lpOFN->lCustData;

			GetDlgItemText(hDlg, IDE_PATH, psMyData->szTest1, sizeof(psMyData->szTest1));
			GetDlgItemText(hDlg, IDE_SELECTED, psMyData->szTest2, sizeof(psMyData->szTest2));
			}
			break;

		case WM_NOTIFY:
			TestNotify(hDlg, (LPOFNOTIFY)lParam);

		default:
			if (uMsg == cdmsgFileOK)
			{
				SetDlgItemText(hDlg, IDE_SELECTED, ((LPOPENFILENAME)lParam)->lpstrFile);
				if (MessageBox(hDlg, "Got the OK button message.\n\nShould I open it?", "ComDlg32 Test", MB_YESNO)
					== IDNO)
				{
					SetWindowLong(hDlg, DWL_MSGRESULT, 1L);
				}
				break;
			}
			else if (uMsg == cdmsgShareViolation)
			{
				SetDlgItemText(hDlg, IDE_SELECTED, (LPSTR)lParam);
				MessageBox(hDlg, "Got a sharing violation message.", "ComDlg32 Test", MB_OK);
				break;
			}
			return FALSE;
	}
	return TRUE;
}

void DlgFileIo::ProcessCDError(DWORD dwErrorCode, HWND hWnd)
{
   WORD  wStringID;
   TCHAR  buf[MAX_PATH];

   switch(dwErrorCode)
      {
	 case CDERR_DIALOGFAILURE:   wStringID=IDS_DIALOGFAILURE;   break;
	 case CDERR_STRUCTSIZE:      wStringID=IDS_STRUCTSIZE;      break;
	 case CDERR_INITIALIZATION:  wStringID=IDS_INITIALIZATION;  break;
	 case CDERR_NOTEMPLATE:      wStringID=IDS_NOTEMPLATE;      break;
	 case CDERR_NOHINSTANCE:     wStringID=IDS_NOHINSTANCE;     break;
	 case CDERR_LOADSTRFAILURE:  wStringID=IDS_LOADSTRFAILURE;  break;
	 case CDERR_FINDRESFAILURE:  wStringID=IDS_FINDRESFAILURE;  break;
	 case CDERR_LOADRESFAILURE:  wStringID=IDS_LOADRESFAILURE;  break;
	 case CDERR_LOCKRESFAILURE:  wStringID=IDS_LOCKRESFAILURE;  break;
	 case CDERR_MEMALLOCFAILURE: wStringID=IDS_MEMALLOCFAILURE; break;
	 case CDERR_MEMLOCKFAILURE:  wStringID=IDS_MEMLOCKFAILURE;  break;
	 case CDERR_NOHOOK:          wStringID=IDS_NOHOOK;          break;
	 case PDERR_SETUPFAILURE:    wStringID=IDS_SETUPFAILURE;    break;
	 case PDERR_PARSEFAILURE:    wStringID=IDS_PARSEFAILURE;    break;
	 case PDERR_RETDEFFAILURE:   wStringID=IDS_RETDEFFAILURE;   break;
	 case PDERR_LOADDRVFAILURE:  wStringID=IDS_LOADDRVFAILURE;  break;
	 case PDERR_GETDEVMODEFAIL:  wStringID=IDS_GETDEVMODEFAIL;  break;
	 case PDERR_INITFAILURE:     wStringID=IDS_INITFAILURE;     break;
	 case PDERR_NODEVICES:       wStringID=IDS_NODEVICES;       break;
	 case PDERR_NODEFAULTPRN:    wStringID=IDS_NODEFAULTPRN;    break;
	 case PDERR_DNDMMISMATCH:    wStringID=IDS_DNDMMISMATCH;    break;
	 case PDERR_CREATEICFAILURE: wStringID=IDS_CREATEICFAILURE; break;
	 case PDERR_PRINTERNOTFOUND: wStringID=IDS_PRINTERNOTFOUND; break;
	 case CFERR_NOFONTS:         wStringID=IDS_NOFONTS;         break;
	 case FNERR_SUBCLASSFAILURE: wStringID=IDS_SUBCLASSFAILURE; break;
	 case FNERR_INVALIDFILENAME: wStringID=IDS_INVALIDFILENAME; break;
	 case FNERR_BUFFERTOOSMALL:  wStringID=IDS_BUFFERTOOSMALL;  break;

	 case 0:   //User may have hit CANCEL or we got a *very* random error
	    return;

	 default:
	    wStringID=IDS_UNKNOWNERROR;
      }

   LoadString(NULL, wStringID, buf, sizeof(buf));
   MessageBox(hWnd, buf, NULL, MB_OK);
   return;
}

