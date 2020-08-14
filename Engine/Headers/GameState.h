////////////////////////////////////////////////////////////////////////////////
// Filename: DebugState.h
////////////////////////////////////////////////////////////////////////////////
#ifndef GameState_H
#define GameState_H

////////////////////////
// MY CLASS INCLUDES  //
////////////////////////
#include "../Headers/Graphics.h"
#include "../Headers/CollisionMgr.h"
#include "../Headers/Input.h"
#include "../Headers/Timer.h"
#include "../Headers/StateMgr.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GameState
////////////////////////////////////////////////////////////////////////////////
class GameState
{
public:
	GameState()
		:m_pGraphics(nullptr), m_pInput(nullptr), m_pTimer(nullptr), m_pCollisions(nullptr),m_sStateName("")
	{}

	virtual bool Initialise() = 0;
	virtual void Release() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void ProcessInputs() = 0;
	virtual void Update(const float& deltaTime) = 0;
	virtual void Draw() = 0;

	string GetStateName() { return string("Current state:- ") + m_sStateName; }
protected:
	Graphics* m_pGraphics;
	CollisionMgr* m_pCollisions;
	Input* m_pInput;
	Timer* m_pTimer;
	string m_sStateName;
	~GameState() {}
};

#endif
