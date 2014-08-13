//----------------------------------------------------
//	Project		:	ODB GUI Tool
//	File		:	MainWin.cpp
//	Description	:	Implementation of MainWin class
//	Environment	:	Borland C++ or Visual C++
//	Author		:	Staffan Flodin
//----------------------------------------------------
#include "odb95.h"
#include <process.h>
#include <math.h>
#include <stdlib.h>




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

	currentDlg=NULL;
	rg=NULL;
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

	wcl.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	//wcl.hIconSm=LoadIcon(NULL,SMALLICON);
	wcl.hCursor=LoadCursor(NULL,IDC_ARROW);
	wcl.lpszMenuName=TEXT("MENU_1");
	
	wcl.cbClsExtra=0;
	wcl.cbWndExtra=0;
	wcl.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);

	dbIco=LoadIcon(hThis,MAKEINTRESOURCE(DATABASEICON));
	typeIco=LoadIcon(hThis,MAKEINTRESOURCE(CLASSICON));
   volTypeIco=LoadIcon(hThis,MAKEINTRESOURCE(7));

	thisInstance=hThis;

	//Register this window class
	if (!RegisterClass(&wcl)) return 0;
	else 
	{//Create the window since it is registered
	
		//hWndMain=CreateWindow(TEXT(WINNAME),TEXT(WINTITLE),
		//			WS_OVERLAPPEDWINDOW,
		//			CW_USEDEFAULT,CW_USEDEFAULT,
		//			400,
		//			500,HWND_DESKTOP,
		//			NULL,hThis,NULL);
		
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

	hWndMain=CreateWindow(TEXT(WINNAME),TEXT(WINTITLE),
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT,CW_USEDEFAULT,
					400,
					500,HWND_DESKTOP,
					NULL,thisInstance,NULL);

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
int x,y,kuk;
HMENU hmH;
POINT pt;
char warning[255];

	paintStruct.fErase=1;
	if (message==WM_RBUTTONUP) message=WM_LBUTTONDBLCLK;
	
	switch (message) {
	case WM_MOUSEMOVE:
		//
		// Follow mouse with selected type in tree
		// drawn on screen
		//

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

   	case WM_LBUTTONDBLCLK:
		x=LOWORD(lParam);
		y=HIWORD(lParam);
		pt.x=x;
		pt.y=y;
		ClientToScreen(hWndThis,&pt);
		currentregion=rg->findRegion(x,y);
		if (currentregion==NULL) break;
        //INVOKEDIALOG(DLG_POPUP,DlgPopup);
		
		hmH=CreatePopupMenu();
		AppendMenu(hmH,MFT_STRING,ID_CREATETYPE,"Create SubType");
		AppendMenu(hmH,MFT_STRING,ID_CREATEPROP,"Create Property");
		AppendMenu(hmH,MFT_STRING,ID_CREATEINST,"Create Instance");
		AppendMenu(hmH,MFT_STRING,ID_INSPECTINST,"Inspect");
		AppendMenu(hmH,MFT_STRING,ID_DELETETYPE,"Delete");
		TrackPopupMenu(hmH,TPM_VCENTERALIGN,pt.x,pt.y,0,hWndThis,NULL);
        InvalidateRect( hWndThis, NULL, TRUE );
		DestroyMenu(hmH);
		
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
		case ID_OPEN:
			INVOKEDIALOG(LOADDATABASE,DlgFileIo)
			currentregion=NULL;
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
	
		case ID_SAVE:
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

      case 280:
			INVOKEDIALOG(DLG_POPUP,DlgPopup)
			InvalidateRect( hWndThis, NULL, TRUE );
			break;
		
		case ID_EXITPROG:
			PostMessage( hWndThis, WM_DESTROY, 0, 0 );
			break;
      
		//case ID_ODBHELP:
      case 2790:
			{
			char *browser=NULL,*url=NULL;

			browser=getenv(BROWSERENV);
			url=getenv(HELPURLENV);

			if (browser==NULL) browser=DEFAULTBROWSER;
			if (url==NULL) url=DEFAULTHELPURL;

			if (spawnlp(P_NOWAIT,browser,url,url,NULL)<0)
			{

				sprintf(warning,"The env vars. %s, %s references %s, %s which could not be launced.",
						BROWSERENV,HELPURLENV,browser,url);
				MessageBox(hWndThis,warning,MESSAGEALERT,MB_OK);

			}

			break;
			}


		case 264:
			{
			char *editor=NULL;

			editor=getenv(EDITORENV);

			if (editor==NULL)  editor=NULLSTRING;

			if (spawnlp(P_NOWAIT,editor,editor,NULL)<0)
			{


				sprintf(warning,"The env var. %s references %s which could not be launced. Launch %s instaed?",
						EDITORENV,editor,DEFAULTEDITOR);
				if (MessageBox(hWndThis,warning,MESSAGEALERT,MB_OKCANCEL)==IDOK)
					spawnlp(P_NOWAIT,DEFAULTEDITOR,DEFAULTEDITOR,NULL);
			}

			break;
			}

		default: break;
		}

	
	break;
	
	case WM_DESTROY:
		userResponse=MessageBox(hWndThis,"Quit ODB for Win95?",
					MESSAGEALERT,MB_OKCANCEL);

		if (userResponse==IDOK)
		{
      	
			odb.initialize();
			PostQuitMessage(0);
         
		}
		break;
	
	case WM_PAINT:
		hdc=BeginPaint(hWndThis,&paintStruct);
		EndPaint(hWndThis,&paintStruct);
		hdc=GetDC(hWndThis);
		kuk=0;
		DrawIcon(hdc,kuk,kuk,dbIco);
		odbName=odb.get_dbname();
		TextOut(hdc,32,7,"                         ",25);
		TextOut(hdc,32,7,odbName,strlen(odbName));

		if (rg==NULL) rg=new region();
		else rg->invalidate();
		
		Drawtypetree(hdc,1,1,"Usertypes",rg,0);
		
		if ((currentregion!=NULL)&&(!currentregion->invalidRegion()))
      		highlightSelection(currentregion, hdc);
			ReleaseDC(hWndThis,hdc);
		break;

	case WM_KEYUP:
		{
		int key;
			key=(int)wParam;

			switch (key) {
			case UP_ARROW:
				hdc=GetDC(hWndThis);
				if (currentregion==NULL) currentregion=rg;
			
				if (currentregion!=NULL) 
				{
					unHighlightSelection(currentregion, hdc);
					currentregion=currentregion->getPrevious(rg);
					
					if (currentregion==NULL) currentregion=rg->getLast();
					
					if (currentregion!=NULL) 
						highlightSelection(currentregion, hdc);
				
				}
				
				ReleaseDC(hWndThis,hdc);
				break;
			
			case DOWN_ARROW:
				hdc=GetDC(hWndThis);
				if (currentregion==NULL) currentregion=rg;
				
				if (currentregion!=NULL)
				{
				
					unHighlightSelection(currentregion, hdc);
					currentregion=currentregion->getNext();
					if (currentregion==NULL) currentregion=rg;
					if (currentregion!=NULL) 
							highlightSelection(currentregion, hdc);
				
				}
				
				ReleaseDC(hWndThis,hdc);
				break;

			default: break;
			}


			break;
		}

	case WM_CHAR:
		{
		char ch;
		int fns;
		ch=(char)wParam;
		if (ch==(char)13)
		{
			//INVOKEDIALOG(DLG_POPUP,DlgPopup);
			InvalidateRect( hWndThis, NULL, TRUE );
		
		}
		
		fns=HIWORD(lParam);
		fns=fns&&256;
		break;
		
		}

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
int mainWindow::Drawtypetree(HDC hdc, int i,int j,char *tp,region *r,int vol){
char *nm;
int x, y;

	if (tp==NULL) return i;

	if (strlen(tp)==0) return i;
	else 
	{
	
		x = j*TYPETREEOFFSET;
		y = i*TYPETREEOFFSET;

      if (vol>0) DrawIcon(hdc,x,y,volTypeIco);
		else DrawIcon(hdc,x,y,typeIco);

		TextOut(hdc,32+x,7+y,"                         ",25);
		TextOut(hdc,32+x,7+y,tp,strlen(tp));
		r->set(x,y,x+TYPETREEOFFSET+10*strlen(tp),y+TYPETREEBOUNDING);
		r->setName(tp);
		nm=odb.getsubtypes(tp);



		while (nm!=0)
		{
		   if ((nm!=NULL)&&(strcmp(nm,"volatile") == 0)) vol=1;
			i=Drawtypetree(hdc,i+1,j+1,nm,r->getNextToSet(),vol);
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
