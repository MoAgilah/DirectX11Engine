////////////////////////////////////////////////////////////////////////////////
// Filename: Mesh.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Mesh_H
#define Mesh_H


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/TextureMgr.h"
#include "../Headers/Utils.h"

/////////////////////////
// MY LIBRARY INCLUDES //
/////////////////////////
#include "../assimp/Importer.hpp"
#include "../assimp/scene.h"
#include "../assimp/postprocess.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Mesh
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class Mesh
{
public:
	Mesh();
	Mesh(const Mesh& other);
	~Mesh();

	Mesh(ID3D11Device* device, vector<VertexTypeTexNorm> vertices, vector<unsigned int> indices, Texture* texture);
	Mesh(ID3D11Device* device, Texture* texture, char* MeshFilename, float size);
	bool Initialise(ID3D11Device* device, Texture* texture, char* MeshFilename, float size);

	void Release();

	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	XMMATRIX GetWorldMat();

	void SetTexture(Texture* texture);
	void SetWorldMat(XMMATRIX world);
	
private:
	bool InitialiseMesh(ID3D11Device* device);
	bool InitialiseMesh(ID3D11Device* device, aiNode* node, const float& size);

	const aiScene* m_pScene;
	vector<VertexTypeTexNorm> m_vVertices;
	vector<unsigned int> m_vIndices;
	ID3D11Buffer *m_pVertexBuffer, *m_pIndexBuffer;
	Texture* m_pTexture;
	XMMATRIX m_WorldMatrix;
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