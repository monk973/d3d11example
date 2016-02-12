#pragma once
#include "GameStatic.h"

class CustomWindow
{
public:
	CustomWindow(int width,int height);

	virtual ~CustomWindow();

	HWND& GetHandle();
	

	LRESULT CustomMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnPaint()=0;
	virtual void OnMove() = 0;


private:
	HWND m_hWnd;


};

