#include "d:\users\staffan\odb\odb95\odb95.h"
#include <stdio.h>
#include <mem.h>

DlgModal::DlgModal(){
	thisInstance=NULL;
   name=NULL;
   hWndThis=NULL;
   initialized=0;
   presetTypeName[0]='\0';
}

DlgModal::DlgModal(HINSTANCE hThis, int nm, HWND parent, mainWindow *mw){
	mainW=mw;
	name=nm;
   thisInstance=hThis;
   hWndThis=parent;
   initialized=0;
   presetTypeName[0]='\0';
}

DlgModal::~DlgModal(){
	//EndDialog(hWnd,state);
}


BOOL DlgModal::callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam){
	switch (mess){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case DID_CANCEL:
					EndDialog(h,0);
					return 1;
			}
	}
	return 0;
}

int DlgModal::create(mainWindow *mwh){
	mainW=mwh;
   //(this->odb)=mwh->getDb();
	state=DialogBox(this->thisInstance,MAKEINTRESOURCE(name),this->hWndThis,
			(DLGPROC)dlgFn);
   return state;
}


int DlgModal::populateCbWithTypes(HWND h, int lb_Id){
ODB_CHAR nm;
int i=0;
	nm=odb->getallsubtypes();
	while (nm!=NULL){
		SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)nm);
		i++;
		nm=odb->getnextsubtype();
	};
	SendDlgItemMessage(h,lb_Id,CB_SETCURSEL,0,0);
	return 1;
}

int DlgModal::populateLbWithTypes(HWND h, int lb_Id){
ODB_CHAR nm;
int i=0;
	nm=odb->getallsubtypes();
	while (nm!=NULL){
		SendDlgItemMessage(h,lb_Id,LB_ADDSTRING,i,(LPARAM)nm);
		i++;
		nm=odb->getnextsubtype();
	};

	return 1;
}

int DlgModal::populateCbWithSubTypes(HWND h,int lb_Id,char *typenm){
ODB_CHAR nm;
int i=0;
	nm=odb->getsubtypes(typenm);
	while (nm!=NULL){
		SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)nm);
		i++;
		nm=odb->getsubtype(typenm);
	};
	SendDlgItemMessage(h,lb_Id,CB_SETCURSEL,0,0);
	return 1;
}

int DlgModal::populateLbWithSubTypes(HWND h, int lb_Id,char *typenm){
ODB_CHAR nm;
int i=0;
	nm=odb->getsubtypes(typenm);
	while (nm!=NULL){
		SendDlgItemMessage(h,lb_Id,LB_ADDSTRING,i,(LPARAM)nm);
		i++;
		nm=odb->getsubtype(typenm);
	};
	return 1;
}

int DlgModal::populateLbWithProps(HWND h, int lb_Id,char *typenm){
char *propnm;
int i=0;
	propnm=odb->getfirstprop(typenm);
	while (propnm!=NULL){
		SendDlgItemMessage(h,lb_Id,LB_ADDSTRING,i,(LPARAM)propnm);
		i++;
		propnm=odb->getnextprop(typenm);
	};

	return 1;
}

int DlgModal::populateLbWithInstances(HWND h, int lb_Id, char *typenm){
query_stream *qst;
object *o;
int i;
outputbuffer obuf=outputbuffer();
	i=0;
   qst=odb->open_stream(typenm);
   o=(*qst).get();
   if (o==NULL){
   	SendDlgItemMessage(h,lb_Id,LB_ADDSTRING,i,(LPARAM)"No Instances");
   	return 1;
   }

   while(o!=NULL){
   	(*o).display(obuf);
      SendDlgItemMessage(h,lb_Id,LB_ADDSTRING,i,(LPARAM)obuf.get_buffer());
      i++;
      o=(*qst).get();
      obuf.reset_buffer();
   }; //while

   odb->close_stream(qst);
   return 1;
}

int DlgModal::populateLbWithQueryResult(HWND h, int lb_Id,ODB_QSTREAM qst)
{
object *o;
int i;
outputbuffer obuf=outputbuffer();

	i=0;
   o=(*qst).get();
   if (o==NULL){
   	SendDlgItemMessage(h,lb_Id,LB_ADDSTRING,i,(LPARAM)"No Instances");
   	return 1;
   }

   while(o!=NULL){
   	(*o).display(obuf);
      SendDlgItemMessage(h,lb_Id,LB_ADDSTRING,i,(LPARAM)obuf.get_buffer());
      i++;
      o=(*qst).get();
      obuf.reset_buffer();
   }; //while
   return 1;
}


int DlgModal::populateLbWithOIDInstances(HWND h, int lb_Id, char *typenm){
query_stream *qst;
object *o;
int oid,i;
char stroid[MAXNAMELENGTH]="";
	i=0;
   qst=odb->open_stream(typenm);
   o=(*qst).get();
   if (o==NULL){
   	SendDlgItemMessage(h,lb_Id,LB_ADDSTRING,i,(LPARAM)"No Instances");
   	return 1;
   }

   while(o!=NULL){
   	oid=(*o).oid();
      sprintf(stroid,"[OID:%i]",oid);
      SendDlgItemMessage(h,lb_Id,LB_ADDSTRING,i,(LPARAM)stroid);
      i++;
      o=(*qst).get();
   }; //while

   odb->close_stream(qst);
   return 1;
}

int DlgModal::populateCbWithProps(HWND h, int lb_Id,char *typenm){
char *propnm;
int i=0;
	propnm=odb->getfirstprop(typenm);
	while (propnm!=NULL){
		SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)propnm);
		i++;
		propnm=odb->getnextprop(typenm);
	};
   SendDlgItemMessage(h,lb_Id,CB_SETCURSEL,0,0);
	return 1;
}

void DlgModal::clearLbox(HWND h, int lb_Id){
int index=0;
	index=SendDlgItemMessage(h,lb_Id,LB_GETCOUNT,0,0);
   while (index>=0){
		SendDlgItemMessage(h,lb_Id,LB_DELETESTRING,index,0);
      index--;
   }
}

void DlgModal::clearCbox(HWND h, int lb_Id){
int index=0;
	index=SendDlgItemMessage(h,lb_Id,CB_GETCOUNT,0,0);
   while (index>=0){
		SendDlgItemMessage(h,lb_Id,CB_DELETESTRING,index,0);
      index--;
   }
}
int DlgModal::populateCbWithLiterals(HWND h, int lb_Id){
int i=0;

	SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)_INT_NM_);
   i++;
   SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)_REAL_NM_);
   i++;
   SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)_CHAR_NM_);
   i++;
   SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)_OBJECT_NM_);
   i++;
   SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)_INT_COLLECTION_NM_);
   i++;
   SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)_REAL_COLLECTION_NM_);
   i++;
   SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)_CHAR_COLLECTION_NM_);
   i++;
   SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)_COLLECTION_NM_);
   
   SendDlgItemMessage(h,lb_Id,CB_SETCURSEL,0,0);
	return 1;
}

void DlgModal::writeLiteralToDf(HWND h,int lb_Id,int tp){
	if (tp==_INT_)
   	SetDlgItemText(h,lb_Id,(LPCSTR)_INT_NM_);
   else if (tp==_REAL_)
   	SetDlgItemText(h,lb_Id,(LPCSTR)_REAL_NM_);
   else if(tp==_CHAR_)
   	SetDlgItemText(h,lb_Id,(LPCSTR)_CHAR_NM_);
   else if(tp==_OBJECT_)
   	SetDlgItemText(h,lb_Id,(LPCSTR)_OBJECT_NM_);
   else if(tp==_COLLECTION_)
   	SetDlgItemText(h,lb_Id,(LPCSTR)_COLLECTION_NM_);
   else if(tp==_INT_COLLECTION_)
   	SetDlgItemText(h,lb_Id,(LPCSTR)_INT_COLLECTION_NM_);
   else if(tp==_REAL_COLLECTION_)
   	SetDlgItemText(h,lb_Id,(LPCSTR)_REAL_COLLECTION_NM_);
   else if(tp==_CHAR_COLLECTION_)
   	SetDlgItemText(h,lb_Id,(LPCSTR)_CHAR_COLLECTION_NM_);
}

void DlgModal::populateCbWithOperators(HWND h, int lb_Id, int tp)
{
int i=0;

	SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)NOOPERATION);
   i++;
   SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)"=");
   i++;
   SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)"!=");
   i++;
   if ((tp==_INT_)||(tp==_REAL_)){
      SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)">");
   	i++;
      SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)">=");
   	i++;
      SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)"<");
   	i++;
      SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)"<=");
   	i++;
   }
   else if(tp==_CHAR_){
   	SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)"Like");
   	i++;
   }
   else if(tp==_OBJECT_){
   	//Insert Object operations here
   }
   else if(tp==_COLLECTION_){
   	SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)"member");
   	i++;
   }
   else if(tp==_INT_COLLECTION_){
   	SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)"member");
   	i++;
   }
   else if(tp==_REAL_COLLECTION_){
   	SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)"member");
   	i++;
   }
   else if(tp==_CHAR_COLLECTION_){
   	SendDlgItemMessage(h,lb_Id,CB_ADDSTRING,i,(LPARAM)"member");
   	i++;
   }
}

void DlgModal::warn(char *warning, HWND h){
MessageBox(h,warning,MESSAGEALERT,MB_OK);
}

void DlgModal::preSelectCBwithType(HWND h, int ID, char *tpnm){
int index=-1;
WPARAM wp;

	index=SendDlgItemMessage(h,ID,CB_FINDSTRINGEXACT,index,(LPARAM)tpnm);

   if (index!=CB_ERR) {
   	SendDlgItemMessage(h,ID,CB_SETCURSEL,index,0);
   	wp=MAKEWPARAM(ID,CBN_SELCHANGE);
      PostMessage(h,WM_COMMAND,(WPARAM)wp,0);
      }
}

void DlgModal::setType(char *nm){
	strcpy(presetTypeName,nm);
}
