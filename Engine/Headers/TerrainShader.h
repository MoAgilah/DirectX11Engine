////////////////////////////////////////////////////////////////////////////////
// Filename: TerrainShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TerrainShader_H_
#define _TerrainShader_H_

#include "Light.h"
#include "Utils.h"




//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainShader
////////////////////////////////////////////////////////////////////////////////
class TerrainShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct LightProperties
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT4 specularColor;
		XMFLOAT4 direction;
		float specularPower;
		float spotAngle;
		float constantAtt;
		float linearAtt;
		float quadAtt;
		int lightType;
		bool IsEnabled;
		float padding;
	};

	struct LightColorBufferType
	{
		LightProperties lights[g_iNUM_LIGHTS];
	};

	struct LightPositionBufferType
	{
		XMFLOAT4 lightPosition[g_iNUM_LIGHTS];
	};

public:
	TerrainShader();
	TerrainShader(const TerrainShader&);
	~TerrainShader();

	bool Initialize(ID3D11Device*, HWND);
	void Release();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, Light* []);
private:
	bool InitializeShader(ID3D11Device*, HWND, LPCWSTR, LPCWSTR);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, LPCWSTR);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, Light* []);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11SamplerState* m_pSampleState;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pLightColorBuffer;
	ID3D11Buffer* m_pLightPositionBuffer;
	ID3D11Buffer* m_pCameraBuffer;
};

#endif