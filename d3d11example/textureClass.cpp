#include "textureClass.h"



textureClass::textureClass()
{
}


textureClass::~textureClass()
{

	SAFE_RELEASE(m_textureView);
}

void textureClass::load(LPCWSTR _dir)
{
	ScratchImage* img=new ScratchImage();
	
	TexMetadata metaData;

	HRESULT result = LoadFromTGAFile(L"data/stone01.tga", &metaData, *img);

	HRESULT result2= CreateShaderResourceView(gameStatic.getDevice(), img->GetImages(), img->GetImageCount(), metaData, &m_textureView);

	delete img;

}

ID3D11ShaderResourceView * textureClass::GetTexture()
{
	return m_textureView;
}
