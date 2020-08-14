#include "../Headers/Bulletpool.h"

BulletPool::BulletPool(ID3D11Device* device, Texture* texture, char* MeshFilename, float size, bool assimp)
{
	//load mesh once
	m_pBullet = new Mesh(device, texture, MeshFilename, size);

	//make entire pool when created
	for (unsigned int i = 0; i < g_MaxBullet; ++i)
	{
		m_vBullets.push_back(new Bullet(m_pBullet));
	}
}

BulletPool::~BulletPool()
{
	//delete all bullets
	for (unsigned int i = 0; i < m_vBullets.size(); ++i)
	{
		delete m_vBullets[i];
	}
	delete m_pBullet;
}

void BulletPool::Create(const XMMATRIX& gunMatrix, const XMVECTOR& Direction)
{
	for (unsigned int i = 0; i < g_MaxBullet; i++)
	{
		//find if inactive
		if (!m_vBullets[i]->GetActive())
		{
			//if so activate and break out of loop
			m_vBullets[i]->Activate(gunMatrix, Direction);
			break;
		}
	}
}

void BulletPool::Update(const double& deltaTime)
{
	for (unsigned int i = 0; i < g_MaxBullet; ++i)
	{
		if (m_vBullets[i]->GetActive())
		{
			//update only the active one if return false for lifeline deactivate
			if (m_vBullets[i]->Update(deltaTime))
			{
				m_vBullets[i]->Deactivate();
			}
		}
	}
}

void BulletPool::Draw(Graphics* graphics)
{
	XMMATRIX viewMatrix, projectionMatrix;
	viewMatrix = *graphics->GetCamera()->GetViewMatrix();
	projectionMatrix = *graphics->GetCamera()->GetProjectionMatrix();

	for (unsigned int i = 0; i < g_MaxBullet; ++i)
	{
		if (m_vBullets[i]->GetActive())
		{
			//draw only active ones
			m_vBullets[i]->Draw(graphics);
			
			if (!graphics->GetShaderMgr()->RenderTextureShader(graphics->GetD3DMgr()->GetDeviceContext(), m_vBullets[i]->GetMesh()->GetIndexCount(), m_vBullets[i]->GetWorldMatrix(), viewMatrix, projectionMatrix, m_vBullets[i]->GetMesh()->GetTexture()))
			{
				MessageBox(*graphics->GetApplicationHandle(), "Failed to set shader parameters - failed to render LightShader", "Error", MB_OK);
			}
		}
	}
}