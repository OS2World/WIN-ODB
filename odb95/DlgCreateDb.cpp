#include "odb95.h"

DlgCreateDb::DlgCreateDb():DlgModal(){
}

DlgCreateDb::~DlgCreateDb(){
}

DlgCreateDb::DlgCreateDb(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
	DlgModal(hThis,name,parent,mw){
	if (mainW==NULL) {
   	odb=NULL;
      return;
   }
   else
   	(this->odb)=mainW->getDb();
}

BOOL DlgCreateDb::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
char dbname[MAXNAMELENGTH]="";
char strsize[MAXNAMELENGTH]="";
ODB_INT size=0;
	switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
				case DID_OK:
					GetDlgItemText(h,DF_NEWDBNAME,dbname,MAXNAMELENGTH);
					GetDlgItemText(h,DF_NEWDBSIZE,strsize,MAXNAMELENGTH);
					if ((strlen(dbname)<1)||(strlen(strsize)<1)){
						MessageBox(h,"Both name and size must be defined",
									MESSAGEALERT,MB_OK);
						break;
					}
					else {
						if (odb->validate(strsize,size)==0){
							MessageBox(h,"Size not valid", MESSAGEALERT,MB_OK);
							break;
							};
						odb->initialize();
						odb->validate(dbname);
						odb->set_name(dbname);
						odb->set_tablesize(size);
                  EndDialog(h,0);
						return 1;
						}
					}
	}
	return 0;
}

