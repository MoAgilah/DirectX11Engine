////////////////////////////////////////////////////////////////////////////////
// Filename: DebugState.h
////////////////////////////////////////////////////////////////////////////////
#ifndef DebugState_H
#define DebugState_H

////////////////////////
// MY CLASS INCLUDES  //
////////////////////////
#include "../Headers/Utils.h"
#include "../Headers/GameState.h"
#include "../Headers/Light.h"
#include "../Headers/Terrain.h"
#include "../Headers/Static3DObj.h"
#include "../Headers/Sprite.h"
#include "../Headers/AnimatedSprite.h"
#include "../Headers/TextMgr.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: DebugState
////////////////////////////////////////////////////////////////////////////////
class DebugState : public GameState
{
public:
	DebugState(Graphics* graphics, CollisionMgr* collisions, Input* input, Timer* timer, string stateName);
	bool Initialise();
	void Release();
	void Pause();
	void Resume();
	void ProcessInputs();
	void Update(const float& deltaTime);
	void Draw();
private:
	Terrain* m_pTerrain;
	Light* m_pLight[4];
	AnimatedSprite* m_pAnimSprite;
	Sprite* crosshair;
};

#endif
