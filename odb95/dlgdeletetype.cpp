#include "odb95.h"


//---------------------------------------------
// Name       :DlgCreateType
// Description:Default constructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgDeleteType::DlgDeleteType():DlgModal(){
}


//---------------------------------------------
// Name       :~DlgDeleteType
// Description:Default destructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgDeleteType::~DlgDeleteType(){
}


//---------------------------------------------
// Name       :DlgDeleteType
// Description:Constructor
// Arguments  :instance, dlg id, parent window
// Author     :Staffan Flodin
//---------------------------------------------
DlgDeleteType::DlgDeleteType(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
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
BOOL DlgDeleteType::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
int index;
char tpnm[MAXNAMELENGTH]="";

   switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
            case DID_OK:
            	index=SendDlgItemMessage(h,41,CB_GETCURSEL,0,0);
            	SendDlgItemMessage(h,41,CB_GETLBTEXT,index,(LPARAM)tpnm);

					if (strlen(tpnm)>0) {
						if (MessageBox(h,"Delete Type?",MESSAGEALERT,MB_OKCANCEL)==ID_OK){
                  	if(odb->delete_type(tpnm)<1) warn("Type could not be deleted",h);
                  }
               }
               EndDialog(h,0);
					return 1;
               break;
            }

      case WM_INITDIALOG:
      	if (initialized==0){
      		populateCbWithTypes(h,41);
            preSelectCBwithType(h, 41, presetTypeName);
         	initialized=1;
            }
         return 1;
	}
	return 0;
}