#include "cBox.h"



cBox::cBox()
{
	m_positionVector.x = 0;
	m_positionVector.y = 0;
	m_positionVector.z = 0;
	m_rotationVector = {};
	m_worldMatrix = XMMatrixIdentity();
}


cBox::~cBox()
{

}

void cBox::init()
{
	CreateVertex();
	gameObject::CreateShaderAndConstantBuffer("triangleVertexShader.cso","trianglePixelShader.cso");
	//CreateRenderStates();
	m_textureClass.load(L"data/stone01.tga");
}


void cBox::draw()
{
	XMMATRIX scale = XMMatrixIdentity();
	XMMATRIX rot = XMMatrixIdentity();
	XMMATRIX trans = XMMatrixTranslation(m_positionVector.x, m_positionVector.y, m_positionVector.z);

	XMMATRIX world = scale*rot*trans;

	UINT stride = sizeof(vertex_pt);
	UINT offset = 0;

	//
	gameObject::SetWorldViewProj(world, gameStatic.GetViewMat(), gameStatic.GetProjMat());

	ID3D11ShaderResourceView* tmpTextureView = m_textureClass.GetTexture();
	gameStatic.getDeviceContext()->PSSetShaderResources(0, 1, &tmpTextureView);

	//
	gameStatic.getDeviceContext()->IASetInputLayout(m_IL);

	gameStatic.getDeviceContext()->VSSetShader(m_vs, NULL, 0);
	gameStatic.getDeviceContext()->PSSetShader(m_ps, NULL, 0);

	gameStatic.getDeviceContext()->PSSetSamplers(0, 1, &m_samplerState);
	
	//
	gameStatic.getDeviceContext()->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);

	gameStatic.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gameStatic.getDeviceContext()->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);
	



	// Render the triangle.
	gameStatic.getDeviceContext()->DrawIndexed(indexCount, 0, 0);


}

void cBox::update()
{
	
	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_positionVector.y += 0.0001f;
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_positionVector.y += -0.0001f;
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_positionVector.x += -0.0001f;

	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_positionVector.x += 0.0001f;

	}

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		m_positionVector.z += 0.0001f;
	}
	if (GetKeyState(VK_TAB) & 0x8000)
	{
		m_positionVector.z += -0.0001f;
	}
	
	
}

void cBox::CreateVertex()
{
	vertex_pt v[] = {
		{XMFLOAT3(-1.f,0.f,0.f),XMFLOAT2(0.f,1.f)},
		{XMFLOAT3(0.f,1.f,0.f),XMFLOAT2(0.f,0.f)},
		{XMFLOAT3(1.f,0.f,0.f),XMFLOAT2(1.f,0.f)}
	};

	int indices[] = {
		0,1,2
	};

	D3D11_BUFFER_DESC VertexDesc = {};
	VertexDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexDesc.ByteWidth = sizeof(vertex_pt) * (sizeof(v)/sizeof(vertex_pt));
	VertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = v;

	UINT stride = sizeof(vertex_pt);
	gameStatic.getDevice()->CreateBuffer(&VertexDesc, &vertexData, &m_VB);

	indexCount = sizeof(indices) / sizeof(int);
	D3D11_BUFFER_DESC indexDesc = {};
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(int)* (indexCount);
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices;

	gameStatic.getDevice()->CreateBuffer(&VertexDesc, &indexData,&m_IB);


}

void cBox::CreateRenderStates()
{

	auto rasterizerDesc = CD3D11_RASTERIZER_DESC(
		D3D11_FILL_SOLID,
		D3D11_CULL_NONE,
		false,
		0, 0, 0, 0,
		false, false, false);
	gameStatic.getDevice()->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);

	// Blend state
	auto blendDesc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
	gameStatic.getDevice()->CreateBlendState(&blendDesc, &m_blendState);

	// Depth state
	auto depthDesc = CD3D11_DEPTH_STENCIL_DESC(
		FALSE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS,
		FALSE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS,
		D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS);
	gameStatic.getDevice()->CreateDepthStencilState(&depthDesc, &m_depthState);
}
