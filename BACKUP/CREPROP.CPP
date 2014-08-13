//=================================================================
//
// Project: Store an object oriented storage manager
// Version: Development ver for os2 Warp
// This file: creprop.cpp dialog box for creating properties
//
//=================================================================
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class CrePropDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
CrePropDlg::CrePropDlg(void){}

CrePropDlg::~CrePropDlg(void)                 // Destructor
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

CrePropDlg::CrePropDlg(HWND hwndParent1,
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

//-----------------------CrePropDlg::Run()----------------------------------

CrePropDlg::Run()
{
    WinDlgBox(  hwndParent,     // Place anywhere on desktop
		hwndOwner,      // Owned by frame
		::DlgCallBack,  // Address of dialog procedure
		Resource,       // Module handle
		ulDlgid,        // Dialog identifier in resource
		NULL); 		// Initialization data
   return(0);
}

//-----------------------CrePropDlg::wmInitDlg()----------------------------

MRESULT CrePropDlg::wmInitDlg(HM12){
int i;
char *nm;
	::SetSysMenu(hwnd);      // remove unused sys. menu items
	::CenterDialog(hwnd);
	USE_HM12;
	hwndlb=WinWindowFromID(hwnd ,ID_DATATYPE);
	i=0;
	WinInsertLboxItem(hwndlb,i,(PSZ)_INT_NM_);
	i++;
	WinInsertLboxItem(hwndlb,i,(PSZ)_REAL_NM_);
	i++;
	WinInsertLboxItem(hwndlb,i,(PSZ)_CHAR_NM_);
	i++;
	WinInsertLboxItem(hwndlb,i,(PSZ)_OTHER_NM_);
	i++;
	WinInsertLboxItem(hwndlb,i,(PSZ)_INT_COLLECTION_NM_);
	i++;
	WinInsertLboxItem(hwndlb,i,(PSZ)_REAL_COLLECTION_NM_);
	i++;
	WinInsertLboxItem(hwndlb,i,(PSZ)_CHAR_COLLECTION_NM_);
	i++;
	WinInsertLboxItem(hwndlb,i,(PSZ)_COLLECTION_NM_);
	hwndnewprop=WinWindowFromID(hwnd ,ID_NEWPROP);
	hwndtypename=WinWindowFromID(hwnd ,ID_TYPENAME);
	nm=odb.getallsubtypes();
	nm=odb.getnextsubtype(); //Usertypes not in listbox
	i=0;
	while (nm!=NULL){
		WinInsertLboxItem(hwndtypename,i,(PSZ)nm);
		i++;
		nm=odb.getnextsubtype();
	};
	return FALSE;
}

//-----------------------CrePropDlg::wmCommand()-----------------------------

MRESULT CrePropDlg::wmCommand(HM12){
char datatype[MAXNAMELENGTH]="";
char typename[MAXNAMELENGTH]="";
char propname[MAXNAMELENGTH]="";
int index;
int indexcheck,indexcre;
USE_HM12;
      switch( SHORT1FROMMP( mp1 ) )
      {
	case DID_OK:
		index=WinQueryLboxSelectedItem(hwndlb); //datatype
		indexcheck=WinQueryButtonCheckstate(hwnd,122);
		WinQueryLboxItemText(hwndlb,index,datatype,MAXNAMELENGTH);
		index=WinQueryLboxSelectedItem(hwndtypename); //typename
		WinQueryLboxItemText(hwndtypename,index,typename,MAXNAMELENGTH);
		WinQueryWindowText(hwndnewprop,MAXNAMELENGTH,propname);
		if (strlen(datatype)==0) {
			WinMessageBox(HWND_DESKTOP,hwndOwner,
			(PCH) "A datatype must be specified",(PCH)"Alert",0,
			MB_NOICON|MB_OK);
			break;
			}
		if (strlen(typename)==0){
			WinMessageBox(HWND_DESKTOP,hwndOwner,
			(PCH) "A typename must be selected",(PCH)"Alert",0,
			MB_NOICON|MB_OK);
			break;
			}
		if (strlen(propname)==0){
			WinMessageBox(HWND_DESKTOP,hwndOwner,
			(PCH) "A property name must be specified",(PCH)"Alert",0,
			MB_NOICON|MB_OK);
			break;
			}
		if ((strlen(propname)>0)&&(strlen(typename)>0)&&(strlen(datatype)>0)){
			validate_name(propname);
			if (strcmp(datatype,_INT_COLLECTION_NM_)==0)
				index=odb.add_property(typename,propname,_INT_COLLECTION_);
			else if (strcmp(datatype,_REAL_COLLECTION_NM_)==0)
				index=odb.add_property(typename,propname,_REAL_COLLECTION_);
			else if (strcmp(datatype,_CHAR_COLLECTION_NM_)==0)
				index=odb.add_property(typename,propname,_CHAR_COLLECTION_);
			else if (strcmp(datatype,_COLLECTION_NM_)==0)
				index=odb.add_property(typename,propname,_COLLECTION_);
			if (strcmp(datatype,_INT_NM_)==0)
				index=odb.add_property(typename,propname,_INT_);
			else if (strcmp(datatype,_REAL_NM_)==0)
				index=odb.add_property(typename,propname,_REAL_);
			else if (strcmp(datatype,_CHAR_NM_)==0)
				index=odb.add_property(typename,propname,_CHAR_);
			else if (strcmp(datatype,_OTHER_NM_)==0)
				index=odb.add_property(typename,propname,_OTHER_);
			if (index>0){
				if (indexcheck==CHECKED) indexcre=odb.create_index(typename,propname);
				else indexcre=1;
				if (indexcre>0)
					WinMessageBox(HWND_DESKTOP,hwndOwner,
					(PCH) "Property created",(PCH)"Success",0,MB_NOICON|MB_OK);
				else
        	WinMessageBox(HWND_DESKTOP,hwndOwner,
					(PCH) "Property created. Index creation failed",(PCH)"Success",0,MB_NOICON|MB_OK);				}
			else
				WinMessageBox(HWND_DESKTOP,hwndOwner,
				(PCH) "Property could not be created",(PCH)"Fail",0,MB_NOICON|MB_OK);
			//WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
			return (MRESULT) FALSE;
			}
		break;
	case DID_CANCEL:
	  if(WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Quit this window?",
			(PCH)"Sanity Check",0,
			MB_NOICON|MB_OKCANCEL)==MBID_OK)
		WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
	  return (MRESULT) FALSE;
	default:
	  return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
      }
      return (MRESULT) FALSE;
}

