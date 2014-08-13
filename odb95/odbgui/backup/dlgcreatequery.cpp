#include "d:\users\staffan\odb\odb95\odb95.h"

DlgCreateQuery::DlgCreateQuery():DlgModal(){
}

DlgCreateQuery::~DlgCreateQuery(){
}

DlgCreateQuery::DlgCreateQuery(HINSTANCE hThis,int name,HWND parent,mainWindow *mw):
	DlgModal(hThis,name,parent,mw){
	if (mainW==NULL) {
   	odb=NULL;
      return;
   }
   else
   	(this->odb)=mainW->getDb();
}

BOOL DlgCreateQuery::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
char datatype[MAXNAMELENGTH]="";
char tpnm[MAXNAMELENGTH]="";
char propname[MAXNAMELENGTH]="";
char newvalue[MAXNAMELENGTH]="";
char optxt[MAXNAMELENGTH]="";
int index;
int dbtype,oktype,i;
ODB_INT intval;
ODB_REAL realval;
ODB_SET coll;
object *obj,*o;
ODB_QSTREAM currQs;

   switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
         	case CMB_CREINSTTYPE:
            	index=SendDlgItemMessage(h,CMB_CREINSTTYPE,CB_GETCURSEL,0,0);
               SendDlgItemMessage(h,CMB_CREINSTTYPE,CB_GETLBTEXT,index,(LPARAM)tpnm);
            	SetDlgItemText(h,DF_DATATYPE,"");
               clearLbox(h,LB_TYPEPROPS);
               clearLbox(h,LB_ASSPROP);
               noassignedprops=0;
               populateLbWithProps(h,LB_TYPEPROPS,tpnm);
               break;


            case LB_ASSPROP: //a prop has been selected, show its data type
					if (HIWORD(wParam)==LBN_SELCHANGE){
						index=SendDlgItemMessage(h,LB_ASSPROP,LB_GETCURSEL,0,0);
						SendDlgItemMessage(h,LB_OPERATORS,LB_GETTEXT,index,(LPARAM)propname);
						SendDlgItemMessage(h,LB_PROPVALS,LB_GETTEXT,index,(LPARAM)tpnm);
						SetDlgItemText(h,DF_VIEWVAL,strcat(propname,tpnm));
						};
					break; //set 96 to newvalue


				case LB_TYPEPROPS:
					if (HIWORD(wParam)==LBN_SELCHANGE){
						index=SendDlgItemMessage(h,LB_TYPEPROPS,LB_GETCURSEL,0,0); //typename
						SendDlgItemMessage(h,LB_TYPEPROPS,LB_GETTEXT,index,(LPARAM)propname);
						index=SendDlgItemMessage(h,CMB_CREINSTTYPE,CB_GETCURSEL,0,0); //typename
						SendDlgItemMessage(h,CMB_CREINSTTYPE,CB_GETLBTEXT,index,(LPARAM)tpnm);
						index=odb->getpropertytype(tpnm,propname);
						GetDlgItemText(h,DF_NEWVALUE,newvalue,MAXNAMELENGTH);

                  clearCbox(h,CB_OPERATORS);
                  populateCbWithOperators(h,CB_OPERATORS,index);

         			writeLiteralToDf(h,DF_DATATYPE,index);

						}
         		break;


				case DID_CANCEL:
					EndDialog(h,0);
					return 1;

            case PB_NEWC:
            	clearLbox(h,LB_OPERATORS);
               clearLbox(h,LB_PROPVALS);
               clearLbox(h,LB_ASSPROP);
               clearLbox(h,LB_QUERYRESULT);
               clearLbox(h,LB_TYPEPROPS);
               clearCbox(h,CB_OPERATORS);
               noassignedprops=0;
               SetDlgItemText(h,DF_DATATYPE,"");
               SetDlgItemText(h,DF_VIEWVAL,"");
               return 1;

         	case PB_SET:
					//assign property a value. The prop is moved to assignedprops
					//and value is validated and moved to hidden listbox
					index=SendDlgItemMessage(h,CMB_CREINSTTYPE,CB_GETCURSEL,0,0); //typename
      			SendDlgItemMessage(h,CMB_CREINSTTYPE,CB_GETLBTEXT,index,(LPARAM)tpnm);

               index=SendDlgItemMessage(h,CB_OPERATORS,CB_GETCURSEL,0,0); //operator
      			SendDlgItemMessage(h,CB_OPERATORS,CB_GETLBTEXT,index,(LPARAM)optxt);

               index=SendDlgItemMessage(h,LB_TYPEPROPS,LB_GETCURSEL,0,0); //typename
      			SendDlgItemMessage(h,LB_TYPEPROPS,LB_GETTEXT,index,(LPARAM)propname);

					GetDlgItemText(h,DF_NEWVALUE,newvalue,MAXNAMELENGTH);
					dbtype=odb->getpropertytype(tpnm,propname);

					if (dbtype==_INT_)
						oktype=odb->validate(newvalue,intval);

					if (dbtype==_OBJECT_){
						oktype=odb->validate(newvalue,intval);
						if (oktype!=0) { //check that there is an object with the oid
							obj=odb->getobjectwithoid("Usertypes",intval);
							if (obj==NULL) oktype=0;
						}
					};

					if (dbtype==_REAL_) oktype=odb->validate(newvalue,realval);

					if (dbtype==_CHAR_) oktype=1;

               if (dbtype==_COLLECTION_){
						coll=new collection(_OBJECT_);
						oktype=odb->validate(newvalue,coll);
                  delete coll;
               }

               if (dbtype==_INT_COLLECTION_){
						coll=new collection(_INT_);
						oktype=odb->validate(newvalue,coll);
                  delete coll;
               }

               if (dbtype==_REAL_COLLECTION_){
						coll=new collection(_REAL_);
						oktype=odb->validate(newvalue,coll);
                  delete coll;
               }

               if (dbtype==_CHAR_COLLECTION_){
               	coll=new collection(_CHAR_);
						oktype=odb->validate(newvalue,coll);
                  delete coll;
               }

					if ((strlen(newvalue)>0)&&(strlen(propname)>0)&&(oktype!=0)
               		&&(strcmp(optxt,NOOPERATION)!=0)){
					//move assigned prop from prop cmb box to assigned props
         			SendDlgItemMessage(h,LB_ASSPROP,LB_ADDSTRING,noassignedprops,(LPARAM)propname);
         			SendDlgItemMessage(h,LB_PROPVALS,LB_ADDSTRING,noassignedprops,(LPARAM)newvalue);
         			SendDlgItemMessage(h,LB_OPERATORS,LB_ADDSTRING,noassignedprops,(LPARAM)optxt);

						noassignedprops++;

         			SendDlgItemMessage(h,LB_TYPEPROPS,LB_DELETESTRING,index,0);
         			SetDlgItemText(h,DF_DATATYPE,"");
         			SetDlgItemText(h,DF_NEWVALUE,"");
               }
					else warn("Missing or erroneous data",h);
					break;


            case DID_OK:
            	index=SendDlgItemMessage(h,CMB_CREINSTTYPE,CB_GETCURSEL,0,0); //typename
      			SendDlgItemMessage(h,CMB_CREINSTTYPE,CB_GETLBTEXT,index,(LPARAM)tpnm);

					if (strlen(tpnm)<1) {
						//type not specified
						warn("A type must be spec.",h);
						break;
					}
					else {


               i=SendDlgItemMessage(h,LB_ASSPROP,LB_GETCOUNT,0,0);
               i--;

               currQs=NULL;
					while(i>=0){
					//check that the data for each prop is ok
					//and set the properies for the object
               SendDlgItemMessage(h,LB_ASSPROP,LB_GETTEXT,i,(LPARAM)propname);
               SendDlgItemMessage(h,LB_PROPVALS,LB_GETTEXT,i,(LPARAM)newvalue);
               SendDlgItemMessage(h,LB_OPERATORS,LB_GETTEXT,i,(LPARAM)optxt);

					dbtype=odb->getpropertytype(tpnm,propname);


					if (dbtype==_COLLECTION_){
						coll=new collection(_OBJECT_);
						if (odb->validate(newvalue,coll)<0) {
                  	(*coll).~collection();
                     dbtype=_OBJECT_;
                  }
                  else {
                  	if(currQs==NULL)
                  		currQs=odb->open_stream(tpnm,propname,(ODB_SET)coll);
                  	else currQs=odb->select(currQs,propname,(ODB_SET)coll);
                  }

					};
					if (dbtype==_INT_COLLECTION_){
						coll=new collection(_INT_);
						if (odb->validate(newvalue,coll)<0) {
                  	(*coll).~collection();
                     dbtype=_INT_;
                  }
                  else {
                  	if(currQs==NULL)
                  		currQs=odb->open_stream(tpnm,propname,(ODB_SET)coll);
                  	else currQs=odb->select(currQs,propname,(ODB_SET)coll);
                  }
					};
               
					if (dbtype==_REAL_COLLECTION_){
						coll=new collection(_REAL_);
						if (odb->validate(newvalue,coll)<0) {
                  	(*coll).~collection();
                     dbtype=_REAL_;
                  }
                  else {
                  	if(currQs==NULL)
                  		currQs=odb->open_stream(tpnm,propname,(ODB_SET)coll);
                  	else currQs=odb->select(currQs,propname,(ODB_SET)coll);
                  }

					};
					if (dbtype==_CHAR_COLLECTION_){
						coll=new collection(_CHAR_);
						if (odb->validate(newvalue,coll)<0) {
                  	(*coll).~collection();
                     dbtype=_CHAR_;
                  }
                  else {
                  	if(currQs==NULL)
                  		currQs=odb->open_stream(tpnm,propname,(ODB_SET)coll);
                  	else currQs=odb->select(currQs,propname,(ODB_SET)coll);
                  }

					};
               if (dbtype==_INT_) {
						odb->validate(newvalue,intval);
                  if(currQs==NULL)
                  	currQs=odb->open_stream(tpnm,propname,optxt,(ODB_INT)intval);
                  else currQs=odb->select(currQs,propname,optxt,(ODB_INT)intval);

					};
					if (dbtype==_OBJECT_) {
						odb->validate(newvalue,intval);
						o=odb->getobjectwithoid("Usertypes",intval);
                  if(currQs==NULL)
                  	currQs=odb->open_stream(tpnm,propname,(ODB_REF)o);
                  else currQs=odb->select(currQs,propname,(ODB_REF)o);

					};
					if (dbtype==_REAL_) {
						odb->validate(newvalue,realval);
                  if(currQs==NULL)
                  	currQs=odb->open_stream(tpnm,propname,optxt,(ODB_REAL)realval);
                  else currQs=odb->select(currQs,propname,optxt,(ODB_REAL)realval);
					};
					if (dbtype==_CHAR_) {
               if(currQs==NULL)
                  	currQs=odb->open_stream(tpnm,propname,(ODB_CHAR)newvalue);
                  else currQs=odb->select(currQs,propname,(ODB_CHAR)newvalue);
               
            }
            i--;
         }; //end while all properties set.
				//deleta all unassigned praps
			//clearLbox(h,LB_ASSPROP);

         if (currQs==NULL) currQs=odb->open_stream(tpnm);
         populateLbWithQueryResult(h,LB_QUERYRESULT,currQs);

			//clearLbox(h,LB_PROPVALS);
			//clearLbox(h,LB_TYPEPROPS);
			//SetDlgItemText(h,DF_VIEWVAL,"");
			//populateLbWithProps(h,LB_TYPEPROPS,tpnm);
			//noassignedprops=0; //no assigned properties anymore


		}; //else		//create the object and set properties
		break;
      }
      case WM_INITDIALOG:
      	if (initialized==0){
         	noassignedprops=0;
         	//populateCbWithLiterals(h,ID_DATATYPE);
            populateCbWithTypes(h,CMB_CREINSTTYPE);
            preSelectCBwithType(h, CMB_CREINSTTYPE, presetTypeName);
         	initialized=1;
            }
         return 1;
	}
	return 0;
}