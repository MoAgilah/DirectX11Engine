#ifndef Paddle_H
#define Paddle_H

#include "../Headers/Input.h"
#include "../Headers/GameObject2D.h"
#include "../Headers/Sprite.h"
#include "../Headers/BoundingCapsule.h"

class Paddle : public GameObject2D
{
public:
	Paddle(Sprite* spr, BoundingCapsule* cap, XMFLOAT2 scrDim, int plyID, bool AiCont = false);
	bool InitialiseSprite(D3DMgr* d3dMgr, Texture* texture);
	~Paddle();
	void MoveUp(float vel);
	void MoveDown(float vel);
	void Update(const float& dt);
	void Render(SpriteBatch* sprBatch);
	BoundingCapsule* GetBoundingCap() { return m_pBoundingCapsule; }
	float GetHighestY();
	float GetLowestY();
	int GetPadID() { return m_iPlayerID; }
private:
	void ProcessInput();
	void ProcessAIInput();
	Sprite* m_pSprite;
	BoundingCapsule* m_pBoundingCapsule;
	bool m_bPlayerControl;
	int m_iPlayerID;
	float m_fHeight;
	float m_fSpeed;
	int m_iScore;
};




#endif // !Paddle_H
