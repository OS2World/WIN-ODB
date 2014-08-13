#ifndef odb95
#define odb95

#define MESSAGEALERT 		"ODB for Win95 Alert"
#define MESSAGENAME 			"ODB for Win95"

#define WINNAME 				"ODB"
#define WINTITLE 				"ODB for Windows 95 Main Window"
#define MAXNAMELENGTH 		256
#define TYPETREEOFFSET 		32
#define TYPETREEBOUNDING 	28

#define LOADDATABASE 		1
#define SAVEDATABASE 		0
#define LOADODBQL				3

#define NOOPERATION 			"None"

#include <windows.h>
#include <string.h>
#include <malloc.h>
#include <commdlg.h>
#include "d:\users\staffan\odb\odb95\odb95res.h"
#include "d:\users\staffan\odb\odb95\odb.h"





//Macro to invoke dialogs
#define INVOKEDIALOG(DIALOGNAME,DLGTYPE)\
	currentDlg=new DLGTYPE(thisInstance,DIALOGNAME,hWndThis,mainW);\
	currentDlg->setMw(mainW);\
	mainW->setDlg(currentDlg);\
	(this->currentDlg)->create(this);\
	delete currentDlg;


#define INVOKEPRESETDIALOG(DIALOGNAME,DLGTYPE)\
	currentDlg=new DLGTYPE(thisInstance,DIALOGNAME,hWndThis,mainW);\
	currentDlg->setMw(mainW);\
   currentDlg->setType((mainW->getCurrentRegion())->getName());\
	mainW->setDlg(currentDlg);\
	(this->currentDlg)->create(this);\
	delete currentDlg;



//Declare windows type to win95
LONG APIENTRY WindowFunc(HWND h, UINT u, WPARAM w, LPARAM l);
BOOL APIENTRY dlgFn(HWND hw, UINT mess, WPARAM wp, LPARAM lp);

class mainWindow;
class DlgModal;
class region;

class region {
private:
	int x1,y1,x2,y2;
	region *next;
	char name[MAXNAMELENGTH];

public:

	int getX1(void){return x1;};
	int getY1(void){return y1;};
	int getX2(void){return x2;};
	int getY2(void){return y2;};
	void setX1(int i){x1=i;};
	void setY1(int i){y1=i;};
	void setX2(int i){x2=i;};
	void setY2(int i){y2=i;};
	char *getName(void);
	void setName(char * nm);
	region *getNext(void);
	region *getNextToSet(void);
	void set(int x1,int y1, int x2, int y2);

	region();
	region(int x1,int y1, int x2, int y2);
	~region();

	region *findRegion(int mx, int my);
	void invalidate();
   int invalidRegion(void);
};

class mainWindow {
private :
	HWND hWndMain;
	HINSTANCE parent;
	HINSTANCE thisInstance;
	WNDCLASS wcl;
	int registerWindow(HINSTANCE hThis);
   HICON dbIco,typeIco;
	DlgModal *currentDlg;
	database odb;
   region *rg,*currentregion;
   Drawtypetree(HDC hdc, int i,int j,char *tp,region *r);
   mainWindow *mainW;
   void highlightSelection(region *r, HDC h);
   void unHighlightSelection(region *r, HDC h);
   void drawSelectedBox(region *r, HDC h);

public:
	mainWindow(void);
	mainWindow(HINSTANCE hThis, HINSTANCE hPrev);
	int create(int nWinMode);
	LONG processMessages(HWND hWndThis,UINT message, WPARAM wParam, LPARAM lParam);
	DlgModal *getDlg(void);
   void setDlg(DlgModal *dlg){currentDlg=dlg;};
	database *getDb(void);
   region *getCurrentRegion(void);
};


class DlgModal {
protected:
	HINSTANCE thisInstance; // Handle to current application passed to WinMain
	int name;        //Resource file identifier
	HWND hWndThis;           //The parent window
	int state;
	mainWindow *mainW;
   database *odb;
   int initialized;
   char presetTypeName[MAXNAMELENGTH];
public:
	DlgModal();
	DlgModal(HINSTANCE hThis, int name, HWND parent, mainWindow *mw);
	~DlgModal();
   void setMw(mainWindow *mw){mainW=mw;};
	virtual BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
	virtual int create(mainWindow *mwh);
   int populateCbWithTypes(HWND h,int lb_Id);
   int populateLbWithTypes(HWND h, int lb_Id);
   int populateCbWithSubTypes(HWND h,int lb_Id,char *typenm);
   int populateLbWithSubTypes(HWND h, int lb_Id,char *typenm);
   int populateLbWithProps(HWND h, int lb_Id,char *typenm);
   int populateCbWithProps(HWND h, int lb_Id,char *typenm);
   int populateCbWithLiterals(HWND h, int lb_Id);
   int populateLbWithInstances(HWND h, int lb_Id, char *typenm);
   int populateLbWithQueryResult(HWND h, int lb_Id,ODB_QSTREAM qs);
   int populateLbWithOIDInstances(HWND h, int lb_Id, char *typenm);
   void clearLbox(HWND h, int lb_Id);
   void clearCbox(HWND h, int lb_Id);
   void writeLiteralToDf(HWND h,int lb_,int tp);
   void warn(char *warning, HWND h);
   void setType(char *nm);
   void preSelectCBwithType(HWND h, int ID, char *tpnm);
   void populateCbWithOperators(HWND h, int ID, int tp);
};

class DlgPopup : public DlgModal{
private:
	DlgModal *currentDlg;
   region *rg;
public:
	DlgPopup();
	~DlgPopup();
	DlgPopup(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
   DlgModal *getDlg(void);
	database *getDb(void);
};


class DlgCreateDb : public DlgModal {

public:
	DlgCreateDb();
	~DlgCreateDb();
	DlgCreateDb(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

class DlgInspectDb : public DlgModal {

public:
	DlgInspectDb();
	~DlgInspectDb();
	DlgInspectDb(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

class DlgInspectTp : public DlgModal {

public:
	DlgInspectTp();
	~DlgInspectTp();
	DlgInspectTp(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

class DlgUpdateInst : public DlgModal {

public:
	DlgUpdateInst();
	~DlgUpdateInst();
	DlgUpdateInst(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

class DlgCreateType : public DlgModal {
public:
	DlgCreateType();
	~DlgCreateType();
	DlgCreateType(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

class DlgDeleteType : public DlgModal {
public:
	DlgDeleteType();
	~DlgDeleteType();
	DlgDeleteType(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

class DlgCreateProp : public DlgModal {
public:
	DlgCreateProp();
	~DlgCreateProp();
	DlgCreateProp(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

class DlgDeleteProp : public DlgModal {
public:
	DlgDeleteProp();
	~DlgDeleteProp();
	DlgDeleteProp(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

class DlgCreateInst : public DlgModal {
private:
	int noassignedprops;
public:
	DlgCreateInst();
	~DlgCreateInst();
	DlgCreateInst(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};
class DlgCreateQuery : public DlgModal {
private:
	int noassignedprops;
public:
	DlgCreateQuery();
	~DlgCreateQuery();
	DlgCreateQuery(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

class DlgDeleteInst : public DlgModal {

public:
	DlgDeleteInst();
	~DlgDeleteInst();
	DlgDeleteInst(HINSTANCE hThis, int name, HWND parent,mainWindow *mw);
	BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
};

typedef struct _MYDATA
{
	char szTest1[80];		// a test buffer containing the file selected
	char szTest2[80];       // a test buffer containing the file path
} MYDATA, FAR * LPMYDATA;


class DlgFileIo : public DlgModal{
private:
	HWND hWnd;
   HWND hwndEdit;
   HWND hWndParent;

   HANDLE hFile;
   DWORD dwBytesRead;
	DWORD dwFileSize;
	OPENFILENAME OpenFileName;
	TCHAR szFile[MAX_PATH];
	char *lpBufPtr;
	MYDATA sMyData;
   int mode;

public:
	DlgFileIo();
   DlgFileIo(HINSTANCE hThis,int name,HWND parent,mainWindow *mw);
   ~DlgFileIo();
   BOOL callbackfn(HWND h, UINT mess, WPARAM wParam, LPARAM lParam);
   BOOL NEAR PASCAL TestNotify(HWND hDlg, LPOFNOTIFY pofn);
   void ProcessCDError(DWORD dwErrorCode, HWND hWnd);
   int create(mainWindow *mwh);
   };

mainWindow *mainWinHandle;
#endif
