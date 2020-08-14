////////////////////////////////////////////////////////////////////////////////
// Filename: BoundingSphere.h
////////////////////////////////////////////////////////////////////////////////
#ifndef BoundingSphere_H
#define BoundingSphere_H

/////////////////
// MY INCLUDES //
/////////////////
#include <DirectXMath.h>
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/BoundingVolume.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Classname: BoundingSpere.h
////////////////////////////////////////////////////////////////////////////////
class BoundingSphere : public BoundingVolume
{
public:
	BoundingSphere(BVType bvType, bool is3Dim);
	BoundingSphere(bool is3Dim, XMFLOAT2 center, float radius);
	BoundingSphere(bool is3Dim, XMFLOAT3 center, float radius);
	BoundingSphere(const BoundingSphere& other);
	~BoundingSphere();

	void Update(XMFLOAT2 Pos, XMFLOAT2 Size);
	void Update(XMFLOAT3 Pos, XMFLOAT3 Size);
	
	float GetRadius() { return m_fRadius; }
	
	bool Contains2DPoint(XMFLOAT2 point);
	bool Contains3DPoint(XMFLOAT3 point);

	bool IntersectTriangle2D(Triangle tri);
	bool IntersectTriangle3D(Triangle tri);

	bool IntersectSphere2D(BoundingSphere* sphere2);
	bool IntersectSphere3D(BoundingSphere* sphere2);

	bool IntersectMovingSphere2D(BoundingSphere* sphere2, XMFLOAT2 velA, XMFLOAT2 velB, float& t);
	bool IntersectMovingSphere3D(BoundingSphere* sphere2, XMFLOAT3 velA, XMFLOAT3 velB, float& t);

	bool IntersectPlane2D(Plane plane);
	bool IntersectPlane3D(Plane plane);

	bool IntersectMovingSphere2Plane2D(XMFLOAT2 v, Plane p, float& t, XMFLOAT2& q);
	bool IntersectMovingSphere2Plane3D(XMFLOAT3 v, Plane p, float& t, XMFLOAT3& q);

	bool IntersectLineSeg2D(LineSeg lineSeg, float& t, XMFLOAT2& q);
	bool IntersectLineSeg3D(LineSeg lineSeg, float& t, XMFLOAT3& q);

	bool IntersectRay2D(XMFLOAT2 p, XMFLOAT2 d, float& t, XMFLOAT2& q);
	bool IntersectRay3D(XMFLOAT3 p, XMFLOAT3 d, float& t, XMFLOAT3& q);

private:
	XMFLOAT2 ClosestPtPointTriangle(XMFLOAT2 pnt, Triangle tri);
	XMFLOAT3 ClosestPtPointTriangle(XMFLOAT3 pnt, Triangle tri);

private:
	float m_fRadius;
};

#endif 
