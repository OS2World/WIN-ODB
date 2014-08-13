#include <windows.h>

#define ODBGIS "ODB Gis"

LRESULT CALLBACK wndCallBack(HWND,UINT,WPARAM,LPARAM);

class mainWindow {
	protected:
   	HDC                hdc;

      char szWinName[32];
      HWND hwnd;
		MSG msg;
		WNDCLASSEX wcl;
      HINSTANCE hThisInst;
   public:
   	mainWindow(void);
   	mainWindow(HINSTANCE hThis);
      mainWindow(HINSTANCE hThis,char *menuName);
      ~mainWindow(void);
      mainWindow *hWindow;
      mainWindow *getWindowHandle(void);
      void setWindowHandle(mainWindow *hW);
      int registerWindow(void);
      void createWindow(int );

      void setPoint(void);

		//static LRESULT CALLBACK wndCallBack(HWND,UINT,WPARAM,LPARAM);

};


mainWindow *hMw;



