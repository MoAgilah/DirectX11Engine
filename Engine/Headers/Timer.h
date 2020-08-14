////////////////////////////////////////////////////////////////////////////////
// Filename: Timer.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Timer_H
#define Timer_H

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Timer
////////////////////////////////////////////////////////////////////////////////
class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();
	float TotalTime() const; // in seconds
	float DeltaTime() const; // in seconds
	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop(); // Call when paused.
	void Tick(); // Call every frame.
private:
	double m_dSecondsPerCount;
	double m_dDeltaTime;
	__int64 m_i64BaseTime;
	__int64 m_i64PausedTime;
	__int64 m_i64StopTime;
	__int64 m_i64PrevTime;
	__int64 m_i64CurrTime;
	bool m_bStopped;
};

#endif