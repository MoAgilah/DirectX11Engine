////////////////////////////////////////////////////////////////////////////////
// Filename: ShaderMgr.h
////////////////////////////////////////////////////////////////////////////////
#ifndef ShaderMgr_H
#define ShaderMgr_H

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/ColourShader.h"
#include "../Headers/TextureShader.h"
#include "../Headers/LightShader.h"
#include "..//Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderMgr
////////////////////////////////////////////////////////////////////////////////
class ShaderMgr
{
public:
	ShaderMgr();
	~ShaderMgr();
	bool InitializeShaders(ID3D11Device* device, HWND hwnd);

	bool RenderColourShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix);
	bool RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	bool RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* lights[]);
	void Release();
private:
	ColourShader* m_pColourShader;
	TextureShader* m_pTextureShader;
	LightShader* m_pLightShader;
	
};



#endif // !ShaderMgr_H
