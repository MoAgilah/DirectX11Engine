////////////////////////////////////////////////////////////////////////////////
// Filename: Sprite.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Sprite.h"

int Sprite::s_iSpriteCounter = -1;

Sprite::Sprite()
{
	m_iSpriteID = ++s_iSpriteCounter;
	m_pSpriteEffect = SpriteEffects::SpriteEffects_None;

	m_pTexture = nullptr;
	m_pColour = Colors::White;
	m_f2Size = XMFLOAT2{0.f,0.f};
	m_f2Scale = XMFLOAT2{ 0.f,0.f };
	m_f2ScreenPos = XMFLOAT2{ 0.f,0.f };
	m_f2Origin = XMFLOAT2{ 0.f,0.f };
	m_fAngle = 0.f;
	m_bPreMulAlpha = false;

	m_pAABB = nullptr;
	m_pBoundingSphere = nullptr;
	m_pBoundingCapsule = nullptr;
}

bool Sprite::Initialise(D3DMgr* d3dMgr, Texture* texture, XMFLOAT2 Pos, XMFLOAT2 Origin, XMFLOAT2 size, float angle, XMVECTORF32 colour, bool premulalpha)
{
	m_bPreMulAlpha = premulalpha;
	m_fAngle = angle;
	m_pColour = colour;
	m_f2Scale = size;

	m_pTexture = texture;
	
	m_pTexture->GetTexture2D()->GetDesc(&m_SpriteDesc);

	m_f2Size.x = float(m_SpriteDesc.Width) * m_f2Scale.x;
	m_f2Size.y = float(m_SpriteDesc.Height) * m_f2Scale.y;

	m_f2ScreenPos = Pos;

	m_f2Origin = XMFLOAT2(0,0);

	return true;
}

Sprite::Sprite(const Sprite& other)
{
	m_pSpriteEffect = other.m_pSpriteEffect;
	m_pTexture = other.m_pTexture;
	m_pColour = other.m_pColour;
	m_f2Size = other.m_f2Size;
	m_fAngle = other.m_fAngle;
	m_f2Scale = other.m_f2Scale;
	m_f2ScreenPos = other.m_f2ScreenPos;
	m_f2Origin = other.m_f2Origin;
	m_bPreMulAlpha = other.m_bPreMulAlpha;
	m_pAABB = other.m_pAABB;
	m_pBoundingSphere = other.m_pBoundingSphere;
	m_pBoundingCapsule = other.m_pBoundingCapsule;
}

Sprite::~Sprite()
{
}

void Sprite::Update(const float& deltaTime)
{
	if (m_pAABB)
	{
		m_pAABB->Update(XMFLOAT2(m_f2ScreenPos.x - m_f2Origin.x, m_f2ScreenPos.y - m_f2Origin.y), m_f2Size);
	}
	
	if (m_pBoundingSphere)
	{
		m_pBoundingSphere->Update(XMFLOAT2(m_f2ScreenPos.x - m_f2Origin.x, m_f2ScreenPos.y - m_f2Origin.y), XMFLOAT2(m_f2Size.x,m_f2Size.y));
	}

	if (m_pBoundingCapsule)
	{
		m_pBoundingCapsule->Update(XMFLOAT2(m_f2ScreenPos.x - m_f2Origin.x, m_f2ScreenPos.y - m_f2Origin.y), m_f2Size);
	}
}

void Sprite::Release()
{
	m_pTexture = nullptr;
}

void Sprite::NRender(SpriteBatch* sprBatch)
{
	sprBatch->Draw(m_pTexture->GetTextureShaderResourceView(), m_f2ScreenPos, nullptr, m_pColour, XMConvertToRadians(m_fAngle), m_f2Origin, m_f2Scale, m_pSpriteEffect);
}

void Sprite::NRender(SpriteBatch* sprBatch, RECT rect)
{
	sprBatch->Draw(m_pTexture->GetTextureShaderResourceView(), m_f2ScreenPos, &rect, m_pColour, XMConvertToRadians(m_fAngle), m_f2Origin, m_f2Scale, m_pSpriteEffect);
}

void Sprite::AssignAABB(AABB* aabb)
{
	m_pAABB = aabb;
	
	m_pAABB->Update(XMFLOAT2(m_f2ScreenPos.x - m_f2Origin.x, m_f2ScreenPos.y - m_f2Origin.y), m_f2Size);
}

AABB* Sprite::GetAABB()
{
	return m_pAABB;
}

void Sprite::AssignBoundingSphere(BoundingSphere* boundingSphere)
{
	m_pBoundingSphere = boundingSphere;
	m_pBoundingSphere->Update(XMFLOAT2(m_f2ScreenPos.x - m_f2Origin.x, m_f2ScreenPos.y - m_f2Origin.y), m_f2Size);
}

BoundingSphere* Sprite::GetBoundingSphere()
{
	return m_pBoundingSphere;
}

void Sprite::AssignBoundingCapsule(BoundingCapsule* boundingCapsule)
{
	m_pBoundingCapsule = boundingCapsule;
	m_pBoundingCapsule->Update(XMFLOAT2(m_f2ScreenPos.x - m_f2Origin.x, m_f2ScreenPos.y - m_f2Origin.y), m_f2Size);
}

BoundingCapsule* Sprite::GetBoundingCapsule()
{
	return m_pBoundingCapsule;
}

ID3D11ShaderResourceView* Sprite::GetTexture()
{
	return m_pTexture->GetTextureShaderResourceView();
}

void Sprite::SetSize(XMFLOAT2 newSize)
{
	m_f2Size = newSize;
}

XMFLOAT2 Sprite::GetSize()
{
	return m_f2Size;
}

void Sprite::SetAngle(float ang)
{
	m_fAngle = ang;
}

int Sprite::GetSpriteID()
{
	return m_iSpriteID;
}

XMFLOAT2 Sprite::GetPosition()
{
	return m_f2ScreenPos;
}

void Sprite::SetPosition(XMFLOAT2 pos)
{
	m_f2ScreenPos.x = pos.x;
	m_f2ScreenPos.y = pos.y;
}

void Sprite::SetOrigin(XMFLOAT2 origin)
{
	m_f2Origin = origin;
}

void Sprite::SetSpriteEffect(const SpriteEffects& sprEfx)
{
	m_pSpriteEffect = sprEfx;
}

void Sprite::SetColour(XMVECTORF32 colour)
{
	m_pColour = colour;
}

void Sprite::SetScale(XMFLOAT2 scale)
{
	m_f2Scale = scale;
}

XMFLOAT2 Sprite::GetScale()
{
	return m_f2Scale;
}

CD3D11_TEXTURE2D_DESC Sprite::GetSprDesc()
{
	return m_SpriteDesc;
}

void Sprite::Move(XMFLOAT2 add)
{
	m_f2ScreenPos.x += add.x;
	m_f2ScreenPos.y += add.y;
}


