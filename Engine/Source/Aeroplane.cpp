#include "../Headers/Aeroplane.h"

#include "../Headers/Input.h"

Aeroplane::Aeroplane(ID3D11Device* device, Texture* texture,const XMFLOAT3& pos, const XMFLOAT3& rot, float size, bool assimp, const char* fileExt)
	:Hiearchal3DObj(device, texture, (char*)"Plane", (char*)"Resources/Data/planeHierarchy.txt", pos, rot,  size, assimp, fileExt)
{
	SetPosition(m_pMeshHiearchy->GetRoot()->GetLocPos());
	SetRotation(m_pMeshHiearchy->GetRoot()->GetLocRot());
	SetWorldMatrix(m_pMeshHiearchy->GetRoot()->GetWorld());

	m_pBulletPool = new BulletPool(device, texture, (char*)"Resources/Models/Plane/bullet.x",1.f,true);

	m_mCamWorldMatrix = XMMatrixIdentity();

	m_f4CamOff = XMFLOAT4(0.0f, 4.5f, -15.0f, 1.0f);
	m_f4CamRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	m_fSpeed = 0.0f;

	m_bGunCam = false;
	m_bJustFired = false;

	m_bPlayerControl = false;
	m_bStunt = false;
}

Aeroplane::~Aeroplane(void)
{
	Hiearchal3DObj::~Hiearchal3DObj();
	delete m_pBulletPool;
}

void Aeroplane::Update(const double& deltaTime)
{
	m_pBulletPool->Update(deltaTime);
	UpdateMatrices();

	if (m_bPlayerControl)
	{
		// You can also impose a take off speed of 0.5 if you like
		//Make the plane pitch upwards when you press "I" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('I'))
		{
			if (m_f4Rotation.x >= -60)
			{
				m_f4Rotation.x -= 1;
			}
		}//return to level when released
		else  if (!Input::GetInput()->GetKeyboard()->KeyIsPressed('I') || !Input::GetInput()->GetKeyboard()->KeyIsPressed('K'))
		{
			if (m_f4Rotation.x < 0)
			{
				m_f4Rotation.x += 1;
			}
		}

		//Make the plane pitch downwards when you press "K" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('K'))
		{
			if (m_f4Rotation.x <= 60)
			{
				m_f4Rotation.x += 1;
			}
		}//return to level when released
		else  if (!Input::GetInput()->GetKeyboard()->KeyIsPressed('I') || !Input::GetInput()->GetKeyboard()->KeyIsPressed('K'))
		{
			if (m_f4Rotation.x > 0)
			{
				m_f4Rotation.x -= 1;
			}
		}

		// Step 3: Make the plane yaw and roll left when you press "L" 
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('L'))
		{

			if (m_f4Rotation.z >= -20)
			{
				m_f4Rotation.z -= 1;
			}
			m_f4Rotation.y += 1;

			
		}//return to level when released
		else  if (!Input::GetInput()->GetKeyboard()->KeyIsPressed('J') || !Input::GetInput()->GetKeyboard()->KeyIsPressed('L'))
		{
			if (m_f4Rotation.z < 0)
			{
				m_f4Rotation.z += 1;
			}
		}

		//Make the plane yaw and roll right when you press "J" 
		// Minimum roll = -20 degrees
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed('J'))
		{

			if (m_f4Rotation.z <= 20)
			{
				m_f4Rotation.z += 1;
			}
			m_f4Rotation.y -= 1;
		}// return to level when released
		else  if (!Input::GetInput()->GetKeyboard()->KeyIsPressed('J') || !Input::GetInput()->GetKeyboard()->KeyIsPressed('L'))
		{
			if (m_f4Rotation.z > 0)
			{
				m_f4Rotation.z -= 1;
			}
		}

		m_pMeshHiearchy->GetRoot()->GetLocRot() = m_f4Rotation;

		//fire a bullet if you haven't just fired
		if (Input::GetInput()->GetKeyboard()->KeyIsPressed(VK_SPACE) && m_bJustFired == false)
		{	//take in relative motion of the plane for the bullet
			XMVECTOR direction = XMVectorAdd(m_mWorldMatrix.r[2] * 0.1f, m_pMeshHiearchy->GetByName("gun")->GetWorld().r[2]);
			m_pBulletPool->Create(m_pMeshHiearchy->GetByName("gun")->GetWorld(), direction);
			m_bJustFired = true;	//delay firing until key up
		}//once key is released allow the option to fire
		else  if (!Input::GetInput()->GetKeyboard()->KeyIsPressed(VK_SPACE) && m_bJustFired == true)
		{
			m_bJustFired = false;
		}

	}//end of if player control

	m_fSpeed += 0.5f;

	if (m_fSpeed > 10){ m_fSpeed = 10; }

	m_pMeshHiearchy->GetByName("prop")->GetLocRot().z += 100.0f * m_fSpeed;
	m_pMeshHiearchy->GetByName("turret")->GetLocRot().y += 0.1f * m_fSpeed;

	m_pMeshHiearchy->GetByName("gun")->GetLocRot().x = (sin((float)XMConvertToRadians(m_pMeshHiearchy->GetByName("turret")->GetLocRot().y * 4.0f)) * 10.0f) - 10.0f;
	
	XMVECTOR vCurrPos = XMLoadFloat4(&m_f4Position);
	vCurrPos += m_vForwardVector * m_fSpeed * (float)deltaTime;

	XMStoreFloat4(&m_f4Position, vCurrPos);
	m_pMeshHiearchy->GetRoot()->GetLocPos() = m_f4Position;
}

void Aeroplane::UpdateMatrices(void)
{
	XMVECTOR pVec;
	XMMATRIX mRotY, mTrans;

	//update entire hiearchie
	m_pMeshHiearchy->GetRoot()->updateMat();
	
	m_f4Position = m_pMeshHiearchy->GetRoot()->GetLocPos();
	m_f4Rotation = m_pMeshHiearchy->GetRoot()->GetLocRot();
	m_mWorldMatrix = m_pMeshHiearchy->GetRoot()->GetWorld();

	mRotY = XMMatrixRotationY(XMConvertToRadians(m_f4Rotation.y));
	pVec = XMLoadFloat4(&m_f4Position);

	mTrans = XMMatrixTranslationFromVector(pVec);

	//Also calculate mPlaneCameraRot which ignores rotations in Z and X for the camera to parent to
	pVec = XMLoadFloat4(&m_f4CamOff);
	m_mCamWorldMatrix = XMMatrixTranslationFromVector(pVec) * mRotY * mTrans;
	
	m_vForwardVector = m_mWorldMatrix.r[2];
	
	pVec = XMLoadFloat4(&m_pMeshHiearchy->GetByName("gun")->GetLocPos());
	mTrans = XMMatrixTranslationFromVector(pVec);

	XMMATRIX mGunTutTrans = mTrans * m_pMeshHiearchy->GetByName("turret")->GetWorld();
	
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_pMeshHiearchy->GetByName("gun")->GetLocRot().y));

	// between parenting the camera to the plane(without X and Z rotations) and the gun based on m_bGunCam
	pVec = XMLoadFloat4(&m_f4CamOff);
	if (m_bGunCam)
		m_mCamWorldMatrix = XMMatrixTranslationFromVector(pVec) * mRotY * mGunTutTrans;

	// Get the camera's world position (m_vCamWorldPos) out of m_mCameraWorldMatrix
	m_vCamWorldPos = XMVectorZero();
	m_vCamWorldPos = XMVector3Transform(m_vCamWorldPos, m_mCamWorldMatrix);
}

void Aeroplane::Render(Graphics* graphics, Light* lights[])
{
	m_pMeshHiearchy->GetRoot()->Draw(graphics, lights);
	m_pBulletPool->Draw(graphics);
}
