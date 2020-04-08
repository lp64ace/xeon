#include <iostream>

#include "./wnd/wndlist.h"

WND *wnd ;

int main ( void ) {
	init_api ( ) ;
	
	wnd = new WND ( ) ;
	wnd->Create ( NULL , ( const TCHAR * ) L"Xeon" , box ( CW_USEDEFAULT , CW_USEDEFAULT , 1024 , 1024 ) ) ;
	wnd->Show ( ) ;

	wnd->CreateContext ( ) ;

	// από default είναι uncapped `APP->setFpsCap ( FPS_UNCAPPED ) ;`
	APP->setFpsCap ( 60.0f ) ;

	while ( APP->PollWindowsEvents ( ) ) {
		APP->appSwapBuffers ( ) ;
	}

	return 0;
}
