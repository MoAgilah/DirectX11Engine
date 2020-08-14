#include "../Headers/ShaderMgr.h"

ShaderMgr::ShaderMgr()
{
}

ShaderMgr::~ShaderMgr()
{
}

bool ShaderMgr::InitializeShaders(ID3D11Device* device, HWND hwnd)
{
	bool result;

	m_pColourShader = new ColourShader;
	if (!m_pColourShader)
	{
		return false;
	}

	result = m_pColourShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the colour shader object.", "Error", MB_OK);
		return false;
	}

	m_pTextureShader = new TextureShader;
	if (!m_pTextureShader)
	{
		return false;
	}

	result = m_pTextureShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}

	m_pLightShader = new LightShader;
	if (!m_pLightShader)
	{
		return false;
	}

	result = m_pLightShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the light shader object.", "Error", MB_OK);
		return false;
	}

	m_pTerrainShader = new TerrainShader;
	if (!m_pTerrainShader)
	{
		return false;
	}

	result = m_pTerrainShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the terrain shader object.", "Error", MB_OK);
		return false;
	}

	return true;
}

bool ShaderMgr::RenderColourShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!m_pColourShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}

	return true;
}

bool ShaderMgr::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	if (!m_pTextureShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture))
	{
		return false;
	}

	return true;
}

bool ShaderMgr::RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* lights[])
{
	if (!m_pLightShader->Render(deviceContext,indexCount,worldMatrix,viewMatrix,projectionMatrix,texture,cameraPosition,lights))
	{
		return false;
	}

	return true;
}

bool ShaderMgr::RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* lights[])
{
	if (!m_pTerrainShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, cameraPosition, lights))
	{
		return false;
	}

	return true;
}

void ShaderMgr::Release()
{
	SAFE_RELEASE(m_pColourShader);
	SAFE_RELEASE(m_pTextureShader);
	SAFE_RELEASE(m_pLightShader);
	SAFE_RELEASE(m_pTerrainShader);
}