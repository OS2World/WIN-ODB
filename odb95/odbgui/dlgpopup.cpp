#include "odb95.h"

DlgPopup::DlgPopup():DlgModal(){
}

DlgPopup::~DlgPopup(){
}

DlgPopup::DlgPopup(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
	DlgModal(hThis,name,parent,mw){

   region *rg=NULL;

	if (mainW==NULL) {
   	odb=NULL;
      return;
   }
   else {
   	(this->odb)=mainW->getDb();
      rg=mainW->getCurrentRegion();
      }
}

BOOL DlgPopup::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
	hWndThis=h;
	switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
				case 103:

            	INVOKEPRESETDIALOG(DLG_CRETP,DlgCreateType)
				mainW->setDlg(this);
         		InvalidateRect( h, NULL, TRUE );
         		EndDialog(h,0);
					return 1;
            case 104:
            	INVOKEPRESETDIALOG(DLG_CREINST,DlgCreateInst)
				mainW->setDlg(this);
         		InvalidateRect( h, NULL, TRUE );
         		EndDialog(h,0);
					return 1;
            case 105:
				INVOKEPRESETDIALOG(DLG_CREPROP,DlgCreateProp)
				mainW->setDlg(this);
         		InvalidateRect( h, NULL, TRUE );
         		EndDialog(h,0);
				return 1;
            case 101:
            	INVOKEPRESETDIALOG(DLG_INSPECTTYPE,DlgInspectTp)
               mainW->setDlg(this);
         		InvalidateRect( h, NULL, TRUE );
         		EndDialog(h,0);
				return 1;
            case 102:
            	INVOKEPRESETDIALOG(DLG_DELETETYPE,DlgDeleteType)
				mainW->setDlg(this);
         		InvalidateRect( h, NULL, TRUE );
         		EndDialog(h,0);
				return 1;
			}
	}
	return 0;
}

//---------------------------------------------
// Name       :getDlg
// Description:Returns the currently active
//             modal dialog
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgModal *DlgPopup::getDlg(void){
	return currentDlg;
}

//---------------------------------------------
// Name       :getDb
// Description:Returns the database in use
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
//database *DlgPopup::getDb(void){
//	return odb;
//}
