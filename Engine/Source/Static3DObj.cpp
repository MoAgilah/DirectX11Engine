#include "../Headers/Static3DObj.h"

Static3DObj::Static3DObj(ID3D11Device* device, Texture* texture, char* MeshFilename, XMFLOAT3 pos, XMFLOAT3 rot, float size)
{
	m_pMesh = new Mesh(device, texture, MeshFilename, size);
	SetPosition(pos.x, pos.y, pos.z);
	SetRotation(rot.x, rot.y, rot.z);
}

Static3DObj::Static3DObj(Mesh* mesh)
{
	m_pMesh = mesh;

	SetPosition(0.f,0.f,0.f);
	SetRotation(0.f, 0.f, 0.f);
}

Static3DObj::Static3DObj(const Static3DObj& other)
{
	m_sName = other.m_sName;
	m_f4Position = other.m_f4Position;
	m_f4Rotation = other.m_f4Rotation;
	m_mWorldMatrix = other.m_mWorldMatrix;
	m_pMesh = other.m_pMesh;
}

Static3DObj::~Static3DObj()
{
	SAFE_RELEASE(m_pMesh);
}

void Static3DObj::UpdateMatrices()
{
	XMVECTOR pVec;
	XMMATRIX mRotX, mRotY, mRotZ, mTrans;

	mRotX = XMMatrixRotationX(XMConvertToRadians(m_f4Rotation.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_f4Rotation.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_f4Rotation.z));

	pVec = XMLoadFloat4(&m_f4Position);

	mTrans = XMMatrixTranslationFromVector(pVec);
	
	m_mWorldMatrix = mRotX * mRotZ * mRotY * mTrans;
}

void Static3DObj::Update(const float& dt)
{
	UpdateMatrices();
}

void Static3DObj::Render(Graphics* graphics)
{
	m_pMesh->Render(graphics->GetD3DMgr()->GetDeviceContext());
}

Mesh* Static3DObj::GetMesh()
{
	return m_pMesh;
}
