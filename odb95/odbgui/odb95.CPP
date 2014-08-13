//----------------------------------------------------
//	Project		:	ODB GUI
//	File		:	odb95.cpp
//	Description	:	Implementation of GUI classes and macros
//	Environment	:	Borland C++ or Visual C++
//	Author		:	Staffan Flodin
//----------------------------------------------------
#include "odb95res.h"
#include "odb95.h"



//
// A global pointer to the mainwindow
//
mainWindow *mainWinHandle=NULL;


//
// A global to hold the currently selected object
//
ODB_REF selectedObject=NULL;






//----------------------------------------------------
//	Method		:	
//	Description	:	Win Main called from Win95, 
//					corresponds to main
//	Author		:	Staffan Flodin
//----------------------------------------------------
int APIENTRY WinMain(HINSTANCE hThis, HINSTANCE hPrev, LPSTR lpszArgs,int nWinMode)
{
MSG msg;

	mainWinHandle=new mainWindow(hThis,hPrev);
	mainWinHandle->create(nWinMode);


	//Create message loop
	while (GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}





//----------------------------------------------------
//	Method		:	
//	Description	:	Main window callback 
//	Author		:	Staffan Flodin
//----------------------------------------------------
LONG APIENTRY WindowFunc(HWND hWndThis,UINT message,
						 WPARAM wParam, LPARAM lParam)
{

	return mainWinHandle->processMessages(hWndThis,message,wParam,lParam);

}




//----------------------------------------------------
//	Method		:	
//	Description	:	Modal Dialog callback 
//	Author		:	Staffan Flodin
//----------------------------------------------------
BOOL APIENTRY dlgFn(HWND hw, UINT mess, WPARAM wp, LPARAM lp){
DlgModal *dlgH;

	//
	// Get the dialog handle from the main window
	// handle
	//
	if (mainWinHandle!=NULL)
	{
	
		dlgH=mainWinHandle->getDlg();
		
		if(dlgH!=NULL) return dlgH->callbackfn(hw,mess,wp,lp);
		else MessageBox(hw,"Dialig Object Do Not Exist",MESSAGEALERT,MB_OK);
	
	}
	
	else return 0;
	
	return 0;

}
