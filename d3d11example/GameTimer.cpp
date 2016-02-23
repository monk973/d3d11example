#include "GameTimer.h"



CGameTimer::CGameTimer()
{

	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;

	int a = 0;
}


CGameTimer::~CGameTimer()
{
}

void CGameTimer::update()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// Time difference between this frame and the previous.
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

	// Prepare for next frame.
	mPrevTime = mCurrTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}

	m_elapsedTime += mDeltaTime;
	m_cnt++;
	if (m_elapsedTime >= 1.f)
	{

		std::ostringstream outs;
		//outs.precision(6);
		outs << "FPS: " << m_cnt << "    ";
		SetWindowText(gameStatic.GetHWND(), outs.str().c_str());

		m_elapsedTime = 0;
		m_cnt = 0;
	}
}

float CGameTimer::getDeltaTime()
{


	return mDeltaTime;
}
