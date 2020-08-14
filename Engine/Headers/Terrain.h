////////////////////////////////////////////////////////////////////////////////
// Filename: Terrain.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Terrain_H_
#define _Terrain_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <stdio.h>
using namespace std;
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Texture.h"
#include "../Headers/Utils.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Terrain
////////////////////////////////////////////////////////////////////////////////
class Terrain
{
private:
	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
		//float r, g, b;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		//float r, g, b;
	};

	struct VectorType
	{
		float x, y, z;
	};
public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Initialize(ID3D11Device* device , Texture* texture, char* setupFilename);
	void Release();
	bool Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	void AssignTexture(Texture* texture);
	ID3D11ShaderResourceView* GetTexture();
private:
	bool LoadSetupFile(char*);
	bool LoadBitmapHeightMap();
	void SetTerrainCoordinates();
	bool CalculateNormals();
	bool LoadColourMap();
	bool BuildTerrainModel();
	bool InitializeBuffers(ID3D11Device*);
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_pVertexBuffer, *m_pIndexBuffer;
	int m_iVertexCount, m_iIndexCount;
	int m_iTerrainHeight, m_iTerrainWidth;
	float m_fHeightScale;
	char* m_cpTerrainFilename, *m_cpColourMapFilename;
	HeightMapType* m_pHeightMap;
	ModelType* m_pTerrainModel;
	Texture* m_pTexture;
};

#endif