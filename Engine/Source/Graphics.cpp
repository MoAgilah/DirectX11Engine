////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Graphics.h"


Graphics::Graphics()
	:m_pApplicationHandle(nullptr), m_pD3DMgr(new D3DMgr), m_pD2DMgr(new D2DMgr), 
	m_pCamera(nullptr), m_pTextMgr(nullptr), m_pTextureMgr(new TextureMgr), m_pShaderMgr(new ShaderMgr), m_pSpriteMgr(nullptr),
	m_f2ScreenDimensions(XMFLOAT2(0.f,0.f))
{}

Graphics::~Graphics()
{
	Release();
}

bool Graphics::Initialise(const int& screenWidth, const int& screenHeight, HWND* hwnd)
{
	m_pApplicationHandle = hwnd;
	bool result;

	assert(m_pD3DMgr != nullptr);
	
	if (!fo_IfFailMsg(m_pD3DMgr->Initialise(screenWidth, screenHeight, g_bVSYNC_ENABLED, *m_pApplicationHandle, g_bFULL_SCREEN, g_fSCREEN_DEPTH, g_fSCREEN_NEAR), "Could not initialize Direct3D"))
		return false;

	m_f2ScreenDimensions = XMFLOAT2((float)screenWidth, (float)screenHeight);

	m_pCamera = new Camera(screenWidth, screenHeight, g_fSCREEN_DEPTH, g_fSCREEN_NEAR);
	
	assert(m_pCamera != nullptr);
	m_pCamera->SetPosition(XMFLOAT3(75.0f, 2.0f, 35.0f));

	assert(m_pD2DMgr != nullptr);

	if (!fo_IfFailMsg(m_pD2DMgr->Initialise(m_pD3DMgr, *m_pApplicationHandle),"Could not initialize Direct2D"))
		return false;

	m_pTextMgr = new TextMgr(m_pD2DMgr);
	if (!m_pTextMgr)
	{
		MessageBox(*m_pApplicationHandle, "Could not initialize Text Mgr", "Error", MB_OK);
		return false;
	}

	assert(m_pTextureMgr != nullptr);
	assert(m_pShaderMgr != nullptr);

	if (!fo_IfFailMsg(m_pShaderMgr->InitializeShaders(m_pD3DMgr->GetDevice(), *hwnd), "Could not initialize Shader Managers"))
		return false;

	m_pSpriteMgr = new SpriteMgr(m_pD3DMgr);
	if (!m_pSpriteMgr)
	{
		MessageBox(*m_pApplicationHandle, "Could not initialize Sprite Manager", "Error", MB_OK);
		return false;
	}

	return true;
}


void Graphics::Release()
{
	SAFE_RELEASE(m_pSpriteMgr);
	SAFE_RELEASE(m_pTextMgr);
	SAFE_RELEASE(m_pShaderMgr);
	SAFE_RELEASE(m_pTextureMgr);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pD2DMgr);
	SAFE_RELEASE(m_pD3DMgr);
}

D3DMgr* Graphics::GetD3DMgr()
{
	return m_pD3DMgr;
}

D2DMgr* Graphics::GetD2DMgr()
{
	return m_pD2DMgr;
}

Camera* Graphics::GetCamera()
{
	return m_pCamera;
}

TextMgr* Graphics::GetTextMgr()
{
	return m_pTextMgr;
}

TextureMgr* Graphics::GetTextureMgr()
{
	return m_pTextureMgr;
}

ShaderMgr* Graphics::GetShaderMgr()
{
	return m_pShaderMgr;
}

SpriteMgr* Graphics::GetSpriteMgr()
{
	return m_pSpriteMgr;
}

HWND* Graphics::GetApplicationHandle()
{
	return m_pApplicationHandle;
}

const XMFLOAT2& Graphics::GetScreenDimensions()
{
	return m_f2ScreenDimensions;
}
