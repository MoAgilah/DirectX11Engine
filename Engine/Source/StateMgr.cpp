////////////////////////////////////////////////////////////////////////////////
// Filename: StateMgr.cpp
////////////////////////////////////////////////////////////////////////////////

#include "../Headers/StateMgr.h"
#include "../Headers/GameState.h"

GameStateManager* GameStateManager::s_pInstance = nullptr;

void GameStateManager::ChangeState(GameState* state)
{
	if (!m_vGameStates.empty())
	{
		m_vGameStates.back()->Release();
		m_vGameStates.pop_back();
	}

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();
}

void GameStateManager::PushState(GameState* state)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();
}

void GameStateManager::PopState()
{
	if (!m_vGameStates.empty())
	{
		m_vGameStates.back()->Release();
		m_vGameStates.pop_back();
	}

	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void GameStateManager::Release()
{
	while (!m_vGameStates.empty())
	{
		m_vGameStates.back()->Release();
		m_vGameStates.pop_back();
	}
}

void GameStateManager::Pause()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();
}

void GameStateManager::Resume()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void GameStateManager::ProcessInputs()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->ProcessInputs();
}

void GameStateManager::Update(const float& deltaTime)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Update(deltaTime);
}

void GameStateManager::Draw()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Draw();
}
