////////////////////////////////////////////////////////////////////////////////
// Filename: HierarchyState.h
////////////////////////////////////////////////////////////////////////////////
#ifndef HierarchyState_H
#define HierarchyState_H

////////////////////////
// MY CLASS INCLUDES  //
////////////////////////
#include "../Headers/Utils.h"
#include "../Headers/GameState.h"
#include "../Headers/Light.h"
#include "../Headers/Terrain.h"
#include "../Headers/Aeroplane.h"
#include "../Headers/Robot.h"

//////////////
// GLOBALS  //
//////////////
enum CameraState
{
	PLANE, GUN, ROBOT, FREE, MAX
};



////////////////////////////////////////////////////////////////////////////////
// Class name: HierarchyState
////////////////////////////////////////////////////////////////////////////////
class HierarchyState : public GameState
{
public:
	HierarchyState(Graphics* graphics, CollisionMgr* collisions, Input* input, Timer* timer, string stateName);
	bool Initialise();
	void Release();
	void Pause();
	void Resume();
	void ProcessInputs();
	void Update(const float& deltaTime);
	void Draw();
private:
	string GetCameraStateName();
	void UpdateCameraState();

	Terrain* m_pTerrain;
	Aeroplane* m_pPlane;
	Robot* m_pRobot;
	
	Light* m_pLight[4];
	float m_fRotationAngle;
	bool m_bWireframe;
	int m_iCameraState;
};

#endif
