#pragma once
#include <chrono>
#include <windows.h>
#include <vector>
#include <cstring>
#include <string>
#include <cassert>

#include "box.h"
#include <glad/glad.h>

class Application;
class WND;

extern Application *APP;

void init_api();

struct Context {
	Context();
	Context(HDC, HGLRC, WND *);

	HDC windowContext;
	HGLRC openglContext;

	WND *window;
};

extern Context *activeOpenGLContext;

void MakeCurrentContext(Context *context);
Context *GetCurrentContext(void);

enum KeyState {
	KEY_PRESSED,
	KEY_RELEASED
};

struct Callbacks {
	void(*key)(int key, KeyState state) = 0;
};

void SetKeyCallbackFunc(void(*func)(int key, KeyState state));

class WND {
	bool inited;
protected:
	WND *daddy;
	virtual void Start() { }

	HGLRC hglrc;
public:
	HWND hwnd;
	HDC hdc;
	std::vector<WND *> children;

	WND();
	virtual ~WND();

	Box position;
	bool locked;

	virtual int Create(WND *daddy, const TCHAR *descr, const Box &b, int wincl = 0);
	virtual LRESULT Dispatch(UINT message, WPARAM wParam, LPARAM lParam);
	void Show(int command = SW_SHOW);
	void Hide() { Show(SW_HIDE); }
	void CreateTimer(int TimerID, int ms);

	void DisableMovement() { locked = true; }
	void EnableMovement() { locked = false; }

	void CreateContext();
	HDC GetContext();

	WND *FindChildren(const wchar_t *name);
	WND *FindChildren(HWND child);

	void Update();
	WND *parent() { return daddy; }
};

class Application {
	HINSTANCE hThisInstance, hPrev;
	std::chrono::system_clock::time_point start;
	std::vector<WNDCLASSEX> wincl;

	float lastTime;
	float fpsCap;
	float deltaTime;
public:
	HFONT font;

	Application(HINSTANCE hThisInstance, HINSTANCE hPrev, COLORREF background);	
	
	HINSTANCE Instance() const { return hThisInstance; }	

	const WNDCLASSEX *getWndClass(int wcid) const { return(wincl.size() > wcid) ? &wincl [ wcid ] : NULL; }

	int addWndClass(const WNDCLASSEX &wndcl) { wincl.push_back(wndcl); }
	
	std::vector<WND *> w;

	float time();

	bool PollWindowsEvents();
	void appSwapBuffers();

	float getDeltaTime() const;
	void setFpsCap(float cap);
};

WNDCLASSEX simpleWinClass(HINSTANCE hThisInstance, HINSTANCE hPrev, const wchar_t *name, int icon_define, int small_define, COLORREF background);
