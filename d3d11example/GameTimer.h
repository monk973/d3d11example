#pragma once
#include "GameStatic.h"

class CGameTimer
{
public:
	CGameTimer();
	virtual ~CGameTimer();

	void update();
	float getDeltaTime();

	static CGameTimer& getInstance() { static CGameTimer inst; return inst; }

private:

	double mSecondsPerCount=0;
	double mDeltaTime = 0;

	__int64 mPrevTime = 0;
	__int64 mCurrTime = 0;

	float m_cnt = 0;
	float m_elapsedTime = 0;
};
#define gameTimer CGameTimer::getInstance()
