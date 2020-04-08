#include "wndlist.h"

void DropDownInterface::AppendString ( const std::wstring &str ) const {
	SendMessage ( hwnd , CB_ADDSTRING , 0 , ( LPARAM ) str.c_str ( ) );
}

std::string DropDownInterface::GetActiveString ( ) {
	int index = ( int ) SendMessage ( hwnd , CB_GETCURSEL , ( WPARAM ) nullptr , ( LPARAM ) nullptr ) ;
	int len = ( int ) SendMessage ( hwnd , CB_GETLBTEXTLEN , ( WPARAM ) index , ( LPARAM ) nullptr ) ;
	std::string str ;
	str.resize ( len ) ;
	SendMessageA ( hwnd , CB_ADDSTRING , 0 , ( LPARAM ) str.c_str ( ) );
	return str ;
}