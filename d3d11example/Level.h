#pragma once
#include "GameStatic.h"
#include "LevelObjects.h"

/*
	만약 툴을 만들게된다면 툴에서 맵을 생성하고 맵에 오브젝트를 배치했을때 
	gameStatic.addGameObject() 함수만을 호출해서 오브젝트를 추가시켜주면된다.

	지금은 툴이없는 상황이므로 하드코딩으로 클래스들을 추가하여 gameStatic.addGameObject()함수를 호출한다.
*/

class CLevel
{
public:
	CLevel();
	virtual ~CLevel();

	void createLevel();
public:
	cBox* box;
};

