#include "../Headers/HierarchyState.h"

HierarchyState::HierarchyState(Graphics* graphics, CollisionMgr* collisions, Input* input, Timer* timer, string stateName)
{
	m_pGraphics = graphics;
	m_pCollisions = collisions;
	m_pInput = input;
	m_pTimer = timer;
	m_sStateName = stateName;
	m_pTimer->Reset();

	m_pTerrain = nullptr;

	m_pPlane = nullptr;
	m_pRobot = nullptr;


	m_pLight[0] = nullptr;
	m_pLight[1] = nullptr;
	m_pLight[2] = nullptr;
	m_pLight[3] = nullptr;

	m_iCameraState = CameraState::PLANE;
}

bool HierarchyState::Initialise()
{
	bool result = false;

	// Create the light object.
	m_pLight[0] = new Light;
	if (!m_pLight[0])
	{
		return false;
	}

	// Initialize the light object.
	m_pLight[0]->SetLightType(LightType::Directional);
	m_pLight[0]->SetIsEnabled(true);
	m_pLight[0]->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_pLight[0]->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLight[0]->SetDirection(0.0f, -1.0f, 0.0f);
	m_pLight[0]->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLight[0]->SetSpecularPower(32.0f);

	m_pLight[1] = new Light;
	if (!m_pLight[1])
	{
		return false;
	}

	// Initialize the second light object.
	m_pLight[1]->SetIsEnabled(false);
	m_pLight[1]->SetLightType(LightType::Point);
	m_pLight[1]->SetAmbientColor(0.025f, 0.0f, 0.0f, 1.0f);
	m_pLight[1]->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_pLight[1]->SetSpecularColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_pLight[1]->SetSpecularPower(32.f);
	m_pLight[1]->SetPosition(-3.0f, 1.0f, 3.0f);
	m_pLight[1]->SetDirection(0.0f, -1.0f, 0.0f);
	m_pLight[1]->SetSpotAngle(XMConvertToRadians(45));
	m_pLight[1]->SetConstantAttenuation(1.0f);
	m_pLight[1]->SetLinearAttenuation(0.08f);
	m_pLight[1]->SetQuadraticAttenuation(0.0f);

	m_pLight[2] = new Light;
	if (!m_pLight[2])
	{
		return false;
	}

	// Initialize the second light object.
	m_pLight[2]->SetIsEnabled(false);
	m_pLight[2]->SetLightType(LightType::Point);
	m_pLight[2]->SetAmbientColor(0.025f, 0.0f, 0.0f, 1.0f);
	m_pLight[2]->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_pLight[2]->SetSpecularColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_pLight[2]->SetSpecularPower(32.f);
	m_pLight[2]->SetPosition(-3.0f, 1.0f, 3.0f);
	m_pLight[2]->SetDirection(0.0f, -1.0f, 0.0f);
	m_pLight[2]->SetSpotAngle(XMConvertToRadians(45));
	m_pLight[2]->SetConstantAttenuation(1.0f);
	m_pLight[2]->SetLinearAttenuation(0.08f);
	m_pLight[2]->SetQuadraticAttenuation(0.0f);

	m_pLight[3] = new Light;
	if (!m_pLight[3])
	{
		return false;
	}

	// Initialize the third light object.
	m_pLight[3]->SetIsEnabled(false);
	m_pLight[3]->SetLightType(LightType::Spot);
	m_pLight[3]->SetAmbientColor(0.0f, 0.15f, 0.0f, 1.0f);
	m_pLight[3]->SetDiffuseColor(0.0f, 0.15f, 0.0f, 1.0f);
	m_pLight[3]->SetSpecularColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_pLight[3]->SetSpecularPower(2.0f);
	m_pLight[3]->SetPosition(280.0f, 10.f, 250.0f);
	m_pLight[3]->SetDirection(0, -1, 0);
	m_pLight[3]->SetSpotAngle(XMConvertToRadians(45.f));
	m_pLight[3]->SetConstantAttenuation(1.0f);
	m_pLight[3]->SetLinearAttenuation(0.2f);
	m_pLight[3]->SetQuadraticAttenuation(0.1f);

	//Load the terrain
	m_pTerrain = new Terrain;
	if (!m_pTerrain)
	{
		return false;
	}
	
	//load terrain's texture
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/test.tga", TexType::Targa);
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/dirt.tga", TexType::Targa);

	result = m_pTerrain->Initialize(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetTextureMgr()->GetTexture("dirt"), (char*)"Resources/Data/Setup.txt");
	if (!result)
	{
		MessageBox(*m_pGraphics->GetApplicationHandle(), "Could not initialize the terrain object.", "Error", MB_OK);
		return false;
	}

	//load the game objects
	m_pPlane = new Aeroplane(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetTextureMgr()->GetTexture("dirt"), XMFLOAT3(280.0f, -1.65f, 250.0f), XMFLOAT3(0, 105, 0), 1.f, true, (char*)".x");
	m_pRobot = new Robot(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetTextureMgr()->GetTexture("dirt"), XMFLOAT3(256, 0, 256), XMFLOAT3(0, 0, 0), 1.f, true, (char*)".x");

	//setup the intial position and lookat of the main camera
	m_pGraphics->GetCamera()->SetPosition(XMFLOAT3(m_pPlane->GetCameraPosition().x, m_pPlane->GetCameraPosition().y, m_pPlane->GetCameraPosition().z));
	m_pGraphics->GetCamera()->SetLookAt(XMFLOAT3(m_pPlane->GetFocusPosition().x, m_pPlane->GetFocusPosition().y, m_pPlane->GetFocusPosition().z));

	return true;
}

void HierarchyState::Release()
{
	SAFE_DELETE(m_pRobot);
	SAFE_DELETE(m_pPlane);
	SAFE_RELEASE(m_pTerrain);
	SAFE_DELETE(m_pLight[0]);
	SAFE_DELETE(m_pLight[1]);
	SAFE_DELETE(m_pLight[2]);
	SAFE_DELETE(m_pLight[3]);
}

void HierarchyState::Pause()
{
	m_pTimer->Stop();
}

void HierarchyState::Resume()
{
	m_pTimer->Start();
}

void HierarchyState::ProcessInputs()
{
	static bool keystates[2] = { false,false };

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('C'))
	{
		if (!keystates[0])
		{
			if (++m_iCameraState == CameraState::MAX)
				m_iCameraState = CameraState::PLANE;

			keystates[0] = true;
		}
	}
	else
	{
		keystates[0] = false;
	}

	if (Input::GetInput()->GetKeyboard()->KeyIsPressed('Z'))
	{
		if (!keystates[1])
		{
			m_bWireframe = !m_bWireframe;

			if (m_bWireframe)
				m_pGraphics->GetD3DMgr()->EnableWireframe();
			else
				m_pGraphics->GetD3DMgr()->DisableWireframe();

			keystates[1] = true;
		}
	}
	else
	{
		keystates[1] = false;
	}
}

void HierarchyState::Update(const float& deltaTime)
{
	
	UpdateCameraState();

	m_pGraphics->GetCamera()->Update();

	m_pPlane->Update(deltaTime);
	m_pRobot->Update((m_iCameraState == ROBOT), deltaTime);


	//Demo Text - utilising the ImGui library
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	//create Demo test window
	ImGui::Begin("Engine Demo",NULL);
	ImGuiID zero;
	ImGui::SetWindowPos(ImVec2(0.f, 0.f));
	ImGui::SetWindowSize(ImVec2(256.f, 256.f));

	if (ImGui::CollapsingHeader("Demo Description:",ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("   A real time 3d techniques  ");
		ImGui::Text("  for games demo that showcases  ");
		ImGui::Text(" what I learnt at university and ");
		ImGui::Text("during self study, it showcases:");
		ImGui::BulletText("Hierachal Animation");
	}

	ImGui::Text(GetStateName().c_str());

	if (ImGui::CollapsingHeader("Demo's Control's:")) 
	{
		ImGui::BulletText("C - Change Camera State");
		ImGui::BulletText("Z - Toggle Wireframe");
	}

	ImGui::Text(GetCameraStateName().c_str());

	if (ImGui::CollapsingHeader("Camera's State Control's:"))
	{
		switch (m_iCameraState)
		{
		case PLANE:
		case GUN:
			ImGui::BulletText("I - Pitch Upward");
			ImGui::BulletText("K - Pitch Downward");
			ImGui::BulletText("L - Yaw and Roll Left");
			ImGui::BulletText("J - Yaw and Roll Right");
			ImGui::BulletText("Space - Fire Gun");
			
			break;
		case ROBOT:
			ImGui::Text("Current anim = %s", m_pRobot->GetCurrentAnimName().c_str());
			ImGui::Text("Key's to change the anim:-");
			ImGui::BulletText("1 - Set to Idle Anim");
			ImGui::BulletText("2 - Set to Attack Anim");
			ImGui::BulletText("3 - Set to Die Anim");
			ImGui::BulletText("F - Single step current Anim");

			break;
		case FREE:
			ImGui::BulletText("I - Pitch Upward");
			ImGui::BulletText("K - Pitch Downward");
			ImGui::BulletText("L - Yaw and Roll Left");
			ImGui::BulletText("J - Yaw and Roll Right");
			ImGui::BulletText("Space - Fire Gun");
		/*case GUN:
			
				Unsure
			
			break;*/
		default:
			break;
		}
	}

	ImGui::End();

	ProcessInputs();
}

void HierarchyState::Draw()
{
	// Clear the buffers to begin the scene.
	m_pGraphics->GetD3DMgr()->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	XMFLOAT4 camPos = *m_pGraphics->GetCamera()->GetPosition();
	
	//render 3D stuffs
	m_pTerrain->Render(m_pGraphics->GetD3DMgr()->GetDeviceContext());

	if (!m_pGraphics->GetShaderMgr()->RenderLightShader(m_pGraphics->GetD3DMgr()->GetDeviceContext(), m_pTerrain->GetIndexCount(), *m_pGraphics->GetD3DMgr()->GetWorldMatrix(), 
		*m_pGraphics->GetCamera()->GetViewMatrix(), *m_pGraphics->GetCamera()->GetProjectionMatrix(), m_pTerrain->GetTexture(), XMFLOAT3(camPos.x, camPos.y, camPos.z), m_pLight))
	{
		MessageBox(*m_pGraphics->GetApplicationHandle(), "Failed to set shader parameters - failed to render LightShader", "Error", MB_OK);
	}

	m_pPlane->Render(m_pGraphics, m_pLight);
	m_pRobot->Render(m_pGraphics, m_pLight);

	//render 2D stuffs
	// Turn off the Z buffer to begin all 2D rendering.
	m_pGraphics->GetD3DMgr()->TurnZBufferOff();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_pGraphics->GetD3DMgr()->TurnZBufferOn();

	//assemble together draw data
	ImGui::EndFrame();
	ImGui::Render();

	//render draw data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_pGraphics->GetD3DMgr()->EndScene();
}

string HierarchyState::GetCameraStateName()
{
	switch (m_iCameraState)
	{
	case PLANE:
		return string("The Camera State is Plane");
		break;
	case GUN:
		return string("The Camera State is Gun");
		break;
	case ROBOT:
		return string("The Camera State is Robot");
		break;
	case FREE:
		return string("The Camera State is Free movemnt");
		break;
	default:
		return string("");
		break;
	}
}

void HierarchyState::UpdateCameraState()
{
	XMFLOAT3 pos, foc;

	switch (m_iCameraState)
	{
	case PLANE:
		m_pPlane->SetGunCamera(false);
		m_pPlane->SetPlayerControl(true);
		pos = XMFLOAT3(m_pPlane->GetCameraPosition().x, m_pPlane->GetCameraPosition().y, m_pPlane->GetCameraPosition().z);
		foc = XMFLOAT3(m_pPlane->GetFocusPosition().x, m_pPlane->GetFocusPosition().y, m_pPlane->GetFocusPosition().z);

		m_pGraphics->GetCamera()->SetPosition(XMFLOAT3(m_pPlane->GetCameraPosition().x, m_pPlane->GetCameraPosition().y, m_pPlane->GetCameraPosition().z));
		m_pGraphics->GetCamera()->SetLookAt(XMFLOAT3(m_pPlane->GetFocusPosition().x, m_pPlane->GetFocusPosition().y, m_pPlane->GetFocusPosition().z));
		break;
	case GUN:
		m_pPlane->SetGunCamera(true);

		//needs changing to first person view with
		pos = XMFLOAT3(m_pPlane->GetCameraPosition().x, m_pPlane->GetCameraPosition().y, m_pPlane->GetCameraPosition().z);
		foc = XMFLOAT3(m_pPlane->GetFocusPosition().x, m_pPlane->GetFocusPosition().y, m_pPlane->GetFocusPosition().z);

		m_pGraphics->GetCamera()->SetPosition(pos);
		m_pGraphics->GetCamera()->SetLookAt(foc);
		break;
	case ROBOT:
		m_pPlane->SetPlayerControl(false);
		m_pRobot->SetRobCamera(true);

		pos = XMFLOAT3(m_pRobot->GetPosition().x + 15.f, m_pRobot->GetPosition().y + 30.f, m_pRobot->GetPosition().z + 40.f);
		foc = XMFLOAT3(m_pRobot->GetFocusPosition().x, m_pRobot->GetFocusPosition().y, m_pRobot->GetFocusPosition().z);

		m_pGraphics->GetCamera()->SetPosition(pos);
		m_pGraphics->GetCamera()->SetLookAt(foc);
		break;
	case FREE:
		m_pRobot->SetRobCamera(false);
		break;
	}
}

