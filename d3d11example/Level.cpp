#include "Level.h"



CLevel::CLevel()
{
}


CLevel::~CLevel()
{
}

void CLevel::createLevel()
{
	box = new cBox();
	gameStatic.addGameObject(box);
	box->SetPosition(XMFLOAT3( 0, 0, 0));

	box2 = new cBox();
	box2->SetPosition(XMFLOAT3(0,2.4f, 0));
	gameStatic.addGameObject(box2);

	box2->SetParent(box);

	for (auto o : gameStatic.getArrayGameObject())
	{
		o->init();
	}
}
