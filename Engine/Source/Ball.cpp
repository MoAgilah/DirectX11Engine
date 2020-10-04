#include "../Headers/Ball.h"

Ball::Ball(Sprite* spr, BoundingSphere* sprbnd, XMFLOAT2 scrDim, int ballID, bool debugCtrl)
{
	m_iBallID = ballID;
	m_f2Position = XMFLOAT2(scrDim.x * 0.5f, scrDim.y * 0.5f);
	m_f2Vel = XMFLOAT2(0, 0);
	m_f2ScrDim = scrDim;
	m_pBoundingSphere = sprbnd;
	m_fSpeed = 160.f;
	m_pSprite = spr;
	m_pBoundingSphere = sprbnd;
	m_bDebugCtrl = debugCtrl;
}

bool Ball::InitialiseSprite(D3DMgr* d3dMgr, Texture* texture)
{
	if (!m_pSprite) return false;

	//get texture description for m_fHeight and width 
	D3D11_TEXTURE2D_DESC texDesc;
	texture->GetTexture2D()->GetDesc(&texDesc);

	return m_pSprite->Initialise(d3dMgr, texture, m_f2Position, XMFLOAT2(texDesc.Width * .5f, texDesc.Height * .5f));
}

Ball::~Ball()
{
}

void Ball::Update(const float& dt)
{
	m_f2Vel.x = m_fSpeed  * dt;
	//m_f2Vel.y += 150 * dt;

	m_pSprite->Move(m_f2Vel);
	m_f2Position = m_pSprite->GetPosition();

	if (m_pBoundingSphere)
	{
		XMFLOAT2 ori = m_pSprite->GetOrigin();
		m_pBoundingSphere->Update(XMFLOAT2(m_f2Position.x - ori.x, m_f2Position.y - ori.y), m_pSprite->GetSize());
	}

	//if the heights of the court are hit invert vertical travelling direction
	if ((m_f2Position.y < GetRadius() * 0.5f) || (m_f2Position.y > m_f2ScrDim.y - GetRadius() * 0.5f))
		m_f2Vel.y *= -1.f;

	//if the ball goes out of the court 
	if (m_f2Position.x < -((GetRadius() * 0.5f)+2) || m_f2Position.x > m_f2ScrDim.x -((GetRadius() * 0.5f) + 2))
	{
		m_f2Position = XMFLOAT2(m_f2ScrDim.x * 0.5f, m_f2ScrDim.y * 0.5f);
		m_pSprite->SetPosition(m_f2Position);
		m_f2Vel.x *= -1.f;
		m_f2Vel.y = 0;
		//AwardPoint(m_f2Position.x);
	}

	ProcessInput();
	
}

void Ball::Render(SpriteBatch* sprBatch)
{
	m_pSprite->NRender(sprBatch);
}

void Ball::Move()
{
	m_pSprite->Move(m_f2Vel);
	m_f2Position = m_pSprite->GetPosition();
}

void Ball::ResolveCol(XMFLOAT2 cp)
{
	XMFLOAT2 dist, pos, u;
	float length, radiiSum;

	XMStoreFloat2(&dist, XMVectorSubtract(XMLoadFloat2(&m_f2Position), XMLoadFloat2(&cp)));
	length = sqrt(dist.x * dist.x + dist.y * dist.y);
	u.x = dist.x / length;
	u.y = dist.y / length;
	radiiSum = GetRadius() + GetRadius();

	pos.x = cp.x + (radiiSum + 1) * u.x;
	pos.y = cp.y + (radiiSum + 1) * u.y;	
	
	m_pSprite->SetPosition(pos);
	SetPosition(m_pSprite->GetPosition());
}

void Ball::RespondToCol(Paddle* pad)
{
	//within the center's margin
	if (m_f2Position.y >= pad->GetBoundingCap()->GetCenter2D().y - 8.f && m_f2Position.y <= pad->GetBoundingCap()->GetCenter2D().y + 8.f)
	{
		m_fSpeed *= -1.f;
	}
	else //outside the center's margin
	{
		//in the bottom's margin 
		if (m_f2Position.y >= pad->GetLowestY() && m_f2Position.y <= pad->GetBoundingCap()->GetCenter2D().y - 8.f)
		{
			//create abit of unpredictability by using distance of the ball from the pad's center
			float poc = pad->GetLowestY() / m_f2Position.y;

			//if going left
			if (pad->GetPadID() == 0)
			{
				//if behind pad
				if (m_f2Position.x < pad->GetPosition().x) m_f2Vel.y = m_f2Vel.x;
				else //in front of pad
				{
					m_f2Vel.x *= -1.f;
					m_f2Vel.y = -m_f2Vel.x * poc; // 
				}
			}
			else //if going right
			{
				//if behind pad
				if (m_f2Position.x > pad->GetPosition().x) m_f2Vel.y = m_f2Vel.x;
				else //in front of pad
				{
					m_f2Vel.x *= -1.f;
					m_f2Vel.y = m_f2Vel.x * poc; //
				}
			}
		}

		if (m_f2Position.y >= pad->GetBoundingCap()->GetCenter2D().y + 8.f && m_f2Position.y <= pad->GetHighestY())
		{
			float poc =   m_f2Position.y / pad->GetHighestY();
			//if going left
			if (pad->GetPadID() == 0)
			{
				//if behind pad
				if (m_f2Position.x < pad->GetPosition().x) m_f2Vel.y = m_f2Vel.x;
				else //in front of pad
				{
					m_f2Vel.x *= -1.f;
					m_f2Vel.y = m_f2Vel.x * poc; //
				}
			}
			else //if going right
			{
				//if behind pad
				if (m_f2Position.x > pad->GetPosition().x) m_f2Vel.y = m_f2Vel.x;
				else //in front of pad
				{
					m_f2Vel.x *= -1.f;
					m_f2Vel.y = -m_f2Vel.x * poc; //
				}
			}
		}
	}
}




void Ball::ProcessInput()
{
	if (m_bDebugCtrl)
	{
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('I') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('K')) {
			m_pSprite->Move(XMFLOAT2(0, -m_fSpeed));
			m_f2Vel.y = -m_fSpeed;
		}

		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('J') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('L')){
			m_pSprite->Move(XMFLOAT2(-m_fSpeed,0));
			m_f2Vel.x = -m_fSpeed;
		}

		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('L') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('J')){
			m_pSprite->Move(XMFLOAT2(m_fSpeed, 0));
			m_f2Vel.x = m_fSpeed;
		}

		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('K') && !Input::GetInput()->GetKeyboard()->KeyIsPressed('I')){
			m_pSprite->Move(XMFLOAT2(0, m_fSpeed));
			m_f2Vel.y = m_fSpeed;
		}

		m_f2Position = m_pSprite->GetPosition();
	}
}