////////////////////////////////////////////////////////////////////////////////
// Filename: BoundingVolume.h
////////////////////////////////////////////////////////////////////////////////
#ifndef BoundingVolume_H
#define BoundingVolume_H

#include "../Headers/Plane.h"
#include "../Headers/LineSeg.h"
#include "../Headers/Triangle.h"

/////////////////
// MY INCLUDES //
/////////////////
#include <DirectXMath.h>
using namespace DirectX;

enum class BVType
{
	AABB2D, AABB3D, OBB2D, OBB3D, Sphere2D, Sphere3D, Capsule2D, Capsule3D
};

////////////////////////////////////////////////////////////////////////////////
// Class name: BoundingVolume
////////////////////////////////////////////////////////////////////////////////
class BoundingVolume
{
protected:
	bool m_bIs3D;
	bool m_bIsCol;
	BVType m_bvType;
	XMFLOAT3 m_f3Center;
	XMFLOAT3 m_f3Size;
	XMFLOAT3 m_f3PrevSize;
	XMFLOAT3 m_f3HalfWidths;
	XMFLOAT3 m_f3Padding;
	XMFLOAT3 m_f3Percentile;
public:
	BoundingVolume(BVType bvType, bool is3D)
	{
		m_bvType = bvType;
		m_bIs3D = is3D;
		m_bIsCol = false;
		m_f3Center = XMFLOAT3(0.f, 0.f, 0.f);
		m_f3Size = XMFLOAT3(0.f, 0.f, 0.f);
		m_f3PrevSize = XMFLOAT3(-1.f, -1.f, -1.f);
		m_f3HalfWidths = XMFLOAT3(0.f, 0.f, 0.f);
		m_f3Percentile = XMFLOAT3(1.f, 1.f, 1.f);
		m_f3Padding = XMFLOAT3(0.f, 0.f, 0.f);
	}
	~BoundingVolume(){}

	virtual void Update(XMFLOAT2 Pos, XMFLOAT2 Size) = 0;
	virtual void Update(XMFLOAT3 Pos, XMFLOAT3 Size) = 0;
	
	BVType GetType() { return m_bvType; }
	bool GetIs3Dim() { return m_bIs3D; }
	bool SetIsCollided(bool col) { 
		m_bIsCol = col; return col; }
	bool GetIsCollided() { return m_bIsCol; }

	void Shrink(XMFLOAT3 percentile) { m_f3Percentile = percentile; }
	void SetPadding(XMFLOAT3 padding) { m_f3Padding = padding; }

	XMFLOAT2 GetCenter2D() { return XMFLOAT2(m_f3Center.x, m_f3Center.y); }
	XMFLOAT3 GetCenter3D() { return m_f3Center; }
	XMFLOAT2 GetSize2D() { return XMFLOAT2(m_f3Size.x, m_f3Size.y); }
	XMFLOAT3 GetSize3D() { return m_f3Size; }
	XMFLOAT2 GetExtent2D() { return XMFLOAT2(m_f3HalfWidths.x, m_f3HalfWidths.y); }
	XMFLOAT3 GetExtent3D() { return m_f3HalfWidths; }
};

#endif