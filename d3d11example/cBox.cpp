#include "cBox.h"


cBox::cBox()
{
}


cBox::~cBox()
{
	SAFE_RELEASE(m_rasterizerState);
	SAFE_RELEASE(m_depthState);
	SAFE_RELEASE(m_blendState);

}

/*
void foo1(char asd[]) {
	char* a;
	asd = a;
}
�� �Լ��� ���� asd�� �����ͷμ� ���ǵȵ��ϴ�.
������� �Ϲ����� char �迭Ÿ������ �νĵɰ��� ���� ��ɵ�ó�� �����ͺ����� �޴°������ڸ� 
�Լ��� ���ڷμ� �ൿ�Ұ�쿡�� ������Ÿ������ �ൿ�ϴ°�ó�����δ�.

*/

/*
char* foo1()
{
	char* a="asd";
	return a;
}
*/

void cBox::init()
{
	importModel.Init("data/cube.txt");

	CreateVertex();

	bool ShaderModel;
	
	/*
	char as[] = "asd";
	���� �������� []�� ������ó�� �����ְ� ���ִ� ������ ������μ� �����һ��̰� 
	�����δ� �����Ͱ��ƴϴ�.
	asd = foo1();
	���� �Լ��� �����غ��� ���� �����Ҽ��ִ� Value�����Ѵٴ� ������ ����.
	���� as�� Ÿ���� ������Ÿ���̿��ٸ� �����߰����� ���������Ƿ� �����Ϸ� ���ο����� ���� asŸ���� ������Ÿ���̾ƴ϶�
	���Ǽ����� �����ϴ� charŸ���̴�.

	�ڵ念�������ؼ��� �˾ƺô�. ��¿�� ���ڿ����� �ڵ念���� �ö󰥱� �����ߴ�.
	char * asd;
	asd=foo1();
	���� �����Ұ�� ������ζ�� foo1�Լ��� ��������� �Լ��� ���������鵵 �����Ǹ鼭 �������� ��������ϴµ� 
	���ϵǾ� �޾ƿ°��� �����ߴ�. �׷��ٴ°��� ������ ������ ���ڿ� ����� �����Ұ�� �� ���ڿ� ����� �ڵ念���� �Էµǰ�
	�ڵ念���� �ִ� ���ڿ� ����� foo1�Լ��� ���������� ���δ�.
	�ڵ念���� ���α׷��� ����ɶ� �����ȴ�. �ڵ������� ����������.

	*/

	/*
	char asd[]= getFeatureLevel >= D3D_FEATURE_LEVEL_11_0 ? "shader 5.0" : "shader 4.0";

	�� ����� ������ ���� ������ ���� �ּ��鿡�� �����Ѱ�ó�� ���� asd�� �����Ϸ������� �̸� ������Ÿ���̾ƴ� 
	char �迭Ÿ������ �ν��Ѵ�. �׸��� ���׿����ڰ� �����ϴ� Ÿ���� ������Ÿ���̴�. 
	�ٽø��ؼ� ���׿����ڵ� foo1()�Լ�ó�� ������ �Լ��̰� foo1�Լ��� �����ϴ� Ÿ�԰� ���������� 
	������Ÿ���̱⶧���� "char �迭Ÿ����" asd�� �̸� ����������. ������ Ÿ���� ������Ÿ�Կ� �޾ƾ��ϰ�
	char �迭Ÿ�Կ��� �� �ڸ����� ��� ���� ������� �Է�������Ѵ�. 
	*/

	string vsDir = getFeatureLevel >= D3D_FEATURE_LEVEL_11_0 ? "shader 5.0" : "shader 4.0";
	string psDir = getFeatureLevel >= D3D_FEATURE_LEVEL_11_0 ? "shader 5.0" : "shader 4.0";

	vsDir.append("/triangleVertexShader.cso");
	psDir.append("/trianglePixelShader.cso");


	gameObject::CreateShaderAndConstantBuffer(vsDir.c_str(), psDir.c_str());
	CreateRenderStates();
	m_textureClass.load(L"data/seafloor.dds");

}


void cBox::draw()
{
	XMFLOAT4X4 worldMat = CalculateWorldMatrix();

	XMMATRIX calc_world = XMLoadFloat4x4(&worldMat);
	XMMATRIX calc_parentMat = XMLoadFloat4x4(&CalculateParentMatrices(m_parent));

	if (m_parent)
	{

		/*
			���� : ������ ȸ��
			���� : ����� �������� ȸ��

			S*R*T*(ParentMat...) ��������
			S*R*T*R*(ParentMat...) ��������

			�̵�Ű�� �������� �ڽ� ������Ʈ�� �̻��� �������� ���ư��ٸ� �ڽ� ������Ʈ�� �̵�Ű�� 
			�������޾� update�Լ��� ȣ���ϴ°��� �ƴ��� Ȯ���غ�����.

			���� ���� ������ �� rotOrbit.z+=10.f * gameTimer.getDeltaTime() �� ����������
			�۵��� �����ʳĴ°��̴�. �ʴ� 10�� ��ŭ rollȸ���� �϶���ϰ������..�� ���ɸ��鼭 �����̳İ�

			3�� 7�� �ذ���.

			��ŸŸ�Ӱ��� ���α׷� ���۽� ���������� ������ �ʱ�ȭ�Ǳ⶧���� ��ŸŸ���� 10���Ѿ�� 0���� �ʱ�ȭ�ϵ����ߴ�.
			�׸��� XMMatrixRotationRollPitchYawFromVector�Լ��� �޴� ���Ͱ��� �������μ� ó���Ѵ�.
		*/


		static XMFLOAT3 rotOrbit = {};
		rotOrbit.z += XMConvertToRadians( 90.f * gameTimer.getDeltaTime() );


		XMMATRIX calc_orbit= XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotOrbit));
		
		calc_world *= calc_orbit;
	}


	calc_world *= calc_parentMat;
	XMStoreFloat4x4(&worldMat, calc_world);

	UINT stride = sizeof(vertex_ptn);
	UINT offset = 0;

	//WVP buffer
	gameObject::SetWorldViewProj(worldMat, gameStatic.GetViewMat(), gameStatic.GetProjMat());

	//texture
	ID3D11ShaderResourceView* tmpTextureView = m_textureClass.GetTexture();
	gameStatic.getDeviceContext()->PSSetShaderResources(0, 1, &(tmpTextureView));

	//light buffer
	XMFLOAT4 diffuseColor = XMFLOAT4(212/255.f, 244/255.f, 250/255.f, 1);

	//XMVECTOR _xmlightDir = XMVector3Normalize(XMLoadFloat3( &XMFLOAT3(0, 0, 1)));
	XMFLOAT3 lightDir = XMFLOAT3(0, 0,1.f);
	//XMStoreFloat3(&lightDir, _xmlightDir);

	gameObject::SetLightBuffer(diffuseColor, lightDir);

	//
	gameStatic.getDeviceContext()->IASetInputLayout(m_IL);

	gameStatic.getDeviceContext()->VSSetShader(m_vs, NULL, 0);
	gameStatic.getDeviceContext()->PSSetShader(m_ps, NULL, 0);

	gameStatic.getDeviceContext()->PSSetSamplers(0, 1, &m_samplerState);
	
	//
	gameStatic.getDeviceContext()->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);

	gameStatic.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//gameStatic.getDeviceContext()->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);
	
	//if you want to disable rasterizer state then just put to null.
	_getDeviceContext->RSSetState(m_rasterizerState);

	// Render the triangle.
	//gameStatic.getDeviceContext()->DrawIndexed(indexCount, 0, 0);
	_getDeviceContext->Draw(importModel.GetVertexCount(), 0);
	

}

void cBox::update()
{
	if (m_parent)
		return;
	/*
	    ����Ƽ ������ ���
        float h = Input.GetAxisRaw ("Horizontal");
        float v = Input.GetAxisRaw ("Vertical");

		Vector3 dirVec=Vector3(h,0,v);
		dirVec.normalize();


	*/


	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_positionVector.y += 1.f * gameTimer.getDeltaTime();
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_positionVector.y += -1.f* gameTimer.getDeltaTime();
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_positionVector.x += -1.f* gameTimer.getDeltaTime();

	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_positionVector.x += 1.f* gameTimer.getDeltaTime();

	}

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		m_positionVector.z += 1.f* gameTimer.getDeltaTime();
	}
	if (GetKeyState(VK_TAB) & 0x8000)
	{
		m_positionVector.z += -1.f* gameTimer.getDeltaTime();
	}
	
	m_rotationVector.y+= XMConvertToRadians(90.f * gameTimer.getDeltaTime());
}


void cBox::CreateVertex()
{
	/*
	vertex_ptn v[] = {
		{XMFLOAT3(-1.f,0.f,0.f),XMFLOAT2(0.f,1.f),XMFLOAT3(0,0,-1)},
		{XMFLOAT3(0.f,1.f,0.f),XMFLOAT2(0.f,0.f),XMFLOAT3(0,0,-1) },
		{XMFLOAT3(1.f,0.f,0.f),XMFLOAT2(1.f,0.f),XMFLOAT3(0,0,-1) }
	};

	int count = sizeof(v) / sizeof(vertex_ptn);
	
	for (int i = 0; i < count; i+=3)
	{
		XMVECTOR v1=XMLoadFloat3(&(v[0].pos));
		XMVECTOR v2=XMLoadFloat3(&(v[1].pos));
		XMVECTOR v3=XMLoadFloat3(&(v[2].pos));

		XMVECTOR v1_face = XMVector3Normalize(XMVector3Cross(v1 - v2, v1 - v3));
		XMVECTOR v2_face = XMVector3Normalize(XMVector3Cross(v2 - v3, v2 - v1));
		XMVECTOR v3_face = XMVector3Normalize(XMVector3Cross(v3 - v1, v3 - v2));


		//XMVector3Normalize
		XMStoreFloat3(&(v[0].normal), v1_face);
		XMStoreFloat3(&(v[1].normal), v2_face);
		XMStoreFloat3(&(v[2].normal), v3_face);

		
		
	}
	
	int indices[] = {
		0,1,2
	};
	*/
	//4��3�� : importModel�� ���ؼ� vertex���۸� �����Ͽ���.
	D3D11_BUFFER_DESC VertexDesc = {};
	VertexDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexDesc.ByteWidth = sizeof(vertex_ptn) * importModel.GetVertexCount();
	VertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	vertex_ptn* tmp = importModel.GetVerticesInfo();
	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = tmp;

	UINT stride = sizeof(vertex_ptn);
	HRESULT hr=_getDevice->CreateBuffer(&VertexDesc, &vertexData, &m_VB);
	
	int a = 0;
}

void cBox::CreateRenderStates()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	_getDevice->CreateRasterizerState(&desc, &m_rasterizerState);






	/*
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
	*/
}
