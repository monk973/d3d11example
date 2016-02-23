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

	box2 = new cBox();
	box2->SetPosition(XMFLOAT3(0,2, 0));
	gameStatic.addGameObject(box2);

	for (auto o : gameStatic.getArrayGameObject())
	{
		o->init();
	}
}
