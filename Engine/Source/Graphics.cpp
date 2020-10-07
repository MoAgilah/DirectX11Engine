////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Graphics.h"


Graphics::Graphics()
	:m_pApplicationHandle(nullptr), m_pD3DMgr(new D3DMgr), m_pD2DMgr(new D2DMgr), 
	m_pCamera(new Camera(gs_SystPref.gf_ScreenWidth, gs_SystPref.gf_ScreenHeight, gs_SystPref.gf_ScreenDepth, gs_SystPref.gf_ScreenNear)), m_pTextMgr(nullptr), m_pTextureMgr(new TextureMgr), m_pShaderMgr(new ShaderMgr), m_pSpriteMgr(nullptr),
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

	assert(m_pD3DMgr.get() != nullptr);
	
	if (!fo_IfFailMsg(m_pD3DMgr->Initialise(screenWidth, screenHeight, gs_SystPref.g_bVsyncEnabled, *m_pApplicationHandle, gs_SystPref.gb_FullScreen, gs_SystPref.gf_ScreenDepth, gs_SystPref.gf_ScreenNear), "Could not initialize Direct3D"))
		return false;

	m_f2ScreenDimensions = XMFLOAT2((float)screenWidth, (float)screenHeight);
	
	assert(m_pCamera != nullptr);
	m_pCamera->SetPosition(XMFLOAT3(75.0f, 2.0f, 35.0f));

	assert(m_pD2DMgr.get() != nullptr);

	if (!fo_IfFailMsg(m_pD2DMgr->Initialise(m_pD3DMgr.get(), *m_pApplicationHandle),"Could not initialize Direct2D"))
		return false;

	m_pTextMgr.reset(new TextMgr(m_pD2DMgr.get()));
	
	assert(m_pTextMgr != nullptr);
	assert(m_pTextureMgr.get() != nullptr);
	assert(m_pShaderMgr.get() != nullptr);

	if (!fo_IfFailMsg(m_pShaderMgr->InitializeShaders(m_pD3DMgr->GetDevice(), *hwnd), "Could not initialize Shader Managers"))
		return false;

	m_pSpriteMgr.reset(new SpriteMgr(m_pD3DMgr.get()));
	assert(m_pSpriteMgr.get() != nullptr);

	return true;
}


void Graphics::Release()
{
	SAFE_RELEASE(m_pTextMgr);
}

D3DMgr* Graphics::GetD3DMgr()
{
	return m_pD3DMgr.get();
}

D2DMgr* Graphics::GetD2DMgr()
{
	return m_pD2DMgr.get();
}

Camera* Graphics::GetCamera()
{
	return m_pCamera.get();
}

TextMgr* Graphics::GetTextMgr()
{
	return m_pTextMgr.get();
}

TextureMgr* Graphics::GetTextureMgr()
{
	return m_pTextureMgr.get();
}

ShaderMgr* Graphics::GetShaderMgr()
{
	return m_pShaderMgr.get();
}

SpriteMgr* Graphics::GetSpriteMgr()
{
	return m_pSpriteMgr.get();
}

HWND* Graphics::GetApplicationHandle()
{
	return m_pApplicationHandle;
}

const XMFLOAT2& Graphics::GetScreenDimensions()
{
	return m_f2ScreenDimensions;
}
