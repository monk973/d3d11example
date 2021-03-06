#include "GameStatic.h"
#include "CustomWindow.h"
#include "Renderer.h"


int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount) 
{
	Renderer renderer(800,600);

	MSG msg = {};

	while (1)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		gameTimer.update();

		renderer.update();
		renderer.Render();



	}
	

	return 0;
}