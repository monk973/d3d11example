#include "CustomWindow.h"

CustomWindow* g_window=0;

LRESULT CALLBACK msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	return g_window->CustomMsgProc(hWnd, message, wParam, lParam);
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

	g_window = this;
}


CustomWindow::~CustomWindow()
{

	g_window = 0;
}

HWND& CustomWindow::GetHandle()
{
	return m_hWnd;
}

LRESULT CustomWindow::CustomMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	int a = 0;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOVE:
		//OnMove();
		break;
	case WM_MOVING:
		a = 0;
		break;
	case WM_PAINT:
		//OnPaint();
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
