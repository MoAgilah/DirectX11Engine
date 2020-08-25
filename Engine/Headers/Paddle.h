#ifndef Paddle_H
#define Paddle_H


#include "../Headers/Input.h"
#include "../Headers/GameObject2D.h"
#include "../Headers/Sprite.h"
#include "../Headers/BoundingCapsule.h"

class Paddle : public GameObject2D
{
public:
	Paddle(Sprite* spr, BoundingCapsule* cap, XMFLOAT2 scrDim, bool AiCont = false);
	bool InitialiseSprite(D3DMgr* d3dMgr, Texture* texture);
	~Paddle();
	void MoveUp();
	void MoveDown();
	void Update(const float& dt);
	void Render(SpriteBatch* sprBatch);
private:
	void ProcessInput();
	void ProcessAIInput();
	Sprite* m_pSprite;
	BoundingCapsule* m_pBoundingCapsule;
	static int s_iPlayerNumber;
	bool m_bPlayerControl;
	float height;
};

int Paddle::s_iPlayerNumber = 0;

Paddle::Paddle(Sprite* spr, BoundingCapsule* cap, XMFLOAT2 scrDim, bool AiCont)
{
	s_iPlayerNumber++;
	if (s_iPlayerNumber == 1)
	{
		m_f2Position = XMFLOAT2(50.f, scrDim.y * 0.5f);
	}
	else
	{
		m_f2Position = XMFLOAT2(scrDim.x - 50.f, scrDim.y * 0.5f);
	}
	height = scrDim.y;
	m_bPlayerControl = AiCont == false ? true : false;

	m_pSprite = spr;
	m_pBoundingCapsule = cap;
}

inline bool Paddle::InitialiseSprite(D3DMgr* d3dMgr, Texture* texture)
{
	if (!m_pSprite) return false;

	//get texture description for height and width 
	D3D11_TEXTURE2D_DESC texDesc;
	texture->GetTexture2D()->GetDesc(&texDesc);

	return m_pSprite->Initialise(d3dMgr, texture, m_f2Position, XMFLOAT2(texDesc.Width * .5f, texDesc.Height * .5f));
}

Paddle::~Paddle()
{
}

inline void Paddle::MoveUp()
{
	m_pSprite->Move(XMFLOAT2(0.f, -1.f));
	SetPosition(m_pSprite->GetPosition());
	if (m_f2Position.y < 0)
	{
		m_pSprite->SetPosition(XMFLOAT2(m_f2Position.x, 0));
		SetPosition(m_pSprite->GetPosition());
	}
}

inline void Paddle::MoveDown()
{
	m_pSprite->Move(XMFLOAT2(0.f, +1.f));
	SetPosition(m_pSprite->GetPosition());
	if (m_f2Position.y > height - m_pSprite->GetSize().y)
	{
		m_pSprite->SetPosition(XMFLOAT2(m_f2Position.x, height - m_pSprite->GetSize().y));
		SetPosition(m_pSprite->GetPosition());
	}
}

inline void Paddle::Update(const float& dt)
{
	XMFLOAT2 ori = m_pSprite->GetOrigin();

	if (m_pBoundingCapsule)
	{
		//sizing isnt working correctly need to reimplement bounding box resize
		m_pBoundingCapsule->Update(XMFLOAT2(m_f2Position.x , m_f2Position.y ), m_pSprite->GetSize());
	}

	ProcessInput();
}

inline void Paddle::Render(SpriteBatch* sprBatch)
{
	m_pSprite->NRender(sprBatch);
}

inline void Paddle::ProcessInput()
{
	if (m_bPlayerControl)
	{
		if (s_iPlayerNumber == 2)
		{
			if (Input::GetInput()->GetKeyboard()->KeyIsPressed('I') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('K'))
			{
				MoveUp();
			}
			

			if (Input::GetInput()->GetKeyboard()->KeyIsPressed('K') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('I'))
			{
				MoveDown();
			}
		}
		else//player 1
		{
			if (Input::GetInput()->GetKeyboard()->KeyIsPressed('W') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('S'))
			{
				MoveUp();
			}


			if (Input::GetInput()->GetKeyboard()->KeyIsPressed('S') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('W'))
			{
				MoveDown();
			}
		}
	}
	else //AI Player
	{
		ProcessAIInput();
	}
}

inline void Paddle::ProcessAIInput()
{
}

#endif // !Paddle_H
