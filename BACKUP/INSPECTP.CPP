//=================================================================
//
// Project: Store an object oriented storage manager
// Version: Development ver for os2 Warp
// This file: inspectdb dialog for inspecting database
//
//=================================================================

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class TpInspectDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//Dbinspectput dialog
TpInspectDlg::TpInspectDlg(void){}

TpInspectDlg::~TpInspectDlg(void)                 // Destructor
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

TpInspectDlg::TpInspectDlg(HWND hwndParent1,
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
				// TpInspectDlg::DlgProc (pure virtual
				// dispatcher, defined as a macro in dlg.h)
    }

//-----------------------TpInspectDlg::Run()----------------------------------

TpInspectDlg::Run()
{
    WinDlgBox(  hwndParent,     // Place anywhere on desktop
		hwndOwner,      // Owned by frame
		::DlgCallBack,  // Address of dialog procedure
		Resource,       // Module handle
		ulDlgid,        // Dialog identifier in resource
		NULL); 		// Initialization data
   return(0);
}

//-----------------------TpInspectDlg::wmInitDlg()----------------------------

MRESULT TpInspectDlg::wmInitDlg(HM12)
{
       ::SetSysMenu(hwnd);      // remove unused sys. menu items
       ::CenterDialog(hwnd);
       USE_HM12;
       hwndtpname=WinWindowFromID(hwnd,61);
       hwndsuptp=WinWindowFromID(hwnd,62);
       hwndsubtp=WinWindowFromID(hwnd,64);
       hwndprops=WinWindowFromID(hwnd,66);
       populate_types(hwndtpname,odb);
	return FALSE;
}

//-----------------------TpInspectDlg::wmCommand()-----------------------------

MRESULT TpInspectDlg::wmCommand(HM12)
{USE_HM12;
      switch( SHORT1FROMMP( mp1 ) )     // Extract the command value
      {
	case DID_CANCEL:
		WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
	  return (MRESULT) FALSE;
	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
      }
}

MRESULT TpInspectDlg::wmControl(HM12){
USE_HM12;
USHORT sender,command;
type *tp;
char typename[MAXNAMELENGTH]="";
char *suptpname;
int index;
	sender=SHORT1FROMMP( mp1 );
	command=SHORT2FROMMP(mp1);
	switch(sender){
	case 61:
		if (command==CBN_EFCHANGE){
			//a type has been selected. Populate propery combobox
			//any values ther must first be removed
			index=WinQueryLboxSelectedItem(hwndtpname); //typename
			WinQueryLboxItemText(hwndtpname,index,typename,MAXNAMELENGTH);
			WinSetWindowText(hwndsuptp,(PSZ)"");
			tp=odb.gettypenamed(typename);
			if (tp==NULL){
				WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH)"Not a type",
				(PCH)"Alert",0,MB_OK);
				break;
				}
			//Populate the listbox with props. First clear then populate
			suptpname=odb.getsupertype(typename);
			WinSetWindowText(hwndsuptp,(PSZ)suptpname);
			clearlbox(hwndprops);
			clearlbox(hwndsubtp);
			populate_props(hwndprops,typename, odb);
			populate_subtypes(hwndsubtp,typename,odb);
		}; //if command
		break;

	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
	} //switch sender
	return (MRESULT) FALSE;
}