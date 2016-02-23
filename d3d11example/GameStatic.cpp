#include "GameStatic.h"

CGameStatic::CGameStatic()
{
}

CGameStatic::~CGameStatic()
{
	for (auto o : m_arrayGameObject)
	{
		delete o;
	}
}

void CGameStatic::addGameObject(gameObject * _obj)
{
	m_arrayGameObject.push_back(_obj);
	
}

vector<class gameObject*>& CGameStatic::getArrayGameObject()
{
	return m_arrayGameObject;
}

void CGameStatic::SetDevice(ID3D11Device * _device)
{
	m_device = _device;
}

ID3D11Device *& CGameStatic::getDevice()
{
	return m_device;
}

void CGameStatic::SetDeviceContext(ID3D11DeviceContext * _context)
{
	m_context = _context;
}

ID3D11DeviceContext *& CGameStatic::getDeviceContext()
{
	return m_context;
}

void CGameStatic::SetViewMat(XMFLOAT4X4&  _view)
{
	m_view = _view;

}

XMFLOAT4X4 CGameStatic::GetViewMat()
{
	return m_view;
}

void CGameStatic::SetProjMat(XMFLOAT4X4&  _proj)
{
	m_proj = _proj;
}

XMFLOAT4X4 CGameStatic::GetProjMat()
{
	return m_proj;
}

void CGameStatic::SetFeatureLevel(D3D_FEATURE_LEVEL _level)
{
	m_featureLevel = _level;
}

int CGameStatic::GetFeatureLevel()
{
	return m_featureLevel;
}

HWND CGameStatic::GetHWND()
{
	return m_hWnd;
}

void CGameStatic::SetHWND(HWND _hWND)
{
	m_hWnd = _hWND;
}

gameObject::gameObject()
{
	m_scaleVector = XMFLOAT3(1, 1, 1);
	m_rotationVector = XMFLOAT3(0,0,0);
	m_positionVector = XMFLOAT3(0,0,0);

	XMStoreFloat4x4(&m_matScale, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matRot, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matTranslation, XMMatrixIdentity());

	XMStoreFloat4x4(&m_matTransform, XMMatrixIdentity());
}

gameObject::~gameObject()
{
	SAFE_RELEASE(m_VB);
	SAFE_RELEASE(m_IB);
	SAFE_RELEASE(m_matrixBuffer);
	SAFE_RELEASE(m_vs);
	SAFE_RELEASE(m_ps);
	SAFE_RELEASE(m_IL);

	SAFE_RELEASE(m_samplerState);

}

void gameObject::CreateShaderAndConstantBuffer(const char * _vsDir, const char * _psDir)
{
	ifstream vsFile(_vsDir, ios::binary);
	vector<char> vsData = { istreambuf_iterator<char>(vsFile),istreambuf_iterator<char>() };

	ifstream psFile(_psDir, ios::binary);
	vector<char> psData = { istreambuf_iterator<char>(psFile),istreambuf_iterator<char>() };

	HRESULT hr= gameStatic.getDevice()->CreateVertexShader(vsData.data(), vsData.size(), 0, &m_vs);
	HRESULT hr2 = gameStatic.getDevice()->CreatePixelShader(psData.data(), psData.size(), 0, &m_ps);

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	gameStatic.getDevice()->CreateInputLayout(layout, 2, vsData.data(), vsData.size(), &m_IL);


	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	gameStatic.getDevice()->CreateSamplerState(&samplerDesc, &m_samplerState);




	D3D11_BUFFER_DESC matrixDesc = {};
	matrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixDesc.ByteWidth = sizeof(matrix_WorldViewProj);
	matrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	gameStatic.getDevice()->CreateBuffer(&matrixDesc, 0, &m_matrixBuffer);

}

void gameObject::SetWorldViewProj(XMFLOAT4X4& _w, XMFLOAT4X4& _v, XMFLOAT4X4& _p)
{
	XMMATRIX world, view, proj;
	world = XMLoadFloat4x4(&_w);
	view = XMLoadFloat4x4(&_v);
	proj = XMLoadFloat4x4(&_p);

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	matrix_WorldViewProj* dataPtr;


	gameStatic.getDeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr = (matrix_WorldViewProj*)mappedResource.pData;
	//dataPtr->world = world;
	//dataPtr->view = view;
	//dataPtr->proj = proj;

	XMStoreFloat4x4(&(dataPtr->world), world);
	XMStoreFloat4x4(&(dataPtr->view), view);
	XMStoreFloat4x4(&(dataPtr->proj), proj);


	gameStatic.getDeviceContext()->Unmap(m_matrixBuffer, 0);

	gameStatic.getDeviceContext()->VSSetConstantBuffers(0, 1, &m_matrixBuffer);


}

XMFLOAT4X4 gameObject::CalculateWorldMatrix()
{
	XMMATRIX scale, rot, trans, world;

	scale = XMMatrixScalingFromVector(XMLoadFloat3(&m_scaleVector));
	rot= XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_rotationVector));
	trans= XMMatrixTranslationFromVector(XMLoadFloat3(&m_positionVector));

	world = scale* rot* trans;

	XMStoreFloat4x4(&m_matScale, scale);
	XMStoreFloat4x4(&m_matRot, rot);
	XMStoreFloat4x4(&m_matTranslation, trans);
	XMStoreFloat4x4(&m_matTransform, world);


	return m_matTransform;
}


