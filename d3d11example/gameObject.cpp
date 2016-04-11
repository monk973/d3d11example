#include "GameObject.h"


gameObject::gameObject()
{
	m_scaleVector = XMFLOAT3(1, 1, 1);
	m_rotationVector = XMFLOAT3(0, 0, 0);
	m_positionVector = XMFLOAT3(0, 0, 0);

	XMStoreFloat4x4(&m_matScale, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matRot, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matTranslation, XMMatrixIdentity());

	XMStoreFloat4x4(&m_matTransform, XMMatrixIdentity());

	m_parent = nullptr;
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

void gameObject::SetParent(gameObject * _p)
{
	m_parent = _p;
}

gameObject * gameObject::GetParent()
{
	return m_parent;
}

void gameObject::CreateShaderAndConstantBuffer(const char * _vsDir, const char * _psDir)
{
	ifstream vsFile(_vsDir, ios::binary);
	vector<char> vsData = { istreambuf_iterator<char>(vsFile),istreambuf_iterator<char>() };

	ifstream psFile(_psDir, ios::binary);
	vector<char> psData = { istreambuf_iterator<char>(psFile),istreambuf_iterator<char>() };

	HRESULT hr = gameStatic.getDevice()->CreateVertexShader(vsData.data(), vsData.size(), 0, &m_vs);
	HRESULT hr2 = gameStatic.getDevice()->CreatePixelShader(psData.data(), psData.size(), 0, &m_ps);

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	int numElements = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	gameStatic.getDevice()->CreateInputLayout(layout, numElements, vsData.data(), vsData.size(), &m_IL);


	//matrix buffer (WVP)
	D3D11_BUFFER_DESC matrixDesc = {};
	matrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixDesc.ByteWidth = sizeof(matrix_WorldViewProj);
	matrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	gameStatic.getDevice()->CreateBuffer(&matrixDesc, 0, &m_matrixBuffer);

	//sampler state (Texture)

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



	//Light Buffer

	D3D11_BUFFER_DESC lightBuffDesc = {};

	lightBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBuffDesc.ByteWidth = sizeof(lightBufferType);
	lightBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBuffDesc.MiscFlags = 0;
	lightBuffDesc.StructureByteStride = 0;

	
	HRESULT hr12312= _getDevice->CreateBuffer(&lightBuffDesc, NULL, &m_lightBuffer);
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

void gameObject::SetLightBuffer(XMFLOAT4 _diffuseColor, XMFLOAT3 _lightDir)
{
	lightBufferType* pdata = 0;
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
	_getDeviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

	pdata = (lightBufferType*)mappedSubresource.pData;

	pdata->diffuseColor = _diffuseColor;
	pdata->lightDir = _lightDir;
	pdata->pad = 0.f;

	_getDeviceContext->Unmap(m_lightBuffer, 0);

	_getDeviceContext->PSSetConstantBuffers(0, 1, &m_lightBuffer);
}

void gameObject::SetPosition(XMFLOAT3 _v)
{
	m_positionVector = _v;
}

void gameObject::SetRotation(XMFLOAT3 _v)
{
	m_rotationVector = _v;

}

void gameObject::SetScale(XMFLOAT3 _v)
{
	m_scaleVector = _v;
}

XMFLOAT4X4 gameObject::CalculateWorldMatrix()
{
	XMMATRIX scale, rot, trans, world;

	scale = XMMatrixScalingFromVector(XMLoadFloat3(&m_scaleVector));
	rot = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_rotationVector));
	trans = XMMatrixTranslationFromVector(XMLoadFloat3(&m_positionVector));

	world = scale* rot* trans;

	XMStoreFloat4x4(&m_matScale, scale);
	XMStoreFloat4x4(&m_matRot, rot);
	XMStoreFloat4x4(&m_matTranslation, trans);
	XMStoreFloat4x4(&m_matTransform, world);


	return m_matTransform;
}

XMFLOAT4X4 gameObject::CalculateParentMatrices(gameObject * _p)
{
	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, XMMatrixIdentity());

	if (_p == NULL)
	{
		return result;
	}

	XMMATRIX world = XMLoadFloat4x4(&(_p->CalculateWorldMatrix()));
	XMMATRIX parentMat = XMLoadFloat4x4(&CalculateParentMatrices(_p->GetParent()));

	world *= parentMat;

	XMStoreFloat4x4(&result, world);

	return result;

}


