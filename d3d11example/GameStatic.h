#pragma once
#include <Windows.h>

#include <d3d11_1.h>
#include <xnamath.h>

#include <vector>
#include <map>
#include <string>

#include <iostream>
#include <stdio.h>
using namespace std;
#include <fstream>

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
	void SetViewMat(XMMATRIX& _view);
	XMMATRIX GetViewMat();
	void SetProjMat(XMMATRIX& _proj);
	XMMATRIX GetProjMat();
	
	void SetFeatureLevel(D3D_FEATURE_LEVEL _level);
	int GetFeatureLevel();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;

	vector<class gameObject*> m_arrayGameObject;

	XMMATRIX m_view;
	XMMATRIX m_proj;

	D3D_FEATURE_LEVEL m_featureLevel;
};
#define gameStatic CGameStatic::GetInstance()
#define getFeatureLevel CGameStatic::GetInstance().GetFeatureLevel()
#define _getDevice CGameStatic::GetInstance().getDevice()
#define _getDeviceContext CGameStatic::GetInstance().getDeviceContext()
#define SAFE_RELEASE(t) if(t) { t->Release(); }

class gameObject
{
public:
	virtual ~gameObject();
public:
	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void update() = 0; 
protected:
	void CreateShaderAndConstantBuffer(const char* _vsDir, const char* _psDir);
	void SetWorldViewProj(XMMATRIX& _w, XMMATRIX& _v, XMMATRIX& _p);

protected:
	ID3D11Buffer* m_VB = 0;
	ID3D11Buffer* m_IB = 0;


	UINT indexCount = 0;

	ID3D11Buffer* m_matrixBuffer = 0;
	ID3D11VertexShader* m_vs = 0;
	ID3D11PixelShader* m_ps = 0;
	ID3D11InputLayout* m_IL = 0;


	ID3D11SamplerState* m_samplerState = nullptr;
};
