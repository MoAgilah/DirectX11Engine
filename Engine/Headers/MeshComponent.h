////////////////////////////////////////////////////////////////////////////////
// Filename: MeshComponent.h
////////////////////////////////////////////////////////////////////////////////
#ifndef MeshComponentH
#define MeshComponentH

//////////////
// INCLUDES //
//////////////
#include <string>
#include <vector>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/D3DMgr.h"
#include "../Headers/Utils.h"
#include "../Headers/Mesh.h"
#include "../Headers/Graphics.h"
#include "../Headers/Light.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: MeshComponent
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class MeshComponent
{
public:
	MeshComponent(ID3D11Device* device, Texture* texture, char* MeshFilename, float size, bool assimp, const XMFLOAT3& pos , const XMFLOAT3& rot);
	~MeshComponent();
	void Release();
	void AddChild(MeshComponent* child);
	XMFLOAT4& GetLocPos();
	XMFLOAT4& GetLocRot();
	XMMATRIX& GetWorld();
	Mesh* GetMesh();
	std::string GetName() const;
	bool GetHasChild();
	void updateMat(const XMMATRIX& world = XMMatrixIdentity());
	void Draw(Graphics* p_Graphics, Light* lights[]);
	bool GetHasAnim() const;
	void SetHasAnim();
	void DeleteChildren();
private:
	XMFLOAT4 m_f4LocalPos;
	XMFLOAT4 m_f4LocalRot;
	XMMATRIX m_mWorldMat;
	Mesh* m_pMesh;
	std::string m_sName;
	std::vector<MeshComponent*> m_vpChildren;
	bool m_bHasAnim;
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

#endif