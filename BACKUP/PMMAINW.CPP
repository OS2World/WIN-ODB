
//Class methods for Class Window
HWND Window::GetFrameHandle(void) {
	return hWndFrame;
	}

HWND Window::GetClient(void) {
	return hWndClient;
	}

BOOL Window::ShowWindow(int nCmdShow) {
	return WinShowWindow(hWndFrame,nCmdShow);
	}

BOOL Window::Update(void) {
	return WinUpdateWindow(hWndFrame);
	}




//Class methods of class Main
// Standard PM Initilization sequence
BOOL Main::Initialize(void) {
	if ((hab = WinInitialize(NULL)) == (HAB)NULL)
		return FALSE;
	if ((hmq = WinCreateMsgQueue(NULL,0)) == (HMQ)NULL)
		return FALSE;
        return TRUE;
    }



// Standard PM Termination sequence
void Main::Terminate(void) {
	WinDestroyMsgQueue( hmq );
	WinTerminate( hab );
    }




// Standard PM Message Loop
BOOL Main::MessageLoop(void) {
BOOL rc;
QMSG qmsg;
	while((rc = (BOOL)WinGetMsg(hab,&qmsg,(HWND)NULL,0,0))!=FALSE)
		WinDispatchMsg(hab,&qmsg);
	return rc;
}


void MainWindow::Register(void){
	if (!WinRegisterClass(Main::hab,(PSZ)szClassName,
	   ::WndProc,              // use _export WndProc
                                    // which will vector the
                                    // call to MainWindow::WndProc
            CS_SIZEREDRAW,
	    sizeof(Window *)))      // Allocate space for the "this"
                                    // pointer after the end of the
                                    // window record PM creates.
                                    // This is used to vector to
                                    // MainWindow::WndProc

		AbortHello(NULL,NULL);
	initialized = TRUE;
}

MainWindow::MainWindow(void){
	if (initialized == FALSE) Register();
	ULONG flCreate = FCF_STANDARD & // Set frame control flags
             ~FCF_SHELLPOSITION;

        Main::tempthis = this;          // Stuff "this" pointer in global
                                        // for use by _export WndProc
                                        // _export WndProc is called
                                        // directly as a result of
                                        // WndCreateStdWindow and will
                                        // shove this in the PM window
                                        // record and use it to vector to
                                        // MainWindow::WndProc

        if ((hWndFrame = WinCreateStdWindow(
               HWND_DESKTOP,            // Desktop window is parent
               0,
               &flCreate, 
               (PSZ)szClassName,
               (PSZ)"",
               0, 
               (HMODULE)NULL,           // Resource is in .EXE file
               ID_WINDOW,               // Frame window identifier 
               &hWndClient 
               )) == (HWND)NULL) AbortHello(hWndFrame, hWndClient);

        // Set title bar fo window
	WinSetWindowText(hWndFrame, (PSZ)"ODB For OS/2 Warp");

        if (!WinSetWindowPos( hWndFrame,
                       HWND_TOP,            // window at position 20, 100,
                       20, 100, 600, 300,   // and size 600, 300.
                       // Flags set to show and activate frame window
                       SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW
                     ))
        AbortHello(hWndFrame, hWndClient); /* Terminate the application    */
    }
    // Destory window when object goes out of scope
//MRESULT WndProc(HWND hWnd,ULONG iMessage,MPARAM mParam1,MPARAM mParam2);
MainWindow::~MainWindow(void){
	WinDestroyWindow(hWndFrame);
}

int MainWindow::Drawtypetree(HPS hps,RECTL  rc,POINTL pt, int i,int j,char *tp){
char *nm;
	if (tp==NULL) return i;
	if (strlen(tp)==0) return i;
	else {
		pt.x = j*TYPETREEOFFSET;
		pt.y = rc.yTop-i*TYPETREEOFFSET;
		WinDrawPointer(hps,pt.x,pt.y,Main::icon[0],DP_NORMAL);
		pt.x = pt.x+TYPETREEOFFSET;
		pt.y=pt.y+10;
		GpiCharStringAt( hps, &pt, strlen(tp),(PSZ)tp);
		nm=odb.getsubtypes(tp);
		while (nm!=0){
			i++;
			i=Drawtypetree(hps,rc,pt,i,j+1,nm);
			nm=odb.getsubtype(tp);
			}
	}
	return i;
}

// MainWindow object window procedure
MRESULT MainWindow::WndProc(HWND hWnd,ULONG iMessage,MPARAM mParam1,MPARAM mParam2 ){
int i;
HPS    hps;
RECTL  rc;
POINTL pt;
	switch( iMessage ){
		case WM_CREATE: // Do window initialization stuff here
				// WinLoadPointer loads icons from the exe.
			Main::icon[1] = WinLoadPointer(HWND_DESKTOP,(HMODULE)NULL,ID_ICON1);
			Main::icon[2] = WinLoadPointer(HWND_DESKTOP,(HMODULE)NULL,ID_ICON2);
			Main::icon[3] = WinLoadPointer(HWND_DESKTOP,(HMODULE)NULL,ID_ICON3);
			break;
		case WM_COMMAND:{
			USHORT command;
			command = SHORT1FROMMP(mParam1);      // Extract the command value
			switch (command){
				case ID_OPTION1:
					FileOpen(mParam2);
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					break;
				case ID_OPTION2://{
					//DbOutDlg DbOutDlg(HWND_DESKTOP,hWndFrame,
					//			(HMODULE)NULL,
					//			(ULONG)DBOUTPUT,
					//			(PVOID)0);
					//DbOutDlg.Run();  }
					FileSave(mParam2);
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					break;
				case ID_NEWDB:{
					DbCreateDlg DbCreateDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)DLG_CREATEDB,
								(PVOID)0);
					DbCreateDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;
				case ID_DELETETYPE:{
					TpDeleteDlg TpDeleteDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)40,
								(PVOID)0);
					TpDeleteDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;
				case ID_UPDATEINST:{
					InstUpdtDlg InstUpdtDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)10,
								(PVOID)0);
					InstUpdtDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;				case ID_DELETEPROP:{
					PropDeleteDlg PropDeleteDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)30,
								(PVOID)0);
					PropDeleteDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;				case ID_INSPECTDB:{
					DbInspectDlg DbInspectDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)DLG_INSPECTDB,
								(PVOID)0);
					DbInspectDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;
				case ID_INSPECTTYPE:{
					TpInspectDlg TpInspectDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)DLG_INSPECTTYPE,
								(PVOID)0);
					TpInspectDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;
				case ID_INSPECTINST:{
					InstInspectDlg InstInspectDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)DLG_INSPECTINSTANCE,
								(PVOID)0);
					InstInspectDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;
				case ID_DELETEINST:{
					InstInspectDlg InstInspectDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)20,
								(PVOID)0);
					InstInspectDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;				case ID_CREATETYPE:{
					CreTpDlg CreTpDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)DLG_CRETP,
								(PVOID)0);
					CreTpDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;
				case ID_CREATEINST: {
					CreInstDlg CreInstDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)DLG_CREINST,
								(PVOID)0);
					CreInstDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;
				case ID_CREATEPROP:{
					CrePropDlg CrePropDlg(HWND_DESKTOP,hWndFrame,
								(HMODULE)NULL,
								(ULONG)DLG_CREPROP,
								(PVOID)0);
					CrePropDlg.Run();
					WinInvalidateRegion( hWnd, (HRGN)NULL, FALSE );
					}
					break;
				case ID_EXITPROG:
					WinPostMsg( hWnd, WM_CLOSE, 0, 0 );
					break;
				default:
					return WinDefWindowProc( hWnd, iMessage , mParam1, mParam2 );
					}
				break;
			} //end WM_COMMAND
		case WM_ERASEBACKGROUND: // Return TRUE to cause PM to paint the window background
					// in SYSCLR_WINDOW.
			return (MRESULT)( TRUE );
		case WM_PAINT:{ // Window draw stuff here
			i=1;
			hps = WinBeginPaint( hWnd, (HPS)NULL, &rc );
			GpiSetColor( hps, CLR_NEUTRAL );         // set text color
			GpiSetBackColor( hps, CLR_BACKGROUND );  // set text background
			GpiSetBackMix( hps, BM_OVERPAINT );      // set text mix
			WinQueryWindowRect(hWnd,&rc);            // get window bounds
			WinFillRect(hps,&rc,CLR_BACKGROUND);
			//Draw the database barrel
			pt.x = 1; //rc.xRight/2-i*4;
			pt.y = rc.yTop-i*40;
			Main::icon[0] = Main::icon[1];
			WinDrawPointer(hps,pt.x,pt.y,Main::icon[0],DP_NORMAL);
			pt.x=pt.x+36;
			pt.y=pt.y+10;
			strcpy(Main::szDatabase,odb.get_dbname());
			GpiCharStringAt( hps, &pt, strlen(Main::szDatabase),(PSZ)Main::szDatabase);
			i++;
			//draw all the types
			Main::icon[0] = Main::icon[2];
			Drawtypetree(hps,rc,pt,i,1,"Usertypes");
			WinEndPaint( hps );
			break;
		}   //End WM_PAINT
		case WM_CLOSE: // Do termination stuff and cleanup here
			if(WinMessageBox(HWND_DESKTOP,hWndFrame,(PCH) "Quit this application?",
			(PCH)"Sanity Check",0,
			MB_NOICON|MB_OKCANCEL|MB_MOVEABLE)==MBID_OK)
			WinPostMsg( hWnd, WM_QUIT, 0, 0 ); // Cause termination
			break;
		default:
			return WinDefWindowProc( hWnd, iMessage , mParam1, mParam2 );
	}
		return FALSE;
}
