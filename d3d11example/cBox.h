#pragma once
#include "GameStatic.h"


class cBox :
	public gameObject
{
public:
	cBox();
	virtual ~cBox();

public:
	virtual void init() override;
	virtual void draw() override;
	virtual void update() override;

private:
	void CreateVertex();
	void CreateRenderStates();
	

private:
	XMFLOAT3 m_positionVector = {};
	XMFLOAT3 m_rotationVector = {};
	XMMATRIX m_worldMatrix = {};

	ID3D11RasterizerState* m_rasterizerState = nullptr;
	ID3D11DepthStencilState* m_depthState = nullptr;
	ID3D11BlendState* m_blendState = nullptr;

	textureClass m_textureClass;
	

};