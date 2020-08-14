////////////////////////////////////////////////////////////////////////////////
// Filename: OBB.h
////////////////////////////////////////////////////////////////////////////////
#ifndef OBB_H
#define OBB_H

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
#include "../Headers/AABB.h"
#include "../Headers/BoundingCapsule.h"
#include "../Headers/BoundingSphere.h"
#include "../Headers/BoundingVolume.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: OBB
////////////////////////////////////////////////////////////////////////////////
class OBB : public BoundingVolume
{
public:
	OBB(BVType bvType, bool is3Dim);
	OBB(XMFLOAT2 min, XMFLOAT2 max);
	OBB(XMFLOAT3 min, XMFLOAT3 max);
	OBB(const OBB& other);
	~OBB();

	void Update(XMFLOAT2 Pos, XMFLOAT2 Size, XMFLOAT2 rot);
	void Update(XMFLOAT3 Pos, XMFLOAT3 Size, XMFLOAT3 rot);

	void SetRotation2D(XMFLOAT2 rot);
	void SetRotation3D(XMFLOAT3 rot);

	XMFLOAT2 GetMin2D() { return XMFLOAT2(m_f3Min.x, m_f3Min.y); }
	XMFLOAT2 GetMax2D() { return XMFLOAT2(m_f3Max.x, m_f3Max.y); }
	XMFLOAT3 GetMin3D() { return m_f3Min; }
	XMFLOAT3 GetMax3D() { return m_f3Max; }
	vector<XMFLOAT3> GetCornerPoints() { return m_vf3CornerPoints; }
	vector<XMFLOAT3> GetLocalAxis() { return m_vf3LocalAxis; }

	void ClosestPtPointOBB2D(XMFLOAT2 p, XMFLOAT2& q);
	void ClosestPtPointOBB3D(XMFLOAT3 p, XMFLOAT3& q);

	bool IntersectPlane2D(Plane p);
	bool IntersectPlane3D(Plane p);

	bool IntersectOBB2D(OBB* other);
	bool IntersectOBB3D(OBB* other);

	bool Intersect2DSphere(BoundingSphere* sphere);
	bool Intersect2DSphere(BoundingSphere* sphere, XMFLOAT2& closestPnt);

	bool Intersect3DSphere(BoundingSphere* sphere);
	bool Intersect3DSphere(BoundingSphere* sphere, XMFLOAT3& closestPnt);
	
	bool IntersectMovingSphere2D(BoundingSphere s, XMFLOAT2 d, float& t);
	bool IntersectMovingSphere3D(BoundingSphere s, XMFLOAT3 d, float& t);

	bool IntersectMovingOBB2D(OBB* obb, XMFLOAT2 velA, XMFLOAT2 VelB, float& tfirst, float& tSecond);
	bool IntersectMovingOBB3D(OBB* obb, XMFLOAT3 velA, XMFLOAT3 VelB, float& tfirst, float& tSecond);

	bool IntersectOBB2D(LineSeg* lingSeg);
	bool IntersectOBB3D(LineSeg* lingSeg);

	bool IntersectRayOBBIntersection2D(XMFLOAT2 origin, XMFLOAT2 direction, float& tmin);
	bool IntersectRayOBBIntersection3D(XMFLOAT3 origin, XMFLOAT3 direction, float& tmin);
private:
	XMFLOAT2 Corner2D(int n);//
	XMFLOAT3 Corner3D(int n);//

	float SqDistPointOBB2D(XMFLOAT2 p);//
	float SqDistPointOBB3D(XMFLOAT3 p);//

	void Update(XMFLOAT2 Pos, XMFLOAT2 Size);
	void Update(XMFLOAT3 Pos, XMFLOAT3 Size);

	XMFLOAT3 m_f3Min;//
	XMFLOAT3 m_f3Max;//

	XMMATRIX m_ModelMAt;
	vector<XMFLOAT3> m_vf3CornerPoints;//
	vector<XMFLOAT3> m_vf3LocalAxis;//
};

#endif // !OBB_H