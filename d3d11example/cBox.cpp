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
	importModel.Init("data/cube.txt");

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
			자전 : 스스로 회전
			공전 : 대상을 기준으로 회전

			S*R*T*(ParentMat...) 자전상태
			S*R*T*R*(ParentMat...) 공전상태

			이동키를 눌를수록 자식 오브젝트가 이상한 방향으로 나아간다면 자식 오브젝트도 이동키에 
			영향을받아 update함수를 호출하는것은 아닌지 확인해보도록.

			이제 남은 문제는 왜 rotOrbit.z+=10.f * gameTimer.getDeltaTime() 이 정상적으로
			작동을 하지않냐는것이다. 초당 10도 만큼 roll회전을 하라고하고싶은데..왜 랙걸리면서 움직이냐고

			3월 7일 해결함.

			델타타임값이 프로그램 시작시 비정상적인 값으로 초기화되기때문에 델타타임이 10을넘어가면 0으로 초기화하도록했다.
			그리고 XMMatrixRotationRollPitchYawFromVector함수가 받는 벡터값은 라디안으로서 처리한다.
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
	//4월3일 : importModel을 통해서 vertex버퍼를 생성하엿다.
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
