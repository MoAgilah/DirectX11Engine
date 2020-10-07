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
#include <memory>
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

	XMFLOAT2 m_f2ScreenDimensions;
	HWND* m_pApplicationHandle;
	std::unique_ptr<D3DMgr> m_pD3DMgr;
	std::unique_ptr<D2DMgr> m_pD2DMgr;
	std::unique_ptr<TextureMgr> m_pTextureMgr;
	std::unique_ptr<ShaderMgr> m_pShaderMgr;
	std::unique_ptr<SpriteMgr> m_pSpriteMgr;
	std::unique_ptr<TextMgr> m_pTextMgr;
    std::unique_ptr<Camera> m_pCamera;	
};

#endif
