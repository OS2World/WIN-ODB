//--------------------------------------------------------------------
// Project: Odb for Windows 95
// descriptopn:MainWindow in GUI
// Platform:Windows95
//Compiler:Borland v5
//--------------------------------------------------------------------

#include "d:\users\staffan\odb\odb95\odb95.h"
#include <process.h>

//---------------------------------------------
// Name:       mainWindow
// Description:Default constructor
// Arguments  :None
// Author     :Staffan Flodin
//---------------------------------------------
mainWindow::mainWindow(void){
	mainW=this;
   currentregion=NULL;
   }

//---------------------------------------------
// Name:       mainWindow
// Description:Constructor
// Arguments  :Handle to instances
// Author     :Staffan Flodin
//---------------------------------------------
mainWindow::mainWindow(HINSTANCE hThis,HINSTANCE p){
	parent=p;
   mainW=this;
   currentregion=NULL;
	this->registerWindow(hThis);
}

//---------------------------------------------
// Name:       egisterWindow
// Description:Registers window class
// Arguments  :Instance handle
// Author     :Staffan Flodin
//---------------------------------------------
int mainWindow::registerWindow(HINSTANCE hThis){
	wcl.hInstance=hThis;
	wcl.lpszClassName=TEXT(WINNAME);
	wcl.lpfnWndProc=WindowFunc;
	wcl.style=CS_DBLCLKS;
	//wcl.cbSize=sizeof(WNDCLASS);
	wcl.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	//wcl.hIconSm=LoadIcon(NULL,IDI_WINLOGO);
	wcl.hCursor=LoadCursor(NULL,IDC_ARROW);
	wcl.lpszMenuName=TEXT("MENU_1");
	wcl.cbClsExtra=0;
	wcl.cbWndExtra=0;
	wcl.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);

	dbIco=LoadIcon(hThis,MAKEINTRESOURCE(DATABASEICON));
	typeIco=LoadIcon(hThis,MAKEINTRESOURCE(CLASSICON));

	thisInstance=hThis;

	//Register this window class
	if (!RegisterClass(&wcl)) return 0;

	else {
		//Create the window since it is registered
		hWndMain=CreateWindow(TEXT(WINNAME),TEXT(WINTITLE),
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT,CW_USEDEFAULT,
					400,
					500,HWND_DESKTOP,
					NULL,hThis,NULL);
		
		return 1;
	}
}

//---------------------------------------------
// Name       :create
// Description:creates the main window
// Arguments  :the mode
// Author     :Staffan Flodin
//---------------------------------------------
int mainWindow::create(int nWinMode){
int userResponse=0;

//Display this window
	ShowWindow(hWndMain,nWinMode);
	UpdateWindow(hWndMain);
	return 1;
}

//---------------------------------------------
// Name       :process messages
// Description:the local callback. Called by the
//             global callback which passes the
//             arguments to this.
// Arguments  :winhandle, message, wparam, lparam
// Author     :Staffan Flodin
//---------------------------------------------
LONG mainWindow::processMessages(HWND hWndThis,UINT message,WPARAM wParam, LPARAM lParam){
HDC hdc;                       	//Handle to device context
PAINTSTRUCT paintStruct;
int userResponse=0;
ODB_NAME odbName;
int x,y;


	paintStruct.fErase=1;
   switch (message) {
   	case WM_LBUTTONDBLCLK:
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			currentregion=rg->findRegion(x,y);
			if (currentregion==NULL) break;
         INVOKEDIALOG(DLG_POPUP,DlgPopup);
         InvalidateRect( hWndThis, NULL, TRUE );
			break;
   case WM_LBUTTONUP:
   		x=LOWORD(lParam);
			y=HIWORD(lParam);
         hdc=GetDC(hWndThis);

         // Deselect current selection
         if (currentregion!=NULL) unHighlightSelection(currentregion, hdc);

         // Process current selection
			currentregion=rg->findRegion(x,y);
			if (currentregion!=NULL) highlightSelection(currentregion, hdc);
         
         ReleaseDC(hWndThis,hdc);
         break;
	case WM_COMMAND:
   	switch(LOWORD(wParam)){
		case ID_OPTION1:
			INVOKEDIALOG(LOADDATABASE,DlgFileIo)
         currentregion=NULL;
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_OPTION2:
			INVOKEDIALOG(SAVEDATABASE,DlgFileIo)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_NEWDB:
			INVOKEDIALOG(DLG_CREATEDB,DlgCreateDb)
         currentregion=NULL;
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
      case ID_OPTION3:
         INVOKEDIALOG(LOADODBQL,DlgFileIo)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_DELETETYPE:
			INVOKEDIALOG(DLG_DELETETYPE,DlgDeleteType)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
      case ID_QUERYDB:
			INVOKEDIALOG(DLG_CREATEQUERY,DlgCreateQuery)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_UPDATEINST:
			INVOKEDIALOG(DLG_UPDATEINST,DlgUpdateInst)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_DELETEPROP:
			INVOKEDIALOG(DLG_DELETEPROP,DlgDeleteProp)
			 InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_INSPECTDB:
			INVOKEDIALOG(DLG_INSPECTDB,DlgInspectDb)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_INSPECTTYPE:
			INVOKEDIALOG(DLG_INSPECTTYPE,DlgInspectTp)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_INSPECTINST:
			INVOKEDIALOG(DLG_INSPECTINSTANCE,DlgDeleteInst)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_DELETEINST:
			INVOKEDIALOG(DLG_DELETEINST,DlgDeleteInst)
			InvalidateRect( hWndThis, NULL, TRUE );
			  break;
 		case ID_CREATETYPE:
			INVOKEDIALOG(DLG_CRETP,DlgCreateType)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_CREATEINST:
			INVOKEDIALOG(DLG_CREINST,DlgCreateInst)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_CREATEPROP:
			INVOKEDIALOG(DLG_CREPROP,DlgCreateProp)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		case ID_EXITPROG:
			PostMessage( hWndThis, WM_DESTROY, 0, 0 );
			break;
      case 264:
      	spawnlp(P_NOWAIT,"notepad.exe","notepad.exe",NULL);
         break;
		default: break;
		}
		break;
	case WM_DESTROY:
		userResponse=MessageBox(hWndThis,"Quit ODB for Win95?",
					MESSAGEALERT,MB_OKCANCEL);
		if (userResponse==IDOK){
      	odb.initialize();
			PostQuitMessage(0);
         }
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWndThis,&paintStruct);
		EndPaint(hWndThis,&paintStruct);
		hdc=GetDC(hWndThis);
		DrawIcon(hdc,0,0,dbIco);
		odbName=odb.get_dbname();
		TextOut(hdc,32,7,"                         ",25);
		TextOut(hdc,32,7,odbName,strlen(odbName));
		if (rg==NULL) rg=new region();
		else rg->invalidate();
		Drawtypetree(hdc,1,1,"Usertypes",rg);
      if ((currentregion!=NULL)&&(!currentregion->invalidRegion()))
      	highlightSelection(currentregion, hdc);
		ReleaseDC(hWndThis,hdc);
		//hdc=GetDC(hWndThis);
		//hBr=CreatePatternBrush((HGDIOBJ)dbIco);
		//SelectObject(hdc,hBr);
		//GetBitmapDimensionEx(dbIco,&icoSize);
		//PatBlt(hdc,0,0,32,32,PATCOPY);
		//ReleaseDC(hWndThis,hdc);
		break;
       default: return DefWindowProc(hWndThis,message,wParam,lParam);
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
DlgModal *mainWindow::getDlg(void){
	return currentDlg;
}

//---------------------------------------------
// Name       :getDb
// Description:Returns the database in use
// Arguments  :none
// Author     :Staffan Flodin
//---------------------------------------------
database *mainWindow::getDb(void){
	return &odb;
}

//---------------------------------------------
// Name       :drawTypeTree
// Description:Draws the type tree.
// Arguments  :dvice context,x cord, y xord, typename
// Author     :Staffan Flodin
//---------------------------------------------
int mainWindow::Drawtypetree(HDC hdc, int i,int j,char *tp,region *r){
char *nm;
int x, y;
	if (tp==NULL) return i;
	if (strlen(tp)==0) return i;
	else {
		x = j*TYPETREEOFFSET;
		y = i*TYPETREEOFFSET;
      DrawIcon(hdc,x,y,typeIco);
      TextOut(hdc,32+x,7+y,"                         ",25);
		TextOut(hdc,32+x,7+y,tp,strlen(tp));
      r->set(x,y,x+TYPETREEOFFSET+10*strlen(tp),y+TYPETREEBOUNDING);
      r->setName(tp);
		nm=odb.getsubtypes(tp);
			while (nm!=0){
			i=Drawtypetree(hdc,i+1,j+1,nm,r->getNextToSet());
			nm=odb.getsubtype(tp);
			}
	}
	return i;
}

void mainWindow::highlightSelection(region *r, HDC h){
	SelectObject(h,GetStockObject(BLACK_PEN));
   drawSelectedBox(r,h);
}

void mainWindow::unHighlightSelection(region *r, HDC h){
	SelectObject(h,GetStockObject(WHITE_PEN));
   drawSelectedBox(r,h);
}

void mainWindow::drawSelectedBox(region *r, HDC h){
   MoveToEx(h,r->getX1(),r->getY1(),NULL);
   LineTo(h,r->getX2(),r->getY1());
   MoveToEx(h,r->getX2(),r->getY1(),NULL);
   LineTo(h,r->getX2(),r->getY2());
   MoveToEx(h,r->getX2(),r->getY2(),NULL);
   LineTo(h,r->getX1(),r->getY2());
   MoveToEx(h,r->getX1(),r->getY2(),NULL);
   LineTo(h,r->getX1(),r->getY1());
}

region *mainWindow::getCurrentRegion(void){
	return currentregion;
}
