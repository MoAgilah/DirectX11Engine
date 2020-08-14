////////////////////////////////////////////////////////////////////////////////
// Filename: Light.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Light_H
#define Light_H


//////////////
// INCLUDES //
//////////////
#include <DirectXMath.h>
using namespace DirectX;

enum LightType
{
	Directional, Point, Spot
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Light
////////////////////////////////////////////////////////////////////////////////
class Light
{
public:
	Light();
	Light(const Light&);
	~Light();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetSpecularColor(float red, float green, float blue, float alpha);
	void SetPosition(float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularPower(float power);
	void SetSpotAngle(float spot);
	void SetConstantAttenuation(float);
	void SetLinearAttenuation(float);
	void SetQuadraticAttenuation(float);
	void SetLightType(LightType);
	void SetIsEnabled(bool);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetSpecularColor();
	XMFLOAT4 GetPosition();
	XMFLOAT4 GetDirection();
	float GetSpecularPower();
	float GetSpotAngle();
	float GetConstantAttenuation();
	float GetLinearAttenuation();
	float GetQuadraticAttenuation();
	int GetLightType();
	bool GetIsEnabled();
private:
	XMFLOAT4 m_f4AmbientColor;
	XMFLOAT4 m_f4DiffuseColor;
	XMFLOAT4 m_f4SpecularColor;
	XMFLOAT4 m_f4Position;
	XMFLOAT4 m_f4Direction;
	float m_fSpecularPower;
	float m_fSpotAngle;
	float m_fConstantAtt;
	float m_fLinearAtt;
	float m_fQuadAtt;
	int m_iLightType;
	bool m_bIsEnabled;
	
};

#endif