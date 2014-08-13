#include "odb95.h"


//---------------------------------------------
// Name       :DlgCreateType
// Description:Default constructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgDeleteProp::DlgDeleteProp():DlgModal(){
}


//---------------------------------------------
// Name       :~DlgDeleteProp
// Description:Default destructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgDeleteProp::~DlgDeleteProp(){
}


//---------------------------------------------
// Name       :DlgDeleteProp
// Description:Constructor
// Arguments  :instance, dlg id, parent window
// Author     :Staffan Flodin
//---------------------------------------------
DlgDeleteProp::DlgDeleteProp(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
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
BOOL DlgDeleteProp::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
int index;
char tpnm[MAXNAMELENGTH]="";
char propnm[MAXNAMELENGTH]="";

   switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
         	case 31:
            	if (HIWORD(wParam)==CBN_SELCHANGE){
               	index=SendDlgItemMessage(h,31,CB_GETCURSEL,0,0);
            		SendDlgItemMessage(h,31,CB_GETLBTEXT,index,(LPARAM)tpnm);
                  clearLbox(h,32);
                  populateLbWithProps(h,32,tpnm);
               };
               break;
				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
            case DID_OK:
            	index=SendDlgItemMessage(h,31,CB_GETCURSEL,0,0);
            	SendDlgItemMessage(h,31,CB_GETLBTEXT,index,(LPARAM)tpnm);

               index=SendDlgItemMessage(h,32,LB_GETCURSEL,0,0);
            	SendDlgItemMessage(h,32,LB_GETTEXT,index,(LPARAM)propnm);
					if ((strlen(tpnm)>0)&&(strlen(propnm)>0)) {
						if (MessageBox(h,"Delete Property?",MESSAGEALERT,MB_OKCANCEL)==ID_OK){
                  	if(odb->delete_property(tpnm,propnm)>0) {
                     	index=SendDlgItemMessage(h,31,CB_GETCURSEL,0,0);
            				SendDlgItemMessage(h,31,CB_GETLBTEXT,index,(LPARAM)tpnm);
                  		clearLbox(h,32);
                  		populateLbWithProps(h,32,tpnm);
                  	}
                     else warn("Failed to delete property",h);
                  };
               }
               else warn("Both type and property must be selected",h);
               break;
            }

      case WM_INITDIALOG:
      	if (initialized==0){
      		populateCbWithTypes(h,31);
         	initialized=1;
            }
         return 1;
	}
	return 0;
}