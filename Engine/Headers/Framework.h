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
#include "../Headers/Utils.h"
#include "../Headers/Timer.h"
#include "../Headers/Input.h"
#include "../Headers/Graphics.h"
#include "../Headers/CollisionMgr.h"

#include "../Headers/StateMgr.h"
#include "../Headers/DebugState.h"
#include "../Headers/BlankState.h"
#include "../Headers/HierarchyState.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Framework
////////////////////////////////////////////////////////////////////////////////
class Framework
{
public:
	Framework();
	Framework(const Framework& other);
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

	void InitWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

private:
	LPCSTR m_lpApplicationName;
	HINSTANCE m_HInstance;
	HWND m_Hwnd;

	bool m_bIsPaused;

	//Modules
	Timer m_Timer;
	Input* m_pInput;
	Graphics* m_pGraphics;
	CollisionMgr* m_pCollisionsMgr;
	GameStateManager* m_pStateManager;
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