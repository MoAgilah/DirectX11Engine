////////////////////////////////////////////////////////////////////////////////
// Filename: Utils.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Utils_H
#define Utils_H

//////////////////
// MY INCLUDES  //
//////////////////
#include <directxmath.h>
using namespace DirectX;

const int g_iNUM_LIGHTS = 4;

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
#define SAFE_DELETE(a)  { if( (a) != NULL ) { delete (a); (a) = nullptr; } }
#define SAFE_DELETEA(a) { if( (a) != NULL ) { delete[] (a); (a) = nullptr; } }
#define SAFE_DELETEV(a){ if (!a.empty()) { int l = (int)a.size(); for (int i = 0; i < l; i++) SAFE_DELETE(a[i]); } }

enum TexType
{
	WIC,DDS,Targa
};

struct VertexTypeCol
{
	XMFLOAT3 position;
	XMFLOAT4 color;

	VertexTypeCol()
	{
		position = XMFLOAT3(0, 0, 0);
		color = XMFLOAT4(0, 0, 0, 1);
	}

	VertexTypeCol(XMFLOAT2 pos, XMFLOAT4 col)
	{
		position = XMFLOAT3(pos.x, pos.y, 1);
		color = col;
	}

	VertexTypeCol(XMFLOAT3 pos, XMFLOAT4 col)
	{
		position = pos;
		color = col;
	}
};

struct VertexTypeTexNorm
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;

	VertexTypeTexNorm()
	{
		position = XMFLOAT3(0, 0, 0);
		normal = XMFLOAT3(0, 0, 0);
		texture = XMFLOAT2(0, 0);
	}

	VertexTypeTexNorm(XMFLOAT2 pos, XMFLOAT2 tex, XMFLOAT2 norm)
	{
		position = XMFLOAT3(pos.x, pos.y, 1);
		texture = tex;
		normal = XMFLOAT3(norm.x, norm.y, 1);
	}

	VertexTypeTexNorm(XMFLOAT3 pos, XMFLOAT2 tex, XMFLOAT3 norm)
	{
		position = pos;
		texture = tex;
		normal = norm;
	}
};

struct VertexTypeTexNormCol
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
	XMFLOAT3 colour;

	VertexTypeTexNormCol()
	{
		position = XMFLOAT3(0, 0, 0);
		normal = XMFLOAT3(0, 0, 0);
		texture = XMFLOAT2(0, 0);
		colour = XMFLOAT3(0, 0, 0);
	}

	VertexTypeTexNormCol(XMFLOAT2 pos, XMFLOAT2 tex, XMFLOAT2 norm, XMFLOAT3 col)
	{
		position = XMFLOAT3(pos.x, pos.y, 1);
		texture = tex;
		normal = XMFLOAT3(norm.x, norm.y, 1);
		colour = XMFLOAT3(col.x, col.y, col.z);
	}

	VertexTypeTexNormCol(XMFLOAT3 pos, XMFLOAT2 tex, XMFLOAT3 norm, XMFLOAT3 col)
	{
		position = pos;
		texture = tex;
		normal = norm;
		colour = col;
	}
};

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct FRECT
{
	float    left;
	float    top;
	float    right;
	float    bottom;
};

#endif // !Utils_h

