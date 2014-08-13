
#include "d:\users\staffan\odb\odb95\odb95res.h"
#include "d:\users\staffan\odb\odb95\odb95.h"



//Win Main called from Win95, corresponds to main
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


#pragma argsused
LONG APIENTRY WindowFunc(HWND hWndThis,UINT message,
													WPARAM wParam, LPARAM lParam)
{
	return mainWinHandle->processMessages(hWndThis,message,wParam,lParam);
}


#pragma argsused
BOOL APIENTRY dlgFn(HWND hw, UINT mess, WPARAM wp, LPARAM lp){
DlgModal *dlgH;
	if (mainWinHandle!=NULL){
		dlgH=mainWinHandle->getDlg();
		if(dlgH!=NULL) return dlgH->callbackfn(hw,mess,wp,lp);
		else MessageBox(hw,"Dialig Object Do Not Exist",MESSAGEALERT,MB_OK);
	}
	else return 0;
}
