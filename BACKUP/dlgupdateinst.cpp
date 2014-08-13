#include "odb95.h"


//---------------------------------------------
// Name       :DlgUpdateInst
// Description:Default constructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgUpdateInst::DlgUpdateInst():DlgModal(){
}


//---------------------------------------------
// Name       :~DlgUpdateInst
// Description:Default destructor
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
DlgUpdateInst::~DlgUpdateInst(){
}


//---------------------------------------------
// Name       :DlgUpdateInst
// Description:Constructor
// Arguments  :instance, dlg id, parent window
// Author     :Staffan Flodin
//---------------------------------------------
DlgUpdateInst::DlgUpdateInst(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
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
BOOL DlgUpdateInst::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
char objectstr[MAXNAMELENGTH]="";
char propnm[MAXNAMELENGTH]="";
char newval[MAXNAMELENGTH]="";
int index,i,dbtype,res=1;
ODB_INT intval;
ODB_REAL realval;
ODB_SET coll;
ODB_OID oid;
object *o,*ov;
	index=0; oid=0; i=0;

   switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
         	case 58:
            	if (HIWORD(wParam)==UPDATE){


				}
            break;

         	case 55:
            	if (HIWORD(wParam)==CBN_SELCHANGE){
               	index=SendDlgItemMessage(h,55,CB_GETCURSEL,0,0);
            		SendDlgItemMessage(h,55,CB_GETLBTEXT,index,(LPARAM)objectstr);

                  clearLbox(h,53);
                  clearCbox(h,65);

                  populateLbWithInstances(h,53,objectstr);
                  populateCbWithProps(h,65,objectstr);

                  index=SendDlgItemMessage(h,65,CB_GETCURSEL,0,0);
            		SendDlgItemMessage(h,65,CB_GETLBTEXT,index,(LPARAM)propnm);

               	writeLiteralToDf(h,57,odb->getpropertytype(objectstr,propnm));
               };
               break;
            case 65:
            	if (HIWORD(wParam)==CBN_SELCHANGE){
               	index=SendDlgItemMessage(h,55,CB_GETCURSEL,0,0);
            		SendDlgItemMessage(h,55,CB_GETLBTEXT,index,(LPARAM)objectstr);

                  index=SendDlgItemMessage(h,65,CB_GETCURSEL,0,0);
            		SendDlgItemMessage(h,65,CB_GETLBTEXT,index,(LPARAM)propnm);

               	writeLiteralToDf(h,57,odb->getpropertytype(objectstr,propnm));

                  //SendDlgItemMessage(h,58,UPDATE,0,0);
                  //
                  // Get the object to populate old value
                  //
                  index=SendDlgItemMessage(h,55,CB_GETCURSEL,0,0);
                  SendDlgItemMessage(h,55,CB_GETLBTEXT,index,(LPARAM)propnm);

                  index=SendDlgItemMessage(h,53,LB_GETCURSEL,0,0);
                  SendDlgItemMessage(h,53,LB_GETTEXT,index,(LPARAM)objectstr);
                  i=0;
						while((objectstr[i]!='(')&&(i<MAXNAMELENGTH))
               	i++;

               	objectstr[i]='\0';
               	odb->validate(objectstr+5,oid); //[OID:xxxx(
                  o=odb->getobjectwithoid(propnm,oid);

                  if (o!=NULL)
                  {
                  	index=SendDlgItemMessage(h,65,CB_GETCURSEL,0,0);
                     SendDlgItemMessage(h,65,CB_GETLBTEXT,index,(LPARAM)propnm);
                     writePropToDf(h,58,o->getpropertynamed(propnm));
                  }

               };
               break;
            case 53:
            	if (HIWORD(wParam)==LBN_SELCHANGE){
               	//
                  // Get the object to populate old value
                  //
                  index=SendDlgItemMessage(h,55,CB_GETCURSEL,0,0);
                  SendDlgItemMessage(h,55,CB_GETLBTEXT,index,(LPARAM)propnm);

                  index=SendDlgItemMessage(h,53,LB_GETCURSEL,0,0);
                  SendDlgItemMessage(h,53,LB_GETTEXT,index,(LPARAM)objectstr);
                  i=0;
						while((objectstr[i]!='(')&&(i<MAXNAMELENGTH))
               	i++;

               	objectstr[i]='\0';
               	odb->validate(objectstr+5,oid); //[OID:xxxx(
                  o=odb->getobjectwithoid(propnm,oid);

                  if (o!=NULL)
                  {
                  	index=SendDlgItemMessage(h,65,CB_GETCURSEL,0,0);
                     SendDlgItemMessage(h,65,CB_GETLBTEXT,index,(LPARAM)propnm);
                     writePropToDf(h,58,o->getpropertynamed(propnm));
                  }
            		//SendDlgItemMessage(h,58,UPDATE,0,0);
               }
               break;
            case DID_OK:
            	index=SendDlgItemMessage(h,53,LB_GETCURSEL,0,0);
            	SendDlgItemMessage(h,53,LB_GETTEXT,index,(LPARAM)objectstr);

            	index=SendDlgItemMessage(h,65,CB_GETCURSEL,0,0);
               SendDlgItemMessage(h,65,CB_GETLBTEXT,index,(LPARAM)propnm);

               i=0;
					while((objectstr[i]!='(')&&(i<MAXNAMELENGTH))
               	i++;

               objectstr[i]='\0';
               odb->validate(objectstr+5,oid); //[OID:xxxx(
               o=odb->getobjectwithoid("Usertypes",oid);

               GetDlgItemText(h,58,newval,MAXNAMELENGTH);
               if ((newval[0]!='\0')&&(o!=NULL)&&(MessageBox(h,"Update object?",
                  						MESSAGEALERT,MB_OKCANCEL)==ID_OK)){

               	index=SendDlgItemMessage(h,55,CB_GETCURSEL,0,0);
            		SendDlgItemMessage(h,55,CB_GETLBTEXT,index,(LPARAM)objectstr);

               	dbtype=odb->getpropertytype(objectstr,propnm);

						if (dbtype==_INT_) {
							if ((res=odb->validate(newval,intval))>0)
							(*o).setproperty_value(propnm,intval);
						};

						if (dbtype==_OBJECT_) {
							if ((res=odb->validate(newval,intval))>0)
								ov=odb->getobjectwithoid("Usertypes",intval);
							if (ov!=NULL) (*o).setproperty_value(propnm,ov);
							else res=0;
						};

						if (dbtype==_REAL_) {
							if ((res=odb->validate(newval,realval))>0)
								(*o).setproperty_value(propnm,realval);
						};

						if (dbtype==_COLLECTION_){
							coll=new collection(_OBJECT_);
							if (odb->validate(newval,coll)<0) (*coll).~collection();
								(*o).setproperty_value(propnm,coll);
						};

						if (dbtype==_INT_COLLECTION_){
							coll=new collection(_INT_);
							if (odb->validate(newval,coll)<0) (*coll).~collection();
								(*o).setproperty_value(propnm,coll);
						};

						if (dbtype==_REAL_COLLECTION_){
							coll=new collection(_REAL_);
							if (odb->validate(newval,coll)<0) (*coll).~collection();
								(*o).setproperty_value(propnm,coll);
						};

						if (dbtype==_CHAR_COLLECTION_){
							coll=new collection(_CHAR_);
							if (odb->validate(newval,coll)<0) (*coll).~collection();
								(*o).setproperty_value(propnm,coll);
						};

                  if (dbtype==_CHAR_)
							(*o).setproperty_value(propnm,newval);

                  clearLbox(h,53);
                  populateLbWithInstances(h,53,objectstr);

               }
            	break;

				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
            
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