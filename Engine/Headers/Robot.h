////////////////////////////////////////////////////////////////////////////////
// Filename: Robot.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Robot_H
#define Robot_H

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Hierarchal3DObj.h"
#include "../Headers/AnimationController.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Robot
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class Robot : public Hiearchal3DObj
{
public:
	Robot(ID3D11Device* device, Texture* texture, const XMFLOAT3& pos, const XMFLOAT3& rot, const double& scalar, bool assimp, const char* fileExt = "");
	~Robot();
	void LoadAnimations(void);
	void Update(bool bPlayerContol,const double& deltatime);
	void Draw(Graphics* graphics, Light* lights[]);
	XMFLOAT4 GetFocusPosition(void){ return GetPosition(); }
	XMFLOAT4 GetCameraPosition(void) { XMFLOAT4 v4Pos; XMStoreFloat4(&v4Pos, m_vCamWorldPos); return v4Pos; }
	//XMFLOAT4 GetPosition(void) { return m_f4Position; }
	void SetRobCamera(bool value) { m_bRobCam = value; }
	string GetCurrentAnimName() { return m_sCurrentAnim; }
	void BeAggressive(MeshComponent* plane);
private:
	void UpdateMatrices(void);
	AnimationController* aControl;

	static bool s_bResourcesReady;

	XMVECTOR m_vForwardVector;
	float m_fSpeed;
	double scaleF;

	XMFLOAT4 m_v4CamRot;							// Local rotation angles
	XMFLOAT4 m_v4CamOff;							// Local offset

	XMVECTOR m_vCamWorldPos;						// World position
	XMMATRIX m_mCamWorldMatrix;						// Camera's world transformation matrix

	bool m_bRobCam;
	bool stopAnim;
	double test;
	string m_sCurrentAnim;
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