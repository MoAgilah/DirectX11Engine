////////////////////////////////////////////////////////////////////////////////
// Filename: D2DMgr.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/D2DMgr.h"

D2DMgr::D2DMgr()
	:m_pYellowBrush(nullptr), m_pWhiteBrush(nullptr), m_pBlackBrush(nullptr),
	m_pTextFormatFPS(nullptr), m_pTextLayoutFPS(nullptr), m_pTextTypography(nullptr),
	m_pWriteFactory(nullptr), m_pFactory(nullptr), m_pDevice(nullptr), m_pDeviceContext(nullptr),
	m_D3Dptr (nullptr)
{}

D2DMgr::~D2DMgr()
{
	Release();
}

bool D2DMgr::Initialise(D3DMgr* d3dptr, HWND hwnd)
{
	m_D3Dptr = d3dptr;

	if (!fo_IfFailMsg(CreateDevice(hwnd),"Failed to create D2D Device"))
		return false;

	if (!fo_IfFailMsg(CreateBitmapRenderTarget(hwnd), "Failed to create D2D bitmap render target"))
		return false;

	if (!fo_IfFailMsg(InitializeTextFormats(hwnd), "Failed to initialise text formats"))
		return false;

	return true;
}

void D2DMgr::Release()
{
	SAFE_RELEASE(m_pYellowBrush);
	SAFE_RELEASE(m_pWhiteBrush);
	SAFE_RELEASE(m_pBlackBrush);
	
	SAFE_RELEASE(m_pTextFormatFPS);
	SAFE_RELEASE(m_pTextLayoutFPS);
	
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);

	SAFE_RELEASE(m_pFactory);
	SAFE_RELEASE(m_pWriteFactory);
	m_D3Dptr = nullptr;
}

IDWriteFactory2* D2DMgr::GetWriteFactory()
{
	return m_pWriteFactory;
}

ID2D1DeviceContext1* D2DMgr::GetDeviceContext()
{
	return m_pDeviceContext;
}

bool D2DMgr::CreateDevice(HWND hwnd)
{
	if (!fo_IfFailMsg(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pWriteFactory)), "Failed to create the DirectWrite factory!"))
		return false;

	// create the Direct2D factory
	D2D1_FACTORY_OPTIONS options;
#ifndef NDEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
	
	if (!fo_IfFailMsg(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory2), &options, reinterpret_cast<void**>(&m_pFactory)), "Failed to create Direct2D Factory!"))
		return false;

	// get the dxgi device
	IDXGIDevice* dxgiDevice;

	if (!fo_IfFailMsg(m_D3Dptr->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)), "Failed to get the DXGI device!"))
		return false;
		
	// create the Direct2D device
	if (!fo_IfFailMsg(m_pFactory->CreateDevice(dxgiDevice, &m_pDevice), "Failed to create the Direct2D device!"))
		return false;
		
	// create its context
	if (!fo_IfFailMsg(m_pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &m_pDeviceContext), "Critical error: Unable to create the Direct2D device context!"))
		return false;

	return true;
}

bool D2DMgr::CreateBitmapRenderTarget(HWND hwnd)
{
	// specify the desired bitmap properties
	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96.0f;
	bp.dpiY = 96.0f;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	// Direct2D needs the DXGI version of the back buffer
	IDXGISurface* dxgiBuffer = nullptr;
	
	if (!fo_IfFailMsg(m_D3Dptr->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&dxgiBuffer)),"Failed to retrieve the back buffer!"))
		return false;

	// create the bitmap
	ID2D1Bitmap1* targetBitmap;
	if (!fo_IfFailMsg(m_pDeviceContext->CreateBitmapFromDxgiSurface(dxgiBuffer, &bp, &targetBitmap), "Critical error: Unable to create the Direct2D bitmap from the DXGI surface!"))
		return false;

	// set the newly created bitmap as render target
	m_pDeviceContext->SetTarget(targetBitmap);

	SAFE_RELEASE(targetBitmap);

	return true;
}

bool D2DMgr::InitializeTextFormats(HWND hwnd)
{
	// create standard brushes
	if (!fo_IfFailMsg(m_pDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_pYellowBrush), "Failed to create the yellow brush!"))
		return false;
	
	if (!fo_IfFailMsg(m_pDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush), "Failed to create the black brush!"))
		return false;
	
	if (!fo_IfFailMsg(m_pDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhiteBrush), "Failed to create the white brush!"))
		return false;

	// set up text formats
	if (!fo_IfFailMsg(m_pWriteFactory->CreateTextFormat(L"Lucida Console", nullptr, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-GB", &m_pTextFormatFPS), "Failed to create text format for FPS information!"))
		return false;
	
	if (!fo_IfFailMsg(m_pTextFormatFPS->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING), "Critical error: Unable to set text alignment!"))
		return false;
	
	if (!fo_IfFailMsg(m_pTextFormatFPS->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR), "Critical error: Unable to set paragraph alignment!"))
		return false;

	return true;
}
