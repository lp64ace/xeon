#pragma once

#include <chrono>

#include <windows.h>
#include <vector>
#include <cstring>
#include <string>
#include <cassert>

#include "box.h"

class Application;
class WND;

extern Application *APP;

void init_api ( ) ;

class WND {
	bool inited ;
protected:
	WND *daddy;
	virtual void Start ( ) { }
public:
	HWND hwnd ;
	HDC hdc ;
	std::vector<WND *> children ;

	WND ( );
	virtual ~WND ( ) ;

	box position ;
	bool locked;

	virtual int Create ( WND *daddy , const TCHAR *descr , const box &b , int wincl = 0 ) ;
	virtual LRESULT Dispatch ( UINT message , WPARAM wParam , LPARAM lParam ) ;
	void Show ( int command = SW_SHOW ) ;
	void Hide ( ) { Show ( SW_HIDE ); }
	void CreateTimer ( int TimerID , int ms );

	void DisableMovement ( ) { locked = true ; }
	void EnableMovement ( ) { locked = false; }

	void CreateContext ( ) ;

	WND *FindChildren ( const wchar_t *name ) ;
	WND *FindChildren ( HWND child ) ;

	void Update ( ) ;
	WND *parent ( ) { return daddy ; }
};

class Application {
	HINSTANCE hThisInstance , hPrev;
	std::chrono::system_clock::time_point start ;
public:
	HFONT font ;
	std::vector<WNDCLASSEX> wincl;
	Application ( HINSTANCE hThisInstance , HINSTANCE hPrev , COLORREF background ) ;
	int addWndClass ( const WNDCLASSEX &wndcl ) { wincl.push_back ( wndcl ) ; }
	HINSTANCE Instance ( ) const { return hThisInstance ; }
	const WNDCLASSEX *getWndClass ( int wcid ) const { return ( wincl.size ( ) > wcid ) ? &wincl [ wcid ] : NULL ; }
	std::vector<WND *> w ;

	float time ( ) ;
};

WNDCLASSEX simpleWinClass ( HINSTANCE hThisInstance , HINSTANCE hPrev , const wchar_t *name , int icon_define , int small_define , COLORREF background ) ;
