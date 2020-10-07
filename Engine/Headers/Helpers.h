#ifndef Helper_H
#define Helper_H

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <string>
#include <vector>

struct SystemPreferences
{
	std::string gs_AppName = "DirectX11";
	bool gb_FullScreen = false;
	bool gb_Bordered = true;
	bool g_bVsyncEnabled = true;
	int gf_ScreenWidth = 800.f;
	int gf_ScreenHeight = 600.f;
	float gf_ScreenDepth = 1000.f;
	float gf_ScreenNear = 0.1f;
};
static SystemPreferences gs_SystPref;

struct IfFailMsg
{
	bool operator()(bool res, std::string errMsg);
	bool operator()(HRESULT res, std::string errMsg);
};
static IfFailMsg fo_IfFailMsg;

#endif
