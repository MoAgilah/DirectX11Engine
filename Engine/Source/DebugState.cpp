////////////////////////////////////////////////////////////////////////////////
// Filename: DebugState.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/DebugState.h"
#include "../Headers/StateMgr.h"
#include "../Headers/BlankState.h"

static bool tiled = false;

DebugState::DebugState(Graphics* graphics, CollisionMgr* collisions, Input* input, Timer* timer, string stateName)
{
	m_pGraphics = graphics;
	m_pCollisions = collisions;
	m_pTimer = timer;
	m_pTimer->Reset();
	m_pTerrain = nullptr;
	m_pLight[0] = nullptr;
	m_pLight[1] = nullptr;
	m_pLight[2] = nullptr;
	m_pLight[3] = nullptr;
	m_pAnimSprite = nullptr;
	m_sStateName = stateName;
}

bool DebugState::Initialise()
{
	bool result = false;

	m_pGraphics->GetCamera()->SetPosition(XMFLOAT3(0, 0, -16));

	m_pTerrain = new Terrain;
	if (!m_pTerrain)
	{
		return false;
	}
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/square.png", TexType::WIC);
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/test.tga", TexType::Targa);
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/smario.png", TexType::WIC);
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/crosshair.png", TexType::WIC);

	result = m_pTerrain->Initialize(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetTextureMgr()->GetTexture("test"), (char*)"Resources/Data/Setup.txt");
	if (!result)
	{
		MessageBox(*m_pGraphics->GetApplicationHandle(), "Could not initialize the terrain object.", "Error", MB_OK);
		return false;
	}

	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/dirt.tga", TexType::Targa);

	
	m_pAnimSprite = new AnimatedSprite(m_pGraphics->GetSpriteMgr()->GetSprite());

	if (!m_pAnimSprite)
	{
		return false;
	}

	XMFLOAT2 scrCenter = m_pGraphics->GetScreenDimensions();
	scrCenter.x *= 0.5f;
	scrCenter.y *= 0.5f;

	result = m_pAnimSprite->Initialise(m_pGraphics->GetD3DMgr(), m_pGraphics->GetTextureMgr()->GetTexture("smario"), XMFLOAT2(400, 300),XMFLOAT2(0,0),XMFLOAT2(2.5,2.5));
	if (!result)
	{
		return false;
	}

	m_pAnimSprite->SetupAnimatedSprite(8, 3, 60, 0, true, 3, 0.5f);
	m_pAnimSprite->SetFrames(std::vector<int>{1, 1, 1, 3, 1, 2, 1, 2});

	m_pAnimSprite->GetSprite()->AssignAABB(m_pCollisions->Add2DAABB());
	m_pAnimSprite->GetSprite()->GetAABB()->SetPadding(XMFLOAT3(5,15,0));
	m_pAnimSprite->GetSprite()->GetAABB()->Shrink(XMFLOAT3(.7f, .76f, 1));

	m_pAnimSprite->GetSprite()->AssignBoundingCapsule(m_pCollisions->Add2DBoundingCapsule());
	m_pAnimSprite->GetSprite()->GetBoundingCapsule()->SetPadding(XMFLOAT3(0, 22, 0));
	
	m_pAnimSprite->GetSprite()->GetBoundingCapsule()->Shrink(XMFLOAT3(.6f, .6f, .6f));

	crosshair = m_pGraphics->GetSpriteMgr()->GetSprite();

	
	
	crosshair->Initialise(m_pGraphics->GetD3DMgr(), m_pGraphics->GetTextureMgr()->GetTexture("crosshair"), XMFLOAT2(0, 0), XMFLOAT2(128.f, 128.f));

	// Create the model object.
	m_pGraphics->GetTextureMgr()->Load(m_pGraphics->GetD3DMgr()->GetDevice(), m_pGraphics->GetD3DMgr()->GetDeviceContext(), (char*)"Resources/Images/stone01.png", TexType::WIC);

	// Create the light object.
	m_pLight[0] = new Light;
	if (!m_pLight)
	{
		return false;
	}

	// Initialize the light object.
	m_pLight[0]->SetLightType(LightType::Directional);
	m_pLight[0]->SetIsEnabled(true);
	m_pLight[0]->SetAmbientColor(.15f, 0.15f, 0.15f, 1.0f);
	m_pLight[0]->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLight[0]->SetPosition(0.f, 5.f, 0.f);
	m_pLight[0]->SetDirection(0.0f, -1.0f, 0.0f);
	m_pLight[0]->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLight[0]->SetSpecularPower(32.f);

	for (int i = 1; i < 4; i++)
	{
		m_pLight[i] = new Light;
		if (!m_pLight)
		{
			return false;
		}
		m_pLight[i]->SetLightType(LightType::Directional);
		m_pLight[i]->SetIsEnabled(false);
	}

	return true;
}

void DebugState::Release()
{
	SAFE_RELEASE(m_pAnimSprite);
	SAFE_DELETE(m_pLight[0]);
	SAFE_DELETE(m_pLight[1]);
	SAFE_DELETE(m_pLight[2]);
	SAFE_DELETE(m_pLight[3]);
	SAFE_RELEASE(m_pTerrain);
}

void DebugState::Pause()
{
	m_pTimer->Stop();
}

void DebugState::Resume()
{
	m_pTimer->Start();
}

void DebugState::ProcessInputs()
{

}

void DebugState::Update(const float& deltaTime)
{
	m_pGraphics->GetCamera()->Update();

	m_pAnimSprite->Update(deltaTime);

	crosshair->Update(deltaTime);

	const wchar_t* wszText = L"Hello World From ... DirectWrite!";

	IDWriteTextLayout* textLayout = m_pGraphics->GetTextMgr()->SetupTextLayout(wszText, D2D1::Point2F(0.0f, 0.0f), D2D1::Matrix3x2F::Translation(50, 50));

	DWRITE_TEXT_RANGE textRange = { 21, 12 };

	textLayout->SetFontSize(100.0f, textRange);

	ProcessInputs();
}

void DebugState::Draw()
{
	//XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

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

	//render 2D stuffs
	// Turn off the Z buffer to begin all 2D rendering.
	m_pGraphics->GetD3DMgr()->TurnZBufferOff();

	m_pGraphics->GetSpriteMgr()->Begin();

	//crosshair->NRender(m_pGraphics->GetSpriteMgr()->GetSprteBatch());
	m_pAnimSprite->NRender(m_pGraphics->GetSpriteMgr()->GetSprteBatch());

	m_pGraphics->GetSpriteMgr()->End();

	m_pCollisions->DrawDebug(m_pGraphics);

	m_pCollisions->m_pDebugDraw->Begin();
	m_pCollisions->m_pDebugDraw->End();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_pGraphics->GetD3DMgr()->TurnZBufferOn();

	//render here for in front of the bitmap
	m_pGraphics->GetTextMgr()->DrawTextLayout(0);

	m_pGraphics->GetD3DMgr()->EndScene();
}
