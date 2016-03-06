#pragma once
#include <Windows.h>

#include <d3d11_1.h>
#include <D3DX11.h>
#include <xnamath.h>

#include <vector>
#include <map>
#include <string>

#include <iostream>
#include <stdio.h>
using namespace std;
#include <fstream>
#include <sstream>

//ADD CUSTOM CLASS HEADER HERE
#include "VertexMatrixTypes.h"
#include "gameCamera.h"
#include <math.h>

#include "DirectXTex.h"
using namespace DirectX;

#include "textureClass.h"

#include "GameTimer.h"

class CGameStatic
{
public:
	CGameStatic();
	~CGameStatic();

public:
	static CGameStatic& GetInstance()
	{
		static CGameStatic instance;

		return instance;
	}

	void addGameObject(class gameObject* _obj);
	vector<class gameObject*>& getArrayGameObject();

	void SetDevice(ID3D11Device* _device);
	ID3D11Device*& getDevice();
	void SetDeviceContext(ID3D11DeviceContext* _context);
	ID3D11DeviceContext*& getDeviceContext();

	
	/*
		Add here View,Proj Mat
	*/
	void SetViewMat(XMFLOAT4X4& _view);
	XMFLOAT4X4 GetViewMat();
	void SetProjMat(XMFLOAT4X4& _proj);
	XMFLOAT4X4 GetProjMat();
	
	void SetFeatureLevel(D3D_FEATURE_LEVEL _level);
	int GetFeatureLevel();

	HWND GetHWND();
	void SetHWND(HWND _hWND);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;

	vector<class gameObject*> m_arrayGameObject;

	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;

	D3D_FEATURE_LEVEL m_featureLevel;

	HWND m_hWnd;
};
#define gameStatic CGameStatic::GetInstance()
#define getFeatureLevel CGameStatic::GetInstance().GetFeatureLevel()
#define _getDevice CGameStatic::GetInstance().getDevice()
#define _getDeviceContext CGameStatic::GetInstance().getDeviceContext()
#define SAFE_RELEASE(t) if(t) { t->Release(); }

class gameObject
{
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

protected:

	XMFLOAT3 m_scaleVector;
	XMFLOAT3 m_positionVector;
	XMFLOAT3 m_rotationVector;

	//

	XMFLOAT4X4 m_matScale = {};
	XMFLOAT4X4 m_matRot = {};
	XMFLOAT4X4 m_matTranslation = {};



	//수시로 업데이트 되지않고 CalculateWorldMatrix 함수 호출시에만 트랜스폼이 업데이트되도록했다.
	//update함수가 끝나자마자 트랜스폼을 업데이트 하는식으로하면 행렬계산을 매 update함수가 끝날시점에
	//해대서 왠지 느려질거같기에 최적화를 위해서 이렇게했다.
	//원하는 오브젝트의 트랜스폼을 얻고싶다면 CalculateWorldMatrix 함수를 호출해서 얻어오도록 한다.
	XMFLOAT4X4 m_matTransform = {};

	gameObject* m_parent = nullptr;
};
