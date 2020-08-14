#include "../Headers/Robot.h"
#include "../Headers/Input.h"


bool Robot::s_bResourcesReady = false;
//
//AnimationController* Robot::aControl = nullptr;

Robot::Robot(ID3D11Device* device, Texture* texture, const XMFLOAT3& pos, const XMFLOAT3& rot, const double& scalar, bool assimp, const char* fileExt)
	:Hiearchal3DObj(device, texture, (char*)"Robot", (char*)"Resources/Data/robotHierarchy.txt", pos, rot, scalar,assimp, fileExt)
{
	scaleF = scalar;
	aControl = new AnimationController(pos, rot, 10.0f);

	m_f4Position = m_pMeshHiearchy->GetRoot()->GetLocPos();
	m_f4Rotation = m_pMeshHiearchy->GetRoot()->GetLocRot();
	m_mWorldMatrix = m_pMeshHiearchy->GetRoot()->GetWorld();

	LoadAnimations();

	m_mCamWorldMatrix = XMMatrixIdentity();

	m_v4CamOff = XMFLOAT4(0.5, 12.0f, 50.0f, 0.0f);
	m_v4CamRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	m_fSpeed = 0.0f;

	m_bRobCam = false;
	stopAnim = false;
	test = 0;
}

Robot::~Robot()
{
	delete aControl;
}

void Robot::LoadAnimations(void)
{
	aControl->LoadAnimation(m_pMeshHiearchy->GetRoot(), "Robot", "Idle", 10.f);
	m_sCurrentAnim = "Idle";
	aControl->LoadAnimation(m_pMeshHiearchy->GetRoot(), "Robot", "Attack", 10.f);
	aControl->LoadAnimation(m_pMeshHiearchy->GetRoot(), "Robot", "Die", 10.f, false);
}

void Robot::Update(bool bPlayerContol, const double& deltatime)
{
	m_pMeshHiearchy->GetRoot()->updateMat();
	map <string, MeshComponent*> hierarchy = m_pMeshHiearchy->GetHierarchy();

	if (bPlayerContol)
	{
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('1'))
		{
			if (!aControl->GetIsActive("Idle"))
			{
				aControl->ChangeTrack("Idle");
				m_sCurrentAnim = "Idle";
			}
		}

		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('2'))
		{
			if (!aControl->GetIsActive("Attack"))
			{
				aControl->ChangeTrack("Attack");
				m_sCurrentAnim = "Attack";
			}
		}

		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('3'))
		{
			if (!aControl->GetIsActive("Die"))
			{
				aControl->ChangeTrack("Die");
				m_sCurrentAnim = "Die";
			}
		}

		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('F'))
		{
			stopAnim = true;
			
			if (test == 0)
			{
				aControl->AdvanceTime(test, hierarchy);
			}
			else if (test >= 0.1666667)
			{
				aControl->AdvanceTime(test, hierarchy);
				test = 0;
			}
			test += deltatime;
				
		}
		else
		{
			stopAnim = false;
		}
	}
	
	if (!stopAnim)
	{
		aControl->AdvanceTime(deltatime, hierarchy);
	}
}

void Robot::Draw(Graphics* graphics, Light* lights[])
{
	m_pMeshHiearchy->Render(graphics,lights);
}

void Robot::UpdateMatrices(void)
{
	XMVECTOR pVec;
	XMMATRIX mRotY, mTrans;

	m_pMeshHiearchy->GetRoot()->updateMat();

	m_f4Position = m_pMeshHiearchy->GetRoot()->GetLocPos();
	m_f4Rotation = m_pMeshHiearchy->GetRoot()->GetLocRot();
	m_mWorldMatrix = m_pMeshHiearchy->GetRoot()->GetWorld();

	mRotY = XMMatrixRotationY(XMConvertToRadians(m_f4Rotation.y));
	pVec = XMLoadFloat4(&m_f4Position);

	mTrans = XMMatrixTranslationFromVector(pVec);

	pVec = XMLoadFloat4(&m_v4CamOff);
	m_mCamWorldMatrix = XMMatrixTranslationFromVector(pVec) * mRotY * mTrans;

	// Get the camera's world position (m_vCamWorldPos) out of m_mCameraWorldMatrix
	m_vCamWorldPos = XMVectorZero();
	m_vCamWorldPos = XMVector3Transform(m_vCamWorldPos, m_mCamWorldMatrix);
}

void Robot::BeAggressive(MeshComponent* plane)
{
	/*float dx = abs(s_Hiearchie->GetByName("body")->GetLocPos().x - plane->GetLocPos().x);
	float dy = abs(s_Hiearchie->GetByName("body")->GetLocPos().y - plane->GetLocPos().y);
	float dz = abs(s_Hiearchie->GetByName("body")->GetLocPos().z - plane->GetLocPos().z);

	float dist = sqrtf((dx*dx) + (dy*dy) + (dz*dz));

	if (dist >= 0 && dist <= 20)
	{
		if (!aControl->GetIsActive("Attack"))
		{

			aControl->ChangeTrack("Attack");
		}
	}
	else
	{
		if (aControl->GetIsActive("Attack") && m_bRobCam == false)
		{
			aControl->ChangeTrack("Idle");

		}
	}*/
}