#include "../Headers/Hierarchal3DObj.h"

Hiearchal3DObj::Hiearchal3DObj(ID3D11Device* device, Texture* texture, char* hierarchyName, char* filename, const XMFLOAT3& initialPos, const XMFLOAT3& initialRot, const float& size, bool assimp, const char* fileExt)
{
	m_pMeshHiearchy = new MeshHierarchy(device, texture, hierarchyName, filename, initialPos, initialRot, size, assimp, fileExt);
	
	SetPosition(GetRoot()->GetLocPos());
	SetRotation(GetRoot()->GetLocRot());

}

Hiearchal3DObj::Hiearchal3DObj(const Hiearchal3DObj& other)
{
}

Hiearchal3DObj::~Hiearchal3DObj()
{
	SAFE_DELETE(m_pMeshHiearchy);
}

void Hiearchal3DObj::UpdateMatrices()
{
	GetRoot()->updateMat();
}

void Hiearchal3DObj::Update(const float& dt)
{
	UpdateMatrices();
	m_mWorldMatrix = GetRoot()->GetWorld();
}

void Hiearchal3DObj::Render(Graphics* graphics, Light* lights[])
{
	m_pMeshHiearchy->Render(graphics, lights);
}

MeshComponent* Hiearchal3DObj::GetRoot()
{
	return m_pMeshHiearchy->GetRoot();
}
