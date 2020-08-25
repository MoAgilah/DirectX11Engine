////////////////////////////////////////////////////////////////////////////////
// Filename: BlankState.cpp
////////////////////////////////////////////////////////////////////////////////
#include "..\Headers\BlankState.h"
#include "..\Headers\Paddle.h"

#include <algorithm>
using namespace std;

XMFLOAT2 g_Point = XMFLOAT2(0, 0);
XMFLOAT2 g_Point2 = XMFLOAT2(400, 200);
float cnt = 1.f; float cnt2 = 1.f;

Paddle* g_paddle;

BlankState::BlankState(Graphics* graphics, CollisionMgr* collisions, Input* input, Timer* timer, string stateName)
{
	m_pGraphics = graphics;
	m_pCollisions = collisions;
	m_pInput = input;
	m_pTimer = timer;
	m_sStateName = stateName;
	m_pTimer->Reset();
}



bool BlankState::Initialise()
{
	bool result = false;

	// Set the initial position of the camera.
	m_pGraphics->GetCamera()->SetPosition(XMFLOAT3(0, 0, -16));

	//load textures here using texture manager in advance
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/square.png", TexType::WIC);
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/crosshair.png", TexType::WIC);
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/paddle.png", TexType::WIC);

	//use a crosshair to signify a visual point in space
	crosshair = m_pGraphics->GetSpriteMgr()->GetSprite();
	crosshair->Initialise(m_pGraphics->GetD3DMgr(), m_pGraphics->GetTextureMgr()->GetTexture("crosshair"), g_Point, XMFLOAT2(128.f, 128.f));

	
	g_paddle = new Paddle(m_pGraphics->GetSpriteMgr()->GetSprite(), m_pCollisions->Add2DBoundingCapsule(), m_pGraphics->GetScreenDimensions());
	g_paddle->InitialiseSprite(m_pGraphics->GetD3DMgr(), m_pGraphics->GetTextureMgr()->GetTexture("paddle"));

	return true;
}

void BlankState::Release()
{
}

void BlankState::Pause()
{
	m_pTimer->Stop();
}

void BlankState::Resume()
{
	m_pTimer->Start();
}

void BlankState::ProcessInputs()
{
	//bool point = false;

	//if (point)
	//{	
	//	//go up "I" 
	//	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('I'))	crosshair->Move(XMFLOAT2(0.f, -1.f));
	//	
	//	//go down "K" 
	//	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('K'))	crosshair->Move(XMFLOAT2(0.f, 1.f));

	//	// go right "L" 
	//	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('L'))	crosshair->Move(XMFLOAT2(1.f, 0.f));
	//	
	//	//go lett "J" 
	//	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('J'))	crosshair->Move(XMFLOAT2(-1.f, 0.f));
	//	
	//}
	//else 
	//{
	//	XMFLOAT2 oldPos = g_Point;
	//	//go up "I" 
	//	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('I'))	g_Point = XMFLOAT2(oldPos.x, oldPos.y - 1.f);

	//	//go down "K" 
	//	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('K'))	g_Point = XMFLOAT2(oldPos.x, oldPos.y + 1.f);

	//	// go right "L" 
	//	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('L'))	g_Point = XMFLOAT2(oldPos.x + 1.f, oldPos.y);

	//	//go lett "J" 
	//	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('J'))	g_Point = XMFLOAT2(oldPos.x - 1.f, oldPos.y);
	//}
}

void BlankState::Update(const float& deltaTime)
{
	m_pGraphics->GetCamera()->Update();

	crosshair->Update(deltaTime);

	g_paddle->Update(deltaTime);

	XMFLOAT2 q;
	float t;
	
	

	ProcessInputs();
}

void BlankState::Draw()
{
	// Clear the buffers to begin the scene.
	m_pGraphics->GetD3DMgr()->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	//render 3D stuffs

	//render 2D stuffs
	// Turn off the Z buffer to begin all 2D rendering.
	m_pGraphics->GetD3DMgr()->TurnZBufferOff();
	
	
	

	//Sprite drawing functionality
	m_pGraphics->GetSpriteMgr()->Begin();
	SpriteBatch* tmp = m_pGraphics->GetSpriteMgr()->GetSprteBatch();
	g_paddle->Render(tmp);

	

	m_pGraphics->GetSpriteMgr()->End();
	
	//Start--primitive batch i.e. debug draw 
#if _DEBUG
	//draw primitives
	m_pCollisions->DrawDebug(m_pGraphics);

	//draw lines, rays, etc.
	m_pCollisions->m_pDebugDraw->Begin();
	//m_pCollisions->m_pDebugDraw->DrawRay(lineSeg.GetBottom2D(), XMFLOAT2(1,-1), m_pGraphics);
	//m_pCollisions->m_pDebugDraw->DrawTriangle(&triangle, m_pGraphics);
	m_pCollisions->m_pDebugDraw->End();
#endif
	//End--primitive batch i.e. debug draw 

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_pGraphics->GetD3DMgr()->TurnZBufferOn();
	
	//imgui test
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//create ImGui test window
	ImGui::Text("Hello, world!");

	//assemble together draw data
	ImGui::EndFrame();
	ImGui::Render();
	//render draw data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_pGraphics->GetD3DMgr()->EndScene();
}
