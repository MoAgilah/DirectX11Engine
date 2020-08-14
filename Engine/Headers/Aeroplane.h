////////////////////////////////////////////////////////////////////////////////
// Filename: Aeroplane.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Aeroplane_H
#define Aeroplane_H

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Hierarchal3DObj.h"
#include "../Headers/BulletPool.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Aeroplane
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class Aeroplane : public Hiearchal3DObj
{
public:
	Aeroplane(ID3D11Device* device, Texture* texture, const XMFLOAT3& pos , const XMFLOAT3& rot , float size, bool assimp, const char* fileExt ="");
	~Aeroplane(void);
	void Update(const double& deltaTime);
	void UpdateMatrices(void);
	void Render(Graphics* graphics, Light* []);

	XMFLOAT4 GetFocusPosition(void) { return GetPosition(); }
	XMFLOAT4 GetCameraPosition(void) { XMFLOAT4 v4Pos; XMStoreFloat4(&v4Pos, m_vCamWorldPos); return v4Pos; }
	void SetGunCamera(bool value) { m_bGunCam = value; }
	void SetPlayerControl(bool control) { m_bPlayerControl = control; }
private:	
	static bool s_bResourcesReady;
	bool m_bJustFired;
	bool m_bStunt;

	XMVECTOR m_vForwardVector;
	float m_fSpeed;

	XMFLOAT4 m_f4CamRot;							// Local rotation angles
	XMFLOAT4 m_f4CamOff;							// Local offset

	XMVECTOR m_vCamWorldPos;						// World position
	XMMATRIX m_mCamWorldMatrix;						// Camera's world transformation matrix

	bool m_bGunCam;
	bool m_bPlayerControl;
	BulletPool* m_pBulletPool;
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

#endif // !Aeroplane_H
