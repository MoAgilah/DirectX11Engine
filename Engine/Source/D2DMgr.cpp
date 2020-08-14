////////////////////////////////////////////////////////////////////////////////
// Filename: D2DMgr.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/D2DMgr.h"

D2DMgr::D2DMgr()
{
	m_pYellowBrush = nullptr;
	m_pWhiteBrush = nullptr;
	m_pBlackBrush = nullptr;

	m_pTextFormatFPS = nullptr;
	m_pTextLayoutFPS = nullptr;

	m_pWriteFactory = nullptr;
	m_pFactory = nullptr;
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;

	m_D3Dptr = nullptr;
}

D2DMgr::D2DMgr(const D2DMgr& other)
{
}

D2DMgr::~D2DMgr()
{
}

bool D2DMgr::Initialise(D3DMgr* d3dptr, HWND hwnd)
{
	m_D3Dptr = d3dptr;

	if (!CreateDevice(hwnd))
	{
		return false;
	}

	
	if (!CreateBitmapRenderTarget(hwnd))
	{
		return false;
	}

	if (!InitializeTextFormats(hwnd))
	{
		return false;
	}

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

void D2DMgr::printFPS(HWND hwnd)
{
	if (true && m_pTextLayoutFPS)
	{
		m_pDeviceContext->BeginDraw();

		/*m_pDeviceContext->SetTransform(D2D1::Matrix3x2F::Translation(100, 100));

		m_pDeviceContext->DrawTextLayout(D2D1::Point2F(0.0f, 0.0f), m_pTextLayoutFPS, m_pYellowBrush);
		if (FAILED(m_pDeviceContext->EndDraw()))
			MessageBox(hwnd, "Critical error: Unable to draw FPS information!", "Error Msg", MB_OK);*/

		m_pDeviceContext->SetTransform(D2D1::Matrix3x2F::Translation(100, 50));
		
		m_pDeviceContext->DrawTextLayout(D2D1::Point2F(0.0f, 0.0f), m_pTextLayoutFPS, m_pBlackBrush);
		if (FAILED(m_pDeviceContext->EndDraw()))
			MessageBox(hwnd, "Critical error: Unable to draw FPS information!", "Error Msg", MB_OK);
	}
}

bool D2DMgr::CreateDevice(HWND hwnd)
{
	// create the DirectWrite factory
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown * *>(&m_pWriteFactory))))
	{
		MessageBox(hwnd, "Critical error: Unable to create the DirectWrite factory!", "Error Msg", MB_OK);
		return false;
	}

	// create the Direct2D factory
	D2D1_FACTORY_OPTIONS options;
#ifndef NDEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
	
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory2), &options, reinterpret_cast<void**>(&m_pFactory))))
	{
		MessageBox(hwnd, "Critical error: Unable to create Direct2D Factory!", "Error Msg", MB_OK);
		return false;
	}

	// get the dxgi device
	IDXGIDevice* dxgiDevice;

	if (FAILED(m_D3Dptr->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice))))
	{
		MessageBox(hwnd, "Critical error: Unable to get the DXGI device!", "Error Msg", MB_OK);
		return false;
	}
		
	// create the Direct2D device
	if (FAILED(m_pFactory->CreateDevice(dxgiDevice, &m_pDevice)))
	{
		MessageBox(hwnd, "Critical error: Unable to create the Direct2D device!", "Error Msg", MB_OK);
		return false;
	}
		
	// create its context
	if (FAILED(m_pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &m_pDeviceContext)))
	{
		MessageBox(hwnd, "Critical error: Unable to create the Direct2D device context!", "Error Msg", MB_OK);
		return false;
	}

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
	IDXGISurface* dxgiBuffer;
	
	if (FAILED(m_D3Dptr->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&dxgiBuffer))))
	{
		MessageBox(hwnd, "Critical error: Unable to retrieve the back buffer!", "Error Msg", MB_OK);
		return false;
	}

	// create the bitmap
	ID2D1Bitmap1* targetBitmap;
	if (FAILED(m_pDeviceContext->CreateBitmapFromDxgiSurface(dxgiBuffer, &bp, &targetBitmap)))
	{
		MessageBox(hwnd, "Critical error: Unable to create the Direct2D bitmap from the DXGI surface!", "Error Msg", MB_OK);
		return false;
	}

	// set the newly created bitmap as render target
	m_pDeviceContext->SetTarget(targetBitmap);

	SAFE_RELEASE(targetBitmap);

	return true;
}

bool D2DMgr::InitializeTextFormats(HWND hwnd)
{
	// create standard brushes
	if (FAILED(m_pDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_pYellowBrush)))
	{
		MessageBox(hwnd, "Critical error: Unable to create the yellow brush!", "Error Msg", MB_OK);
		return false;
	}
	if (FAILED(m_pDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush)))
	{
		MessageBox(hwnd, "Critical error: Unable to create the black brush!", "Error Msg", MB_OK);
		return false;
	}
	if (FAILED(m_pDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhiteBrush)))
	{
		MessageBox(hwnd, "Critical error: Unable to create the white brush!", "Error Msg", MB_OK);
		return false;
	}

	// set up text formats

	// FPS text
	if (FAILED(m_pWriteFactory->CreateTextFormat(L"Lucida Console", nullptr, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-GB", &m_pTextFormatFPS)))
	{
		MessageBox(hwnd, "Critical error: Unable to create text format for FPS information!", "Error Msg", MB_OK);
		return false;
	}
	if (FAILED(m_pTextFormatFPS->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)))
	{
		MessageBox(hwnd, "Critical error: Unable to set text alignment!", "Error Msg", MB_OK);
		return false;
	}
	if (FAILED(m_pTextFormatFPS->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)))
	{
		MessageBox(hwnd, "Critical error: Unable to set paragraph alignment!", "Error Msg", MB_OK);
		return false;
	}

	return true;
}
