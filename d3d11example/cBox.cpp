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
이 함수의 인자 asd는 포인터로서 정의된듯하다.
본래라면 일반적인 char 배열타입으로 인식될것이 위의 명령들처럼 포인터변수를 받는것을보자면 
함수의 인자로서 행동할경우에는 포인터타입으로 행동하는것처럼보인다.

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
	CreateVertex();

	bool ShaderModel;
	
	/*
	char as[] = "asd";
	위의 구문에서 []는 포인터처럼 쓸수있게 해주는 간편한 기능으로서 존재할뿐이고 
	실제로는 포인터가아니다.
	asd = foo1();
	위의 함수를 실행해보면 값이 수정할수있는 Value여야한다는 오류를 낸다.
	변수 as의 타입이 포인터타입이였다면 성공했겠지만 실패했으므로 컴파일러 내부에서는 변수 as타입은 포인터타입이아니라
	편의성으로 동작하는 char타입이다.

	코드영역에대해서도 알아봤다. 어쩔때 문자열들이 코드영역에 올라갈까 실험했다.
	char * asd;
	asd=foo1();
	위를 실행할경우 원래대로라면 foo1함수가 종료됐을때 함수의 지역변수들도 삭제되면서 정보들이 사라져야하는데 
	리턴되어 받아온값이 온전했다. 그렇다는것은 포인터 변수에 문자열 상수를 대입할경우 그 문자열 상수는 코드영역에 입력되고
	코드영역에 있는 문자열 상수를 foo1함수가 던진것으로 보인다.
	코드영역은 프로그램이 종료될때 삭제된다. 자동영역과 마찬가지로.

	*/

	/*
	char asd[]= getFeatureLevel >= D3D_FEATURE_LEVEL_11_0 ? "shader 5.0" : "shader 4.0";

	이 명령이 오류가 나는 이유는 위의 주석들에서 설명한것처럼 변수 asd는 컴파일러에서는 이를 포인터타입이아닌 
	char 배열타입으로 인식한다. 그리고 삼항연산자가 리턴하는 타입은 포인터타입이다. 
	다시말해서 삼항연산자도 foo1()함수처럼 일종의 함수이고 foo1함수가 리턴하는 타입과 마찬가지로 
	포인터타입이기때문에 "char 배열타입인" asd는 이를 받을수없다. 포인터 타입은 포인터타입에 받아야하고
	char 배열타입에는 그 자리에서 즉시 문자 상수열을 입력해줘야한다. 
	*/

	string vsDir = getFeatureLevel >= D3D_FEATURE_LEVEL_11_0 ? "shader 5.0" : "shader 4.0";
	string psDir = getFeatureLevel >= D3D_FEATURE_LEVEL_11_0 ? "shader 5.0" : "shader 4.0";

	vsDir.append("/triangleVertexShader.cso");
	psDir.append("/trianglePixelShader.cso");


	gameObject::CreateShaderAndConstantBuffer(vsDir.c_str(), psDir.c_str());
	CreateRenderStates();
	m_textureClass.load(L"data/stone01.tga");
}


void cBox::draw()
{
	/*
	XMMATRIX scale = XMMatrixIdentity();
	XMMATRIX rot = XMMatrixIdentity();
	XMMATRIX trans = XMMatrixTranslation(m_positionVector.x, m_positionVector.y, m_positionVector.z);

	XMMATRIX world = scale*rot*trans;
	*/

	XMFLOAT4X4 worldMat= CalculateWorldMatrix();

	UINT stride = sizeof(vertex_pt);
	UINT offset = 0;

	//
	gameObject::SetWorldViewProj(worldMat, gameStatic.GetViewMat(), gameStatic.GetProjMat());

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
	
	//if you want to disable rasterizer state then just put to null.
	//_getDeviceContext->RSSetState(m_rasterizerState);

	// Render the triangle.
	gameStatic.getDeviceContext()->DrawIndexed(indexCount, 0, 0);

	

}

void cBox::update()
{
	/*
	    유니티 엔진의 방식
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
	

}

void cBox::SetPosition(XMFLOAT3 _v)
{
	m_positionVector = _v;
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
	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = D3D11_FILL_WIREFRAME;
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
