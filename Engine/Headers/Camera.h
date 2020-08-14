////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Camera_H
#define Camera_H

//////////////
// INCLUDES //
//////////////
#include <directxmath.h>
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:

	Camera(int screenWidth, int screenHeight, float screenDepth, float screenNear);
	~Camera();
	void CreateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane);
	void MoveForward(float units);
	void Strafe(float units);
	void MoveUp(float units);

	void Yaw(float radians);
	void Pitch(float radians);
	void Roll(float radians);
	void Update();

	void SetPosition(const XMFLOAT3& pPosition);
	void SetLookAt(const XMFLOAT3& pLookAt);
	void SetFOV(float fov);
	void SetAspectRatio(float aspect);
	void SetNearPlane(float plane);
	void SetFarPlane(float plane);
	void SetMaxVelocity(float maxVelocity);
	void SetInvertY(bool invert);
	void SetMaxPitch(float maxPitch);

	XMMATRIX* GetViewMatrix();
	XMMATRIX* GetProjectionMatrix();
	XMFLOAT4* GetPosition();
	XMFLOAT4* GetLookAt();
	float GetFOV();
	float GetAspectRatio();
	float GetNearPlane();
	float GetFarPlane();
	float GetMaxVelocity();
	bool  GetInvertY();
	float GetPitch();
	float GetYaw();
	float GetMaxPitch();

private:
	XMMATRIX  m_mViewMatrix;
	XMMATRIX  m_mProjMatrix;
	XMFLOAT4 m_f4Right;
	XMFLOAT4 m_f4Up;
	XMFLOAT4 m_f4Look;
	XMFLOAT4 m_f4Position;
	XMFLOAT4 m_f4LookAt;
	XMFLOAT4 m_f4Velocity;
	float       m_fYaw;
	float       m_fPitch;
	float       m_fMaxPitch;
	float       m_fMaxVelocity;
	float       m_fFov;
	float       m_fAspect;
	float       m_fNearPlane;
	float       m_fFarPlane;
	bool        m_bInvertY;
	bool        m_bEnableYMovement;
};

#endif // !Camera_H

