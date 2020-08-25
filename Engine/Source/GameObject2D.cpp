#include "..\Headers\GameObject2D.h"

GameObject2D::GameObject2D()
{
	m_sName = "";
	m_f2Position = XMFLOAT2(0.f, 0.f);
	m_fAngle = 0.f;
}

GameObject2D::GameObject2D(const GameObject2D& other)
{
	m_sName = other.m_sName;
	m_f2Position = other.m_f2Position;
	m_fAngle = other.m_fAngle;
}

GameObject2D::~GameObject2D()
{
}

string GameObject2D::GetName()
{
	return m_sName;
}

void GameObject2D::SetPosition(const XMFLOAT2& pos)
{
	m_f2Position = pos;
}

void GameObject2D::SetPosition(float x, float y)
{
	m_f2Position = XMFLOAT2(x, y);
}

XMFLOAT2 GameObject2D::GetPosition()
{
	return m_f2Position;
}

void GameObject2D::SetRotation(float angle)
{
	m_fAngle = angle;
}
