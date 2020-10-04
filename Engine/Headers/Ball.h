#ifndef Ball_H
#define Ball_H

#include "../Headers/Input.h"
#include "../Headers/GameObject2D.h"
#include "../Headers/Sprite.h"
#include "../Headers/BoundingSphere.h"
#include "../Headers/Paddle.h"

class Ball : public GameObject2D
{
public:
	Ball(Sprite* spr, BoundingSphere* sprBnd, XMFLOAT2 scrDim, int ballID, bool debugCtrl);
	bool InitialiseSprite(D3DMgr* d3dMgr, Texture* texture);
	~Ball();
	void Update(const float& dt);
	void Render(SpriteBatch* sprBatch);
	void setpos(XMFLOAT2 pos) {
		m_pSprite->SetPosition(pos);
		SetPosition(m_pSprite->GetPosition());
	}
	void Move();
	void ResolveCol(XMFLOAT2 cp);
	void RespondToCol(Paddle *pad);
	BoundingSphere* GetBoundingSphere() { return m_pBoundingSphere; }
	XMFLOAT2 GetVelocity() { return m_f2Vel; }
	float GetRadius() { return  m_pBoundingSphere->GetRadius(); }
	
	XMFLOAT2 m_f2Vel;
private:
	void ProcessInput();
	bool m_bActive;
	Sprite* m_pSprite;
	BoundingSphere* m_pBoundingSphere;
	bool m_bDebugCtrl;
	int m_iBallID;
	float m_fSpeed;
	XMFLOAT2 m_f2Dir;
	XMFLOAT2 m_f2ScrDim;
};


#endif // !Ball_H
