////////////////////////////////////////////////////////////////////////////////
// Filename: Static3DObj.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Static3DObj_H
#define Static3DObj_H

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Graphics.h"
#include "../Headers/GameObject3D.h"
#include "../Headers/Mesh.h"
#include "../Headers/Utils.h"

/////////////
// GLOBALS //
/////////////
enum BasicGeo
{
	Cube, Sphere
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Static3DObj
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class Static3DObj : public GameObject3D
{
public:
	Static3DObj(ID3D11Device* device, Texture* texture, char* MeshFilename, XMFLOAT3 pos, XMFLOAT3 rot, float size);
	Static3DObj(Mesh* mesh);
	Static3DObj(const Static3DObj& other);
	~Static3DObj();

	void UpdateMatrices();
	void Update(const float& dt);
	void Render(Graphics* graphics);
	Mesh* GetMesh();
private:
	Mesh* m_pMesh;
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

#endif // !Static3DObj_H
