////////////////////////////////////////////////////////////////////////////////
// Filename: LineSeg.h
////////////////////////////////////////////////////////////////////////////////
#ifndef LineSeg_H
#define LineSeg_H

/////////////////
// MY INCLUDES //
/////////////////
#include <DirectXMath.h>
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Plane.h"

class AABB;
class OBB;
class BoundingSphere;
class BoundingCapsule;
////////////////////////////////////////////////////////////////////////////////
// Class name: LineSeg
////////////////////////////////////////////////////////////////////////////////
class LineSeg
{
public:
	LineSeg();
	LineSeg(XMFLOAT2 bot, XMFLOAT2 top);
	LineSeg(XMFLOAT3 bot, XMFLOAT3 top);
	~LineSeg();

	XMFLOAT2 GetCenter2D() { return XMFLOAT2(m_f3Center.x, m_f3Center.y); }
	void SetCenter2D(XMFLOAT2 cent) { m_f3Center = XMFLOAT3(cent.x, cent.y, 1.f); };

	XMFLOAT3 GetCenter3D() { return m_f3Center; }
	void SetCenter3D(XMFLOAT3 cent) { m_f3Center = cent; };
			 
	XMFLOAT2 GetBottom2D() { return XMFLOAT2(m_f3Bottom.x, m_f3Bottom.y); }
	void SetBottom2D(XMFLOAT2 bot) { m_f3Bottom = XMFLOAT3(bot.x, bot.y, 1.f); };

	XMFLOAT3 GetBottom3D() { return m_f3Bottom; }
	void SetBottom3D(XMFLOAT3 bot) { m_f3Bottom = bot; };
			 
	XMFLOAT2 GetTop2D() { return XMFLOAT2(m_f3Top.x, m_f3Top.y); }
	void SetTop2D(XMFLOAT2 top) { m_f3Top = XMFLOAT3(top.x, top.y, 1.f); };

	XMFLOAT3 GetTop3D() { return m_f3Top; }
	void SetTop3D(XMFLOAT3 top) { m_f3Top = top; };

public:
	bool IntersectLineSeg2D(LineSeg lineSeg, float& t, XMFLOAT2 pnt);
	//intersection of 3d line segment rarely collide

	bool IntersectOBB2D(OBB* obb);
	bool IntersectOBB3D(OBB* obb);

	bool IntersectBoundingSphere2D(BoundingSphere* sphere);
	bool IntersectBoundingSphere3D(BoundingSphere* sphere);

	bool IntersectAABB2D(AABB* aabb);
	bool IntersectAABB3D(AABB* aabb);

	bool IntersectPlane2D(Plane plane, float& t, XMFLOAT2 q);
	bool IntersectPlane2D(XMFLOAT2 a, XMFLOAT2 b, XMFLOAT2 c, float& t, XMFLOAT2 q);

	bool IntersectPlane3D(Plane plane, float& t, XMFLOAT3 q);
	bool IntersectPlane3D(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, float& t, XMFLOAT3 q);

	bool IntersectCapsule2D(BoundingCapsule capsule, float& t);
	bool IntersectCapsule3D(BoundingCapsule capsule, float& t);

	void ClosestPtPointSegment2D(XMFLOAT2 pnt, float& t, XMFLOAT2& c);
	void ClosestPtPointSegment3D(XMFLOAT3 pnt, float& t, XMFLOAT3& c);

	float SquareDistPointSegment2D(XMFLOAT2 point);
	float SquareDistPointSegment3D(XMFLOAT3 point);

	float ClosestPtSegmentSegment2D(LineSeg lineSeg, float& s, float& t, XMFLOAT2& c1, XMFLOAT2& c2);
	float ClosestPtSegmentSegment3D(LineSeg lineSeg, float& s, float& t, XMFLOAT3& c1, XMFLOAT3& c2);

private:
	float Clamp(float n, float min, float max);
	float Signed2DTriArea(XMFLOAT2 pnt1, XMFLOAT2 pnt2, XMFLOAT2 pnt3);

	XMFLOAT3 m_f3Center;
	XMFLOAT3 m_f3Bottom;
	XMFLOAT3 m_f3Top;
};

#endif // !LineSeg_H
