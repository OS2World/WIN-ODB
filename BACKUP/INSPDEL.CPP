//=================================================================
//
// Project: Store an object oriented storage manager
// Version: Development ver for os2 Warp
// This file: inspectdb dialog for inspecting database
//
//=================================================================

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class TpDeleteDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//Dbinspectput dialog
TpDeleteDlg::TpDeleteDlg(void){}

TpDeleteDlg::~TpDeleteDlg(void)                 // Destructor
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

TpDeleteDlg::TpDeleteDlg(HWND hwndParent1,
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
				// TpDeleteDlg::DlgProc (pure virtual
				// dispatcher, defined as a macro in dlg.h)
    }

//-----------------------TpDeleteDlg::Run()----------------------------------

TpDeleteDlg::Run()
{
    WinDlgBox(  hwndParent,     // Place anywhere on desktop
		hwndOwner,      // Owned by frame
		::DlgCallBack,  // Address of dialog procedure
		Resource,       // Module handle
		ulDlgid,        // Dialog identifier in resource
		NULL); 		// Initialization data
   return(0);
}

//-----------------------TpDeleteDlg::wmInitDlg()----------------------------

MRESULT TpDeleteDlg::wmInitDlg(HM12)
{
       ::SetSysMenu(hwnd);      // remove unused sys. menu items
       ::CenterDialog(hwnd);
       USE_HM12;
       hwndtypelist=WinWindowFromID(hwnd ,41);
       populate_types(hwndtypelist,odb);
	return FALSE;
}

//-----------------------TpDeleteDlg::wmCommand()-----------------------------

MRESULT TpDeleteDlg::wmCommand(HM12)
{USE_HM12;
int index;
char typename[MAXNAMELENGTH]="";
      switch( SHORT1FROMMP( mp1 ) )     // Extract the command value
      {
	case DID_CANCEL:
		WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
	  return (MRESULT) FALSE;
	case DID_OK:
		index=WinQueryLboxSelectedItem(hwndtypelist); //typename
		WinQueryLboxItemText(hwndtypelist,index,typename,MAXNAMELENGTH);
		index=strlen(typename);
		if (index>0) {
			if(WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH)"Delete Type?",
				(PCH)"Alert",0,MB_OKCANCEL)==MBID_OK){
				odb.delete_type(typename);
				WinDismissDlg( hwnd, TRUE );
				};
			break;
			}
		else WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH)"Select a type",
				(PCH)"Alert",0,MB_OK);
		break;
	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
	}
      return (MRESULT) FALSE;
}



//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class PropDeleteDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//Dbinspectput dialog
PropDeleteDlg::PropDeleteDlg(void){}

PropDeleteDlg::~PropDeleteDlg(void)                 // Destructor
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

PropDeleteDlg::PropDeleteDlg(HWND hwndParent1,
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
				// PropDeleteDlg::DlgProc (pure virtual
				// dispatcher, defined as a macro in dlg.h)
    }

//-----------------------PropDeleteDlg::Run()----------------------------------

PropDeleteDlg::Run()
{
    WinDlgBox(  hwndParent,     // Place anywhere on desktop
		hwndOwner,      // Owned by frame
		::DlgCallBack,  // Address of dialog procedure
		Resource,       // Module handle
		ulDlgid,        // Dialog identifier in resource
		NULL); 		// Initialization data
   return(0);
}

//-----------------------PropDeleteDlg::wmInitDlg()----------------------------

MRESULT PropDeleteDlg::wmInitDlg(HM12)
{
       ::SetSysMenu(hwnd);      // remove unused sys. menu items
       ::CenterDialog(hwnd);
       USE_HM12;
       hwndtypelist=WinWindowFromID(hwnd ,31);
       populate_types(hwndtypelist,odb);
       hwndproplist=WinWindowFromID(hwnd ,32);
       return FALSE;
}

//-----------------------PropDeleteDlg::wmCommand()-----------------------------

MRESULT PropDeleteDlg::wmCommand(HM12)
{USE_HM12;
int index,index1;
char typename[MAXNAMELENGTH]="";
char propname[MAXNAMELENGTH]="";
      switch( SHORT1FROMMP( mp1 ) )     // Extract the command value
      {
	case DID_CANCEL:
		WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
	  return (MRESULT) FALSE;
	case DID_OK:
		index=WinQueryLboxSelectedItem(hwndtypelist); //typename
		WinQueryLboxItemText(hwndtypelist,index,typename,MAXNAMELENGTH);
		index=WinQueryLboxSelectedItem(hwndproplist); //typename
		WinQueryLboxItemText(hwndproplist,index,propname,MAXNAMELENGTH);		index=strlen(typename);
		index1=strlen(propname);
		if ((index1>0)&&(index>0)) {
			if(WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH)"Delete Property?",
				(PCH)"Alert",0,MB_OKCANCEL)==MBID_OK){
				odb.delete_property(typename,propname);
				clearlbox(hwndproplist);
				populate_props(hwndproplist,typename, odb);				//WinDismissDlg( hwnd, TRUE );
				};
			break;
			}
		else WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH)"Select a type",
				(PCH)"Alert",0,MB_OK);
		break;
	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
	}
	return (MRESULT) FALSE;
}

MRESULT PropDeleteDlg::wmControl(HM12){
USE_HM12;
USHORT sender,command;
char typename[MAXNAMELENGTH]="";
int index;
	sender=SHORT1FROMMP( mp1 );
	command=SHORT2FROMMP(mp1);
	switch(sender){
	case 31:
		if (command==CBN_EFCHANGE){
			//a type has been selected. Populate propery combobox
			//any values ther must first be removed
			index=WinQueryLboxSelectedItem(hwndtypelist); //typename
			WinQueryLboxItemText(hwndtypelist,index,typename,MAXNAMELENGTH);
			//Populate the listbox with props. First clear then populate
			clearlbox(hwndproplist);
			populate_props(hwndproplist,typename, odb);
		}; //if command
		break;

	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
	} //switch sender
	return (MRESULT) FALSE;
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class InstUpdtDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//Dbinspectput dialog
InstUpdtDlg::InstUpdtDlg(void){}

InstUpdtDlg::~InstUpdtDlg(void)                 // Destructor
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

InstUpdtDlg::InstUpdtDlg(HWND hwndParent1,
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
				// InstUpdtDlg::DlgProc (pure virtual
				// dispatcher, defined as a macro in dlg.h)
    }

//-----------------------InstUpdtDlg::Run()----------------------------------

InstUpdtDlg::Run()
{
    WinDlgBox(  hwndParent,     // Place anywhere on desktop
		hwndOwner,      // Owned by frame
		::DlgCallBack,  // Address of dialog procedure
		Resource,       // Module handle
		ulDlgid,        // Dialog identifier in resource
		NULL); 		// Initialization data
   return(0);
}

//-----------------------InstUpdtDlg::wmInitDlg()----------------------------

MRESULT InstUpdtDlg::wmInitDlg(HM12)
{

       ::SetSysMenu(hwnd);      // remove unused sys. menu items
       ::CenterDialog(hwnd);
       USE_HM12;
       hwndtpname=WinWindowFromID(hwnd,55);
       hwndinspector=WinWindowFromID(hwnd,53);
       hwndpropnm=WinWindowFromID(hwnd,56);
       hwndnewval=WinWindowFromID(hwnd,58);
       hwnddatatype=WinWindowFromID(hwnd,57);
       populate_types(hwndtpname,odb);
	return FALSE;
}

//-----------------------InstUpdtDlg::wmCommand()-----------------------------

MRESULT InstUpdtDlg::wmCommand(HM12)
{USE_HM12;
char objectstr[MAXNAMELENGTH]="",typename[MAXNAMELENGTH]="",propname[MAXNAMELENGTH]="",newvalue[MAXNAMELENGTH]="";
int index,i,dbtype,res=1;
ODB_INT intval;
ODB_OID oid;
ODB_REAL realval;
ODB_SET coll;
object *o,*ov;
	index=0; oid=0; i=0;
      switch( SHORT1FROMMP( mp1 ) )     // Extract the command value
      {
	case DID_OK:
		index=WinQueryLboxSelectedItem(hwndinspector);
		WinQueryLboxItemText(hwndinspector,index,objectstr,MAXNAMELENGTH);
		index=WinQueryLboxSelectedItem(hwndpropnm);
		WinQueryLboxItemText(hwndpropnm,index,propname,MAXNAMELENGTH);
		if ((strlen(objectstr)<1)&&(strlen(propname)<1)){
			WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "An object must be selected.",
			(PCH)"Alert",0,MB_NOICON|MB_OK);
			break;
			}
		else {
			i=0;
			while((objectstr[i]!='(')&&(index<MAXNAMELENGTH)) i++;
			objectstr[i]='\0';
			validate_integer(objectstr+5,oid); //[OID:xxxx(
			o=odb.getobjectwithoid("Usertypes",oid);
			if (o!=NULL){
				WinQueryWindowText(hwndnewval,MAXNAMELENGTH,newvalue);
				index=WinQueryLboxSelectedItem(hwndpropnm);
				WinQueryLboxItemText(hwndpropnm,index,propname,MAXNAMELENGTH);
				index=WinQueryLboxSelectedItem(hwndtpname); //typename
				WinQueryLboxItemText(hwndtpname,index,typename,MAXNAMELENGTH);
				dbtype=odb.getpropertytype(typename,propname);
				if (dbtype==_INT_) {
					if ((res=validate_integer(newvalue,intval))>0)
						(*o).setproperty_value(propname,intval);
					};
				if (dbtype==_OTHER_) {
					if ((res=validate_integer(newvalue,intval))>0)
						ov=odb.getobjectwithoid("Usertypes",intval);
						if (ov!=NULL) (*o).setproperty_value(propname,ov);
						else res=0;
					};
				if (dbtype==_REAL_) {
					if ((res=validate_real(newvalue,realval))>0)
						(*o).setproperty_value(propname,realval);
					};
				if (dbtype==_COLLECTION_){
					coll=new collection(_OTHER_);
					if (validate_set(newvalue,&odb,coll)<0) (*coll).~collection();
					(*o).setproperty_value(propname,coll);
					};
				if (dbtype==_INT_COLLECTION_){
					coll=new collection(_INT_);
					if (validate_set(newvalue,&odb,coll)<0) (*coll).~collection();
					(*o).setproperty_value(propname,coll);
					};
				if (dbtype==_REAL_COLLECTION_){
					coll=new collection(_REAL_);
					if (validate_set(newvalue,&odb,coll)<0) (*coll).~collection();
					(*o).setproperty_value(propname,coll);
					};
				if (dbtype==_CHAR_COLLECTION_){
					coll=new collection(_CHAR_);
					if (validate_set(newvalue,&odb,coll)<0) (*coll).~collection();
					(*o).setproperty_value(propname,coll);
					};				if (dbtype==_CHAR_)
					(*o).setproperty_value(propname,newvalue);
					} //if o!=NULL
			}
			if (res!=1)
				WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Erroneous value",
				(PCH)"Alert",0,MB_NOICON|MB_OK);
			else {
			       WinSetWindowText(hwndnewval,(PSZ)"");
			       clearlbox(hwndinspector);
				populate_inspectinstances(hwndinspector,typename,odb);
				}
			break;
	case DID_CANCEL:
		WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
	  return (MRESULT) FALSE;
	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
      }
      return (MRESULT) FALSE;
}

MRESULT InstUpdtDlg::wmControl(HM12){
USE_HM12;
USHORT sender,command;
type *tp;
char typename[MAXNAMELENGTH]="",propname[MAXNAMELENGTH]="";
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
			clearlbox(hwndpropnm);
			clearlbox(hwndinspector);
			populate_inspectinstances(hwndinspector,typename,odb);
			populate_props(hwndpropnm,typename, odb);
			WinSetWindowText(hwnddatatype,(PSZ)"");
		}; //if command
		break;
	case 56:
		if (command==CBN_EFCHANGE){
			index=WinQueryLboxSelectedItem(hwndpropnm);
			WinQueryLboxItemText(hwndpropnm,index,propname,MAXNAMELENGTH);
			index=WinQueryLboxSelectedItem(hwndtpname); //typename
			WinQueryLboxItemText(hwndtpname,index,typename,MAXNAMELENGTH);
			index=odb.getpropertytype(typename,propname);
			if (index==_INT_)
				WinSetWindowText(hwnddatatype,(PSZ)_INT_NM_);
			else if (index==_REAL_)
				WinSetWindowText(hwnddatatype,(PSZ)_REAL_NM_);
			else if(index==_CHAR_)
				WinSetWindowText(hwnddatatype,(PSZ)_CHAR_NM_);
			else if(index==_OTHER_)
				WinSetWindowText(hwnddatatype,(PSZ)_OTHER_NM_);
			else if(index==_COLLECTION_)
				WinSetWindowText(hwnddatatype,(PSZ)_COLLECTION_NM_);
			else if(index==_INT_COLLECTION_)
				WinSetWindowText(hwnddatatype,(PSZ)_INT_COLLECTION_NM_);
			else if(index==_REAL_COLLECTION_)
				WinSetWindowText(hwnddatatype,(PSZ)_REAL_COLLECTION_NM_);
			else if(index==_CHAR_COLLECTION_)
				WinSetWindowText(hwnddatatype,(PSZ)_CHAR_COLLECTION_NM_);			}
		break;
	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
	} //switch sender
	return (MRESULT) FALSE;
}