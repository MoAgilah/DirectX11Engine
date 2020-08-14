#include "../Headers/GameObject3D.h"

GameObject3D::GameObject3D()
{
	m_sName = "";
	m_f4Position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_f4Rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_mWorldMatrix = XMMatrixIdentity();
}

GameObject3D::GameObject3D(const GameObject3D& other)
{
	m_f4Position = other.m_f4Position;
	m_f4Rotation = other.m_f4Rotation;
	m_mWorldMatrix = other.m_mWorldMatrix;
}

GameObject3D::~GameObject3D()
{
}

void GameObject3D::SetPosition( XMFLOAT4& pos)
{
	m_f4Position = pos;
}

void GameObject3D::SetPosition( float x,  float y,  float z)
{
	m_f4Position = XMFLOAT4(x, y, z, 1.0f);
}

XMFLOAT4 GameObject3D::GetPosition()
{
	return m_f4Position;
}

void GameObject3D::SetRotation( XMFLOAT4& rot)
{
	m_f4Rotation = rot;
}

void GameObject3D::SetRotation( float x,  float y,  float z)
{
	m_f4Rotation = XMFLOAT4(x, y, z, 1.0f);
}

XMFLOAT4 GameObject3D::GetRotation()
{
	return m_f4Rotation;
}

void GameObject3D::SetWorldMatrix( XMMATRIX& world)
{
	m_mWorldMatrix = world;
}

XMMATRIX GameObject3D::GetWorldMatrix()
{
	return m_mWorldMatrix;
}

string GameObject3D::GetName()
{
	return m_sName;
}
