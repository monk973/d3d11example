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

	for (auto o : gameStatic.getArrayGameObject())
	{
		o->init();
	}
}
