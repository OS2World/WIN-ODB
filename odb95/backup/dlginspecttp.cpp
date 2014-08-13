#include "d:\users\staffan\odb\odb95\odb95.h"


//---------------------------------------------
// Name       :DlgInspectTp
// Description:Default constructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgInspectTp::DlgInspectTp():DlgModal(){
}


//---------------------------------------------
// Name       :~DlgInspectTp
// Description:Default destructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgInspectTp::~DlgInspectTp(){
}


//---------------------------------------------
// Name       :DlgInspectTp
// Description:Constructor
// Arguments  :instance, dlg id, parent window
// Author     :Staffan Flodin
//---------------------------------------------
DlgInspectTp::DlgInspectTp(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
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
BOOL DlgInspectTp::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
char objectstr[MAXNAMELENGTH]="";
int index,i;
ODB_OID oid;

	index=0; oid=0; i=0;

   switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
         	case 55:
            	if (HIWORD(wParam)==CBN_SELCHANGE){
               	index=SendDlgItemMessage(h,55,CB_GETCURSEL,0,0);
            		SendDlgItemMessage(h,55,CB_GETLBTEXT,index,(LPARAM)objectstr);
                  clearLbox(h,64);
                  clearLbox(h,66);
                  populateLbWithSubTypes(h,64,objectstr);
                  populateLbWithProps(h,66,objectstr);
                  SetDlgItemText(h,62,odb->getsupertype(objectstr));
               };
               break;
				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
            
            }

      case WM_INITDIALOG:
      	if (initialized==0){
      		populateCbWithTypes(h,55);
            preSelectCBwithType(h, 55, presetTypeName);
         	initialized=1;
            }
         return 1;
	}
	return 0;
}