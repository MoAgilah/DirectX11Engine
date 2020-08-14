////////////////////////////////////////////////////////////////////////////////
// Filename: D2DMgr.h
////////////////////////////////////////////////////////////////////////////////
#ifndef D2DMgr_H
#define D2DMgr_H

/////////////
// LINKING //
/////////////
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

//////////////
// INCLUDES //
//////////////
#include <d2d1_2.h>
#include <dwrite_2.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/D3DMgr.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: D2DMgr
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class D2DMgr
{
public:
	D2DMgr();
	D2DMgr(const D2DMgr& other);
	~D2DMgr();
	bool Initialise(D3DMgr* d3dptr, HWND hwnd);
	void Release();
	IDWriteFactory2* GetWriteFactory();
	ID2D1DeviceContext1* GetDeviceContext();

	void printFPS(HWND hwnd);

	// brushes
	ID2D1SolidColorBrush* m_pYellowBrush;
	ID2D1SolidColorBrush* m_pWhiteBrush;
	ID2D1SolidColorBrush* m_pBlackBrush;

	// text formats
	IDWriteTextFormat* m_pTextFormatFPS;

	// text layouts
	IDWriteTextLayout* m_pTextLayoutFPS;

	IDWriteTypography* m_pTextTypography;
private:
	IDWriteFactory2* m_pWriteFactory;	// pointer to the DirectWrite factory
	ID2D1Factory2* m_pFactory;			// pointer to the Direct2D factory
	ID2D1Device1* m_pDevice;				// pointer to the Direct2D device
	ID2D1DeviceContext1* m_pDeviceContext;		// pointer to the device context
	
	D3DMgr* m_D3Dptr;

	bool CreateDevice(HWND hwnd);					// creates the device and its context
	bool CreateBitmapRenderTarget(HWND hwnd);		// creates the bitmap render target, set to be the same as the backbuffer already in use for Direct3D
	bool InitializeTextFormats(HWND hwnd);			// initializes the different formats, for now, only a format to print FPS information will be created

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

