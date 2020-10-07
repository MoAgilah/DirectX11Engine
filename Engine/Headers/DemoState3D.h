////////////////////////////////////////////////////////////////////////////////
// Filename: DemoState3D.h
////////////////////////////////////////////////////////////////////////////////
#ifndef DemoState3D_H
#define DemoState3D_H

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
// Class name: DemoState3D
////////////////////////////////////////////////////////////////////////////////
class DemoState3D : public GameState
{
public:
	DemoState3D(Graphics* graphics, CollisionMgr* collisions, Timer* timer, string stateName);
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
