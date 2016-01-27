#include "CustomWindow.h"


LRESULT CALLBACK msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

CustomWindow::CustomWindow(int width,int height)
{
	WNDCLASS wc = {};
	wc.style = CS_OWNDC;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.lpfnWndProc = msgProc;
	wc.lpszClassName = "direct11";

	RegisterClass(&wc);

	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	int ScreenY = GetSystemMetrics(SM_CYSCREEN);

	int centerX = (ScreenX*0.5) - (width*0.5);
	int centerY = (ScreenY*0.5) - (height*0.5);

	m_hWnd = CreateWindow("direct11", "directx11", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		centerX, centerY, width, height, 0, 0, 0, 0);

	UpdateWindow(m_hWnd);
}


CustomWindow::~CustomWindow()
{
}

HWND& CustomWindow::GetHandle()
{
	return m_hWnd;
}
