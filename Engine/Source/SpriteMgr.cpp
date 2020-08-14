#include "../Headers/SpriteMgr.h"

SpriteMgr::SpriteMgr(D3DMgr* d3dMgr)
{
	m_pSpriteBatch = new SpriteBatch(d3dMgr->GetDeviceContext());
	m_pStates = new CommonStates(d3dMgr->GetDevice());
	m_bPreMulAlpha = false;
}

SpriteMgr::~SpriteMgr()
{
}

void SpriteMgr::Release()
{
	if (!m_vpSprites.empty())
	{
		for (size_t i = 0; i < m_vpSprites.size(); i++)
		{
			SAFE_DELETE(m_vpSprites[i]);
		}
	}
}

SpriteBatch* SpriteMgr::GetSprteBatch()
{
	return m_pSpriteBatch;
}

Sprite* SpriteMgr::GetSprite()
{
	m_vpSprites.push_back(new Sprite);
	return m_vpSprites.back();
}

Sprite* SpriteMgr::GetSprite(int id)
{
	return m_vpSprites[id];
}

void SpriteMgr::Begin()
{
	if (m_bPreMulAlpha)
		m_pSpriteBatch->Begin();
	else
		m_pSpriteBatch->Begin(SpriteSortMode_Deferred, m_pStates->NonPremultiplied());
}

void SpriteMgr::End()
{
	m_pSpriteBatch->End();
}
