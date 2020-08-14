////////////////////////////////////////////////////////////////////////////////
// Filename: AnimatedSprite.h
////////////////////////////////////////////////////////////////////////////////
#ifndef AnimatedSpriteH		
#define AnimatedSpriteH

//////////////
// INCLUDES //
//////////////
#include <string>
#include <iostream>
#include <vector>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "..//Headers/Sprite.h"
#include "../Headers/TextureMgr.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: AnimatedSprite
////////////////////////////////////////////////////////////////////////////////
class AnimatedSprite
{
public:
	AnimatedSprite(Sprite* spr);
	AnimatedSprite(const AnimatedSprite& other);
	bool Initialise(D3DMgr* d3dMgr, Texture* texture, XMFLOAT2 Pos = XMFLOAT2(0, 0), XMFLOAT2 Origin = XMFLOAT2(0, 0), XMFLOAT2 Size = XMFLOAT2(1, 1), float angle = 0.f, XMVECTORF32 colour = Colors::White, bool premulalpha = false);
	void SetupAnimatedSprite(int rows, int columns, float framesPerSec, int id, bool symmetrical, int initialAnim, float animationSpeed);
	~AnimatedSprite();
	void Release();

	void Update(float dt);
	void NRender(SpriteBatch* sprBatch);

	void ChangeAnim(int animNum);

	void SetFrames(std::vector<int> numFrames);

	Sprite* GetSprite();
	XMFLOAT2 GetFrameSize();
	bool PlayedNumTimes(int val);
	bool playedOnce();
private:
	bool m_bSymmetrical;
	bool m_bFlipDir;

	int m_iAnimEnd;
	int m_iColumns, m_iRows;
	int m_iCurrentFrame, m_iMaxFrame;
	int m_iCurrentAnim, m_iMaxAnim;
	std::vector<int> m_viNumFrames;

	float m_fFramesPerSecond, m_fAnimSpeed;
	float m_fCurrentTime, m_fFrameTime;

	RECT m_rFrameRect;
	Sprite* m_pSprite;
	Texture* m_pTexture;
	XMFLOAT2 m_f2FrameSize;
};

#endif