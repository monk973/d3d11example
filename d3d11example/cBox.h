#pragma once
#include "GameStatic.h"
#include "ImportModel.h"


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

	ID3D11RasterizerState* m_rasterizerState = nullptr;
	ID3D11DepthStencilState* m_depthState = nullptr;
	ID3D11BlendState* m_blendState = nullptr;

	textureClass m_textureClass;


	ImportModel importModel;
	

};