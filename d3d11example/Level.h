#pragma once
#include "GameStatic.h"
#include "LevelObjects.h"

/*
	���� ���� ����Եȴٸ� ������ ���� �����ϰ� �ʿ� ������Ʈ�� ��ġ������ 
	gameStatic.addGameObject() �Լ����� ȣ���ؼ� ������Ʈ�� �߰������ָ�ȴ�.

	������ ���̾��� ��Ȳ�̹Ƿ� �ϵ��ڵ����� Ŭ�������� �߰��Ͽ� gameStatic.addGameObject()�Լ��� ȣ���Ѵ�.
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

