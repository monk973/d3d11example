#pragma once

#include "GameStatic.h"
#include "Level.h"
#include "CustomWindow.h"

class Renderer : public CustomWindow
{
public:

	Renderer(int width,int height);
	virtual ~Renderer();

	void CreateProjMat();
	void CreateDevice();
	void CreateRenderTargetView();
	void Render();
	void update();

private:
	virtual void OnPaint() override;
	virtual void OnMove() override;

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_RenderTartgetView;
	ID3D11DepthStencilView* m_DepthStencilView;
	ID3D11Texture2D* m_depthStencilBuffer;
	D3D11_VIEWPORT m_viewport;

	D3D_FEATURE_LEVEL m_featureLevel;
	D3D11_TEXTURE2D_DESC m_backBufferDesc;
	IDXGISwapChain* m_swapChain = nullptr;

	CLevel OneLevel;
	gameCamera m_camera;

};

