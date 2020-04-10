#include "wndlist.h"

inline HWND mCreateWindow(WND *dad, const char *class_name, int style, const TCHAR *descr, const Box &b, int ws_ex_command = 0) {
	wprintf(L"Window Class Name::%s - Title::%s\n", class_name, descr);
	HWND hwnd = CreateWindowEx(ws_ex_command,(LPCSTR)class_name, descr, style, b.left, b.top, b.right, b.bottom,
		(dad == NULL) ? HWND_DESKTOP : dad->hwnd, NULL, APP->Instance(), NULL);
	SendMessage(hwnd, WM_SETFONT,(WPARAM) APP->font, TRUE);
	assert(hwnd);
	return hwnd;
}

int WND::Create(WND *daddy1, const TCHAR *descr, const Box &b, int wincl) {
	daddy = daddy1;
	hwnd = mCreateWindow(daddy,(const char *)APP->getWndClass(wincl)->lpszClassName, WS_VISIBLE | WS_OVERLAPPEDWINDOW, descr, b);
	if(daddy)daddy->children.push_back(this);
	return(hwnd) ? 1 : 0;
}


int ChildWND::Create(WND *daddy1, const TCHAR *descr, const Box &b, int wincl) {
	daddy = daddy1;
	hwnd = mCreateWindow(daddy,(const char *) APP->getWndClass(wincl)->lpszClassName, WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | ES_AUTOHSCROLL, descr, b);
	if(daddy)daddy->children.push_back(this);
	return(hwnd) ? 1 : 0;
}

int EditWND::Create(WND *daddy1, const TCHAR *descr, const Box &b, int wincl) {
	daddy = daddy1;
	hwnd = mCreateWindow(daddy, "EDIT", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, descr, b);
	if(daddy)daddy->children.push_back(this);
	return(hwnd) ? 1 : 0;
}

int MultiLineEditWND::Create(WND *daddy1, const TCHAR *descr, const Box &b, int wincl) {
	daddy = daddy1;
	hwnd = mCreateWindow(daddy, "EDIT", WS_BORDER | WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_VSCROLL | ES_MULTILINE | ES_AUTOHSCROLL, descr, b);
	if(daddy)daddy->children.push_back(this);
	return(hwnd) ? 1 : 0;
}

int DropDownWND::Create(WND *daddy1, const TCHAR *descr, const Box &b, int wincl) {
	daddy = daddy1;
	hwnd = mCreateWindow(daddy, "COMBOBOX", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE, descr, b);
	if(daddy)daddy->children.push_back(this);
	return(hwnd) ? 1 : 0;
}

int ComboBoxWND::Create(WND *daddy1, const TCHAR *descr, const Box &b, int wincl) {
	daddy = daddy1;
	hwnd = mCreateWindow(daddy, "COMBOBOX", LBS_COMBOBOX | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE, descr, b);
	if(daddy)daddy->children.push_back(this);
	return(hwnd) ? 1 : 0;
}

