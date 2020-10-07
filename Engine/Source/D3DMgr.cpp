////////////////////////////////////////////////////////////////////////////////
// Filename: D3DMgr.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/D3DMgr.h"


D3DMgr::D3DMgr()
	:m_VSyncEnabled(false), m_VideoCardMemory(0), m_VideoCardDescription(""),m_pSwapChain(nullptr), 
	m_pDevice(nullptr), m_pDeviceContext(nullptr), m_pRenderTargetView(nullptr), m_pDepthStencilBuffer(nullptr), 
	m_pDepthStencilState(nullptr), m_pDepthStencilView(nullptr), m_pRasterState(nullptr), m_pRasterStateWireframe(nullptr), 
	m_pDepthDisabledStencilState(nullptr), m_ProjMatrix(XMMatrixIdentity()), m_OrthoMatrix(XMMatrixIdentity()), m_WorldMatrix(XMMatrixIdentity())
{}


D3DMgr::~D3DMgr()
{
	Release();
}

bool D3DMgr::Initialise(const int& screenWidth, const int& screenHeight, const bool& vsync, const HWND& hwnd, const bool& fullscreen,
	const float& screenDepth, const float& screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes = 0;
	unsigned int numerator = 0; unsigned int denominator = 0;
	DXGI_MODE_DESC* displayModeList;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

	// Store the vsync setting.
	m_VSyncEnabled = vsync;
	
	if (!fo_IfFailMsg(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), "Failed to create DXGIFactory"))
		return false;

	// Use the factory to create an adapter for the primary graphics interface (video card).
	if (!fo_IfFailMsg(factory->EnumAdapters(0, &adapter), "Failed to create adapter"))
		return false;

	// Enumerate the primary adapter output (monitor).
	if (!fo_IfFailMsg(adapter->EnumOutputs(0, &adapterOutput), "Failed to enumerate the adapter output"))
		return false;

	// Get the number of modes that fit the DXGI_FORMAT_B8G8R8A8_UNORM display format for the adapter output (monitor).
	if (!fo_IfFailMsg(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL), "Failed to get display mode list"))
		return false;

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	// Now fill the display mode list structures.
	if (!fo_IfFailMsg(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList), "Failed to fill display mode list struct"))
		return false;

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (UINT)screenWidth)
		{
			if (displayModeList[i].Height == (UINT)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	SAFE_DELETEA(displayModeList);
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);
	
	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set up the swap chain description
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = fullscreen ? false : true;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Set the refresh rate of the back buffer.
	if (m_VSyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	// Create the swap chain, Direct3D device, and Direct3D device context.
	if (!fo_IfFailMsg(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext), "Failed to create device and swap chain"))
		return false;

	// Get the pointer to the back buffer.
	if (!fo_IfFailMsg(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& backBufferPtr), "Failed to get the pointer to the back buffer"))
		return false;

	// Create the render target view with the back buffer pointer.
	if (!fo_IfFailMsg(m_pDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_pRenderTargetView), "Failed to create the render target view"))
		return false;

	SAFE_RELEASE(backBufferPtr);

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	if (!fo_IfFailMsg(m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer), "Failed to create the texture for the depth buffer"))
		return false;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	if (!fo_IfFailMsg(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState), "Failed to create the depth stencil state"))
		return false;

	// Set the depth stencil state.
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	if (!fo_IfFailMsg(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView), "Failed to create the depth stencil view"))
		return false;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	if (!fo_IfFailMsg(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState), "Failed to create the rasterizer state"))
		return false;

	// Now set the rasterizer state.
	m_pDeviceContext->RSSetState(m_pRasterState);

	// Setup a raster description which enables wire frame rendering.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the wire frame rasterizer state.
	if (!fo_IfFailMsg(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterStateWireframe), "Failed to create the wireframe rasterizer state"))
		return false;

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_pDeviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;//45 degrees
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_ProjMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_WorldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_OrthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	if(!fo_IfFailMsg(m_pDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pDepthDisabledStencilState),"Failed to create the depth disabled stencil state"))
		return false;

	return true;
}

void D3DMgr::Release()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(false, NULL);
	
	SAFE_RELEASE(m_pDepthDisabledStencilState);
	SAFE_RELEASE(m_pRasterState);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pDepthStencilBuffer);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

void D3DMgr::BeginScene(const float& red, const float& green, const float& blue, const float& alpha)
{
	float color[4]{ red,green,blue,alpha };

	// Clear the back buffer.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// Clear the depth buffer.
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DMgr::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_VSyncEnabled)
		// Lock to screen refresh rate.
		m_pSwapChain->Present(1, 0);
	else
		// Present as fast as possible.
		m_pSwapChain->Present(0, 0);
}

ID3D11Device* D3DMgr::GetDevice()
{
	return m_pDevice;
}

IDXGISwapChain* D3DMgr::GetSwapChain()
{ 
	return m_pSwapChain.Get(); 
}

ID3D11DeviceContext* D3DMgr::GetDeviceContext()
{
	return m_pDeviceContext;
}

XMMATRIX* D3DMgr::GetProjectionMatrix()
{
	return &m_ProjMatrix;
}

XMMATRIX* D3DMgr::GetWorldMatrix()
{
	return &m_WorldMatrix;
}

XMMATRIX* D3DMgr::GetOrthoMatrix()
{
	return &m_OrthoMatrix;
}

void D3DMgr::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_VideoCardDescription);
	memory = m_VideoCardMemory;
}

void D3DMgr::TurnZBufferOn()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}

void D3DMgr::TurnZBufferOff()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);
}

void D3DMgr::EnableWireframe()
{
	m_pDeviceContext->RSSetState(m_pRasterStateWireframe);
}


void D3DMgr::DisableWireframe()
{
	m_pDeviceContext->RSSetState(m_pRasterState);
}

bool D3DMgr::OnResize(HWND* appHwnd)
{
	m_pDeviceContext->ClearState();
	m_pRenderTargetView = nullptr;
	m_pDepthStencilView = nullptr;

	//resize the swap chain
	if (!fo_IfFailMsg(m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0), "Failed to resize the swap chain on resize"))
		return false;

	ID3D11Texture2D* backbuffer = nullptr;
	
	if (!fo_IfFailMsg(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backbuffer)), "Failed to get back buffer on resize"))
		return false;
	
	if (!fo_IfFailMsg(m_pDevice->CreateRenderTargetView(backbuffer, NULL, &m_pRenderTargetView), "Failed to create render target view on resize"))
		return false;

	// create the depth and stencil buffer
	D3D11_TEXTURE2D_DESC dsd;
	ID3D11Texture2D* dsBuffer;
	backbuffer->GetDesc(&dsd);

	SAFE_RELEASE(backbuffer);

	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (!fo_IfFailMsg(m_pDevice->CreateTexture2D(&dsd, NULL, &dsBuffer), "Failed to create texture2d on resize"))
		return false;
		
	if (!fo_IfFailMsg(m_pDevice->CreateDepthStencilView(dsBuffer, NULL, &m_pDepthStencilView), "Failed to create depth stencil view on resize"))
		return false;

	// activate the depth and stencil buffer
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// set the viewport to the entire backbuffer
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (float)dsd.Width;
	vp.Height = (float)dsd.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &vp);
	
	// Setup the projection matrix.
	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = vp.Width / vp.Height;

	// Create the projection matrix for 3D rendering.
	m_ProjMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, vp.MinDepth, vp.MaxDepth);

	// Initialize the world matrix to the identity matrix.
	m_WorldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_OrthoMatrix = XMMatrixOrthographicLH(vp.Width, vp.Height, vp.MinDepth, vp.MaxDepth);

	return true;
}

