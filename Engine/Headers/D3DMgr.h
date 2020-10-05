////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.h
////////////////////////////////////////////////////////////////////////////////
#ifndef D3DMgr_H
#define D3DMgr_H

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Helpers.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DMgr
////////////////////////////////////////////////////////////////////////////////
class D3DMgr
{
public:	
	D3DMgr();
	~D3DMgr();

	bool Initialise(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
		float screenDepth, float screenNear);
	void Release();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();
	IDXGISwapChain* GetSwapChain();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	XMMATRIX* GetProjectionMatrix();
	XMMATRIX* GetWorldMatrix();
	XMMATRIX* GetOrthoMatrix();

	void GetVideoCardInfo(char* cardName, int& memory);

	bool OnResize(HWND* hwnd);
	void TurnZBufferOn();
	void TurnZBufferOff();
	void EnableWireframe();
	void DisableWireframe();
private:
	bool m_VSyncEnabled;
	int m_VideoCardMemory;
	char m_VideoCardDescription[128];
	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RasterizerState* m_pRasterState;
	ID3D11RasterizerState* m_pRasterStateWireframe;
	ID3D11DepthStencilState* m_pDepthDisabledStencilState;

	XMMATRIX m_ProjMatrix;
	XMMATRIX m_WorldMatrix;
	XMMATRIX m_OrthoMatrix;
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

#endif