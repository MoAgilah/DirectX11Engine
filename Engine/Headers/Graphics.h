////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Graphics_H
#define Graphics_H

//////////////////////
// Library Includes //
/////////////////////
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

//////////////
// INCLUDES //
//////////////
#include <string>
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
//////////////////////
#include "../Headers/D3DMgr.h"
#include "../Headers/D2DMgr.h"
#include "../Headers/Camera.h"
#include "../Headers/TextMgr.h"
#include "../Headers/TextureMgr.h"
#include "../Headers/SpriteMgr.h"
#include "../Headers/ShaderMgr.h"

#include "../Headers/Helpers.h"
#include "../Headers/Utils.h"

/////////////
// GLOBALS //
/////////////
const bool g_bFULL_SCREEN = false;
const bool g_bBORDERED = true;
const bool g_bVSYNC_ENABLED = true;
const float g_fSCREEN_DEPTH = 1000.0f;
const float g_fSCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: Graphics
////////////////////////////////////////////////////////////////////////////////
class Graphics
{
public:
	Graphics();
	~Graphics();
	bool Initialise(const int& screenWidth, const int& screenHeight, HWND* hwnd);
	void Release();
	D3DMgr* GetD3DMgr();
	D2DMgr* GetD2DMgr();
	Camera* GetCamera();
	TextMgr* GetTextMgr();
	TextureMgr* GetTextureMgr();
	ShaderMgr* GetShaderMgr();
	SpriteMgr* GetSpriteMgr();
	HWND* GetApplicationHandle();
	const XMFLOAT2& GetScreenDimensions();
private:
	HWND* m_pApplicationHandle;
	D3DMgr* m_pD3DMgr;
	D2DMgr* m_pD2DMgr;
	TextureMgr* m_pTextureMgr;
	ShaderMgr* m_pShaderMgr;
	SpriteMgr* m_pSpriteMgr;
	TextMgr* m_pTextMgr;
	Camera* m_pCamera;
	XMFLOAT2 m_f2ScreenDimensions;
	
};

#endif
