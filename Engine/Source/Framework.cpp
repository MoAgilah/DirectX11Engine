////////////////////////////////////////////////////////////////////////////////
// Filename: Framework.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Framework.h"

Framework::Framework()
{
	m_lpApplicationName = nullptr;
	m_HInstance = nullptr;
	m_Hwnd = nullptr;

	m_bIsPaused = false;

	m_pInput = nullptr;
	m_pGraphics = nullptr;
	m_pCollisionsMgr = nullptr;
}


Framework::Framework(const Framework& other)
{
	m_lpApplicationName = other.m_lpApplicationName;
	m_HInstance = other.m_HInstance;
	m_Hwnd = other.m_Hwnd;

	m_bIsPaused = other.m_bIsPaused;

	m_pInput = other.m_pInput;
	m_pGraphics = other.m_pGraphics;
	m_pCollisionsMgr = other.m_pCollisionsMgr;
}


Framework::~Framework()
{
}


bool Framework::Initialise()
{
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitWindows(screenWidth, screenHeight);

	m_pInput = Input::GetInput();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_pGraphics = new Graphics;
	if (!m_pGraphics) return false;

	// Initialize the graphics object.
	result = m_pGraphics->Initialise(screenWidth, screenHeight, &m_Hwnd);
	if (!result)
	{
		MessageBox(m_Hwnd, "Failed to initialise graphics", "Error Msg", MB_OK);
		return false;
	}

	//setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	static ImGuiIO& io = ImGui::GetIO();
	bool test = ImGui_ImplWin32_Init(m_Hwnd);
	test = ImGui_ImplDX11_Init(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext());
	ImGui::StyleColorsDark();

	m_pCollisionsMgr = new CollisionMgr(m_pGraphics);
	if (!m_pCollisionsMgr)
	{
		return false;
	}

	m_pStateManager = GameStateManager::GetStateMgr();
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
	SAFE_DELETE(m_pInput);

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void Framework::Run()
{
	MSG msg;
	bool running;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	m_Timer.Reset();

	// Loop until there is a quit message from the window or the user.
	running = true;
	while (running)
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
			running = false;
		}
		else
		{
			if (!m_bIsPaused)
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
		// Check if a key has been pressed on the keyboard.
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
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		m_pInput->GetKeyboard()->OnKeyReleased(static_cast<unsigned char>(wparam));
		return 0;
	}

	
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
		return 0;
	}

	//Mouse Messages--
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		m_pInput->GetMouse()->OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		m_pInput->GetMouse()->OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		m_pInput->GetMouse()->OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		m_pInput->GetMouse()->OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		m_pInput->GetMouse()->OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		m_pInput->GetMouse()->OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		m_pInput->GetMouse()->OnMiddleReleased(x, y);
		return 0;
	}
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
		return 0;
	}

	//active messages
	case WM_ACTIVATE:
		if (LOWORD(wparam) == WA_INACTIVE)
		{
			m_bIsPaused = true;
			m_Timer.Stop();
		}
		else
		{
			m_bIsPaused = false;
			m_Timer.Start();
		}


		// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the
		// average time it takes to render one frame. These stats
		// are appeneded to the window caption bar.
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	// Compute averages over one second period.
	if ((m_Timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::string out = m_lpApplicationName;
		out += " FPS: ";
		out += std::to_string(fps);
		out += " Frame Time: ";

		out += std::to_string(mspf);
		out += " (ms)";



		SetWindowTextA(m_Hwnd, (LPCSTR)out.c_str());
		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
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

	// Check if the user pressed escape and wants to exit the application.
	if (m_pInput->GetKeyboard()->KeyIsPressed(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

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


void Framework::InitWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	gs_pApplicationHandle = this;

	// Get the instance of this application.
	m_HInstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_lpApplicationName = "Engine";

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
	wc.lpszClassName = m_lpApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);


	// Register the window class.
	RegisterClassEx(&wc);

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
	m_Hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_lpApplicationName, m_lpApplicationName,
		style, posX, posY, screenWidth, screenHeight, NULL, NULL, m_HInstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_Hwnd, SW_SHOW);
	SetForegroundWindow(m_Hwnd);
	SetFocus(m_Hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);
	SetCursorPos(200, 300);

	return;
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
	UnregisterClass(m_lpApplicationName, m_HInstance);
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
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return gs_pApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}