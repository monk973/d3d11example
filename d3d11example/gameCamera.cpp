#include "gameCamera.h"



gameCamera::gameCamera()
{
}


gameCamera::~gameCamera()
{
}

void gameCamera::init()
{
	
	XMVECTOR up(XMLoadFloat3(&m_upVector));
	XMVECTOR pos(XMLoadFloat3(&m_positionVector));
	XMVECTOR lookAt(XMLoadFloat3(&m_lookAtVector));

	m_view = XMMatrixLookAtLH(pos, lookAt, up);

	gameStatic.SetViewMat(m_view);
}

void gameCamera::update()
{

}
