#include "d:\users\staffan\odb\odb95\odb95.h"

DlgCreateProp::DlgCreateProp():DlgModal(){
}

DlgCreateProp::~DlgCreateProp(){
}

DlgCreateProp::DlgCreateProp(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
	DlgModal(hThis,name,parent,mw){
	if (mainW==NULL) {
   	odb=NULL;
      return;
   }
   else
   	(this->odb)=mainW->getDb();
}

BOOL DlgCreateProp::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
char datatype[MAXNAMELENGTH]="";
char tpnm[MAXNAMELENGTH]="";
char propname[MAXNAMELENGTH]="";
int index;
int indexcheck,indexcre;

   switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
            case DID_OK:
					index=SendDlgItemMessage(h,ID_DATATYPE,CB_GETCURSEL,0,0);
               SendDlgItemMessage(h,ID_DATATYPE,CB_GETLBTEXT,index,(LPARAM)datatype);

               indexcheck=IsDlgButtonChecked(h,122);

               index=SendDlgItemMessage(h,ID_TYPENAME,CB_GETCURSEL,0,0);
               SendDlgItemMessage(h,ID_TYPENAME,CB_GETLBTEXT,index,(LPARAM)tpnm);

               GetDlgItemText(h,ID_NEWPROP,propname,MAXNAMELENGTH);

         		if (strlen(datatype)==0) {
						warn("A datatype must be specified",h);
						break;
					}
					if (strlen(tpnm)==0){
						warn("A typename must be selected",h);
						break;
					}
					if (strlen(propname)==0){
						warn("A property name must be specified",h);
						break;
					}
					if ((strlen(propname)>0)&&(strlen(tpnm)>0)&&
               	 (strlen(datatype)>0)){
               	odb->validate(propname);
						if (strcmp(datatype,_INT_COLLECTION_NM_)==0)
							index=odb->add_property(tpnm,propname,_INT_COLLECTION_);
						else if (strcmp(datatype,_REAL_COLLECTION_NM_)==0)
							index=odb->add_property(tpnm,propname,_REAL_COLLECTION_);
						else if (strcmp(datatype,_CHAR_COLLECTION_NM_)==0)
							index=odb->add_property(tpnm,propname,_CHAR_COLLECTION_);
						else if (strcmp(datatype,_COLLECTION_NM_)==0)
							index=odb->add_property(tpnm,propname,_COLLECTION_);
						if (strcmp(datatype,_INT_NM_)==0)
							index=odb->add_property(tpnm,propname,_INT_);
						else if (strcmp(datatype,_REAL_NM_)==0)
							index=odb->add_property(tpnm,propname,_REAL_);
						else if (strcmp(datatype,_CHAR_NM_)==0)
							index=odb->add_property(tpnm,propname,_CHAR_);
						else if (strcmp(datatype,_OBJECT_NM_)==0)
							index=odb->add_property(tpnm,propname,_OBJECT_);
						if (index>0){
							if (indexcheck==1)
                     	indexcre=odb->create_index(tpnm,propname);
							else indexcre=1;
						if (indexcre>0)
						warn("Property created",h);
						else
        					warn("Property created. Index creation failed",h);
               }
					else
					warn("Property could not be created",h);
					//WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
					return 1;
					}
            break;
				}
      case WM_INITDIALOG:
      	if (initialized==0){
         	populateCbWithLiterals(h,ID_DATATYPE);
            populateCbWithTypes(h,ID_TYPENAME);
            preSelectCBwithType(h, ID_TYPENAME, presetTypeName);
         	initialized=1;
            }
         return 1;
	}
	return 0;
}