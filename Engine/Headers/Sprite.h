////////////////////////////////////////////////////////////////////////////////
// Filename: Sprite.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Sprite_H
#define Sprite_H

//////////////
// INCLUDES //
//////////////
#include "../DirectXTemplate/SpriteBatch.h"
#include "../DirectXTemplate/CommonStates.h"
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/D3DMgr.h"
#include "../Headers/Texture.h"
#include "../Headers/AABB.h"
#include "../Headers/BoundingSphere.h"
#include "../Headers/BoundingCapsule.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Sprite
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class Sprite
{
public:
	Sprite();
	Sprite(const Sprite& other);
	~Sprite();
	
	bool Initialise(D3DMgr* d3dMgr, Texture* texture, XMFLOAT2 Pos = XMFLOAT2(0,0), XMFLOAT2 Origin = XMFLOAT2(0, 0), XMFLOAT2 Size = XMFLOAT2(1, 1),
		float angle = 0.f, XMVECTORF32 colour = Colors::White, bool premulalpha = false);
	void Release();

	void Update(const float& deltaTime);
	void NRender(SpriteBatch* sprBatch);
	void NRender(SpriteBatch* sprBatch, RECT rect);

	ID3D11ShaderResourceView* GetTexture();
	void SetSpriteEffect(const SpriteEffects& sprEfx);
	CD3D11_TEXTURE2D_DESC GetSprDesc();

	int GetSpriteID();
	XMFLOAT2 GetPosition();
	void SetPosition(XMFLOAT2 pos);
	void Move(XMFLOAT2 add);

	void SetSize(XMFLOAT2 newSize);
	XMFLOAT2 GetSize();

	void SetAngle(float ang);
	
	void SetScale(XMFLOAT2 scale);
	XMFLOAT2 GetScale();

	void SetOrigin(XMFLOAT2 origin);
	void SetColour(XMVECTORF32 colour);

	//debugging code 
	void AssignAABB(AABB* AABB);
	AABB* GetAABB();
	void AssignBoundingSphere(BoundingSphere* AABB);
	BoundingSphere* GetBoundingSphere();
	void AssignBoundingCapsule(BoundingCapsule* AABB);
	BoundingCapsule* GetBoundingCapsule();
private:
	static int s_iSpriteCounter;
	int m_iSpriteID;
	Texture* m_pTexture;
	CD3D11_TEXTURE2D_DESC m_SpriteDesc;
	SpriteEffects m_pSpriteEffect;
	XMVECTORF32 m_pColour;
	RECT m_rFrameRect;
	XMFLOAT2 m_f2Size;
	XMFLOAT2 m_f2ScreenPos;
	XMFLOAT2 m_f2Origin;
	XMFLOAT2 m_f2Scale;
	float m_fAngle;
	bool m_bPreMulAlpha;

	//debugging code 
	BoundingCapsule* m_pBoundingCapsule;
	BoundingSphere* m_pBoundingSphere;
	AABB* m_pAABB;
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

#endif