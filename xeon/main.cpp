#include <iostream>

#include "./wnd/wnd.h"
#include "./wnd/wndlist.h"

class Window : public WND {
	RECT rect ;
public:
	LRESULT Dispatch ( UINT message , WPARAM wParam , LPARAM lParam ) override {
		switch ( message ) {
			case WM_SIZE: {
				GetClientRect ( hwnd , &rect ) ;
				// glViewport ( 0 , 0 , rect.right - rect.left , rect.bottom - rect.top ) ;
			}break;
		}
		return  DefWindowProc ( hwnd , message , wParam , lParam ) ;
	}
}*wnd;

int main ( void ) {
	init_api ( ) ;
	
	wnd = new Window ( ) ;
	wnd->Create ( NULL , ( const TCHAR * ) L"Xeon" , box ( CW_USEDEFAULT , CW_USEDEFAULT , 512 , 512 ) ) ;
	wnd->Show ( ) ;

	wnd->CreateContext ( ) ;

	MSG messages;

	while ( TRUE ) {

		while ( PeekMessage ( &messages , NULL , 0 , 0 , PM_REMOVE ) ) {
			TranslateMessage ( &messages );
			DispatchMessage ( &messages );
		}
		if ( messages.message == WM_QUIT ) {
			break;
		}

		// draw

		SwapBuffers ( wnd->hdc ) ;

	}

	return 0;
}
