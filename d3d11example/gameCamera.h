#pragma once
#include "GameStatic.h"

class gameCamera
{
public:
	gameCamera();
	virtual ~gameCamera();

	void init();
	void update();

private:
	XMFLOAT4X4 m_view = {};
	XMFLOAT3 m_positionVector= XMFLOAT3(0, 0, -6.f);
	XMFLOAT3 m_upVector=XMFLOAT3(0,1,0);
	XMFLOAT3 m_lookAtVector=XMFLOAT3(0,0,0);

};

