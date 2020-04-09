#pragma once

#include "wnd.h"

class ChildWND : public WND {
public:
	int Create(WND *daddy, const TCHAR *descr, const Box &b, int wincl = 0) override;
};

class EditWND : public WND {
public:
	int Create(WND *daddy, const TCHAR *descr, const Box &b, int wincl = 0) override;
};

class MultiLineEditWND : public WND {
public:
	int Create(WND *daddy, const TCHAR *descr, const Box &b, int wincl = 0) override;
};

class DropDownInterface : public WND {
public:
	virtual int Create(WND *daddy, const TCHAR *descr, const Box &b, int wincl = 0);

	void AppendString(const std::wstring &str) const;
	std::string GetActiveString();
};

class DropDownWND : public DropDownInterface {
public:
	int Create(WND *daddy, const TCHAR *descr, const Box &b, int wincl = 0) override;
};

class ComboBoxWND : public DropDownInterface {
public:
	int Create(WND *daddy, const TCHAR *descr, const Box &b, int wincl = 0) override;
};
