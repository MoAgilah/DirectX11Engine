////////////////////////////////////////////////////////////////////////////////
// Filename: LightShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LightShader_H_
#define _LightShader_H_

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
// Class name: LightShader
////////////////////////////////////////////////////////////////////////////////
class LightShader
{
private:

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
	LightShader();
	LightShader(const LightShader& other);
	~LightShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Release();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* lights[]);
private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCWSTR vsFilename, LPCWSTR psFilename);
	void OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, LPCWSTR shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* lights[]);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

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