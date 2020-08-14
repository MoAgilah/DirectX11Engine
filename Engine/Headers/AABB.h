////////////////////////////////////////////////////////////////////////////////
// Filename: AABB.h
////////////////////////////////////////////////////////////////////////////////
#ifndef AABB_H
#define AABB_H

/////////////////
// MY INCLUDES //
/////////////////
#include <DirectXMath.h>
#include <vector>
#include <algorithm>
using namespace std;
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/BoundingVolume.h"
#include "../Headers/BoundingSphere.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: AABB
////////////////////////////////////////////////////////////////////////////////
class AABB : public BoundingVolume
{
public:
	AABB(BVType bvType, bool is3Dim);
	AABB(XMFLOAT2 min, XMFLOAT2 max);
	AABB(XMFLOAT3 min, XMFLOAT3 max);
	AABB(const AABB& other);
	~AABB();

	void Update(XMFLOAT2 Pos, XMFLOAT2 Size);
	void Update(XMFLOAT3 Pos, XMFLOAT3 Size);

	void Expand2D(float radius);
	void Expand3D(float radius);

	XMFLOAT2 GetMin2D() { return XMFLOAT2(m_f3Min.x, m_f3Min.y); }
	XMFLOAT2 GetMax2D() { return XMFLOAT2(m_f3Max.x, m_f3Max.y); }
	XMFLOAT3 GetMin3D() { return m_f3Min; }
	XMFLOAT3 GetMax3D() { return m_f3Max; }
	vector<XMFLOAT3> GetCornerPoints() { return m_vf3CornerPoints; }

public:
	bool Contains2DPoint(XMFLOAT2 pnt);
	bool Contains3DPoint(XMFLOAT3 pnt);

	bool IntersectLineSeg2D(LineSeg* lingSeg);
	bool IntersectLineSeg3D(LineSeg* lingSeg);

	bool IntersectRayAABB2D(XMFLOAT2 p, XMFLOAT2 d, float& tmin, XMFLOAT2& q);
	bool IntersectRayAABB3D(XMFLOAT3 p, XMFLOAT3 d, float& tmin, XMFLOAT3& q);

	bool Intersect2DPlane(Plane plane);
	bool IntersectMovingAABB2Plane2D(XMFLOAT2 v, Plane p, float& t, XMFLOAT2& q);

	bool Intersect3DPlane(Plane plane);
	bool IntersectMovingAABB2Plane3D(XMFLOAT3 v, Plane p, float& t, XMFLOAT3& q);

	bool Intersect2DTriangle(Triangle tri);
	bool Intersect3DTriangle(Triangle tri);

	bool Intersect2DAABB(AABB* AABB2D_2);
	bool IntersectMovingAABB2D(AABB* aabb, XMFLOAT2 velA, XMFLOAT2 VelB, float& tfirst, float& tSecond);

	bool Intersect3DAABB(AABB* AABB2D_2);
	bool IntersectMovingAABB3D(AABB* aabb, XMFLOAT3 velA, XMFLOAT3 VelB, float& tfirst, float& tSecond);

	bool Intersect2DSphere(BoundingSphere* sphere);
	bool Intersect2DSphere(BoundingSphere* sphere, XMFLOAT2& closestPnt);
	bool IntersectMovingSphere2D(BoundingSphere* sphere, XMFLOAT2 SphDirVec, float& t);
	
	bool Intersect3DSphere(BoundingSphere* sphere);
	bool Intersect3DSphere(BoundingSphere* sphere, XMFLOAT3& closestPnt);
	bool IntersectMovingSphere3D(BoundingSphere* sphere, XMFLOAT3 SphDirVec, float& t);
		
private:
	XMFLOAT2 Corner2D(int n);
	XMFLOAT3 Corner3D(int n);

	float Max3(float a, float b, float c);
	float Min3(float a, float b, float c);

	bool TestAxis2D(XMFLOAT2 u, XMFLOAT2 f, XMFLOAT2 locAxis[2], XMFLOAT2 v[3]);
	bool TestAxis3D(XMFLOAT3 u, XMFLOAT3 f, XMFLOAT3 locAxis[3], XMFLOAT3 v[3]);

	bool TestAxis3D(XMFLOAT3 v[3], XMFLOAT3 axis);


	float SqDistPointAABB2D(XMFLOAT2 point);
	float SqDistPointAABB3D(XMFLOAT3 point);

	XMFLOAT2 ClosestPtPointAABB2D(XMFLOAT2 pnt);
	XMFLOAT3 ClosestPtPointAABB3D(XMFLOAT3 pnt);

private:
	XMFLOAT3 m_f3Min;
	XMFLOAT3 m_f3Max;
	vector<XMFLOAT3> m_vf3CornerPoints;
};

#endif // !AABB_H