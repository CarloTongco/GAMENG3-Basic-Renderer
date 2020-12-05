#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	bool init();
	bool broadcast();
	bool release();
	bool isRun();

	RECT getClientWindowRect();
	HWND getWindowHandle();
	void setHWND(HWND hwnd);

	//EVENT METHODS
	virtual void onCreate()=0;
	virtual void onUpdate()=0;
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	~Window();

protected:
	HWND m_hwnd;
	bool m_is_run = true;
};
