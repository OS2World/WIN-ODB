//=================================================================
//
// Project: Store an object oriented storage manager
// Version: Development ver for os2 Warp
// This file: pmstore.h header file for the graphical user intfc
//
//=================================================================


#define ID_DELETE	274
#define ID_DELETETYPE	275
#define ID_DELETEPROP	276
#define ID_DELETEINST	277
#define ID_UPDATEINST	278
#define DLG_INSPECTINSTANCE	50
#define CUR_WAIT	5
#define Store	1
#define CHECKED 1

#define DLG_DBOUTPUT    100
#define ML_DBOUT	101
#define ID_TYPENAME	121
#define DLG_CREPROP	120
#define ID_DATATYPE	127
#define ID_NEWPROP	125

#define DLG_CRETP       110

#define DLG_CREINST	90
#define CMB_CREINSTTYPE 91
#define LB_TYPEPROPS    92
#define DF_DATATYPE     94
#define LB_PROPVALS	100
#define LB_ASSPROP	95
#define DF_VIEWVAL      96
#define PB_SET          99
#define DF_NEWVALUE     97

#define DLG_CREATEDB 80
#define DF_NEWDBNAME 81
#define DF_NEWDBSIZE 84
#define MSGBOXID	1001

#define DLG_INSPECTDB	70
#define DLG_INSPECTTYPE 60

#define ID_WINDOW   256

#define ID_OPTIONS  257
#define ID_OPTION1  258
#define ID_OPTION2  259
#define ID_OPTION3  260
#define ID_CREATETYPE 265
#define ID_CREATEPROP 262
#define ID_CREATEINST 263
#define ID_CREATEOPTIONS 264
#define ID_EXITPROG 261
#define ID_NEWDB 269
#define ID_INSPECT 270
#define ID_INSPECTDB 271
#define ID_INSPECTTYPE 272
#define ID_INSPECTINST 273

#define ID_ICON1    266
#define ID_ICON2    267
#define ID_ICON3    268


#ifndef __MT_H__
#define __MT_H__
#define MESSAGELEN 100
#define IDD_FILEOPEN 2000
#define IDS_FILEOPENNEXT 1
#define ID_OK 1
//Stringtable constants
#define IDS_FILEOPENEXT 1
#define IDS_OPENDLGTITLE 2
#define IDS_OPENDLGBUTTON 3
#define IDS_SAVEDLGTITLE 4
#define IDS_SAVEDLGBUTTON 5

#define MAXNAMELENGTH 256
#define TYPETREEOFFSET 36
// If you want to debug the macro code replace the macro code
// manually in the class defination.
// MFT_DEF must be declared within the class defination.
// MFT_INIT and MFT_INIT_END pair must be outside the class
// defination.
// Message Function Table MACROS MFT_DEF (,)and MFT_INIT(,,)

#define MFT_DEF(className, _CALLBACK_) typedef\
   MRESULT (className::*MSGFNPTR)(HWND hwnd,ULONG msg, MPARAM mp1,MPARAM mp2);\
   typedef struct {\
	    int    msg;\
	    MSGFNPTR  function;\
   }MFT;\
   static MFT msgFunction[];\
   MRESULT _CALLBACK_  (HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2);

// IMPORTANT: For window procedure only this WndProc is used
// for dialog procedures same stuff has to be declared, but call
// to WinDefDlgProc() is to be made instead of WinDefWindowProc()

#define MFT_INIT(className, _CALLBACK_, _DEFAULT_CALLBACK_) \
MRESULT className:: _CALLBACK_ (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2){\
   int      i;\
   int 	   done   = FALSE;\
   MRESULT retVal = (MRESULT)FALSE;\
   for(i=0; msgFunction[i].msg && !done; i++) {\
	    if(msg == msgFunction[i].msg){\
		    retVal = (this->*(msgFunction[i].function))(hwnd, msg, mp1, mp2);\
		    done = TRUE;\
	    }\
   }\
   if(!done)\
	    retVal = _DEFAULT_CALLBACK_ (hwnd, msg, mp1, mp2);\
   return(retVal);\
}\
className::MFT  className::msgFunction[]={

#define MFT_INIT_END \
			{ 0L,          NULL}\
			};

// standard parameters. H(handle) M(message) 1(mp1) 2(mp2)
#define HM12       HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2
#define USE_HM12   hwnd; msg; mp1; mp2;

#endif
//Forward declarations
class Window;
class Dialog;

extern VOID AbortHello(HWND hwndFrame,HWND hwndClient);

MRESULT EXPENTRY _export WndProc(HWND hWnd,ULONG iMessage,MPARAM mParam1, MPARAM mParam2);
MRESULT EXPENTRY _export WinCallBack(HM12);
MRESULT EXPENTRY _export DlgCallBack(HM12);
class CallBack
{public:
	static Window *winThis;
// Used to hold the "this" pointer that _export WinCallBack(must be
// global)uses to vector to the correct member function WndProc(dispatcher)
	static Dialog *dlgThis;
// Used to hold the "this" pointer
// that _export DlgCallBack(must be
// global) uses to vector
// to the correct member function
// DlgProc(diapatcher)
};

Window *CallBack::winThis = NULL;
Dialog *CallBack::dlgThis = NULL;

class Window
{ protected:

public: HWND hWndFrame;
	HWND hWndClient;
	HWND GetFrameHandle(void);
	HWND GetClient(void);
	BOOL ShowWindow(int nCmdShow);
	BOOL Update(void);
    // Pure virtual function makes "Window" an abstract class
	virtual MRESULT WndProc(HWND hWnd, ULONG iMessage, MPARAM mParam1, MPARAM mParam2)=0;
 };

// Class main is a container of program global functions and variables
class Main
{
public:
	static HAB      hab;
	static HMQ      hmq;
	static char *szUsertypes;
	static char *szDatabase;
	static HPOINTER icon[4];
	static Window   *tempthis;          // Used to hold the "this" pointer
                                        // that _export WndProc uses to vector
                                        // to the correct member function
					// WndProc
	static BOOL Initialize(void);
	static BOOL MessageLoop(void);
	static void Terminate(void);
	static HWND DbOutDlg;
	static HWND CreTpDlg;
};
// Declare and initialize all globals in Main
HAB Main::hab;
HMQ Main::hmq;
Window *Main::tempthis = NULL;
HPOINTER Main::icon[4];
char *Main::szUsertypes = "Usertypes";
char *Main::szDatabase = "DATABASE";


class MainWindow : public Window
{
private:
    static char szClassName[14];
    static BOOL initialized;
public:
    // Unlike Windows, in PM window class registration always
    // should happen within the app before the first time a
    // window is created. "initialized" keeps track of this
    // The first class Register (and thus the first creation of a
    // Mainwindow instance) must happen AFTER Main::Initialize()
    // has been called.
    static void Register(void);
    MainWindow(void);
    MRESULT WndProc(HWND hWnd,ULONG iMessage,MPARAM mParam1,MPARAM mParam2 );
    ~MainWindow(void);
    int Drawtypetree(HPS hps,RECTL rc,POINTL pt, int i,int j,char *tp);
};

char MainWindow::szClassName[14] = "MyWindow";  // Must be unique in the app
BOOL MainWindow::initialized = FALSE;



#ifndef __DLG_H__
#define __DLG_H__
//---------------------------class Dialog{}-------------------------------
class Dialog{
public:
    HWND hwndParent;
    HWND hwndOwner;
    PFNWP pDlgProc;
    HMODULE  Resource;
    ULONG ulDlgid;
    PVOID pCreateParams;
    ULONG ulResult;
    Dialog::Dialog( HWND hwndParent1,
		HWND hwndOwner1,
		HMODULE Resource1,
		ULONG ulDlgid1,
		PVOID pCreateParams1);
     Dialog(void) {};
    // Pure virtual function makes "Window" an abstract class
    virtual MRESULT DlgProc(HM12) = 0;
    int clearlbox(HWND lboxptr);
    int populate_props(HWND lboxptr,char *typenm, database &db);
    int populate_types(HWND lboxptr,database &db);
    int populate_subtypes(HWND lboxptr,char *tpnm,database &db);
    int populate_instances(HWND lboxptr,char *tpnm,database &db);
    int populate_inspectinstances(HWND lboxptr,char *tpnm,database &db);

};

int Dialog::clearlbox(HWND lboxptr){

	if(lboxptr!=NULL){
		WinSendMsg(lboxptr,LM_DELETEALL,0,0);
		return 1;
		}
	else return 0;
}

int Dialog::populate_props(HWND lboxptr, char *typenm, database &db){
char *propnm;
int i;
	i=0;
	if (lboxptr!=NULL){
		propnm=db.getfirstprop(typenm);
		while (propnm!=NULL){
			WinInsertLboxItem(lboxptr,i,(PSZ)propnm);
			i++;
			propnm=db.getnextprop(typenm);
			}
		return 1;
		}
	else return 0;
}

int Dialog::populate_types(HWND lboxptr,database &db){
char *nm;
int i=0;
	if (lboxptr!=NULL){
		nm=db.getallsubtypes();
		while (nm!=NULL){
			WinInsertLboxItem(lboxptr,i,(PSZ)nm);
			i++;
			nm=db.getnextsubtype();
		};
		return 1;
	}
	else return 0;
}

int Dialog::populate_subtypes(HWND lboxptr,char *tpnm,database &db){
char *nm;
int i=0;
	if (lboxptr!=NULL){
		nm=db.getsubtypes(tpnm);
		while (nm!=NULL){
			WinInsertLboxItem(lboxptr,i,(PSZ)nm);
			i++;
			nm=db.getsubtype(tpnm);
		};
		return 1;
	}
	else return 0;
}

int Dialog::populate_inspectinstances(HWND lboxptr,char *tpnm,database &db){
outputbuffer obuf=outputbuffer();
query_stream *qst;
object *o;
int i;
	if (lboxptr!=NULL) {
		qst=db.open_stream(tpnm);
		o=(*qst).get();
		i=0;
		while (o!=NULL){
			(*o).display(obuf);
			WinInsertLboxItem(lboxptr,i,(PSZ)obuf.get_buffer());
			obuf.reset_buffer();
			i++;
			o=(*qst).get();
			} //while
		return 1;
		}
	else return 0;
}

int Dialog::populate_instances(HWND lboxptr,char *tpnm,database &db){
query_stream *qst;
object *o;
int oid,i;
char stroid[MAXNAMELENGTH]="";
	if (lboxptr!=NULL){
		i=0;
		qst=db.open_stream(tpnm);
		o=(*qst).get();
		if (o==NULL) {
			WinInsertLboxItem(lboxptr,i,(PSZ)"No Instances");
			}
		else {
			while(o!=NULL){
				oid=(*o).oid();
				sprintf(stroid,"[OID:%i]",oid);
				WinInsertLboxItem(lboxptr,i,(PSZ)stroid);
				i++;
				o=(*qst).get();
				}; //while
		};//else
		db.close_stream(qst);
		return 1;
	}
	else return 0;
}

Dialog::Dialog( HWND hwndParent1,
		HWND hwndOwner1,
		HMODULE Resource1,
		ULONG ulDlgid1,
		PVOID pCreateParams1)
{
    hwndParent = hwndParent1;  // must have differnt names
    hwndOwner  = hwndOwner1;
    Resource   = Resource1;
    ulDlgid    = ulDlgid1;
    pCreateParams = pCreateParams1;
}

#endif

//--------------------------class DboutDlg{}-------------------------------

class DbOutDlg : public Dialog
{
private:
    BOOL initialized;
public:
   DbOutDlg(void);               // Constructor
   ~DbOutDlg(void);                 // Destructor

   DbOutDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);

   Run();
   MRESULT wmInitDlg(HM12);
   MRESULT wmCommand(HM12);
   MFT_DEF(DbOutDlg, DlgProc)     // Message Function defination for DbOutDlg
};
// This is the Message Function Table.(MFT) initialization
// This is only for the messages not for the mp1 and mp2 params.
MFT_INIT(DbOutDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
MFT_INIT_END



//--------------------------class DbInspectDlg{}-------------------------------

class DbInspectDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwnddbname,hwnddbsize,hwndtypelist;
public:
   DbInspectDlg(void);               // Constructor
   ~DbInspectDlg(void);                 // Destructor

   DbInspectDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);

   Run();
   MRESULT wmInitDlg(HM12);
   MRESULT wmCommand(HM12);
   MFT_DEF(DbInspectDlg, DlgProc)     // Message Function defination for DbOutDlg
};
// This is the Message Function Table.(MFT) initialization
// This is only for the messages not for the mp1 and mp2 params.
MFT_INIT(DbInspectDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
MFT_INIT_END


//--------------------------class TpDeleteDlg{}-------------------------------

class TpDeleteDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwndtypelist;
public:
   TpDeleteDlg(void);               // Constructor
   ~TpDeleteDlg(void);                 // Destructor

   TpDeleteDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);

   Run();
   MRESULT wmInitDlg(HM12);
   MRESULT wmCommand(HM12);
   MFT_DEF(TpDeleteDlg, DlgProc)     // Message Function defination for DbOutDlg
};
// This is the Message Function Table.(MFT) initialization
// This is only for the messages not for the mp1 and mp2 params.
MFT_INIT(TpDeleteDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
MFT_INIT_END


//--------------------------class PropDeleteDlg{}-------------------------------

class PropDeleteDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwndtypelist,hwndproplist;
public:
   PropDeleteDlg(void);               // Constructor
   ~PropDeleteDlg(void);                 // Destructor

   PropDeleteDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);

   Run();
   MRESULT wmInitDlg(HM12);
   MRESULT wmCommand(HM12);
   MRESULT wmControl(HM12);
   MFT_DEF(PropDeleteDlg, DlgProc)     // Message Function defination for DbOutDlg
};
// This is the Message Function Table.(MFT) initialization
// This is only for the messages not for the mp1 and mp2 params.
MFT_INIT(PropDeleteDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
	{ WM_CONTROL, wmControl },
MFT_INIT_END

//--------------------------class TpInspectDlg{}-------------------------------

class TpInspectDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwndtpname,hwndsuptp,hwndsubtp,hwndprops;

public:
   TpInspectDlg(void);               // Constructor
   ~TpInspectDlg(void);                 // Destructor

   TpInspectDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);

   Run();
   MRESULT wmInitDlg(HM12);
   MRESULT wmCommand(HM12);
   MRESULT wmControl(HM12);
   MFT_DEF(TpInspectDlg, DlgProc)     // Message Function defination for DbOutDlg
};

// This is the Message Function Table.(MFT) initialization
// This is only for the messages not for the mp1 and mp2 params.
MFT_INIT(TpInspectDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
	{ WM_CONTROL, wmControl },
MFT_INIT_END


//--------------------------class InstInspectDlg{}-------------------------------

class InstInspectDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwndtpname,hwndinspector;
public:
   InstInspectDlg(void);               // Constructor
   ~InstInspectDlg(void);                 // Destructor

   InstInspectDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);

   Run();
   MRESULT wmInitDlg(HM12);
   MRESULT wmCommand(HM12);
   MRESULT wmControl(HM12);
   MFT_DEF(InstInspectDlg, DlgProc)     // Message Function defination for DbOutDlg

};

// This is the Message Function Table.(MFT) initialization
// This is only for the messages not for the mp1 and mp2 params.
MFT_INIT(InstInspectDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
	{ WM_CONTROL, wmControl },
MFT_INIT_END


//--------------------------class InstInspectDlg{}-------------------------------

class InstUpdtDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwndtpname,hwndinspector,hwndpropnm,hwndnewval,hwnddatatype;
public:
   InstUpdtDlg(void);               // Constructor
   ~InstUpdtDlg(void);                 // Destructor

   InstUpdtDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);

   Run();
   MRESULT wmInitDlg(HM12);
   MRESULT wmCommand(HM12);
   MRESULT wmControl(HM12);
   MFT_DEF(InstUpdtDlg, DlgProc)     // Message Function defination for DbOutDlg

};

// This is the Message Function Table.(MFT) initialization
// This is only for the messages not for the mp1 and mp2 params.
MFT_INIT(InstUpdtDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
	{ WM_CONTROL, wmControl },
MFT_INIT_END

//--------------------------class DbCreateDlg{}-------------------------------

class DbCreateDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwnddbname,hwndtblsize;
public:
   DbCreateDlg(void);               // Constructor
   ~DbCreateDlg(void);                 // Destructor

   DbCreateDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);

   Run();
   MRESULT wmInitDlg(HM12);
   MRESULT wmCommand(HM12);
   MFT_DEF(DbCreateDlg, DlgProc)     // Message Function defination for DbCreateDlg
};
// This is the Message Function Table.(MFT) initialization
// This is only for the messages not for the mp1 and mp2 params.
MFT_INIT(DbCreateDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
MFT_INIT_END


//--------------------------class CreTpDlg{}-------------------------------

class CreTpDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwndlb;
    HWND hwndef;
public:
	CreTpDlg(void);
	~CreTpDlg(void);
	CreTpDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);
	Run();
	MRESULT wmInitDlg(HM12);
	MRESULT wmCommand(HM12);
	MFT_DEF(CreTpDlg, DlgProc)     // Message Function defination for DbCreateDlg
};

MFT_INIT(CreTpDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
MFT_INIT_END

//--------------------------class CrePropDlg{}-------------------------------

class CrePropDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwndlb;
    HWND hwndnewprop,hwndtypename;
public:
	CrePropDlg(void);
	~CrePropDlg(void);
	CrePropDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);
	Run();
	MRESULT wmInitDlg(HM12);
	MRESULT wmCommand(HM12);
	MFT_DEF(CrePropDlg, DlgProc)     // Message Function defination for DbCreateDlg
};

MFT_INIT(CrePropDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
MFT_INIT_END

//--------------------------class CreInstDlg{}-------------------------------

class CreInstDlg : public Dialog
{
private:
    BOOL initialized;
    HWND hwndlbtypes, hwndlbprops,hwnddatatype,hwndnewvalue;
    HWND hwndpropval,hwndassignedprop,hwndviewval;
    int noassignedprops;  //Counts no of properties given values
public:
	CreInstDlg(void);
	~CreInstDlg(void);
	CreInstDlg(HWND hwndParent1,
	 HWND hwndOwner1,
	 HMODULE Resource1,
	 ULONG ulDlgid1,
	 PVOID pCreateParams1);
	Run();
	MRESULT wmInitDlg(HM12);
	MRESULT wmCommand(HM12);
	MRESULT wmControl(HM12);
	MFT_DEF(CreInstDlg, DlgProc)     // Message Function defination for DbCreateDlg
};

MFT_INIT(CreInstDlg, DlgProc, WinDefDlgProc)
//-------- Message --- callBack -----------------
	{ WM_COMMAND, wmCommand },
	{ WM_INITDLG, wmInitDlg },
	{ WM_CONTROL, wmControl },
MFT_INIT_END



static VOID SetSysMenu(HWND hwndDlg);
void CenterDialog(HWND hDlg);
//------- Database declarations -----------------
database odb;
char szFullPath[256]=""; //full pathname to file
