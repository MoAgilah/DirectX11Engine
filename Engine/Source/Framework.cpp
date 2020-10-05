////////////////////////////////////////////////////////////////////////////////
// Filename: Framework.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Framework.h"

Framework::Framework()
	:m_ApplicationName(nullptr),m_HInstance(nullptr),m_Hwnd(nullptr),
	m_IsPaused(false), m_IsResizing(false), m_IsMinimised(false), m_IsMaximised(false), m_FrameCnt(0), m_TimeElapsed(0.0f),m_FrameTime(0.f),
	m_pInput(Input::GetInput()),m_pGraphics(new Graphics),m_pCollisionsMgr(nullptr),m_pStateManager(GameStateManager::GetStateMgr())
{}

Framework::~Framework()
{
	Release();
}

bool Framework::Initialise()
{
	int screenWidth(0), screenHeight(0);

	// Initialize the windows api.
	if (!fo_IfFailMsg(InitWindows(screenWidth, screenHeight), "Failed to Initialise Window")) return false;

	// Initialize the graphics module.
	assert(m_pGraphics != nullptr);	
	if (!fo_IfFailMsg(m_pGraphics->Initialise(screenWidth, screenHeight, &m_Hwnd), "Failed to initialise graphics module")) return false;

	// Initialise the collision module 
	m_pCollisionsMgr = new CollisionMgr(m_pGraphics);
	assert(m_pCollisionsMgr != nullptr);

	//setup ImGui--------------temp
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	static ImGuiIO& io = ImGui::GetIO();
	bool test = ImGui_ImplWin32_Init(m_Hwnd);
	test = ImGui_ImplDX11_Init(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext());
	ImGui::StyleColorsDark();
	//------------------------------

	m_pStateManager->ChangeState(new BlankState(m_pGraphics, m_pCollisionsMgr, m_pInput, &m_Timer, "Hierarchy State"));

	return true;
}


void Framework::Release()
{
	SAFE_RELEASE(m_pStateManager);

	// Shutdown
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SAFE_RELEASE(m_pGraphics);
	SAFE_RELEASE(m_pInput);

	// Shutdown the window.
	ShutdownWindows();
}

void Framework::Run()
{
	MSG msg;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	m_Timer.Reset();

	// Loop until there is a quit message from the window or the user.
	while (true)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			break;
		}
		else
		{
			if (!m_IsPaused)
			{
				m_Timer.Tick();

				UpdateScene(m_Timer.DeltaTime());
				DrawScene();
			}
			else
			{
				Sleep(1000);
			}
		}
	}

	return;
}

LRESULT CALLBACK Framework::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		//keyboard messages--
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			if (m_pInput->GetKeyboard()->IsKeysAutoRepeat())
			{
				m_pInput->GetKeyboard()->OnKeyPressed(static_cast<unsigned char>(wparam));
			}
			else
			{
				const bool wasPressed = lparam & 0x40000000;

				if (!wasPressed)
				{
					m_pInput->GetKeyboard()->OnKeyPressed(static_cast<unsigned char>(wparam));
				}
			}
		}
		return 0;
	

		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_pInput->GetKeyboard()->OnKeyReleased(static_cast<unsigned char>(wparam));
		}
		return 0;

		case WM_CHAR:
		{
			if (m_pInput->GetKeyboard()->IsCharsAutoRepeat())
			{
				m_pInput->GetKeyboard()->OnChar(static_cast<unsigned char>(wparam));
			}
			else
			{
				const bool wasPressed = lparam & 0x40000000;
				if (!wasPressed)
				{
					m_pInput->GetKeyboard()->OnChar(static_cast<unsigned char>(wparam));
				}
			}
		}
		return 0;

		//Mouse Messages--
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			m_pInput->GetMouse()->OnMouseMove(x, y);
		}
		return 0;

		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			m_pInput->GetMouse()->OnLeftPressed(x, y);
		}
		return 0;

		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			m_pInput->GetMouse()->OnRightPressed(x, y);
		}
		return 0;
	

		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			m_pInput->GetMouse()->OnMiddlePressed(x, y);
		}
		return 0;
	

		case WM_LBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			m_pInput->GetMouse()->OnLeftReleased(x, y);
		}
		return 0;
	

		case WM_RBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			m_pInput->GetMouse()->OnRightReleased(x, y);
		}
		return 0;
	

		case WM_MBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			m_pInput->GetMouse()->OnMiddleReleased(x, y);
		}
		return 0;

		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
			{
				m_pInput->GetMouse()->OnWheelUp(x, y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
			{
				m_pInput->GetMouse()->OnWheelDown(x, y);
			}
		}
		return 0;
	
		//OnResizing--
		case WM_ENTERSIZEMOVE:
		{
			m_IsPaused = true;
			m_IsResizing = true;
			m_Timer.Stop();
		}
		return 0;
	
		case WM_EXITSIZEMOVE:
		{
			m_IsPaused = false;
			m_IsResizing = false;
			m_Timer.Start();
			m_pGraphics->GetD3DMgr()->OnResize(&m_Hwnd);
		}
		return 0;
	

		//OnActivate/Deactive--
		case WM_ACTIVATE:
		{
			if (LOWORD(wparam) == WA_INACTIVE)
			{
				m_IsPaused = true;
				m_Timer.Stop();
			}
			else
			{
				m_IsPaused = false;
				m_Timer.Start();
			}
		}
		return 0;

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the
		// average time it takes to render one frame. These stats
		// are appeneded to the window caption bar.
	m_FrameCnt++;
	// Compute averages over one second period.
	if ((m_Timer.TotalTime() - m_TimeElapsed) >= 1.0f)
	{
		float fps = (float)m_FrameCnt; // fps = frameCnt / 1
		m_FrameTime = 1000.0f / fps;

		// Reset for next average.
		m_FrameCnt = 0;
		m_TimeElapsed += 1.0f;
	}
}

void Framework::ProcessInput()
{
	while (!m_pInput->GetKeyboard()->CharBufferIsEmpty())
	{
		unsigned char ch = m_pInput->GetKeyboard()->ReadChar();
	}

	while (!m_pInput->GetKeyboard()->KeyBufferIsEmpty())
	{

		unsigned char keycode = m_pInput->GetKeyboard()->ReadKey().GetKeyCode();
	}

	while (!m_pInput->GetMouse()->EventBufferIsEmpty())
	{
		MouseEvent me = m_pInput->GetMouse()->ReadEvent();
		std::string outmsg = "X: ";
		outmsg += std::to_string(me.GetPosX());
		outmsg += " Y: ";
		outmsg += std::to_string(me.GetPosY());
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_pInput->GetKeyboard()->KeyIsPressed(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	/*
	float cameraSpeed = 20.0f;

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed(VK_UP))
	{
		m_pGraphics->GetCamera()->Pitch(XMConvertToRadians(-cameraSpeed * m_Timer.DeltaTime()));
	}

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed(VK_DOWN))
	{
		m_pGraphics->GetCamera()->Pitch(XMConvertToRadians(cameraSpeed * m_Timer.DeltaTime()));
	}

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed(VK_LEFT))
	{
		m_pGraphics->GetCamera()->Yaw(XMConvertToRadians(-cameraSpeed * m_Timer.DeltaTime()));
	}

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed(VK_RIGHT))
	{
		m_pGraphics->GetCamera()->Yaw(XMConvertToRadians(cameraSpeed * m_Timer.DeltaTime()));
	}

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('W'))
	{
		m_pGraphics->GetCamera()->MoveForward(cameraSpeed * m_Timer.DeltaTime());
	}

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('A'))
	{
		m_pGraphics->GetCamera()->Strafe(-cameraSpeed * m_Timer.DeltaTime());
	}

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('S'))
	{
		m_pGraphics->GetCamera()->MoveForward(-cameraSpeed * m_Timer.DeltaTime());
	}

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('D'))
	{
		m_pGraphics->GetCamera()->Strafe(cameraSpeed * m_Timer.DeltaTime());
	}
	*/
}

void Framework::UpdateScene(const float& deltatime)
{
	ProcessInput();
	m_pStateManager->Update(deltatime);
}

void Framework::DrawScene()
{
	m_pStateManager->Draw();
}


bool Framework::InitWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.	
	gs_pApplicationHandle = this;

	// Get the instance of this application.
	m_HInstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_ApplicationName = "Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_HInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_ApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	if (!RegisterClassEx(&wc)) return false;

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (g_bFULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	int style;
	if (g_bBORDERED)
	{
		style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;
	}
	else
	{
		style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	}

	// Create the window with the screen settings and get the handle to it.
	m_Hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName,
		style, posX, posY, screenWidth, screenHeight, NULL, NULL, m_HInstance, NULL);

	assert(m_Hwnd != nullptr);

	// Bring the window up on the screen and set it as main focus.
	if(!ShowWindow(m_Hwnd, SW_SHOW)) return false;
	if(!SetForegroundWindow(m_Hwnd)) return false;
	m_Hwnd = SetFocus(m_Hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);
	if (!SetCursorPos(posX, posY)) return false;

	return true;
}


void Framework::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (g_bFULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_Hwnd);
	m_Hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_ApplicationName, m_HInstance);
	m_HInstance = NULL;

	// Release the pointer to this class.
	gs_pApplicationHandle = NULL;

	return;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{

	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		return true;

	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 0;

	// Check if the window is being closed.
	case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		return 0;

	// All other messages pass to the message handler in the system class.
	default:
		{
			return gs_pApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}

	return gs_pApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
}