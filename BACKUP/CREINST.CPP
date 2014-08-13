//=================================================================
//
// Project: Store an object oriented storage manager
// Version: Development ver for os2 Warp
// This file: creinst.cpp dialog box for creating instances
//
//=================================================================
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//             Implementation of class CreInstDlg
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
CreInstDlg::CreInstDlg(void){}

CreInstDlg::~CreInstDlg(void)
{
	initialized = FALSE;
	CallBack::dlgThis = NULL;
}

CreInstDlg::CreInstDlg(HWND hwndParent1,
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

//-----------------------CreInstDlg::Run()----------------------------------

CreInstDlg::Run()
{
	WinDlgBox( hwndParent,hwndOwner,::DlgCallBack,Resource,ulDlgid,NULL);
	// Initialization data
   return(0);
}

//-----------------------CreInstDlg::wmInitDlg()----------------------------

MRESULT CreInstDlg::wmInitDlg(HM12){

	::SetSysMenu(hwnd);
	::CenterDialog(hwnd);
	USE_HM12;
	noassignedprops=0; //counts no of assigned props
	//assign window handles from their resource id's
	hwndlbtypes=WinWindowFromID(hwnd ,CMB_CREINSTTYPE);
	hwndlbprops=WinWindowFromID(hwnd,LB_TYPEPROPS);
	hwnddatatype=WinWindowFromID(hwnd,DF_DATATYPE);
	hwndnewvalue=WinWindowFromID(hwnd ,DF_NEWVALUE);
	hwndpropval=WinWindowFromID(hwnd ,LB_PROPVALS);
	hwndassignedprop=WinWindowFromID(hwnd ,LB_ASSPROP);
	hwndviewval=WinWindowFromID(hwnd ,DF_VIEWVAL);
	//populate cmb box with all types in db
	populate_types(hwndlbtypes,odb);
	return FALSE;
}

//-----------------------CreInstDlg::wmCommand()-----------------------------

MRESULT CreInstDlg::wmCommand(HM12){
USE_HM12;
USHORT command;
char typename[MAXNAMELENGTH]="";
char propname[MAXNAMELENGTH]="";
char newvalue[MAXNAMELENGTH]="";
int index,dbtype,oktype,i;
ODB_INT intval;
ODB_REAL realval;
ODB_SET coll;
object *obj,*o;
command=SHORT1FROMMP( mp1 ) ;
	switch(command)
	{
	case DID_OK:  //ok button pressed. Create object
		index=WinQueryLboxSelectedItem(hwndlbtypes);
		WinQueryLboxItemText(hwndlbtypes,index,typename,MAXNAMELENGTH);
		if (strlen(typename)<1) {
			//type not specified
			WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "A type must be spec.",
			(PCH)"Alert",0,MB_NOICON|MB_OK);
			break;
			}
		else {
			obj=odb.create_instance(typename);
			if (obj==NULL) {
				//object could not be created, possibly because
				//extentless type selectd
				WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Could not create object",
				(PCH)"Alert",0,MB_NOICON|MB_OK);
				break;
			};
			i=WinQueryLboxCount(hwndassignedprop);
			i--;
			while(i>=0){
				//check that the data for each prop is ok
				//and set the properies for the object
				WinQueryLboxItemText(hwndassignedprop,i,propname,MAXNAMELENGTH);
				WinQueryLboxItemText(hwndassignedprop,i,propname,MAXNAMELENGTH);
				WinQueryLboxItemText(hwndpropval,i,newvalue,MAXNAMELENGTH);
				dbtype=odb.getpropertytype(typename,propname);

				if (dbtype==_INT_) {
					validate_integer(newvalue,intval);
					(*obj).setproperty_value(propname,intval);
					};
				if (dbtype==_OTHER_) {
					validate_integer(newvalue,intval);
					o=odb.getobjectwithoid("Usertypes",intval);
					(*obj).setproperty_value(propname,o);
					};
				if (dbtype==_REAL_) {
					validate_real(newvalue,realval);
					(*obj).setproperty_value(propname,realval);
					};
				if (dbtype==_COLLECTION_){
					coll=new collection(_OTHER_);
					if (validate_set(newvalue,&odb,coll)<0) (*coll).~collection();
					(*obj).setproperty_value(propname,coll);
					};
				if (dbtype==_INT_COLLECTION_){
					coll=new collection(_INT_);
					if (validate_set(newvalue,&odb,coll)<0) (*coll).~collection();
					(*obj).setproperty_value(propname,coll);
					};
				if (dbtype==_REAL_COLLECTION_){
					coll=new collection(_REAL_);
					if (validate_set(newvalue,&odb,coll)<0) (*coll).~collection();
					(*obj).setproperty_value(propname,coll);
					};
				if (dbtype==_CHAR_COLLECTION_){
					coll=new collection(_CHAR_);
					if (validate_set(newvalue,&odb,coll)<0) (*coll).~collection();
					(*obj).setproperty_value(propname,coll);
					};
				if (dbtype==_CHAR_) (*obj).setproperty_value(propname,newvalue);
				i--;
				}; //end while all properties set.
				//deleta all unassigned praps
			(*this).clearlbox(hwndassignedprop);
			clearlbox(hwndpropval);
			clearlbox(hwndlbprops);
			WinSetWindowText(hwndviewval,(PSZ)"");
			populate_props(hwndlbprops,typename,  odb);
			noassignedprops=0; //no assigned properties anymore
			WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Instance created.",
			(PCH)"Success",0,MB_NOICON|MB_OK);
		}; //else		//create the object and set properties
		return (MRESULT) FALSE;
		//break;
	case DID_CANCEL:  //quit this window
	  if(WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Quit this window?",
			(PCH)"Sanity Check",0,
			MB_NOICON|MB_OKCANCEL)==MBID_OK)
		WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
	  return (MRESULT) FALSE;
	case PB_SET:
		//assign property a value. The prop is moved to assignedprops
		//and value is validated and moved to hidden listbox
		index=WinQueryLboxSelectedItem(hwndlbtypes); //typename
		WinQueryLboxItemText(hwndlbtypes,index,typename,MAXNAMELENGTH);
		index=WinQueryLboxSelectedItem(hwndlbprops); //propname
		WinQueryLboxItemText(hwndlbprops,index,propname,MAXNAMELENGTH);
		WinQueryWindowText(hwndnewvalue,MAXNAMELENGTH,newvalue);
		dbtype=odb.getpropertytype(typename,propname);
		if (dbtype==_INT_)
			oktype=validate_integer(newvalue,intval);
		if (dbtype==_OTHER_){
			oktype=validate_integer(newvalue,intval);
			if (oktype!=0) {
				//check that there is an object with the oid
				obj=odb.getobjectwithoid("Usertypes",intval);
				if (obj==NULL) oktype=0;
				}
			};
		if (dbtype==_REAL_)
			oktype=validate_real(newvalue,realval);
		if (dbtype==_CHAR_) oktype=1;
		if ((strlen(newvalue)>0)&&(strlen(propname)>0)&&(oktype!=0)){
			//move assigned prop from prop cmb box to assigned props
			WinInsertLboxItem(hwndassignedprop,noassignedprops,(PSZ)propname);
			WinInsertLboxItem(hwndpropval,noassignedprops,(PSZ)newvalue);
			noassignedprops++;
			WinDeleteLboxItem(hwndlbprops,index);
			WinSetWindowText(hwnddatatype,(PSZ)"");
			WinSetWindowText(hwndnewvalue,(PSZ)"");
			}
		else
		WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH) "Missing or erroneous data",
			(PCH)"Alert",0,MB_OKCANCEL);
		break;
	default:
	  return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
      }
      return (MRESULT) FALSE;
}

MRESULT CreInstDlg::wmControl(HM12){
USE_HM12;
USHORT sender,command;
type *tp;
char typename[MAXNAMELENGTH]="";
char propname[MAXNAMELENGTH]="";
char newvalue[MAXNAMELENGTH]="";
int index;
	sender=SHORT1FROMMP( mp1 );
	command=SHORT2FROMMP(mp1);
	switch(sender){
	case CMB_CREINSTTYPE:
		if (command==CBN_EFCHANGE){
			//a type has been selected. Populate propery combobox
			//any values ther must first be removed
			index=WinQueryLboxSelectedItem(hwndlbtypes); //typename
			WinQueryLboxItemText(hwndlbtypes,index,typename,MAXNAMELENGTH);
			WinSetWindowText(hwnddatatype,(PSZ)"");
			tp=odb.gettypenamed(typename);
			if (tp==NULL){
				WinMessageBox(HWND_DESKTOP,hwndOwner,(PCH)"Not a type",
				(PCH)"Alert",0,MB_OK);
				break;
				}
			//Populate the listbox with props. First clear then populate
			clearlbox(hwndlbprops);
			clearlbox(hwndassignedprop);
			noassignedprops=0; //no assigned properties anymore
			populate_props(hwndlbprops,typename, odb);
		}; //if command
		break;
	case LB_ASSPROP: //a prop has been selected, show its data type
		if (command==LN_SELECT){
			index=WinQueryLboxSelectedItem(hwndassignedprop); //typename
			WinQueryLboxItemText(hwndassignedprop,index,propname,MAXNAMELENGTH);
			WinQueryLboxItemText(hwndpropval,index,typename,MAXNAMELENGTH);
			WinSetWindowText(hwndviewval,(PSZ)typename);
			};
		break; //set 96 to newvalue
	case LB_TYPEPROPS:
		if (command==LN_SELECT){
			index=WinQueryLboxSelectedItem(hwndlbprops); //typename
			WinQueryLboxItemText(hwndlbprops,index,propname,MAXNAMELENGTH);
			index=WinQueryLboxSelectedItem(hwndlbtypes); //typename
			WinQueryLboxItemText(hwndlbtypes,index,typename,MAXNAMELENGTH);
			index=odb.getpropertytype(typename,propname);
			WinQueryWindowText(hwndnewvalue,MAXNAMELENGTH,newvalue);
			if (index==_INT_)
				WinSetWindowText(hwnddatatype,(PSZ)_INT_NM_);
			else if (index==_REAL_)
				WinSetWindowText(hwnddatatype,(PSZ)_REAL_NM_);
			else if(index==_CHAR_)
				WinSetWindowText(hwnddatatype,(PSZ)_CHAR_NM_);
			else if(index==_OTHER_)
				WinSetWindowText(hwnddatatype,(PSZ)_OTHER_NM_);
			else if(index==_COLLECTION_) {
				WinSetWindowText(hwnddatatype,(PSZ)_COLLECTION_NM_);
				if (strlen(newvalue)==0) WinSetWindowText(hwndnewvalue,(PSZ)"{}");
				}
			else if(index==_INT_COLLECTION_) {
				WinSetWindowText(hwnddatatype,(PSZ)_INT_COLLECTION_NM_);
				if (strlen(newvalue)==0) WinSetWindowText(hwndnewvalue,(PSZ)"{}");
				}			else if(index==_REAL_COLLECTION_) {
				WinSetWindowText(hwnddatatype,(PSZ)_REAL_COLLECTION_NM_);
				if (strlen(newvalue)==0) WinSetWindowText(hwndnewvalue,(PSZ)"{}");
				}			else if(index==_CHAR_COLLECTION_) {
				WinSetWindowText(hwnddatatype,(PSZ)_CHAR_COLLECTION_NM_);
				if (strlen(newvalue)==0) WinSetWindowText(hwndnewvalue,(PSZ)"{}");
				}
			}		break; //LB_TYPEPROPS
	default: return  WinDefDlgProc( hwnd, msg, mp1, mp2 );
	} //switch sender
	return (MRESULT) FALSE;
}

