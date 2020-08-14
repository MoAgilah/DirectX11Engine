#ifndef SpriteMgr_H
#define SpriteMgr_H

//////////////
// INCLUDES //
//////////////
#include "../Headers/D3DMgr.h"
#include "../Headers/Sprite.h"

#include "../DirectXTemplate/SpriteBatch.h"
#include "../DirectXTemplate/CommonStates.h"




class SpriteMgr
{
public:
	SpriteMgr(D3DMgr* d3dmgr);
	~SpriteMgr();
	void Release();

	SpriteBatch* GetSprteBatch();
	Sprite* GetSprite();
	Sprite* GetSprite(int id);
	
	void Begin();
	void End();

private:
	vector<Sprite*> m_vpSprites;
	SpriteBatch* m_pSpriteBatch;
	CommonStates* m_pStates;
	bool m_bPreMulAlpha;
};


#endif