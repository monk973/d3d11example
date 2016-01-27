#include "Renderer.h"




Renderer::Renderer(HWND _hWnd)
{
	CreateDevice(_hWnd);
	CreateRenderTargetView();

	gameStatic.SetDevice(m_device);
	gameStatic.SetDeviceContext(m_deviceContext);

	OneLevel.createLevel();
	m_camera.init();
	CreateProjMat();

}


Renderer::~Renderer()
{

}

void Renderer::CreateDevice(HWND _hWnd)
{

	// Define our swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = _hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;

	// Create the swap chain, device and device context
	auto result = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION,
		&swapChainDesc, &m_swapChain,
		&m_device, nullptr, &m_deviceContext);

	// Check for error
	if (result != S_OK) {
		MessageBox(nullptr, "Error creating DX11", "Error", MB_OK);
		exit(0);
	}

}

void Renderer::CreateRenderTargetView()
{
	ID3D11Texture2D* backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	m_device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTartgetView);

	backBuffer->GetDesc(&m_backBufferDesc);
	backBuffer->Release();



	D3D11_TEXTURE2D_DESC depthStencilDesc;

	RECT rc;
	GetClientRect(0, &rc);

	depthStencilDesc.Width =rc.right;
	depthStencilDesc.Height = rc.bottom;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	UINT msaaQuality;
	m_device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality);

	if (msaaQuality)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = msaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	m_device->CreateTexture2D(&depthStencilDesc, 0, &m_depthStencilBuffer);
	m_device->CreateDepthStencilView(m_depthStencilBuffer, 0, &m_DepthStencilView);

	// Bind render target
	m_deviceContext->OMSetRenderTargets(1, &m_RenderTartgetView,m_DepthStencilView);

	// Set viewport
	auto viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height);
	m_deviceContext->RSSetViewports(1, &viewport);

}
void Renderer::CreateProjMat()
{
	float pi = atan(1.f)*4.f;
	XMMATRIX projMat;

	RECT rc;
	GetClientRect(0, &rc);

	float width = rc.right;
	float height = rc.bottom;
	projMat = XMMatrixPerspectiveFovLH(0.25f*pi, width / height, 1.0f, 1000.0f);
	gameStatic.SetProjMat(projMat);

}


void Renderer::Render()
{


	// Set the background color
	float clearColor[] = { .25f, .5f, 1, 1 };
	m_deviceContext->ClearRenderTargetView(m_RenderTartgetView, clearColor);

	m_deviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	/*
	vector<gameObject*>::iterator it;
	
	for (it = m_arrayGameObject.begin(); it != m_arrayGameObject.end(); ++it)
	{
		(*it)->draw();
	}
	*/
	for (auto o : gameStatic.getArrayGameObject())
	{
		o->draw();
	}

	m_swapChain->Present(0, 0);
}

void Renderer::update()
{
	m_camera.update();

	for (auto o : gameStatic.getArrayGameObject())
	{
		o->update();
	}
}
