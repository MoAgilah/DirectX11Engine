////////////////////////////////////////////////////////////////////////////////
// Filename: DebugState.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/DebugState.h"
#include "../Headers/StateMgr.h"

static bool tiled = false;

DebugState::DebugState(Graphics* graphics, CollisionMgr* collisions, Timer* timer, string stateName)
{
	m_pGraphics = graphics;
	m_pCollisions = collisions;
	m_pTimer = timer;
	m_pTimer->Reset();
	m_sStateName = stateName;
}

bool DebugState::Initialise()
{
	return true;
}

void DebugState::Release()
{
}

void DebugState::Pause()
{
	m_pTimer->Stop();
}

void DebugState::Resume()
{
	m_pTimer->Start();
}

void DebugState::ProcessInputs()
{

}

void DebugState::Update(const float& deltaTime)
{
	ProcessInputs();
}

void DebugState::Draw()
{
	// Clear the buffers to begin the scene.
	m_pGraphics->GetD3DMgr()->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	//render 3D stuffs

	//render 2D stuffs
	// Turn off the Z buffer to begin all 2D rendering.
	m_pGraphics->GetD3DMgr()->TurnZBufferOff();

	//render sprites
	m_pGraphics->GetSpriteMgr()->Begin();
	m_pGraphics->GetSpriteMgr()->End();

	//m_pCollisions->DrawDebug(m_pGraphics);
	//render debug shapes
	m_pCollisions->m_pDebugDraw->Begin();
	m_pCollisions->m_pDebugDraw->End();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_pGraphics->GetD3DMgr()->TurnZBufferOn();;

	m_pGraphics->GetD3DMgr()->EndScene();
}
