#include "gameCamera.h"



gameCamera::gameCamera()
{
	//m_positionVector = XMFLOAT3(0, 0, -12.f);


}


gameCamera::~gameCamera()
{
}

void gameCamera::init()
{
	XMMATRIX view;

	XMVECTOR up(XMLoadFloat3(&m_upVector));
	XMVECTOR pos(XMLoadFloat3(&m_positionVector));
	XMVECTOR lookAt(XMLoadFloat3(&m_lookAtVector));

	view = XMMatrixLookAtLH(pos, lookAt, up);

	XMStoreFloat4x4(&m_view, view);

	gameStatic.SetViewMat(m_view);
}

void gameCamera::update()
{

}
