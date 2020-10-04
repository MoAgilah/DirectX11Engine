////////////////////////////////////////////////////////////////////////////////
// Filename: BlankState.h
////////////////////////////////////////////////////////////////////////////////
#ifndef BlankState_H
#define BlankState_H

////////////////////////
// MY CLASS INCLUDES  //
///////////////////////
#include "../Headers/Utils.h"
#include "../Headers/GameState.h"
#include "../Headers/Light.h"

#include "../Headers/Paddle.h"
#include "../Headers/Ball.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: BlankState
////////////////////////////////////////////////////////////////////////////////
class BlankState : public GameState
{
public:
	BlankState(Graphics* graphics, CollisionMgr* collisions, Input* input, Timer* timer, string stateName);
	bool Initialise();
	void Release();
	void Pause();
	void Resume();
	void ProcessInputs();
	void Update(const float& deltaTime);
	void Draw();
private:
	Paddle* m_pPaddles[2];
	Ball* m_pBall;

};

#endif
