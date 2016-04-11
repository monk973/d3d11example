#pragma once
#include "GameStatic.h"

class gameObject
{
private:
	struct lightBufferType {
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDir;
		float pad;

	};

public:
	gameObject();
	virtual ~gameObject();
public:
	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void update() = 0;

	void SetParent(gameObject* _p);
	gameObject* GetParent();

	XMFLOAT4X4 CalculateWorldMatrix();

	void SetPosition(XMFLOAT3 _v);
	void SetRotation(XMFLOAT3 _v);
	void SetScale(XMFLOAT3 _v);
protected:
	void CreateShaderAndConstantBuffer(const char* _vsDir, const char* _psDir);
	void SetWorldViewProj(XMFLOAT4X4& _w, XMFLOAT4X4& _v, XMFLOAT4X4& _p);
	void SetLightBuffer(XMFLOAT4 _diffuseColor, XMFLOAT3 _lightDir);

	XMFLOAT4X4 CalculateParentMatrices(gameObject* _p);

protected:
	ID3D11Buffer* m_VB = 0;
	ID3D11Buffer* m_IB = 0;


	UINT indexCount = 0;

	ID3D11Buffer* m_matrixBuffer = 0;
	ID3D11VertexShader* m_vs = 0;
	ID3D11PixelShader* m_ps = 0;
	ID3D11InputLayout* m_IL = 0;


	ID3D11SamplerState* m_samplerState = nullptr;

	ID3D11Buffer* m_lightBuffer = nullptr;
protected:

	XMFLOAT3 m_scaleVector;
	XMFLOAT3 m_positionVector;
	XMFLOAT3 m_rotationVector;

	//

	XMFLOAT4X4 m_matScale = {};
	XMFLOAT4X4 m_matRot = {};
	XMFLOAT4X4 m_matTranslation = {};



	//���÷� ������Ʈ �����ʰ� CalculateWorldMatrix �Լ� ȣ��ÿ��� Ʈ�������� ������Ʈ�ǵ����ߴ�.
	//update�Լ��� �����ڸ��� Ʈ�������� ������Ʈ �ϴ½������ϸ� ��İ���� �� update�Լ��� ����������
	//�ش뼭 ���� �������Ű��⿡ ����ȭ�� ���ؼ� �̷����ߴ�.
	//���ϴ� ������Ʈ�� Ʈ�������� ���ʹٸ� CalculateWorldMatrix �Լ��� ȣ���ؼ� �������� �Ѵ�.
	XMFLOAT4X4 m_matTransform = {};

	gameObject* m_parent = nullptr;


};