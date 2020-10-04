////////////////////////////////////////////////////////////////////////////////
// Filename: Timer.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Timer.h"

Timer::Timer()
	: m_dSecondsPerCount(0.0), m_dDeltaTime(-1.0), m_i64BaseTime(0),
	m_i64PausedTime(0), m_i64PrevTime(0), m_i64CurrTime(0), m_bStopped(false), m_i64StopTime(0)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)& countsPerSec);
	m_dSecondsPerCount = 1.0 / (double)countsPerSec;
}


Timer::Timer(const Timer& other)
{
	m_dSecondsPerCount = other.m_dSecondsPerCount;
	m_dDeltaTime = other.m_dDeltaTime;
	m_i64BaseTime = other.m_i64BaseTime;
	m_i64PausedTime = other.m_i64PausedTime;
	m_i64PrevTime = other.m_i64PrevTime;
	m_i64CurrTime = other.m_i64CurrTime;
	m_bStopped = other.m_bStopped;
	m_i64StopTime = other.m_i64StopTime;
	m_dSecondsPerCount = other.m_dSecondsPerCount;
}

Timer::~Timer()
{}

float Timer::TotalTime()const 
{
	// If we are stopped, do not count the time that has passed
// since we stopped. Moreover, if we previously already had
// a pause, the distance mStopTime - mBaseTime includes paused
// time,which we do not want to count. To correct this, we can
// subtract the paused time from mStopTime:
	if (m_bStopped)
	{
		return (float)(((m_i64StopTime - m_i64PausedTime) -
			m_i64BaseTime) * m_dSecondsPerCount);
	}
	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count. To correct this, we can subtract
	// the paused time from mCurrTime:
	else
	{
		return (float)(((m_i64CurrTime - m_i64PausedTime) -
			m_i64BaseTime) * m_dSecondsPerCount);
	}

}

float Timer::DeltaTime()const // in seconds
{
	return (float)m_dDeltaTime;
}

void Timer::Reset() 
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)& currTime);
	m_i64BaseTime = currTime;
	m_i64PrevTime = currTime;
	m_i64StopTime = 0;
	m_bStopped = false;
}

void Timer::Start() 
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)& startTime);
	
	if (m_bStopped)
	{
		// then accumulate the paused time.
		m_i64PausedTime += (startTime - m_i64StopTime);

		// since we are starting the timer back up, the current
		// previous time is not valid, as it occurred while paused.
		// So reset it to the current time.
		m_i64PrevTime = startTime;
		
		m_i64StopTime = 0;
		m_bStopped = false;
	}

}

void Timer::Stop()
{
	if (!m_bStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)& currTime);
		
		m_i64StopTime = currTime;
		m_bStopped = true;
	}

}

void Timer::Tick() // Call every frame.
{
	if (m_bStopped)
	{
		m_dDeltaTime = 0.0;
		return;
	}

	// Get the time this frame.
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)& currTime);
	m_i64CurrTime = currTime;

	// Time difference between this frame and the previous.
	m_dDeltaTime = (m_i64CurrTime - m_i64PrevTime) * m_dSecondsPerCount;
	// Prepare for next frame.
	m_i64PrevTime = m_i64CurrTime;

	// Force nonnegative. 
	if (m_dDeltaTime < 0.0)
	{
		m_dDeltaTime = 0.0;
	}

}

