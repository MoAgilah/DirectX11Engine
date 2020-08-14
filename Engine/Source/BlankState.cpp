////////////////////////////////////////////////////////////////////////////////
// Filename: BlankState.cpp
////////////////////////////////////////////////////////////////////////////////
#include "..\Headers\BlankState.h"

#include <algorithm>
using namespace std;

XMFLOAT2 g_Point = XMFLOAT2(100, 200);
XMFLOAT2 g_Point2 = XMFLOAT2(400, 200);
float cnt = 1.f; float cnt2 = 1.f;

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
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/textbox.png", TexType::WIC);

	//use a crosshair to signify a visual point in space
	crosshair = m_pGraphics->GetSpriteMgr()->GetSprite();
	crosshair->Initialise(m_pGraphics->GetD3DMgr(), m_pGraphics->GetTextureMgr()->GetTexture("crosshair"), g_Point, XMFLOAT2(128.f, 128.f));

	

	obox1 = m_pCollisions->Add2DOBB();
	obox1->Update(g_Point, XMFLOAT2(256, 256),XMFLOAT2(XMConvertToRadians(90),0));

	/*obox1 = m_pCollisions->Add2DOBB();
	obox1->Update(g_Point, XMFLOAT2(256, 256), XMFLOAT2(XMConvertToRadians(180), 0));*/

	/*box1 = m_pCollisions->Add2DAABB();
	box1->Update(g_Point, XMFLOAT2(256, 256));

	sph1 = m_pCollisions->Add2DBoundingSphere();
	sph1->Update(g_Point, XMFLOAT2(256, 256));

	

	cap2 = m_pCollisions->Add2DBoundingCapsule();
	cap2->Update(g_Point2, XMFLOAT2(256, 256));
	
	lineSeg = LineSeg(XMFLOAT2(100, 100), XMFLOAT2(300, 400));

	triangle = Triangle(XMFLOAT2(400, 400), XMFLOAT2(700, 400), XMFLOAT2(550, 100));*/

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
	bool point = false;

	if (point)
	{	
		//go up "I" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('I'))	crosshair->Move(XMFLOAT2(0.f, -1.f));
		
		//go down "K" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('K'))	crosshair->Move(XMFLOAT2(0.f, 1.f));

		// go right "L" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('L'))	crosshair->Move(XMFLOAT2(1.f, 0.f));
		
		//go lett "J" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('J'))	crosshair->Move(XMFLOAT2(-1.f, 0.f));
		
	}
	else 
	{
		XMFLOAT2 oldPos = g_Point;
		//go up "I" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('I'))	g_Point = XMFLOAT2(oldPos.x, oldPos.y - 1.f);

		//go down "K" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('K'))	g_Point = XMFLOAT2(oldPos.x, oldPos.y + 1.f);

		// go right "L" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('L'))	g_Point = XMFLOAT2(oldPos.x + 1.f, oldPos.y);

		//go lett "J" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('J'))	g_Point = XMFLOAT2(oldPos.x - 1.f, oldPos.y);
	}
}

void BlankState::Update(const float& deltaTime)
{
	m_pGraphics->GetCamera()->Update();

	crosshair->Update(deltaTime);

	XMFLOAT2 q;
	float t;
	
	//g_Point2.x -= 10.f;
	//g_Point.x += 10.f;

	cnt += 0.5f;
	cnt2 += deltaTime * 10.f;

	if (cnt2 >= 360)
		cnt2 = 0.0f;

	if (cnt >= 360)
		cnt = 0.0f;

	if (cnt > 360) cnt = 0;

	obox1->Update(g_Point, XMFLOAT2(256, 256), XMFLOAT2(XMConvertToRadians(cnt), XMConvertToRadians(cnt2)));

	//sph1->Update(g_Point, XMFLOAT2(256, 256));

	//g_Point2 = XMFLOAT2(500, -600);
	//float d;
	//XMVECTOR ba = XMVectorSubtract(XMLoadFloat2(&g_Point2), XMLoadFloat2(&lineSeg.GetBottom2D()));
	//float baLen = *XMVector2Length(ba).m128_f32;
	//XMFLOAT2 dir; XMStoreFloat2(&dir,
	//ba / baLen);
	//

	//sph1->IntersectRay2D(lineSeg.GetBottom2D(), dir, t,q);

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
	Sprite spr;
	spr.Initialise(m_pGraphics->GetD3DMgr(), m_pGraphics->GetTextureMgr()->GetTexture("textbox"), g_Point, XMFLOAT2(0, 0), XMFLOAT2(1, 1), cnt);
	spr.Update(1 / 60.f);
	spr.NRender(tmp);

	//crosshair->NRender(tmp);

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
