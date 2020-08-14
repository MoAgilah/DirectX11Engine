////////////////////////////////////////////////////////////////////////////////
// Filename: Bullet.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Bullet_H
#define Bullet_H

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Static3DObj.h"
#include "../Headers/Mesh.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Bullet
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class Bullet : public Static3DObj
{
public:
	Bullet(Mesh* p_Bullet);
	~Bullet();
	void Activate(const XMMATRIX& gunMatrix, const XMVECTOR& Direction);
	void Deactivate();
	bool Update(const double& deltaTime);
	void Draw(Graphics* graphics);
	bool GetActive(void) const;
private:
	void UpdateMatrices(void);

	bool m_bActive;

	double m_dTime;

	XMVECTOR m_vForwardVector;
	XMVECTOR m_vGravity;

	Bullet* m_pNextBullet;
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