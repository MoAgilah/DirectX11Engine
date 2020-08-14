#include "../Headers/Camera.h"

Camera::Camera(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	m_fFov = XM_PI / 4.0f;//45 degrees
	m_fAspect = (float)screenWidth / (float)screenHeight;
	m_fNearPlane = screenNear;
	m_fFarPlane = screenDepth;


	m_fMaxPitch = XMConvertToRadians(89.0f);
	m_fMaxVelocity = 1.0f;
	m_bInvertY = false;
	m_bEnableYMovement = true;
	m_f4Position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_f4Velocity = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_f4Look = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	CreateProjectionMatrix(m_fFov, m_fAspect, m_fNearPlane, m_fFarPlane);

	Update();
}

Camera::~Camera()
{
}

void Camera::CreateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane)
{
	m_fFov = fov;
	m_fAspect = aspect;
	m_fNearPlane = nearPlane;
	m_fFarPlane = farPlane;

	m_mProjMatrix = XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNearPlane, m_fFarPlane);
}

void Camera::MoveForward(float units)
{
	XMVECTOR velocity = XMLoadFloat4(&m_f4Velocity);

	if (m_bEnableYMovement)
	{
		velocity += XMLoadFloat4(&m_f4Look) * units;
		XMStoreFloat4(&m_f4Velocity, velocity);
	}
	else
	{

		XMVECTOR moveVector = XMVectorSet(m_f4Look.x, 0.0f, m_f4Look.z, m_f4Look.w);
		moveVector = XMVector4Normalize(moveVector);
		moveVector *= units;
		velocity += moveVector;
		XMStoreFloat4(&m_f4Velocity, velocity);
	}
}

void Camera::Strafe(float units)
{
	XMVECTOR velocity = XMLoadFloat4(&m_f4Velocity);

	velocity += XMLoadFloat4(&m_f4Right) * units;
	XMStoreFloat4(&m_f4Velocity, velocity);
}

void Camera::MoveUp(float units)
{
	if (m_bEnableYMovement)
	{
		m_f4Velocity.y += units;
	}
}

void Camera::Yaw(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}

	XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat4(&m_f4Up), radians);
	XMVECTOR right, look;

	right = XMVector3TransformNormal(XMLoadFloat4(&m_f4Right), rotation);
	XMStoreFloat4(&m_f4Right, right);

	look = XMVector3TransformNormal(XMLoadFloat4(&m_f4Look), rotation);
	XMStoreFloat4(&m_f4Look, look);
}

void Camera::Pitch(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}

	radians = (m_bInvertY) ? -radians : radians;
	m_fPitch -= radians;

	if (m_fPitch > m_fMaxPitch)
	{
		radians += m_fPitch - m_fMaxPitch;
	}
	else if (m_fPitch < -m_fMaxPitch)
	{
		radians += m_fPitch + m_fMaxPitch;
	}

	XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat4(&m_f4Right), radians);
	XMVECTOR up, look;

	up = XMVector3TransformNormal(XMLoadFloat4(&m_f4Up), rotation);
	XMStoreFloat4(&m_f4Up, up);

	look = XMVector3TransformNormal(XMLoadFloat4(&m_f4Look), rotation);
	XMStoreFloat4(&m_f4Look, look);
}

void Camera::Roll(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}

	XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat4(&m_f4Look), radians);
	XMVECTOR right, up;

	right = XMVector3TransformNormal(XMLoadFloat4(&m_f4Right), rotation);
	XMStoreFloat4(&m_f4Right, right);

	up = XMVector3TransformNormal(XMLoadFloat4(&m_f4Up), rotation);
	XMStoreFloat4(&m_f4Up, up);
}

void Camera::Update()
{

	XMVECTOR vel = XMLoadFloat4(&m_f4Velocity);
	XMFLOAT4 vf4;
	XMStoreFloat4(&vf4, XMVector4LengthEst(vel));
	
	// Cap velocity to max velocity
	if (vf4.x > m_fMaxVelocity)
	{
		XMStoreFloat4(&m_f4Velocity,
			XMVector4Normalize(XMLoadFloat4(&m_f4Velocity)) * m_fMaxVelocity);
	}

	// Move the camera
	XMVECTOR newPos = XMLoadFloat4(&m_f4Position);
	newPos += XMLoadFloat4(&m_f4Velocity);
	XMStoreFloat4(&m_f4Position, newPos);

	// Could decelerate here. I'll just stop completely.
	m_f4Velocity = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR lookAt = XMVectorAdd(newPos, XMLoadFloat4(&m_f4Look));

	// Calculate the new view matrix
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	m_mViewMatrix = XMMatrixLookAtLH(newPos, lookAt, Up);

	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, m_mViewMatrix);

	// Set the camera axes from the view matrix
	m_f4Right.x = view._11;
	m_f4Right.y = view._21;
	m_f4Right.z = view._31;
	m_f4Up.x = view._12;
	m_f4Up.y = view._22;
	m_f4Up.z = view._32;
	m_f4Look.x = view._13;
	m_f4Look.y = view._23;
	m_f4Look.z = view._33;

	// Calculate yaw and pitch
	float lookLengthOnXZ = sqrtf(m_f4Look.z * m_f4Look.z + m_f4Look.x * m_f4Look.x);
	m_fPitch = atan2f(m_f4Look.y, lookLengthOnXZ);
	m_fYaw = atan2f(m_f4Look.x, m_f4Look.z);
}

void Camera::SetPosition(const XMFLOAT3& pPos)
{
	m_f4Position = XMFLOAT4(pPos.x, pPos.y, pPos.z, 1.0f);
}

void Camera::SetLookAt(const XMFLOAT3& pLookAt)
{
	m_f4LookAt = XMFLOAT4(pLookAt.x, pLookAt.y, pLookAt.z, 1.0f);
	XMVECTOR look = XMVector4Normalize(XMVectorSubtract(XMLoadFloat4(&m_f4LookAt), XMLoadFloat4(&m_f4Position)));
	XMStoreFloat4(&m_f4Look, look);
}


void Camera::SetFOV(float fov)
{
	CreateProjectionMatrix(fov, m_fAspect, m_fNearPlane, m_fFarPlane);
}

void Camera::SetAspectRatio(float aspect)
{
	CreateProjectionMatrix(m_fFov, aspect, m_fNearPlane, m_fFarPlane);
}

void Camera::SetNearPlane(float plane)
{
	CreateProjectionMatrix(m_fFov, m_fAspect, plane, m_fFarPlane);
}

void Camera::SetFarPlane(float plane)
{
	CreateProjectionMatrix(m_fFov, m_fAspect, m_fNearPlane, plane);
}

void Camera::SetMaxVelocity(float maxVelocity)
{
	m_fMaxVelocity = maxVelocity;
}

void Camera::SetInvertY(bool invert)
{
	m_bInvertY = invert;
}

void Camera::SetMaxPitch(float maxPitch)
{
	m_fMaxPitch = maxPitch;
}

XMMATRIX* Camera::GetViewMatrix()
{
	return &m_mViewMatrix;
}

XMMATRIX* Camera::GetProjectionMatrix()
{
	return &m_mProjMatrix;
}

XMFLOAT4* Camera::GetPosition()
{
	return &m_f4Position;
}

XMFLOAT4* Camera::GetLookAt()
{
	return &m_f4LookAt;
}

float Camera::GetFOV()
{
	return m_fFov;
}

float Camera::GetAspectRatio()
{
	return m_fAspect;
}

float Camera::GetNearPlane()
{
	return m_fNearPlane;
}

float Camera::GetFarPlane()
{
	return m_fFarPlane;
}

float Camera::GetMaxVelocity()
{
	return m_fMaxVelocity;
}

bool  Camera::GetInvertY()
{
	return m_bInvertY;
}

float Camera::GetPitch()
{
	return m_fPitch;
}

float Camera::GetYaw()
{
	return m_fYaw;
}

float Camera::GetMaxPitch()
{
	return m_fMaxPitch;
}

