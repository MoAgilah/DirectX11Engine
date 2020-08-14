////////////////////////////////////////////////////////////////////////////////
// Filename: Hiearchy.h
////////////////////////////////////////////////////////////////////////////////
#ifndef HiearchyH
#define HiearchyH

//////////////
// INCLUDES //
//////////////
#include <algorithm>
#include <fstream>
#include <sstream> 
#include <vector>
#include <string>
#include <map>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/MeshComponent.h"
#include "../Headers/D3DMgr.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Hiearchy
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class Hiearchy
{
public:
	Hiearchy(std::string fName, std::string hfName, const XMFLOAT3& iPos = XMFLOAT3(0, 0, 0), const XMFLOAT3& iRot = XMFLOAT3(0, 0, 0), const double& s = 1);
	~Hiearchy();
	MeshComponent* GetByName(const std::string& name);
	MeshComponent* GetRoot();
	std::map<std::string, MeshComponent*> GetHierarchy(void);
	void LoadFromFile(ID3D11Device* device, TextureMgr* texMgr);
	void Draw(ID3D11DeviceContext* deviceContext);
private:
	std::string fName, Root, hfName;
	XMFLOAT3 iP, iR;
	double sF;
	static MeshComponent* s_Root;
	std::map<std::string, MeshComponent*> hierachie;
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