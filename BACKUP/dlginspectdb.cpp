#include "odb95.h"
#include <stdio.h>

DlgInspectDb::DlgInspectDb():DlgModal(){
}

DlgInspectDb::~DlgInspectDb(){
}

DlgInspectDb::DlgInspectDb(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
	DlgModal(hThis,name,parent,mw){
	if (mainW==NULL) {
   	odb=NULL;
      return;
   }
   else
   	(this->odb)=mainW->getDb();
}

BOOL DlgInspectDb::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
char dbname[MAXNAMELENGTH]="";
char strsize[MAXNAMELENGTH]="";
int size;

	switch (mess){
   	case WM_INITDIALOG:
      if (odb==NULL) {
      	warn("No database available",h);
         break;
      }
      
      strcpy(dbname,odb->get_dbname());
      SetDlgItemText(h,71,dbname);

      size=odb->get_tablesize();
      sprintf(strsize,"%i",size);
      SetDlgItemText(h,73,strsize);

      populateLbWithTypes(h,74);
      break;
      
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
				case DID_OK:
						break;
         }
	}
	return 0;
}
