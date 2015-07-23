#include "Window.h"
#include <string>

Window::Window(WNDPROC messageHandler, LPCTSTR title, int x, int y, int width, int height)
{
	// UINT style - The style of the window.
	windowClass.style = NULL;

	// WNDPROC lpfnWndProc - The message handler to use.
	windowClass.lpfnWndProc = messageHandler;

	// int cbClsExtra & cbWndExtra - Extra bytes to assign.
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;

	// HINSTANCE hInstance - The thread where this window will be created.
	windowClass.hInstance = GetModuleHandle(NULL);

	// HICON hIcon - The big icon for this window class.
	windowClass.hIcon = NULL;

	// HCURSOR hCursor - The mouse cursor to show in this window.
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	// HRBUSH hbrBackground - The background colour.
	windowClass.hbrBackground = NULL;

	// LPCTSTR lpszMenuName - The menu resource to use.
	windowClass.lpszMenuName = NULL;

	// LPCTSTR lpszClassName - What name to define to this new window class.
	windowClass.lpszClassName = title;

	RegisterClass(&windowClass);

	window = CreateWindow(
		// LPCTSTR lpClassName - The class name of WNDCLASS that we just registered.
		windowClass.lpszClassName,

		// LPCTSTR lpWindowName - The title to give the window.
		title,

		// DWORD dwStyle - The style of the window.
		WS_OVERLAPPEDWINDOW,

		// int x & y - The location of the window.
		x, y,

		// int nWidth & nHeight - The size of the window.
		width, height, 
		
		// HWND hWndParent - This is the window's parent.
		GetDesktopWindow(), 
		
		// HMENU hMenu - The menu to assign, or use.
		NULL, 
		
		// HINSTANCE hInstance - The instance of the module associated with this window.
		windowClass.hInstance, 
		
		// LPVOID lpParam - Not for our purposes.
		NULL);

	if (window == NULL) throw(TEXT("Window Constructor: Failed to create a new window!"));
}

Window::~Window() {
	// We'll unregister the window now, using the parameters saved in windowClass.
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	// And we'll destroy the window too, just to be safe.
	if (window != NULL) { DestroyWindow(window); window = NULL; }
}