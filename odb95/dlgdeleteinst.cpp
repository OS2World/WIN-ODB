#include "odb95.h"


//---------------------------------------------
// Name       :DlgCreateType
// Description:Default constructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgDeleteInst::DlgDeleteInst():DlgModal(){
}


//---------------------------------------------
// Name       :~DlgDeleteInst
// Description:Default destructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgDeleteInst::~DlgDeleteInst(){
}


//---------------------------------------------
// Name       :DlgDeleteInst
// Description:Constructor
// Arguments  :instance, dlg id, parent window
// Author     :Staffan Flodin
//---------------------------------------------
DlgDeleteInst::DlgDeleteInst(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
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
BOOL DlgDeleteInst::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
char objectstr[MAXOBJTEXTLENGTH]="", details[MAXOBJTEXTLENGTH]="";
int index,i;
ODB_OID oid;
object *o;
HWND hH,hh;
	index=0; oid=0; i=0;

   switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
         	case 55:
            	if (HIWORD(wParam)==CBN_SELCHANGE){
               	index=SendDlgItemMessage(h,55,CB_GETCURSEL,0,0);
            		SendDlgItemMessage(h,55,CB_GETLBTEXT,index,(LPARAM)objectstr);
                  clearLbox(h,53);
                  populateLbWithInstances(h,53,objectstr);
               };
               break;
			case 53:
				if (HIWORD(wParam)==LBN_SELCHANGE){
					index=SendDlgItemMessage(h,53,LB_GETCURSEL,0,0);
               SendDlgItemMessage(h,53,LB_GETTEXT,index,(LPARAM)details);

					/*i=0;
					while((objectstr[i]!='(')&&(i<MAXNAMELENGTH)) i++;

					objectstr[i]='\0';
					odb->validate(objectstr+5,oid); //[OID:xxxx(
					o=odb->getobjectwithoid("Usertypes",oid);
				*/
					hh=CreateWindow((LPCTSTR)"STATIC",NULL,
								WS_CHILD | WS_VISIBLE | SS_BLACKFRAME,
								10,200,570,190,
								h,NULL,thisInstance,NULL);
					hH=CreateWindow((LPCTSTR)"STATIC",details,
								WS_CHILD | WS_VISIBLE | SS_LEFT ,
								15,205,540,180,
								h,NULL,thisInstance,NULL);
				}
				break;
			case DID_CANCEL:
					EndDialog(h,0);
					return 1;
         case DID_OK:
            	index=SendDlgItemMessage(h,53,LB_GETCURSEL,0,0);
            	SendDlgItemMessage(h,53,LB_GETTEXT,index,(LPARAM)objectstr);

               i=0;
					while((objectstr[i]!='(')&&(i<MAXNAMELENGTH))
               	i++;

               objectstr[i]='\0';
               odb->validate(objectstr+5,oid); //[OID:xxxx(
               o=odb->getobjectwithoid("Usertypes",oid);

               if ((o!=NULL)&&(MessageBox(h,"Delete Property?",
                  						MESSAGEALERT,MB_OKCANCEL)==ID_OK)){
               	odb->delete_object(o);
                  o=NULL;
                  details[0]='\0';
                  SendDlgItemMessage(h,53,LB_DELETESTRING,index,0);
               }
            	break;
         case 2:
            	index=SendDlgItemMessage(h,53,LB_GETCURSEL,0,0);
            	SendDlgItemMessage(h,53,LB_GETTEXT,index,(LPARAM)objectstr);

               i=0;
					while((objectstr[i]!='(')&&(i<MAXNAMELENGTH))
               	i++;

               objectstr[i]='\0';
               odb->validate(objectstr+5,oid); //[OID:xxxx(
               o=odb->getobjectwithoid("Usertypes",oid);

               if (o!=NULL){
               	selectedObject=o;
               }
			   EndDialog(h,0);
				return 1;
            break;
            }

      case WM_INITDIALOG:
      	if (initialized==0){
      		populateCbWithTypes(h,55);
         	initialized=1;
            }
         return 1;
	}
	return 0;
}