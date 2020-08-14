#ifndef BulletPool_H
#define BulletPool_H

/////////////////
// MY INCLUDES //
/////////////////
#include <vector>
#include <assert.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Mesh.h"
#include "../Headers/Bullet.h"

/////////////
// GLOBALS //
/////////////
#define g_MaxBullet 100 

////////////////////////////////////////////////////////////////////////////////
// Class name: BulletPool
////////////////////////////////////////////////////////////////////////////////
class BulletPool
{
public:
	BulletPool(ID3D11Device* device, Texture* texture, char* MeshFilename, float size, bool assimp);
	~BulletPool();
	void Create(const XMMATRIX& gunMatrix, const XMVECTOR& Direction);
	void Update(const double& deltaTime);
	void Draw(Graphics* graphics);
private:
	std::vector<Bullet*> m_vBullets;
	Mesh* m_pBullet;
};

#endif