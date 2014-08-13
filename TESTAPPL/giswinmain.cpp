#include <windows.h>
#include "..\testappl\giswinmain.h"
#include "..\testappl\wingismenu.h"


mainWindow::mainWindow(void){}


mainWindow::~mainWindow(void){}


mainWindow::mainWindow(HINSTANCE hThis){

	strcpy(szWinName,ODBGIS);

   hThisInst=hThis;

	wcl.hInstance=hThisInst;
   wcl.lpszClassName=szWinName;
   wcl.lpfnWndProc=wndCallBack;
   wcl.style=0;
   wcl.cbSize=sizeof(WNDCLASSEX);
   wcl.hIcon=LoadIcon(NULL,IDI_APPLICATION);
   wcl.hIconSm=LoadIcon(NULL,IDI_WINLOGO);
   wcl.hCursor=LoadCursor(NULL,IDC_ARROW);
   //wcl.lpszMenuName=MAINMENU;
   wcl.lpszMenuName=NULL;
   wcl.cbClsExtra=0;
   wcl.cbWndExtra=0;
   wcl.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);

}


mainWindow::mainWindow(HINSTANCE hThis,char *menuName){

	strcpy(szWinName,ODBGIS);

   hThisInst=hThis;

	wcl.hInstance=hThisInst;
   wcl.lpszClassName=szWinName;
   wcl.lpfnWndProc=wndCallBack;
   wcl.style=0;
   wcl.cbSize=sizeof(WNDCLASSEX);
   wcl.hIcon=LoadIcon(NULL,IDI_APPLICATION);
   wcl.hIconSm=LoadIcon(NULL,IDI_WINLOGO);
   wcl.hCursor=LoadCursor(NULL,IDC_ARROW);
   wcl.lpszMenuName=menuName;
   wcl.cbClsExtra=0;
   wcl.cbWndExtra=0;
   wcl.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);


}


mainWindow *mainWindow::getWindowHandle(void){

		return hWindow;
}


void mainWindow::setWindowHandle(mainWindow *hW){

	mainWindow::hWindow=hW;

}



int  mainWindow::registerWindow(void){

	if(!RegisterClassEx(&wcl)) return 0;

   mainWindow::setWindowHandle(this);

   return 1;
}

void mainWindow::createWindow(int mode){

	hwnd=CreateWindow(szWinName,ODBGIS,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,
                     CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,HWND_DESKTOP,
                     NULL,hThisInst,NULL);

   ShowWindow(hwnd,mode);
   UpdateWindow(hwnd);
}






LRESULT CALLBACK wndCallBack(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){

	switch (message) {
   	case WM_COMMAND:
      	switch (LOWORD(wParam)){
         case IDM_OPEN:
         					break;
			case IDM_CLOSE:
         					break;
			case IDM_SAVE:
         					break;
			case IDM_EXIT:
         					break;
			case IDM_POINT:
         					break;
			case IDM_LINE:
         					break;
			case IDM_BOUNDEDLINE:
         					break;
			case IDM_AREA:
         					break;
         }
         break;
         
   	case WM_DESTROY: PostQuitMessage(0); break;
      default: return DefWindowProc(hwnd,message,wParam,lParam);
   }

   return 0;
}



int WINAPI WinMain(HINSTANCE hThisInst,HINSTANCE hPrevInst,LPSTR lpszArgs,int nWinMode){
MSG msg;

   hMw= new mainWindow(hThisInst,MAINMENU);
   if(!hMw->registerWindow()) return 0;

   hMw->createWindow(nWinMode);


   while(GetMessage(&msg,NULL,0,0)){

   	TranslateMessage(&msg);
      DispatchMessage(&msg);

   }

   return msg.wParam;

}

