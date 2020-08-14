#include "../Headers/MeshHierarchy.h"

bool isUneededChar(char c)
{
	return (c == '\"' || c == ',');
}

MeshHierarchy::MeshHierarchy()
{
}

MeshHierarchy::MeshHierarchy(ID3D11Device* device, Texture* texture, char* hierarchyname, char* filename, const XMFLOAT3& initialPos, const XMFLOAT3& initialRot, const float& size, bool assimp, const char* fileExt)
	:m_f3InitialPosition(initialPos), m_f3InitialRotation(initialRot), m_fSize(size),m_sHierarchyName(hierarchyname),m_bAssimp(assimp), m_sFileExt(fileExt)
{
	LoadFromFile(device, texture, filename);
}

MeshHierarchy::~MeshHierarchy()
{
	for (std::map<std::string, MeshComponent*>::iterator it = m_mHierachyMap.begin(); it != m_mHierachyMap.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
}

void MeshHierarchy::Initialise(ID3D11Device* device, Texture* texture, char* hierarchyName, char* filename, const XMFLOAT3& initialPos, const XMFLOAT3& initialRot, const float& size)
{
	m_f3InitialPosition = initialPos;
	m_f3InitialRotation = initialRot;
	m_fSize = size;
	m_sHierarchyName = hierarchyName;

	LoadFromFile(device, texture, filename);
}

MeshComponent* MeshHierarchy::GetByName(const string& name)
{
	auto it = m_mHierachyMap.find(name);
	if (it == m_mHierachyMap.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

MeshComponent* MeshHierarchy::GetRoot()
{
	return m_pRootMesh;
}

std::map<std::string, MeshComponent*> MeshHierarchy::GetHierarchy()
{
	return m_mHierachyMap;
}

bool MeshHierarchy::LoadFromFile(ID3D11Device* device, Texture* texture, char* filename)
{
	string currLine;
	string line;
	vector<string> lines;
	float posX, posY, posZ;
	ifstream in;

	in.open(filename, fstream::out);

	if (in.fail())
	{
		return false;
	}

	while (getline(in, line)) {
		line.erase(remove_if(line.begin(), line.end(), isUneededChar), line.end());
		lines.push_back(line);
	}

	in.close();

	stringstream pos(lines[2]);
	pos >> posX;	pos >> posY;	pos >> posZ;
	posX /= m_fSize;	posY /= m_fSize;	posZ /= m_fSize;
	string filepath = "";

	if (m_bAssimp)
	{
		filepath = "Resources/Models/" + m_sHierarchyName + "/" + lines[0] + m_sFileExt;
	}
	else
	{
		filepath = "Resources/Models/" + m_sHierarchyName + "/" + lines[0] + ".txt";
	}

	
	m_pRootMesh = new MeshComponent(device, texture, (char*)filepath.c_str(), m_fSize, m_bAssimp, XMFLOAT3(posX + m_f3InitialPosition.x, posY + m_f3InitialPosition.y, posZ + m_f3InitialPosition.z), m_f3InitialRotation);
	m_mHierachyMap.insert(std::pair<string, MeshComponent*>(lines[0], m_pRootMesh));

	//extract information for the hierarchy
	for (int i = 3; i < lines.size(); i += 3)
	{
		stringstream pos(lines[i + 2]);
		pos >> posX;	pos >> posY;	pos >> posZ;
		posX /= m_fSize;	posY /= m_fSize;	posZ /= m_fSize;

		if (m_bAssimp)
		{
			filepath = "Resources/Models/" + m_sHierarchyName + "/" + lines[i] + m_sFileExt;
		}
		else
		{
			filepath = "Resources/Models/" + m_sHierarchyName + "/" + lines[i] + ".txt";
		}
		
		m_mHierachyMap.insert(std::pair<string, MeshComponent*>(lines[i], new MeshComponent(device, texture, (char*)filepath.c_str(), m_fSize, m_bAssimp, XMFLOAT3(posX, posY, posZ), XMFLOAT3(0, 0, 0))));
	}

	//create Hierarchy from newly created mesh components
	for (int i = 3; i < lines.size(); i += 3)
	{
		if (lines[i + 1] != "")
		{
			GetByName(lines[i + 1])->AddChild(GetByName(lines[i]));
		}
	}

	return true;
}

void MeshHierarchy::Render(Graphics* graphics, Light* lights[])
{
	m_pRootMesh->Draw(graphics,lights);
}
