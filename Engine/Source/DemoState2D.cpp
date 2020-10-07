////////////////////////////////////////////////////////////////////////////////
// Filename: DemoState2D.cpp
////////////////////////////////////////////////////////////////////////////////
#include "..\Headers\DemoState2D.h"

#include <algorithm>
using namespace std;

DemoState2D::DemoState2D(Graphics* graphics, CollisionMgr* collisions, Timer* timer, string stateName)
{
	m_pGraphics = graphics;
	m_pCollisions = collisions;
	m_pTimer = timer;
	m_sStateName = stateName;
	m_pTimer->Reset();
}

bool DemoState2D::Initialise()
{
	bool res = false;

	// Set the initial position of the camera.
	m_pGraphics->GetCamera()->SetPosition(XMFLOAT3(0, 0, -16));

	//load textures here using texture manager in advance
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/paddle.png", TexType::WIC);
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/ball1.png", TexType::WIC);

	m_pPaddles[0] = new Paddle(m_pGraphics->GetSpriteMgr()->GetSprite(), m_pCollisions->Add2DBoundingCapsule(), XMFLOAT2(gs_SystPref.gf_ScreenWidth, gs_SystPref.gf_ScreenHeight),0,false);
	res = m_pPaddles[0]->InitialiseSprite(m_pGraphics->GetD3DMgr(), m_pGraphics->GetTextureMgr()->GetTexture("paddle"));
	if (!res)
	{
		//error msg
	}

	m_pPaddles[1] = new Paddle(m_pGraphics->GetSpriteMgr()->GetSprite(), m_pCollisions->Add2DBoundingCapsule(), XMFLOAT2(gs_SystPref.gf_ScreenWidth, gs_SystPref.gf_ScreenHeight),1,false);
	res = m_pPaddles[1]->InitialiseSprite(m_pGraphics->GetD3DMgr(), m_pGraphics->GetTextureMgr()->GetTexture("paddle"));
	if (!res)
	{
		//error msg
	}

	m_pBall = new Ball(m_pGraphics->GetSpriteMgr()->GetSprite(), m_pCollisions->Add2DBoundingSphere(), XMFLOAT2(gs_SystPref.gf_ScreenWidth, gs_SystPref.gf_ScreenHeight), 0, false);
	res = m_pBall->InitialiseSprite(m_pGraphics->GetD3DMgr(), m_pGraphics->GetTextureMgr()->GetTexture("ball1"));
	if (!res)
	{
		//error msg
	}

	return true;
}

void DemoState2D::Release()
{
	SAFE_DELETE(m_pPaddles[0]);
	SAFE_DELETE(m_pPaddles[1]);
	SAFE_DELETE(m_pBall);
}

void DemoState2D::Pause()
{
	m_pTimer->Stop();
}

void DemoState2D::Resume()
{
	m_pTimer->Start();
}

void DemoState2D::ProcessInputs()
{
	
}

void DemoState2D::Update(const float& deltaTime)
{
	m_pGraphics->GetCamera()->Update();

	m_pPaddles[0]->Update(deltaTime);
	m_pPaddles[1]->Update(deltaTime);

	m_pBall->Update(deltaTime);
	
//process collisions
	float t;
	XMFLOAT2 cp;
	for (int i = 0; i < 2; i++)
	{
		//skip collision checks if ball is moving away from the paddle
		if ((i == 0 && m_pBall->GetVelocity().x > 0) || (i == 1 && m_pBall->GetVelocity().x < 0)) continue;

		//early out if no rudimentary collision
		if (m_pPaddles[i]->GetBoundingCap()->IntersectSphere2D(m_pBall->GetBoundingSphere()))
		{
			//get the closest distance on the paddle to the ball's center
			m_pPaddles[i]->GetBoundingCap()->GetLineSeg().ClosestPtPointSegment2D(m_pBall->GetPosition(), t, cp);

			//perform more accurate collision check for accurate collision resolution
			if (m_pBall->GetBoundingSphere()->IntersectMovingSphere2D(&BoundingSphere(false, cp, m_pPaddles[i]->GetBoundingCap()->GetRadius()), m_pBall->GetVelocity(), XMFLOAT2(0, 0), t))
			{
				m_pBall->ResolveCol(cp);
				m_pBall->RespondToCol(m_pPaddles[i]);
				break;
			}
		}
	}

	ProcessInputs();
}

void DemoState2D::Draw()
{
	// Clear the buffers to begin the scene.
	m_pGraphics->GetD3DMgr()->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	//render 3D--------------------------------------------------
	//end--------------------------------------------------------

	
	// Turn off the Z buffer to begin all 2D rendering.
	m_pGraphics->GetD3DMgr()->TurnZBufferOff();
	//render 2D--------------------------------------------------

	//Begin the sprite batch
	m_pGraphics->GetSpriteMgr()->Begin();
	SpriteBatch* tmp = m_pGraphics->GetSpriteMgr()->GetSprteBatch();
	
	//render player paddles
	m_pPaddles[0]->Render(tmp);
	m_pPaddles[1]->Render(tmp);

	m_pBall->Render(tmp);

	m_pGraphics->GetSpriteMgr()->End();
	//End the sprite batch
	
	//Start--primitive batch i.e. debug draw 
#if _DEBUG
	//draw primitives
	m_pCollisions->DrawDebug(m_pGraphics);
#endif
	//End--primitive batch i.e. debug draw 

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_pGraphics->GetD3DMgr()->TurnZBufferOn();
	
	//imgui information form
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
