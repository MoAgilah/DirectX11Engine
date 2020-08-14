////////////////////////////////////////////////////////////////////////////////
// Filename: TextureShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef TextureShader_H
#define TextureShader_H

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShader
////////////////////////////////////////////////////////////////////////////////
class TextureShader
{
public:
	TextureShader();
	TextureShader(const TextureShader& other);
	~TextureShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Release();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCWSTR vsFilename, LPCWSTR psFilename);
	void OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, LPCWSTR shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11SamplerState* m_pSampleState;
};

#endif