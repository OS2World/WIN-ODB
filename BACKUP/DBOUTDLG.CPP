//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class DbOutDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//Dboutput dialog
DbOutDlg::DbOutDlg(void){}

DbOutDlg::~DbOutDlg(void)                 // Destructor
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

DbOutDlg::DbOutDlg(HWND hwndParent1,
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
				// DbOutDlg::DlgProc (pure virtual
				// dispatcher, defined as a macro in dlg.h)
    }

//-----------------------DbOutDlg::Run()----------------------------------

DbOutDlg::Run()
{
    WinDlgBox(  hwndParent,     // Place anywhere on desktop
		hwndOwner,      // Owned by frame
		::DlgCallBack,  // Address of dialog procedure
		Resource,       // Module handle
		ulDlgid,        // Dialog identifier in resource
		NULL); 		// Initialization data
   return(0);
}

//-----------------------DbOutDlg::wmInitDlg()----------------------------

MRESULT DbOutDlg::wmInitDlg(HM12)
{
       ::SetSysMenu(hwnd);      // remove unused sys. menu items
       ::CenterDialog(hwnd);
       USE_HM12;
       return FALSE;
}

//-----------------------DbOutDlg::wmCommand()-----------------------------

MRESULT DbOutDlg::wmCommand(HM12)
{USE_HM12;
      switch( SHORT1FROMMP( mp1 ) )     // Extract the command value
      {
	case DID_OK:
	case DID_CANCEL:
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
}

