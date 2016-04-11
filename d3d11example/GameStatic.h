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

#include <math.h>


//ADD CUSTOM CLASS or HEADER HERE
#include "gameObject.h"
#include "VertexMatrixTypes.h"
#include "gameCamera.h"

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


