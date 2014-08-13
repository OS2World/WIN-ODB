//=================================================================
//
// Project: Store an object oriented storage manager
// Version: Development ver for os2 Warp
// This file: dbcreate.cpp dialog box for creating database
//
//=================================================================
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class DbCreateDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//Dboutput dialog
DbCreateDlg::DbCreateDlg(void){}

DbCreateDlg::~DbCreateDlg(void)                 // Destructor
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

DbCreateDlg::DbCreateDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1):Dialog(hwndParent1,
				      hwndOwner1,
				      Resource1,
				      ulDlgid1,
				      pCreateParams1)
    {
    initialized = TRUE;
    CallBack::dlgThis = this;   // Stuff "this" pointer in global dlgThis
				// for use by _export DlgCallBack.
				// It was assigned as a
				// call back function by WinDlgBox().
				// dlgThis is used to call
				// DbCreateDlg::DlgProc (pure virtual
				// dispatcher, defined as a macro in dlg.h)
    }

//-----------------------DbCreateDlg::Run()----------------------------------

DbCreateDlg::Run()
{
    WinDlgBox(  hwndParent,     // Place anywhere on desktop
		hwndOwner,      // Owned by frame
		::DlgCallBack,  // Address of dialog procedure
		Resource,       // Module handle
		ulDlgid,        // Dialog identifier in resource
		NULL); 		// Initialization data
   return(0);
}

//-----------------------DbCreateDlg::wmInitDlg()----------------------------

MRESULT DbCreateDlg::wmInitDlg(HM12)
{
       ::SetSysMenu(hwnd);      // remove unused sys. menu items
       ::CenterDialog(hwnd);
       USE_HM12;
       hwnddbname=WinWindowFromID(hwnd ,DF_NEWDBNAME);
       hwndtblsize=WinWindowFromID(hwnd ,DF_NEWDBSIZE);
       return FALSE;
}

//-----------------------DbCreateDlg::wmCommand()-----------------------------

MRESULT DbCreateDlg::wmCommand(HM12)
{USE_HM12;
char dbname[MAXNAMELENGTH]="";
char strsize[MAXNAMELENGTH]="";
ODB_INT size;
switch( SHORT1FROMMP( mp1 ) )     // Extract the command value
      {
	case DID_OK:
		WinQueryWindowText(hwnddbname,MAXNAMELENGTH,dbname);
		WinQueryWindowText(hwndtblsize,MAXNAMELENGTH,strsize);
		if ((strlen(dbname)<1)||(strlen(strsize)<1)){
			WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Both name and size must be defined",
			(PCH)"Alert",0,MB_NOICON|MB_OK);
			break;
			}
		else {
			if (validate_integer(strsize,size)==0){
				WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Size not valid",
				(PCH)"Alert",0,MB_NOICON|MB_OK);
				break;
				};
			odb.initialize();
			validate_name(dbname);
			odb.set_name(dbname);
			odb.set_tablesize(size);
			strcpy(Main::szDatabase,dbname);
			//old stuff must be freed here
			WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Database created",
				(PCH)"Success",0,MB_NOICON|MB_OK);
			WinDismissDlg( hwnd, TRUE );
			break;
			}	case DID_CANCEL:
	  if(WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Quit this window?",
			(PCH)"Sanity Check",0,
			MB_NOICON|MB_OKCANCEL)==MBID_OK)
		WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
	  return (MRESULT) FALSE;
	default:
      /*
       * Any event messages that the dialog procedure has not processed
       * come here and are processed by WinDefDlgProc.
       * This call MUST exist in your dialog procedure.
       */
	return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
      }
      return (MRESULT) FALSE;
}
