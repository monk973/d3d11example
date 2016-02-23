#include "Renderer.h"



Renderer::Renderer(int width,int height)
	:CustomWindow(width, height)
{
	CreateDevice();
	gameStatic.SetDevice(m_device);
	gameStatic.SetDeviceContext(m_deviceContext);

	CreateRenderTargetView();


	OneLevel.createLevel();
	m_camera.init();
	CreateProjMat();

	gameStatic.SetHWND(GetHandle());
}


Renderer::~Renderer()
{
	SAFE_RELEASE(m_device);
	
	if (m_deviceContext){
		m_deviceContext->ClearState();
		SAFE_RELEASE(m_deviceContext);
	}

	SAFE_RELEASE(m_RenderTartgetView);
	SAFE_RELEASE(m_DepthStencilView);
	SAFE_RELEASE(m_depthStencilBuffer);
	SAFE_RELEASE(m_swapChain);


}

void Renderer::CreateDevice()
{
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,                 // no software device
		0,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&m_device,
		&m_featureLevel,
		&m_deviceContext);

	RECT rc;
	GetClientRect(GetHandle(), &rc);

	gameStatic.SetFeatureLevel(m_featureLevel);
	


	DXGI_SWAP_CHAIN_DESC sd = {};

	sd.BufferDesc.Width = rc.right;
	sd.BufferDesc.Height = rc.bottom;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	UINT msaaQuality;
	m_device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality);

	if (msaaQuality)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = msaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = GetHandle();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// Create the swap chain, device and device context

	IDXGIDevice* dxgiDevice = 0;
	m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(m_device, &sd, &m_swapChain);

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	int a = 0;
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
	GetClientRect(GetHandle(), &rc);

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
	m_deviceContext->OMSetRenderTargets(1, &m_RenderTartgetView, m_DepthStencilView);

	// Set viewport

	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<float>(rc.right);
	m_viewport.Height = static_cast<float>(rc.bottom);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;


	m_deviceContext->RSSetViewports(1, &m_viewport);

}
void Renderer::CreateProjMat()
{
	float pi = atan(1.f)*4.f;
	XMMATRIX projMat;

	RECT rc;
	GetClientRect(GetHandle(), &rc);

	float width = rc.right;
	float height = rc.bottom;
	projMat = XMMatrixPerspectiveFovLH(0.25f*pi, width / height, 1.0f, 1000.0f);

	XMFLOAT4X4 storeMat;
	XMStoreFloat4x4(&storeMat, projMat);

	gameStatic.SetProjMat(storeMat);

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

void Renderer::OnPaint()
{
	//
}

void Renderer::OnMove()
{
	//
}
