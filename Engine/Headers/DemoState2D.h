////////////////////////////////////////////////////////////////////////////////
// Filename: DemoState2D.h
////////////////////////////////////////////////////////////////////////////////
#ifndef DemoState2D_H
#define DemoState2D_H

////////////////////////
// MY CLASS INCLUDES  //
///////////////////////
#include "../Headers/Utils.h"
#include "../Headers/GameState.h"

#include "../Headers/Paddle.h"
#include "../Headers/Ball.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: DemoState2D
////////////////////////////////////////////////////////////////////////////////
class DemoState2D : public GameState
{
public:
	DemoState2D(Graphics* graphics, CollisionMgr* collisions, Timer* timer, string stateName);
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
