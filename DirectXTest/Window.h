#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

class Window
{
public:
	Window(WNDPROC messageHandler, LPCTSTR title, int x, int y, int width, int height);
	~Window();
	//*** Accessors/Modifiers ***
	HWND getHandle() { return window; }

	//*** Methods ***
	void show() { ShowWindow(window, SW_SHOWDEFAULT); }

	void setSize(int width, int height) {
		SetWindowPos(window, HWND_NOTOPMOST, NULL, NULL, width, height, SWP_NOACTIVATE | SWP_NOMOVE);
	}
protected:
	//*** Fields ***
	WNDCLASS windowClass;
	HWND window;
};

#endif // !WINDOW_H
