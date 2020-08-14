////////////////////////////////////////////////////////////////////////////////
// Filename: Hiearchal3DObj.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Hiearchal3DObj_H
#define Hiearchal3DObj_H

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Graphics.h"
#include "../Headers/GameObject3D.h"
#include "../Headers/MeshHierarchy.h"
#include "../Headers/Light.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Hiearchal3DObj
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class Hiearchal3DObj : public GameObject3D
{
public:
	Hiearchal3DObj(ID3D11Device* device, Texture* texture, char* hierarchyName, char* filename, const XMFLOAT3& initialPos, const XMFLOAT3& initialRot, const float& size, bool assimp, const char* fileExt);
	Hiearchal3DObj(const Hiearchal3DObj& other);
	~Hiearchal3DObj();

	virtual void UpdateMatrices();
	virtual void Update(const float& dt);
	void Render(Graphics* MeshComponent, Light* lights[]);
	MeshComponent* GetRoot();
protected:
	MeshHierarchy* m_pMeshHiearchy;
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

#endif // !Hiearchal3DObj_H
