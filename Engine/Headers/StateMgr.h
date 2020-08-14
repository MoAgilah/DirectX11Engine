////////////////////////////////////////////////////////////////////////////////
// Filename: StateMgr.h
////////////////////////////////////////////////////////////////////////////////
#ifndef StateMgr_H
#define StateMgr_H

//////////////////
// MY INCLUDES  //
//////////////////
#include <vector>

//////////////////////////
// FORWARD DECLARATION  //
//////////////////////////
class GameState;

////////////////////////////////////////////////////////////////////////////////
// Class name: GameStateManager
////////////////////////////////////////////////////////////////////////////////
class GameStateManager
{
public:
	static GameStateManager* GetStateMgr()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new GameStateManager;
		}

		return s_pInstance;
	}
	void ChangeState(GameState* state);
	void PushState(GameState * state);
	void PopState();
	void Release();
	void Pause();
	void Resume();
	void ProcessInputs();
	void Update(const float& deltaTime);
	void Draw();
private:
	static GameStateManager* s_pInstance;
	std::vector<GameState*> m_vGameStates;
};


#endif // !1

