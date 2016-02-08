#pragma once
#include "GameStatic.h"

class textureClass
{
public:
	textureClass();
	virtual ~textureClass();


public:

	void load(LPCWSTR _dir);
	ID3D11ShaderResourceView* GetTexture();


private:
	ID3D11ShaderResourceView* m_textureView;
};

/*
나중에 해보기

struct TargaHeader
{
unsigned char data1[12];
unsigned short width;
unsigned short height;
unsigned char bpp;
unsigned char data2;
};

unsigned char* m_targaData;
*/