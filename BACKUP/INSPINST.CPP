//=================================================================
//
// Project: Store an object oriented storage manager
// Version: Development ver for os2 Warp
// This file: inspectdb dialog for inspecting database
//
//=================================================================

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class InstInspectDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//Dbinspectput dialog
InstInspectDlg::InstInspectDlg(void){}

InstInspectDlg::~InstInspectDlg(void)                 // Destructor
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

InstInspectDlg::InstInspectDlg(HWND hwndParent1,
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
				// InstInspectDlg::DlgProc (pure virtual
				// dispatcher, defined as a macro in dlg.h)
    }

//-----------------------InstInspectDlg::Run()----------------------------------

InstInspectDlg::Run()
{
    WinDlgBox(  hwndParent,     // Place anywhere on desktop
		hwndOwner,      // Owned by frame
		::DlgCallBack,  // Address of dialog procedure
		Resource,       // Module handle
		ulDlgid,        // Dialog identifier in resource
		NULL); 		// Initialization data
   return(0);
}

//-----------------------InstInspectDlg::wmInitDlg()----------------------------

MRESULT InstInspectDlg::wmInitDlg(HM12)
{

       ::SetSysMenu(hwnd);      // remove unused sys. menu items
       ::CenterDialog(hwnd);
       USE_HM12;
       hwndtpname=WinWindowFromID(hwnd,55);
       hwndinspector=WinWindowFromID(hwnd,53);
       populate_types(hwndtpname,odb);
	return FALSE;
}

//-----------------------InstInspectDlg::wmCommand()-----------------------------

MRESULT InstInspectDlg::wmCommand(HM12)
{USE_HM12;
char objectstr[MAXNAMELENGTH]="";
int index,i;
ODB_OID oid;
object *o;
	index=0; oid=0; i=0;
      switch( SHORT1FROMMP( mp1 ) )     // Extract the command value
      {
	case DID_OK:
		index=WinQueryLboxSelectedItem(hwndinspector);
		WinQueryLboxItemText(hwndinspector,index,objectstr,MAXNAMELENGTH);
		if (strlen(objectstr)<1)
			{WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "An object must be selected.",
			(PCH)"Alert",0,MB_NOICON|MB_OK);
			break;
			}
		else {
			i=0;
			while((objectstr[i]!='(')&&(index<MAXNAMELENGTH)) i++;
			objectstr[i]='\0';
			validate_integer(objectstr+5,oid); //[OID:xxxx(
			o=odb.getobjectwithoid("Usertypes",oid);
			if (o!=NULL)
				if(WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Delete object?",
				(PCH)"Sanity Check",0,MB_NOICON|MB_OKCANCEL)==MBID_OK){
					(*o).deleteobject();
					WinDeleteLboxItem(hwndinspector,index);
					};
			}
			break;
	case DID_CANCEL:
		WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
	  return (MRESULT) FALSE;
	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
      }
      return (MRESULT) FALSE;
}

MRESULT InstInspectDlg::wmControl(HM12){
USE_HM12;
USHORT sender,command;
type *tp;
char typename[MAXNAMELENGTH]="";
int index;
	sender=SHORT1FROMMP( mp1 );
	command=SHORT2FROMMP(mp1);
	switch(sender){
	case 55:
		if (command==CBN_EFCHANGE){
			//a type has been selected. Populate propery combobox
			//any values ther must first be removed
			index=WinQueryLboxSelectedItem(hwndtpname); //typename
			WinQueryLboxItemText(hwndtpname,index,typename,MAXNAMELENGTH);
			tp=odb.gettypenamed(typename);
			if (tp==NULL){
				WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH)"Not a type",
				(PCH)"Alert",0,MB_OK);
				break;
				}
			//Populate the listbox with instances. First clear then populate

			clearlbox(hwndinspector);
			populate_inspectinstances(hwndinspector,typename,odb);
		}; //if command
		break;

	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
	} //switch sender
	return (MRESULT) FALSE;
}