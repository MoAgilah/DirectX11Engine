////////////////////////////////////////////////////////////////////////////////
// Filename: Framework.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Framework_H
#define Framework_H

/////////////////
// MY INCLUDES //
/////////////////
#include <string>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Input.h"
#include "../Headers/Graphics.h"
#include "../Headers/CollisionMgr.h"

#include "../Headers/Timer.h"
#include "../Headers/Helpers.h"

#include "../Headers/StateMgr.h"
#include "../Headers/DebugState.h"
#include "../Headers/DemoState3D.h"
#include "../Headers/DemoState2D.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Framework
////////////////////////////////////////////////////////////////////////////////
class Framework
{
public:
	Framework();
	~Framework();

	bool Initialise();
	void Release();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	void CalculateFrameStats();
private:
	void ProcessInput();
	void UpdateScene(const float& deltaTime);
	void DrawScene();

	bool InitWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

private:
	LPCSTR m_ApplicationName;
	HINSTANCE m_HInstance;
	HWND m_Hwnd;

	bool m_IsPaused;
	bool m_IsResizing;
	bool m_IsMinimised;
	bool m_IsMaximised;

	int m_FrameCnt;
	float m_TimeElapsed;
	float m_FrameTime;

	//Modules
	Timer m_Timer;
	unique_ptr<Input> m_pInput;
	unique_ptr<Graphics> m_pGraphics;
	unique_ptr<CollisionMgr> m_pCollisionsMgr;
	unique_ptr<GameStateManager> m_pStateManager;
};

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/////////////
// GLOBALS //
/////////////
static Framework* gs_pApplicationHandle = nullptr;


#endif