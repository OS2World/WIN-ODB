//=================================================================
//
// Project: Store an object oriented storage manager
// Version: Development ver for os2 Warp
// This file: cretype.cpp dialog box for creating types
//
//=================================================================//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class CreTpDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
CreTpDlg::CreTpDlg(void){}

CreTpDlg::~CreTpDlg(void)                 // Destructor
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

CreTpDlg::CreTpDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1):Dialog(hwndParent1,
				      hwndOwner1,
				      Resource1,
				      ulDlgid1,
				      pCreateParams1){

	initialized = TRUE;
    CallBack::dlgThis = this;
    }

//-----------------------CreTpDlg::Run()----------------------------------

CreTpDlg::Run()
{
    WinDlgBox(  hwndParent,     // Place anywhere on desktop
		hwndOwner,      // Owned by frame
		::DlgCallBack,  // Address of dialog procedure
		Resource,       // Module handle
		ulDlgid,        // Dialog identifier in resource
		NULL); 		// Initialization data
   return(0);
}

//-----------------------CreTpDlg::wmInitDlg()----------------------------

MRESULT CreTpDlg::wmInitDlg(HM12)
{ int i=0;
char *nm;
	::SetSysMenu(hwnd);      // remove unused sys. menu items
       ::CenterDialog(hwnd);
       USE_HM12;
	hwndlb=WinWindowFromID(hwnd ,114);
	nm=odb.getallsubtypes();
	while (nm!=NULL){
		WinInsertLboxItem(hwndlb,i,(PSZ)nm);
		i++;
		nm=odb.getnextsubtype();
	};
	hwndef=WinWindowFromID(hwnd,112);
	return FALSE;
}

//-----------------------CreTpDlg::wmCommand()-----------------------------

MRESULT CreTpDlg::wmCommand(HM12)
{int index;
 type *tp;
char supertype[MAXNAMELENGTH]="";
char typename[MAXNAMELENGTH]="";
USE_HM12;
      switch( SHORT1FROMMP( mp1 ) )     // Extract the command value
      {
	case DID_OK:                    // The Enter pushbutton or key.
					// WinQueryWindowText copies the
					// contents of the entry field
					// into szLocation.
		index=WinQueryLboxSelectedItem(hwndlb);
		WinQueryLboxItemText(hwndlb,index,supertype,MAXNAMELENGTH);
		WinQueryWindowText(hwndef,MAXNAMELENGTH,typename);
		if (strlen(typename)==0) {
			WinMessageBox(HWND_DESKTOP,hwndOwner,
			(PCH) "A type name must be specified",(PCH)"Alert",0,
			MB_NOICON|MB_OK);
			break;
			}
		if (strlen(supertype)>0){
			tp=odb.gettypenamed(supertype);
			if (tp==NULL) {
				WinMessageBox(HWND_DESKTOP,hwndOwner,
				(PCH) "Specified subtype does not exist",(PCH)"Alert",0,
				MB_NOICON|MB_OK);
				break;
				}
			else {
				validate_name(typename);
				if (odb.create_type(typename,(*tp).getname())!=NULL) {
					WinMessageBox(HWND_DESKTOP,hwndOwner,
						(PCH) "Type created",(PCH)"Sucess",0,
						MB_NOICON|MB_OK);
					WinSetWindowText(hwndef,(PSZ)"");
						}
				else {
					WinMessageBox(HWND_DESKTOP,hwndOwner,
						(PCH) "Type not be created",(PCH)"Alert",0,
						MB_NOICON|MB_OK);
					WinSetWindowText(hwndef,(PSZ)"");
					};				//WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
				return (MRESULT) FALSE;
				}
			} //tp==NULL
		else {
			if (odb.create_type(typename)!=NULL){
				WinMessageBox(HWND_DESKTOP,hwndOwner,
						(PCH) "Type created",(PCH)"Sucess",0,
						MB_NOICON|MB_OK);
				WinSetWindowText(hwndef,(PSZ)"");
				}
			else {
					WinMessageBox(HWND_DESKTOP,hwndOwner,
						(PCH) "Type not be created",(PCH)"Alert",0,
						MB_NOICON|MB_OK);
					WinSetWindowText(hwndef,(PSZ)"");
					};			return (MRESULT) FALSE;
			}
	case DID_CANCEL:         	// The Cancel pushbutton or Escape key
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

