#include "wnd.h"

Application *APP = NULL;

#define FPS_UNCAPPED				100000.0f

static wchar_t className [ ] = L"Xeon::Window::Class";

Callbacks callbackfuncs;

void SetKeyCallbackFunc(void(*func)(int key, KeyState state)) {
	callbackfuncs.key = func;
}

void DelKeyCallbackFunc(void) {
	SetKeyCallbackFunc(0);
}

float Application::time() {
	return std::chrono::duration<float>(std::chrono::system_clock::now() - APP->start).count();
}

WND::WND() {
	assert(APP);
	APP->w.push_back(this);
	inited = false;
	hwnd = NULL;
	hdc = NULL;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT WND::Dispatch(UINT message, WPARAM wParam, LPARAM lParam) {
	if(!inited) {
		inited = true;
		Start();
		if(!hdc) hdc = GetDC(hwnd);
	}

	switch(message) {
		case WM_DESTROY: {
			DefWindowProc(hwnd, message, wParam, lParam);
			delete this;
		}return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void WND::Show(int option) {
	assert(hwnd);
	SendMessage(hwnd, WM_SETFONT,(WPARAM) APP->font, TRUE);
	ShowWindow(hwnd, option);
}

void WND::Update() {
	assert(hwnd);
	UpdateWindow(hwnd);
}

void WND::CreateTimer(int TimerID, int ms) {
	SetTimer(hwnd, TimerID, ms, NULL);
}

WND::~WND() {
	std::vector<WND *>::iterator itr;
	if(daddy && daddy->children.size()) {
		itr = find(daddy->children.begin(), daddy->children.end(), this);
		if(itr != daddy->children.end())
			daddy->children.erase(itr);
	}
	itr = find(APP->w.begin(), APP->w.end(), this);
#ifdef EXTRA_OPTIONS_ENABLED
	if(hdc)
		ReleaseDC(hwnd, hdc);
#endif
	APP->w.erase(itr);
	if(APP->w.empty())
		exit(0);
	printf("Closed window %d APP windows left", APP->w.size());
}

WND *WND::FindChildren(const wchar_t *name) {
	for(auto itr : children) {
		wchar_t title [ 256 ];
		GetWindowText(itr->hwnd, title, 256);
		if(std::wstring(title) == name)
			return itr;
	}
	return NULL;
}

WND *WND::FindChildren(HWND child) {
	for(auto itr : children) {
		if(itr->hwnd == child)
			return itr;
	}
	return NULL;
}

HFONT cfont(WND *win, int size) {
	int h = size;
	int w =(h + 1) / 2;
	int height = -MulDiv(h, GetDeviceCaps(win->hdc, LOGPIXELSY), 72);
	int width = -MulDiv(w, GetDeviceCaps(win->hdc, LOGPIXELSX), 72);

	return CreateFont(height, width, 0, 0,
		FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Courier New Thin"));
}

Box GetWindowBox(HWND hwnd) {
	Box result;
	RECT rect; GetWindowRect(hwnd, &rect);

	result.left = rect.left;
	result.top = rect.top;
	result.right = rect.right - rect.left;
	result.bottom = rect.bottom - rect.top;

	return result;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if(APP) {
		WND *receiver = NULL;
		for(size_t i = 0; i < APP->w.size() && !receiver; i++)
			if(APP->w [ i ]->hwnd == hwnd)
				receiver = APP->w [ i ];
		if(receiver) {

			SelectObject(receiver->hdc, APP->font);
			int result = receiver->Dispatch(message, wParam, lParam);

			switch(message) {
				case WM_DESTROY: {
					delete receiver;
				}break;
				case WM_SIZE: {
					APP->font = cfont(receiver, 8);
					if(!receiver->locked)
						receiver->position = GetWindowBox(hwnd);
					if(receiver == activeOpenGLContext->window) {
						glViewport(0, 0, receiver->position.right - receiver->position.left, receiver->position.bottom - receiver->position.top);
					}
				}break;
				case WM_MOVE: {
					if(!receiver->locked)
						receiver->position = GetWindowBox(hwnd);
				}break;
				case WM_KEYDOWN: {
					if(callbackfuncs.key) callbackfuncs.key(wParam, KeyState::KEY_PRESSED);
				}break;
				case WM_KEYUP: {
					if(callbackfuncs.key) callbackfuncs.key(wParam, KeyState::KEY_RELEASED);
				}break;
				case WM_LBUTTONDOWN: {
					HWND focus = GetWindow(hwnd, GW_CHILD);
					if(focus) {
						SetFocus(focus);
					}
				}break;
			}

			return result ;
		}
	}
	return DefWindowProc ( hwnd , message , wParam , lParam );
}

WNDCLASSEX simpleWinClass ( HINSTANCE hThisInstance , HINSTANCE hPrev , const wchar_t *name , int ICON , int SMALL , COLORREF background ) {
	WNDCLASSEX result = { 0 } ;

	HBRUSH hBrsh;

	hBrsh = CreateSolidBrush ( background );

	/* The Window structure */
	result.hInstance = hThisInstance;
	result.lpszClassName = name;
	result.lpfnWndProc = WindowProcedure;
	result.style = CS_HREDRAW | CS_VREDRAW;

	result.cbSize = sizeof(WNDCLASSEX);

	if(ICON) result.hIcon = LoadIcon(hThisInstance, MAKEINTRESOURCE(ICON));
	if(SMALL) result.hIconSm = LoadIcon(hThisInstance, MAKEINTRESOURCE(SMALL));
	result.hCursor = LoadCursor(nullptr, IDC_ARROW);
	result.lpszMenuName = NULL;
  
	result.cbClsExtra = 0;
	result.cbWndExtra = 0;

	result.hbrBackground = hBrsh;

	if(hPrev == 0) {
		ATOM a = RegisterClassEx(&result);
		assert(a);
	}

	return result;

}

Application::Application(HINSTANCE hThisInstance, HINSTANCE hPrev, COLORREF background) {
	this->hThisInstance = hThisInstance;
	this->hPrev = hPrev;
	wincl.push_back(simpleWinClass(hThisInstance, hPrev, className, NULL, NULL, background));
	start = std::chrono::system_clock::now();
	fpsCap = FPS_UNCAPPED;
}

bool Application::PollWindowsEvents() {
	static MSG messages;

	float now;

	do {

		while(PeekMessage(&messages, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
		if(messages.message == WM_QUIT) {
			return false;
		}

		now = time();

	} while((now - lastTime) <(1.0 / fpsCap));
	
	deltaTime = now - lastTime;
	lastTime = now;

	fpsCap = FPS_UNCAPPED;

	return true;
}

void Application::setFpsCap(float cap) {
	fpsCap = cap;
}

float Application::getDeltaTime() const {
	return deltaTime;
}

void Application::appSwapBuffers(void) {
	SwapBuffers(activeOpenGLContext->windowContext);
}

void init_api() {
	APP = new Application(GetModuleHandle(NULL), NULL, RGB(0, 0, 0));
}
