#include "../Headers/Paddle.h"



Paddle::Paddle(Sprite* spr, BoundingCapsule* cap, XMFLOAT2 scrDim, int plyID, bool AiCont)
{
	m_iPlayerID = plyID;
	if (m_iPlayerID == 0)
		m_f2Position = XMFLOAT2(50.f, scrDim.y * 0.5f);
	else
		m_f2Position = XMFLOAT2(scrDim.x - 50.f, scrDim.y * 0.5f);
	
	m_fHeight = scrDim.y;
	m_bPlayerControl = AiCont == false ? true : false;

	m_pSprite = spr;
	m_pBoundingCapsule = cap;
	m_fSpeed = 7.f;
	m_iScore = 0;
}

bool Paddle::InitialiseSprite(D3DMgr* d3dMgr, Texture* texture)
{
	if (!m_pSprite) return false;

	//get texture description for m_fHeight and width 
	D3D11_TEXTURE2D_DESC texDesc;
	texture->GetTexture2D()->GetDesc(&texDesc);

	return m_pSprite->Initialise(d3dMgr, texture, m_f2Position, XMFLOAT2(texDesc.Width * .5f, texDesc.Height * .5f));
}

Paddle::~Paddle()
{
}



inline void Paddle::MoveUp(float vel)
{
	m_pSprite->Move(XMFLOAT2(0.f, -7.f));
	SetPosition(m_pSprite->GetPosition());
	if (m_f2Position.y < 0 + m_pSprite->GetSize().y * .5f)
	{
		m_pSprite->SetPosition(XMFLOAT2(m_f2Position.x, +m_pSprite->GetSize().y * .5f));
		SetPosition(m_pSprite->GetPosition());
	}
}

inline void Paddle::MoveDown(float vel)
{
	m_pSprite->Move(XMFLOAT2(0.f, +7.f));
	SetPosition(m_pSprite->GetPosition());
	if (m_f2Position.y > m_fHeight - m_pSprite->GetSize().y * .5f)
	{
		m_pSprite->SetPosition(XMFLOAT2(m_f2Position.x, m_fHeight - m_pSprite->GetSize().y * .5f));
		SetPosition(m_pSprite->GetPosition());
	}
}

inline void Paddle::Update(const float& dt)
{
	ProcessInput();

	XMFLOAT2 ori = m_pSprite->GetOrigin();

	m_fSpeed = m_fSpeed * dt;

	if (m_pBoundingCapsule)
	{
		//sizing isnt working correctly need to reimplement bounding box resize
		m_pBoundingCapsule->Update(XMFLOAT2(m_f2Position.x, m_f2Position.y), m_pSprite->GetSize());
	}
}

 void Paddle::Render(SpriteBatch* sprBatch)
{
	m_pSprite->NRender(sprBatch);
}

 float Paddle::GetHighestY()
 {
	 return m_pBoundingCapsule->GetLineSeg().GetBottom2D().y + (m_pBoundingCapsule->GetRadius() * .5f);
 }

 float Paddle::GetLowestY()
 {
	 return m_pBoundingCapsule->GetLineSeg().GetTop2D().y - (m_pBoundingCapsule->GetRadius() * .5f);
 }

inline void Paddle::ProcessInput()
{
	if (m_bPlayerControl)
	{
		if (m_iPlayerID == 0)
		{
			if (Input::GetInput()->GetKeyboard()->KeyIsPressed('W') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('S'))
				MoveUp(m_fSpeed);

			if (Input::GetInput()->GetKeyboard()->KeyIsPressed('S') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('W'))
				MoveDown(m_fSpeed);
		}
		else//player 1
		{
			if (Input::GetInput()->GetKeyboard()->KeyIsPressed('I') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('K'))
				MoveUp(m_fSpeed);

			if (Input::GetInput()->GetKeyboard()->KeyIsPressed('K') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('I'))
				MoveDown(m_fSpeed);
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

