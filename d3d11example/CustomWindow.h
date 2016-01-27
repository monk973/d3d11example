#pragma once
#include "GameStatic.h"

class CustomWindow
{
public:
	CustomWindow(int width,int height);

	virtual ~CustomWindow();

	HWND& GetHandle();

private:
	HWND m_hWnd;


};

