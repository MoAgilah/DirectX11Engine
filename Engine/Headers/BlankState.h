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
#include "../Headers/AABB.h"
#include "../Headers/OBB.h"
#include "../Headers/BoundingCapsule.h"
#include "../Headers/BoundingSphere.h"
using namespace DirectX;

enum CollType
{
	AABB2AABB, PNT2AABB, SPH2AABB, PNT2SPH, SPH2SPH, SPH2CAP, CAP2CAP, PNT2CAP, AABB2CAP
};

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
	Sprite* crosshair;
	Triangle triangle;
	LineSeg lineSeg;
	BoundingCapsule* cap1, *cap2;
	BoundingSphere *sph1, *sph2;
	AABB *box1, *box2;
	OBB *obox1, *obox2;
};

#endif
