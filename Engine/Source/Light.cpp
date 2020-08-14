////////////////////////////////////////////////////////////////////////////////
// Filename: Light.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Light.h"

Light::Light()
{
}


Light::Light(const Light& other)
{
}


Light::~Light()
{
}

void Light::SetAmbientColor(float r, float g, float b, float a)
{
	m_f4AmbientColor = XMFLOAT4(r, g, b, a);
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_f4DiffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_f4SpecularColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetPosition(float x, float y, float z)
{
	m_f4Position = XMFLOAT4(x, y, z, 1.0f);
}

void Light::SetDirection(float x, float y, float z)
{
	m_f4Direction = XMFLOAT4(x, y, z, 1.f);
}

void Light::SetSpecularPower(float power)
{
	m_fSpecularPower = power;
}

void Light::SetSpotAngle(float spot)
{
	m_fSpotAngle = spot;
}

void Light::SetConstantAttenuation(float constAtt)
{
	m_fConstantAtt = constAtt;
}

void Light::SetLinearAttenuation(float linearAtt)
{
	m_fLinearAtt = linearAtt;
}

void Light::SetQuadraticAttenuation(float quadAtt)
{
	m_fQuadAtt = quadAtt;
}

void Light::SetLightType(LightType type)
{
	m_iLightType = type;
}

void Light::SetIsEnabled(bool enabled)
{
	m_bIsEnabled = enabled;
}


XMFLOAT4 Light::GetAmbientColor()
{
	return m_f4AmbientColor;
}

XMFLOAT4 Light::GetDiffuseColor()
{
	return m_f4DiffuseColor;
}

XMFLOAT4 Light::GetSpecularColor()
{
	return m_f4SpecularColor;
}

float Light::GetSpecularPower()
{
	return m_fSpecularPower;
}

float Light::GetSpotAngle()
{
	return m_fSpotAngle;
}

float Light::GetConstantAttenuation()
{
	return m_fConstantAtt;
}

float Light::GetLinearAttenuation()
{
	return m_fLinearAtt;
}

float Light::GetQuadraticAttenuation()
{
	return m_fQuadAtt;
}

XMFLOAT4 Light::GetPosition()
{
	return m_f4Position;
}

XMFLOAT4 Light::GetDirection()
{
	return m_f4Direction;
}

int Light::GetLightType()
{
	return m_iLightType;
}

bool Light::GetIsEnabled()
{
	return m_bIsEnabled;
}
