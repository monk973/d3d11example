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

