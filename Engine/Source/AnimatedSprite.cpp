////////////////////////////////////////////////////////////////////////////////
// Filename: AnimatedSprite.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(Sprite* spr)
{
	m_fFrameTime = 0;
	m_iMaxAnim = 0;
	m_pSprite = spr;
	m_pTexture = nullptr;
	m_iCurrentFrame = 0;
	m_iCurrentAnim = 0;
	m_iAnimEnd = 0;
	m_fAnimSpeed = 0;
	m_fCurrentTime = 0;
	m_fFramesPerSecond = 0.0f;
	m_fAnimSpeed = 0.0f;
	m_bFlipDir = false;
	m_bSymmetrical = true;
	m_iColumns = 0;
	m_iRows = 0;

	//set single frame size
	m_f2FrameSize = XMFLOAT2(0, 0);

	//set a max frame if symmetrical
	m_iMaxFrame = 0;

	m_rFrameRect.left =
		m_rFrameRect.top =
		m_rFrameRect.right =
		m_rFrameRect.bottom = 0;
}

AnimatedSprite::AnimatedSprite(const AnimatedSprite& other)
{
	m_fFrameTime = other.m_fFrameTime;
	m_iMaxAnim = other.m_iMaxAnim;
	m_pSprite = other.m_pSprite;
	m_pTexture = other.m_pTexture;
	m_iAnimEnd = other.m_iAnimEnd;
	m_iCurrentFrame = other.m_iCurrentFrame;
	m_iCurrentAnim = other.m_iCurrentAnim;
	m_fAnimSpeed = other.m_fAnimSpeed;
	m_fCurrentTime = other.m_fCurrentTime;
	m_fFramesPerSecond = other.m_fFramesPerSecond;
	m_bSymmetrical = other.m_bSymmetrical;
	m_iColumns = other.m_iColumns;
	m_iRows = other.m_iRows;

	//set single frame size
	m_f2FrameSize = other.m_f2FrameSize;

	//set a max frame if symmetrical
	m_iMaxFrame = other.m_iMaxFrame;

	m_rFrameRect = other.m_rFrameRect;
}

bool AnimatedSprite::Initialise(D3DMgr* d3dMgr, Texture* texture, XMFLOAT2 Pos, XMFLOAT2 Origin , XMFLOAT2 Size , float angle , XMVECTORF32 colour , bool premulalpha)
{
	bool result = false;

	//m_pSprite = new Sprite;
	if (!m_pSprite)
	{
		return false;
	}

	result = m_pSprite->Initialise(d3dMgr, texture, Pos, Origin, Size, angle,colour,premulalpha);
	if (!result)
	{
		return false;
	}

	return true;
}

void AnimatedSprite::SetupAnimatedSprite(int rows, int columns, float framesPerSec, int id, bool symmetrical, int initialAnim, float animationSpeed)
{
	m_iCurrentFrame = 0;
	m_iCurrentAnim = initialAnim;
	m_iAnimEnd = 0;
	m_fCurrentTime = 0;
	m_fFramesPerSecond = framesPerSec / 1000.0f;
	m_fAnimSpeed = animationSpeed;
	m_bSymmetrical = symmetrical;

	float width((float)(m_pSprite->GetSprDesc().Width)), height((float)(m_pSprite->GetSprDesc().Height));

	m_iColumns = columns;
	m_iRows = rows;

	//set single frame size
	m_f2FrameSize = XMFLOAT2(width / m_iColumns, height / m_iRows);

	//set a max frame if symmetrical
	m_iMaxFrame = (int)(width / m_f2FrameSize.x);

	m_rFrameRect.left = (LONG)(m_f2FrameSize.x * m_iCurrentFrame);
	m_rFrameRect.top = (LONG)(m_iCurrentAnim * m_f2FrameSize.y);
	m_rFrameRect.right = (LONG)(m_rFrameRect.left + m_f2FrameSize.x);
	m_rFrameRect.bottom = (LONG)(m_rFrameRect.top + m_f2FrameSize.y);

	
	m_pSprite->SetSize(XMFLOAT2(m_f2FrameSize.x * m_pSprite->GetScale().x, m_f2FrameSize.y * m_pSprite->GetScale().y));
	XMFLOAT2 size = m_pSprite->GetSize();
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::Release()
{
	SAFE_RELEASE(m_pSprite);
	m_pTexture = nullptr;
}

void AnimatedSprite::Update(float dt)
{
	m_fCurrentTime += m_fAnimSpeed * dt;

	if (m_fCurrentTime >= m_fFramesPerSecond)
	{
		m_fCurrentTime = 0.0f;

		++m_iCurrentFrame;

		if (m_bSymmetrical)
		{
			if (m_iCurrentFrame >= m_iMaxFrame)
			{
				m_iCurrentFrame = 0;
			}
		}
		else
		{
			if (m_iCurrentFrame >= m_viNumFrames[m_iCurrentAnim])
			{
				m_iCurrentFrame = 0;
				++m_iAnimEnd;
			}
		}

		//set new frame
		m_rFrameRect.left = (LONG)(m_f2FrameSize.x * m_iCurrentFrame);
		m_rFrameRect.top = (LONG)(m_iCurrentAnim * m_f2FrameSize.y);
		m_rFrameRect.right = (LONG)(m_rFrameRect.left + m_f2FrameSize.x);
		m_rFrameRect.bottom = (LONG)(m_rFrameRect.top + m_f2FrameSize.y);

		//if going left flip animation
		if (m_bFlipDir == false)
		{
			// flip X
			m_pSprite->SetSpriteEffect(SpriteEffects::SpriteEffects_FlipHorizontally);
		}
		else
		{
			m_pSprite->SetSpriteEffect(SpriteEffects::SpriteEffects_None);
		}

	}

	m_pSprite->Update(dt);
}

void AnimatedSprite::NRender(SpriteBatch* sprBatch)
{
	m_pSprite->NRender(sprBatch, m_rFrameRect);
}

void  AnimatedSprite::ChangeAnim(int animNum)
{
	m_iAnimEnd = 0;
	m_iCurrentAnim = animNum;
}

void AnimatedSprite::SetFrames(std::vector<int> numFrames)
{
	m_viNumFrames = numFrames;
}

Sprite* AnimatedSprite::GetSprite()
{
	return m_pSprite;
}

XMFLOAT2 AnimatedSprite::GetFrameSize()
{
	return m_f2FrameSize;
}

bool AnimatedSprite::PlayedNumTimes(int val)
{
	return m_iAnimEnd == val;
}

bool AnimatedSprite::playedOnce()
{
	if (m_iAnimEnd > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


