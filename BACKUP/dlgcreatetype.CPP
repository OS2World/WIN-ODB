#include "odb95.h"


//---------------------------------------------
// Name       :DlgCreateType
// Description:Default constructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgCreateType::DlgCreateType():DlgModal(){
}


//---------------------------------------------
// Name       :~DlgCreateType
// Description:Default destructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgCreateType::~DlgCreateType(){
}


//---------------------------------------------
// Name       :DlgCreateType
// Description:Constructor
// Arguments  :instance, dlg id, parent window
// Author     :Staffan Flodin
//---------------------------------------------
DlgCreateType::DlgCreateType(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
	DlgModal(hThis,name,parent,mw){
	if (mainW==NULL) {
   	odb=NULL;
      return;
   }
   else
   	(this->odb)=mainW->getDb();
}


//---------------------------------------------
// Name       :callbackfn
// Description:the callback function of the dlg
// Arguments  :std callback fn args
// Author     :Staffan Flodin
//---------------------------------------------
BOOL DlgCreateType::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
int index;
type *tp;
char supertype[MAXNAMELENGTH]="";
char tpnm[MAXNAMELENGTH]="";

   switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
            case DID_OK:
            	index=SendDlgItemMessage(h,114,CB_GETCURSEL,0,0);
            	SendDlgItemMessage(h,114,CB_GETLBTEXT,index,(LPARAM)supertype);
					GetDlgItemText(h,112,tpnm,MAXNAMELENGTH);

					if (strlen(tpnm)==0) {
						MessageBox(h,WINTITLE,"A type name must be specified",MB_OK);
						break;
            	}
					if (strlen(supertype)>0){
						tp=odb->gettypenamed(supertype);
						if (tp==NULL) {
               		MessageBox(h,WINTITLE,"Specified subtype does not exist",MB_OK);
							break;
               	}
						else {
							odb->validate(tpnm);
							if (odb->create_type(tpnm,(*tp).getname())!=NULL) {
                  		EndDialog(h,0);
								return 1;
							}
							else {
								MessageBox(h,WINTITLE,"Type could not be created",MB_OK);
								SetDlgItemText(h,112,"");
							};				//WinDismissDlg( hwnd, TRUE );  // Removes the dialog box
							return 1;
							}
						} //tp==NULL
					else {
						if (odb->create_type(tpnm)!=NULL){
							EndDialog(h,0);
							return 1;
						}
						else {
								MessageBox(h,WINTITLE,"Type could not be created",MB_OK);
								SetDlgItemText(h,112,"");
						};
            	return 1;
					}
				}
      case WM_INITDIALOG:
      	if (initialized==0){
      		populateCbWithTypes(h,114);
            preSelectCBwithType(h, 114, presetTypeName);
         	initialized=1;
            }
         return 1;
	}
	return 0;
}
