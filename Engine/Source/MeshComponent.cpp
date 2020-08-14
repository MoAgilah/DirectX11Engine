////////////////////////////////////////////////////////////////////////////////
// Filename: MeshComponent.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/MeshComponent.h"

MeshComponent::MeshComponent(ID3D11Device* device, Texture* texture, char* MeshFilename, float size, bool assimp, const XMFLOAT3& pos, const XMFLOAT3& rot)
{
	m_mWorldMat = XMMatrixIdentity();
	m_f4LocalRot = XMFLOAT4(rot.x, rot.y, rot.z, 0.0f);
	m_f4LocalPos = XMFLOAT4(pos.x, pos.y, pos.z, 0.0f);

	m_sName = MeshFilename;

	//remove filepath
	m_sName = m_sName.substr(m_sName.find_last_of('/') + 1);

	//remove file extension
	m_sName = m_sName.substr(0, m_sName.find('.'));

	if (m_sName != "root")
	{
		m_pMesh = new Mesh;
		m_pMesh->Initialise(device, texture, MeshFilename, size);
	}
	else
		m_pMesh = nullptr;

	m_bHasAnim = false;
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Release()
{
	if (m_pMesh != nullptr)
	{
		SAFE_RELEASE(m_pMesh);
	}
}

void MeshComponent::AddChild(MeshComponent* child)
{
	m_vpChildren.push_back(child);
}

XMFLOAT4& MeshComponent::GetLocPos(void)
{
	return m_f4LocalPos;
}

XMFLOAT4& MeshComponent::GetLocRot(void)
{
	return m_f4LocalRot;
}

XMMATRIX& MeshComponent::GetWorld(void)
{
	return m_mWorldMat;
}

bool MeshComponent::GetHasChild(void)
{
	return m_vpChildren.empty();
}

Mesh* MeshComponent::GetMesh(void)
{
	return m_pMesh;
}

std::string MeshComponent::GetName(void) const
{
	return m_sName;
}

void MeshComponent::updateMat(const XMMATRIX& world)
{
	XMVECTOR pVec;
	XMMATRIX mRotX, mRotY, mRotZ, mTrans;

	mRotX = XMMatrixRotationX(XMConvertToRadians(GetLocRot().x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(GetLocRot().y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(GetLocRot().z));

	pVec = XMLoadFloat4(&GetLocPos());

	mTrans = XMMatrixTranslationFromVector(pVec);

	if (GetHasAnim())
	{
		m_mWorldMat = mRotX * mRotY * mRotZ * mTrans * world;
	}
	else
	{
		m_mWorldMat = mRotX * mRotZ * mRotY * mTrans * world;
	}

	

	if (!GetHasChild())
	{
		for (int i = 0; i < m_vpChildren.size(); i++)
		{
			m_vpChildren[i]->updateMat(m_mWorldMat);
		}
	}
}

bool MeshComponent::GetHasAnim(void) const
{
	return m_bHasAnim;
}

void MeshComponent::SetHasAnim()
{
	m_bHasAnim = true;
	if (!GetHasChild())
	{
		for (int i = 0; i < m_vpChildren.size(); ++i)
		{
			m_vpChildren[i]->SetHasAnim();
		}
	}
}

void MeshComponent::Draw(Graphics* p_Graphics, Light* lights[])
{
	XMFLOAT4 camPos = *p_Graphics->GetCamera()->GetPosition();
	
	if (m_pMesh != nullptr)
	{
		
		m_pMesh->Render(p_Graphics->GetD3DMgr()->GetDeviceContext());
		
		if (!p_Graphics->GetShaderMgr()->RenderLightShader(p_Graphics->GetD3DMgr()->GetDeviceContext(), m_pMesh->GetIndexCount(), GetWorld(), *p_Graphics->GetCamera()->GetViewMatrix(),
			*p_Graphics->GetD3DMgr()->GetProjectionMatrix(), m_pMesh->GetTexture(), XMFLOAT3(camPos.x, camPos.y, camPos.z),lights))
		{
			MessageBox(*p_Graphics->GetApplicationHandle(), "Failed to set shader parameters - failed to render LightShader", "Error", MB_OK);
		}
	}

	if (!m_vpChildren.empty())
	{
		for (int i = 0; i < m_vpChildren.size(); ++i)
		{
			m_vpChildren[i]->Draw(p_Graphics,lights);
		}
	}
}

void MeshComponent::DeleteChildren(void)
{
	for (int i = 0; i < m_vpChildren.size(); ++i)
	{
		if (m_vpChildren[i]->GetHasChild())
		{
			m_vpChildren[i]->DeleteChildren();
		}
		SAFE_RELEASE(m_vpChildren[i]);
	}
}