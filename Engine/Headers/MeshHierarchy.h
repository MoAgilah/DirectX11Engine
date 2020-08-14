////////////////////////////////////////////////////////////////////////////////
// Filename: MeshHierarchy.h
////////////////////////////////////////////////////////////////////////////////
#ifndef MeshHierarchy_H
#define MeshHierarchy_H

//////////////
// INCLUDES //
//////////////
#include <string>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/MeshComponent.h"
#include "../Headers/Graphics.h"
#include "../Headers/Light.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: MeshHierarchy
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class MeshHierarchy
{
public:
	MeshHierarchy();
	MeshHierarchy(ID3D11Device* device, Texture* texture, char* hierarchyName, char* filename, const XMFLOAT3& initialPos, const XMFLOAT3& initialRot, const float& size, bool assimp, const char* fileExt);
	~MeshHierarchy();
	void Initialise(ID3D11Device* device, Texture* texture, char* hierarchyName, char* filename, const XMFLOAT3& initialPos, const XMFLOAT3& initialRot, const float& size );
	MeshComponent* GetByName(const string& name);
	MeshComponent* GetRoot();
	std::map<std::string, MeshComponent*> GetHierarchy();
	
	void Render(Graphics* graphics, Light* lights[]);
private:
	bool LoadFromFile(ID3D11Device* device, Texture* texture, char* filename);

	string m_sHierarchyName;
	XMFLOAT3 m_f3InitialPosition;
	XMFLOAT3 m_f3InitialRotation;
	float m_fSize;
	bool m_bAssimp;
	string m_sFileExt;
	MeshComponent* m_pRootMesh;
	map <string, MeshComponent*> m_mHierachyMap;
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

#endif // !Hierarchy_H

