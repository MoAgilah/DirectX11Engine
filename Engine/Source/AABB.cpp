#include "../Headers/AABB.h"
#include "../Headers/BoundingCapsule.h"

AABB::AABB(BVType bvType, bool is3Dim)
	:BoundingVolume(bvType, is3Dim)
{
	m_f3Min = XMFLOAT3(0.f, 0.f, 0.f);
	m_f3Max = XMFLOAT3(0.f, 0.f, 0.f);

	if (is3Dim) m_vf3CornerPoints.resize(8);
	else m_vf3CornerPoints.resize(4);
}

AABB::AABB(XMFLOAT2 min, XMFLOAT2 max)
	:BoundingVolume(BVType::AABB2D, false)
{
	m_vf3CornerPoints.resize(4);
	m_f3Min = m_vf3CornerPoints[0] = XMFLOAT3(min.x,min.y, 1.f);
	m_f3Max = m_vf3CornerPoints[2] = XMFLOAT3(max.x, min.y, 1.f);

	m_f3Center = XMFLOAT3((m_f3Min.x + m_f3Max.x) * 0.5f, (m_f3Min.y + m_f3Max.y) * 0.5f, 1.f);
	m_vf3CornerPoints[1] = XMFLOAT3(m_f3Min.x, m_f3Max.y, 1.f);
	m_vf3CornerPoints[3] = XMFLOAT3(m_f3Max.x, m_f3Min.y, 1.f);
}

AABB::AABB(XMFLOAT3 min, XMFLOAT3 max)
	: BoundingVolume(BVType::AABB3D, true)
{
	m_vf3CornerPoints.resize(8);

	m_f3Min = m_vf3CornerPoints[0] = min;
	m_f3Max = m_vf3CornerPoints[1] = max;

	m_f3Center = XMFLOAT3((m_f3Min.x + m_f3Max.x) * 0.5f, (m_f3Min.y + m_f3Max.y) * 0.5f, (m_f3Min.z + m_f3Max.z) * 0.5f);
	m_vf3CornerPoints[2] = XMFLOAT3(m_f3Min.x, m_f3Max.y, m_f3Min.z);
	m_vf3CornerPoints[3] = XMFLOAT3(m_f3Min.x, m_f3Min.y, m_f3Max.z);
	m_vf3CornerPoints[4] = XMFLOAT3(m_f3Min.x, m_f3Max.y, m_f3Max.z);
	m_vf3CornerPoints[5] = XMFLOAT3(m_f3Max.x, m_f3Min.y, m_f3Min.z);
	m_vf3CornerPoints[6] = XMFLOAT3(m_f3Max.x, m_f3Max.y, m_f3Min.z);
	m_vf3CornerPoints[7] = XMFLOAT3(m_f3Max.x, m_f3Min.y, m_f3Max.z);
}

AABB::AABB(const AABB& other)
	:BoundingVolume(other.m_bvType, other.m_bIsCol)
{
	m_f3Center = m_f3Center;
	m_f3Size = other.m_f3Size;
	m_f3PrevSize = other.m_f3PrevSize;
	m_f3HalfWidths = other.m_f3HalfWidths;
	m_f3Padding = other.m_f3Padding;
	m_f3Percentile = other.m_f3Percentile;
	m_f3Min = other.m_f3Min;
	m_f3Max = other.m_f3Max;
	m_vf3CornerPoints = other.m_vf3CornerPoints;
}

AABB::~AABB()
{
	
}

void AABB::Update(XMFLOAT2 Pos, XMFLOAT2 Size)
{
	//set size and halfwidths 
	m_f3Size = XMFLOAT3(Size.x * m_f3Percentile.x, Size.y * m_f3Percentile.y, 1.f);
	m_f3HalfWidths = XMFLOAT3(m_f3Size.x * 0.5f, m_f3Size.y * 0.5f, 1.f);

	XMMATRIX trans = DirectX::XMMatrixTranslationFromVector(XMLoadFloat2(&XMFLOAT2(Pos.x, Pos.y)));
	trans.r[3].m128_f32[0];
	XMFLOAT3X3 mat;
	XMStoreFloat3x3(&mat, trans);

	

	//set min, max and center
	m_f3Min = m_vf3CornerPoints[0] = XMFLOAT3(trans.r[3].m128_f32[0] + m_f3Padding.x, trans.r[3].m128_f32[1] + m_f3Padding.y, 1.f);
	m_f3Center = XMFLOAT3(m_f3Min.x + m_f3HalfWidths.x, m_f3Min.y + m_f3HalfWidths.y, 1.f);
	m_f3Max = m_vf3CornerPoints[2] = XMFLOAT3(m_f3Min.x + m_f3Size.x, m_f3Min.y + m_f3Size.y, 1.f);
	
	//set remaining corners using min and max
	m_vf3CornerPoints[1] = XMFLOAT3(m_f3Min.x, m_f3Max.y, 1.f);
	m_vf3CornerPoints[3] = XMFLOAT3(m_f3Max.x, m_f3Min.y, 1.f);
}

void AABB::Update(XMFLOAT3 Pos, XMFLOAT3 Size)
{
	//set size and halfwidths 
	m_f3Size = XMFLOAT3(Size.x * m_f3Percentile.x, Size.y * m_f3Percentile.y, Size.z * m_f3Percentile.z);
	m_f3HalfWidths = XMFLOAT3(m_f3Size.x * 0.5f, m_f3Size.y * 0.5f, m_f3Size.z * 0.5f);
	
	//set min, max and center
	m_f3Min = m_vf3CornerPoints[0] = XMFLOAT3(Pos.x + m_f3Padding.x, Pos.y + m_f3Padding.y, Pos.z + m_f3Padding.z);
	m_f3Center = XMFLOAT3(m_f3Min.x + m_f3HalfWidths.x, m_f3Min.y + m_f3HalfWidths.y, m_f3Min.z + m_f3HalfWidths.z);
	m_f3Max = m_vf3CornerPoints[1] = XMFLOAT3(m_f3Min.x + m_f3Size.x, m_f3Min.y + m_f3Size.y, Pos.z + m_f3Size.z);

	//set remaining corners using min and max
	m_vf3CornerPoints[2] = XMFLOAT3(m_f3Min.x, m_f3Max.y, m_f3Min.z);
	m_vf3CornerPoints[3] = XMFLOAT3(m_f3Min.x, m_f3Min.y, m_f3Max.z);
	m_vf3CornerPoints[4] = XMFLOAT3(m_f3Min.x, m_f3Max.y, m_f3Max.z);
	m_vf3CornerPoints[5] = XMFLOAT3(m_f3Max.x, m_f3Min.y, m_f3Min.z);
	m_vf3CornerPoints[6] = XMFLOAT3(m_f3Max.x, m_f3Max.y, m_f3Min.z);
	m_vf3CornerPoints[7] = XMFLOAT3(m_f3Max.x, m_f3Min.y, m_f3Max.z);
}

void AABB::Expand2D(float radius)
{
	m_f3Min = XMFLOAT3(m_f3Min.x - radius, m_f3Min.y - radius, 1.f);
	m_f3Max = XMFLOAT3(m_f3Max.x + radius, m_f3Max.y + radius, 1.f);
}

void AABB::Expand3D(float radius)
{
	m_f3Min = XMFLOAT3(m_f3Min.x - radius, m_f3Min.y - radius, m_f3Min.z - radius);
	m_f3Max = XMFLOAT3(m_f3Max.x + radius, m_f3Max.y + radius, m_f3Max.z + radius);
}

bool AABB::Contains2DPoint(XMFLOAT2 pnt)
{ 
	return SetIsCollided(SqDistPointAABB2D(pnt) == 0); 
}

bool AABB::Contains3DPoint(XMFLOAT3 pnt) 
{ 
	return SetIsCollided(SqDistPointAABB3D(pnt) == 0); 
}

bool AABB::Intersect2DPlane(Plane plane)
{
	//compute the projection interval radius of the aabb onto L(t) = aabb.c + t * p.n
	float r = m_f3HalfWidths.x * abs(plane.Get2DNorm().x) + m_f3HalfWidths.y * abs(plane.Get2DNorm().y);

	//compute the square distance from the aabb center from plane
	float sqDist = *XMVector2Dot(XMLoadFloat2(&plane.Get2DNorm()), XMLoadFloat2(&GetCenter2D())).m128_f32 - plane.GetDistance();

	//intersection occurs when sqDist falls within [-r,+r] interval
	return SetIsCollided(abs(sqDist) <= r);
}

bool AABB::Intersect3DPlane(Plane plane)
{
	//compute the projection interval radius of the aabb onto L(t) = aabb.c + t * p.n
	float r = m_f3HalfWidths.x * abs(plane.Get3DNorm().x) + m_f3HalfWidths.y * abs(plane.Get3DNorm().y) + m_f3HalfWidths.z * abs(plane.Get3DNorm().z);

	//compute the square distance from the aabb center from plane
	float sqDist = *XMVector3Dot(XMLoadFloat3(&plane.Get3DNorm()), XMLoadFloat3(&GetCenter3D())).m128_f32 - plane.GetDistance();

	//intersection occurs when sqDist falls within [-r,+r] interval
	return abs(sqDist) <= r;
}

bool AABB::IntersectMovingAABB2Plane2D(XMFLOAT2 v, Plane p, float& t, XMFLOAT2& q)
{
	//compute the projected radius r = e0|nx| + e1|ny|
	float r = m_f3HalfWidths.x * p.Get2DNorm().x + m_f3HalfWidths.y * p.Get2DNorm().y;

	//then it becomes moving sphere to plane check
	float dist = *XMVector2Dot(XMLoadFloat2(&p.Get2DNorm()), XMLoadFloat2(&GetCenter2D())).m128_f32 - p.GetDistance();

	if (abs(dist) <= r)
	{
		//the box is already overlapping the plane, set the time of intersection to zero
		//and q to box center
		t = 0.f;	
		q = GetCenter2D();
		return true;
	}
	else
	{
		float denom = *XMVector2Dot(XMLoadFloat2(&p.Get2DNorm()), XMLoadFloat2(&v)).m128_f32;
		if (denom >= 0) return false;
		else
		{
			t = (r + dist) / denom;
			// there will not necessarily be a unique point of contact, 
			//the contact region(q) might be an edge or a face of the AABB
			//q = box.center + t * v - r * plane.normal	 
			XMStoreFloat2(&q, XMLoadFloat2(&GetCenter2D()) + t * XMLoadFloat2(&v) - r * XMLoadFloat2(&p.Get2DNorm()));
			return true;
		}
	}

	return false;
}

bool AABB::IntersectMovingAABB2Plane3D(XMFLOAT3 v, Plane p, float& t, XMFLOAT3& q)
{
	//compute the projected radius r = e0|nx| + e1|ny|
	float r = m_f3HalfWidths.x * p.Get3DNorm().x + m_f3HalfWidths.y * p.Get3DNorm().y + m_f3HalfWidths.z * p.Get3DNorm().z;
	//then it becomes moving sphere to plane check
	float dist = *XMVector3Dot(XMLoadFloat3(&p.Get3DNorm()), XMLoadFloat3(&GetCenter3D())).m128_f32 - p.GetDistance();

	if (abs(dist) <= r)
	{
		//the box is already overlapping the plane, set the time of intersection to zero
		//and q to box center
		t = 0.f;
		q = GetCenter3D();
		return true;
	}
	else
	{
		float denom = *XMVector3Dot(XMLoadFloat3(&p.Get3DNorm()), XMLoadFloat3(&v)).m128_f32;
		if (denom >= 0) return false;
		else
		{
			t = (r + dist) / denom;

			// there will not necessarily be a unique point of contact, 
			//the contact region(q) might be an edge or a face of the AABB
			//q = box.center + t * v - r * plane.normal
			XMStoreFloat3(&q, XMLoadFloat3(&GetCenter3D()) + t * XMLoadFloat3(&v) - r * XMLoadFloat2(&p.Get2DNorm()));
			return true;
		}
	}

	return false;
}

bool AABB::Intersect2DTriangle(Triangle tri)
{
	
	if (Contains2DPoint(tri.Get2DPntByIdx(0))) return SetIsCollided(true);
	if (Contains2DPoint(tri.Get2DPntByIdx(1))) return SetIsCollided(true);
	if (Contains2DPoint(tri.Get2DPntByIdx(2))) return SetIsCollided(true);

	//closest point box triangle to optimise
	if (tri.Contains(XMFLOAT2(m_vf3CornerPoints[0].x, m_vf3CornerPoints[0].y))) return SetIsCollided(true);
	if (tri.Contains(XMFLOAT2(m_vf3CornerPoints[1].x, m_vf3CornerPoints[1].y))) return SetIsCollided(true);
	if (tri.Contains(XMFLOAT2(m_vf3CornerPoints[2].x, m_vf3CornerPoints[2].y))) return SetIsCollided(true);
	if (tri.Contains(XMFLOAT2(m_vf3CornerPoints[3].x, m_vf3CornerPoints[3].y))) return SetIsCollided(true);

	if (IntersectLineSeg2D(&LineSeg(tri.Get2DPntByIdx(0), tri.Get2DPntByIdx(1)))) return SetIsCollided(true);
	if (IntersectLineSeg2D(&LineSeg(tri.Get2DPntByIdx(1), tri.Get2DPntByIdx(2)))) return SetIsCollided(true);
	if (IntersectLineSeg2D(&LineSeg(tri.Get2DPntByIdx(2), tri.Get2DPntByIdx(0)))) return SetIsCollided(true);

	return SetIsCollided(false);
}

bool AABB::Intersect3DTriangle(Triangle tri)
{
	XMFLOAT3 exts = GetExtent3D();
	XMVECTOR c = XMLoadFloat3(&GetCenter3D());

	//translate triangle as conceptually moving aabb to origin
	XMFLOAT3 v[3];
	XMStoreFloat3(&v[0], XMVectorSubtract(XMLoadFloat3(&tri.Get3DPntByIdx(0)), c));
	XMStoreFloat3(&v[1], XMVectorSubtract(XMLoadFloat3(&tri.Get3DPntByIdx(1)), c));
	XMStoreFloat3(&v[2], XMVectorSubtract(XMLoadFloat3(&tri.Get3DPntByIdx(2)), c));

	XMFLOAT3 f[3];
	XMStoreFloat3(&f[0], XMVectorSubtract(XMLoadFloat3(&v[1]), XMLoadFloat3(&v[0])));
	XMStoreFloat3(&f[1], XMVectorSubtract(XMLoadFloat3(&v[2]), XMLoadFloat3(&v[1])));
	XMStoreFloat3(&f[2], XMVectorSubtract(XMLoadFloat3(&v[0]), XMLoadFloat3(&v[2])));

	//test 9 axes given by the cross products of combination of edges from both

	//a00 = u0 × f0 = (1, 0, 0) × f0 = (0, −f0z, f0y)
	if (TestAxis3D(v, XMFLOAT3(0, -f[0].z, f[0].y))) return false;
	//a01 = u0 × f1 = (1, 0, 0) × f1 = (0, −f1z, f1y)
	if (TestAxis3D(v, XMFLOAT3(0, -f[1].z, f[1].y))) return false;
	//a02 = u0 × f2 = (1, 0, 0) × f2 = (0, −f2z, f2y)
	if (TestAxis3D(v, XMFLOAT3(0, -f[2].z, f[2].y))) return false;

	//a10 = u1 × f0 = (0, 1, 0) × f0 = ( f0z, 0, −f0x )
	if (TestAxis3D(v, XMFLOAT3(f[0].z, 0, -f[0].x))) return false;
	//a11 = u1 × f1 = (0, 1, 0) × f1 = (f1z, 0, −f1x)
	if (TestAxis3D(v, XMFLOAT3(f[1].z, 0, -f[1].x))) return false;
	//a12 = u1 × f2 = (0, 1, 0) × f2 = ( f2z, 0, −f2x )
	if (TestAxis3D(v, XMFLOAT3(f[2].z, 0, -f[2].x))) return false;

	//a20 = u2 × f0 = (0, 0, 1) × f0 = (−f0y, f0x , 0)
	if (TestAxis3D(v, XMFLOAT3(-f[0].y, f[0].x, 0))) return false;
	//a21 = u2 × f1 = (0, 0, 1) × f1 = (−f1y, f1x , 0)
	if (TestAxis3D(v, XMFLOAT3(-f[1].y, f[0].x, 0))) return false;
	//a22 = u2 × f2 = (0, 0, 1) × f2 = (−f2y, f2x , 0)
	if (TestAxis3D(v, XMFLOAT3(-f[2].y, f[0].x, 0))) return false;

	// Test the three axes corresponding to the face normals of AABB b
	if (Max3(v[0].x, v[1].x, v[2].x) < -m_f3HalfWidths.x || Min3(v[0].x, v[1].x, v[2].x) > m_f3HalfWidths.x)
		return false;

	if (Max3(v[0].y, v[1].y, v[2].y) < -m_f3HalfWidths.y || Min3(v[0].y, v[1].y, v[2].y) > m_f3HalfWidths.y)
		return false;

	if (Max3(v[0].z, v[1].z, v[2].z) < -m_f3HalfWidths.z || Min3(v[0].z, v[1].z, v[2].z) > m_f3HalfWidths.z)
		return false;

	//test seperating axis corresponding to triangle face normal
	XMFLOAT3 pn; XMStoreFloat3(&pn, XMVector3Cross(XMLoadFloat3(&f[0]), XMLoadFloat3(&f[1])));
	float pd = *XMVector3Dot(XMLoadFloat3(&pn), XMLoadFloat3(&v[0])).m128_f32;

	return Intersect3DPlane(Plane(pn, pd));
}

bool AABB::Intersect2DAABB(AABB* other)
{
	// Exit with no intersection if separated along an axis
	if (m_f3Max.x < other->m_f3Min.x || m_f3Min.x > other->m_f3Max.x) return SetIsCollided(other->SetIsCollided(false));
	if (m_f3Max.y < other->m_f3Min.y || m_f3Min.y > other->m_f3Max.y) return SetIsCollided(other->SetIsCollided(false));

	// Overlapping on all axes means AABBs are intersecting
	return SetIsCollided(other->SetIsCollided(true));
}

bool AABB::Intersect3DAABB(AABB* other)
{
	// Exit with no intersection if separated along an axis
	if (m_f3Max.x < other->m_f3Min.x || m_f3Min.x > other->m_f3Max.x) return SetIsCollided(other->SetIsCollided(false));
	if (m_f3Max.y < other->m_f3Min.y || m_f3Min.y > other->m_f3Max.y) return SetIsCollided(other->SetIsCollided(false));
	if (m_f3Max.z < other->m_f3Min.z || m_f3Min.z > other->m_f3Max.z) return SetIsCollided(other->SetIsCollided(false));

	// Overlapping on all axes means AABBs are intersecting
	return SetIsCollided(other->SetIsCollided(true));
}

bool AABB::Intersect2DSphere(BoundingSphere* sphere)
{
	float sqDist = SqDistPointAABB2D(sphere->GetCenter2D());
	float sqRadius = sphere->GetRadius() * sphere->GetRadius();

	return SetIsCollided(sphere->SetIsCollided(sqDist < sqRadius));
}

bool AABB::Intersect3DSphere(BoundingSphere* sphere)
{
	float sqDist = SqDistPointAABB3D(sphere->GetCenter3D());
	float sqRadius = sphere->GetRadius() * sphere->GetRadius();

	return SetIsCollided(sphere->SetIsCollided(sqDist < sqRadius));
}

bool AABB::Intersect2DSphere(BoundingSphere* sphere, XMFLOAT2& closestPnt)
{
	// Find point p on AABB closest to sphere center
	closestPnt = ClosestPtPointAABB2D(sphere->GetCenter2D());

	// Sphere and AABB intersect if the (squared) distance from sphere
	// center to point p is less than the (squared) sphere radius
	XMVECTOR v = XMVectorSubtract(XMLoadFloat2(&closestPnt), XMLoadFloat2(&sphere->GetCenter2D()));
	
	//sqDist < sqRadius
	return SetIsCollided(sphere->SetIsCollided(*XMVector2Dot(v, v).m128_f32 <= sphere->GetRadius() * sphere->GetRadius()));
}

bool AABB::IntersectMovingSphere2D(BoundingSphere* sphere, XMFLOAT2 SphDirVec, float& t)
{
	// Compute the AABB resulting from expanding b by sphere radius r
	AABB e = AABB(GetMin2D(), GetMax2D());
	e.Expand2D(sphere->GetRadius());

	// Intersect ray against expanded AABB e. Exit with no intersection if ray
	// misses e, else get intersection point p and time t as result
	XMFLOAT2 p;
	if (!e.IntersectRayAABB2D(sphere->GetCenter2D(), SphDirVec, t, p) || t > 1.f) 
		return SetIsCollided(sphere->SetIsCollided(false));

	// Compute which min and max faces of b the intersection point p lies
	// outside of. Note, u and v cannot have the same bits set and
	// they must have at least one bit set among them
	int u = 0; int v = 0;
	if (p.x < GetMin2D().x) u |= 1;		if (p.x > GetMax2D().x) v |= 1;
	if (p.y < GetMin2D().y) u |= 2;		if (p.y > GetMax2D().y) v |= 2;

	// ‘Or’ all set bits together into a bit mask (note: here u + v == u | v)
	int m = u + v;

	XMFLOAT2 sphCenDir;	XMStoreFloat2(&sphCenDir, XMVectorAdd(XMLoadFloat2(&sphere->GetCenter2D()), XMLoadFloat2(&SphDirVec)));

	// Define line segment [c, c+d] specified by the sphere movement
	LineSeg seg(sphere->GetCenter2D(), sphCenDir);

	//if all 2 bits set (m == 3) then p is in a vertex region
	if (m == 3)
	{
		// Must now intersect segment [c, c+d] against the capsules of the three
		// edges meeting at the vertex and return the best time, if one or more hit
		float tmin = FLT_MAX;

		if (seg.IntersectCapsule2D(BoundingCapsule(Corner2D(v), Corner2D(v ^ 1), sphere->GetRadius()), t))
			tmin = min(t, tmin);
		if (seg.IntersectCapsule2D(BoundingCapsule(Corner2D(v), Corner2D(v ^ 2), sphere->GetRadius()), t))
			tmin = min(t, tmin);

		if (tmin == FLT_MAX) return SetIsCollided(sphere->SetIsCollided(false));
		t = tmin;
		return SetIsCollided(sphere->SetIsCollided(true));
	}

	//if only one bit set in m, then p is in a face region
	if ((m & (m-1)) == 0)
	{
		// Do nothing. Time t from intersection with
		// expanded box is correct intersection time
		return SetIsCollided(sphere->SetIsCollided(true));
	}

	//p is in an edge region. intersect against capsule at the edge
	return SetIsCollided(sphere->SetIsCollided(seg.IntersectCapsule2D(BoundingCapsule(Corner2D(u ^ 3), Corner2D(v), sphere->GetRadius()), t)));
}

bool AABB::IntersectMovingSphere3D(BoundingSphere* sphere, XMFLOAT3 SphDirVec, float& t)
{
	// Compute the AABB resulting from expanding b by sphere radius r
	AABB e(*this);	e.Expand3D(sphere->GetRadius());

	// Intersect ray against expanded AABB e. Exit with no intersection if ray
	// misses e, else get intersection point p and time t as result
	XMFLOAT3 p;
	if (!e.IntersectRayAABB3D(sphere->GetCenter3D(), SphDirVec, t, p))	return false;

	// Compute which min and max faces of b the intersection point p lies
	// outside of. Note, u and v cannot have the same bits set and
	// they must have at least one bit set among them
	int u = 0; int v = 0;
	if (p.x < m_f3Min.x) u |= 1;		if (p.x > m_f3Max.x) v |= 1;
	if (p.y < m_f3Min.y) u |= 2;		if (p.y > m_f3Max.y) v |= 2;
	if (p.z < m_f3Min.z) u |= 4;		if (p.z > m_f3Max.z) v |= 4;

	// ‘Or’ all set bits together into a bit mask (note: here u + v == u | v)
	int m = u + v;

	XMFLOAT3 sphCenDir;	XMStoreFloat3(&sphCenDir, XMVectorAdd(XMLoadFloat3(&sphere->GetCenter3D()), XMLoadFloat3(&SphDirVec)));

	// Define line segment [c, c+d] specified by the sphere movement
	LineSeg seg(sphere->GetCenter3D(), sphCenDir);

	//if all 3 bits set (m == 7) then p is in a vertex region
	if (m == 7)
	{
		// Must now intersect segment [c, c+d] against the capsules of the three
		// edges meeting at the vertex and return the best time, if one or more hit
		float tmin = FLT_MAX;

		if (seg.IntersectCapsule3D(BoundingCapsule(Corner3D(v), Corner3D(v ^ 1), sphere->GetRadius()), t))
			tmin = min(t, tmin);
		if (seg.IntersectCapsule3D(BoundingCapsule(Corner3D(v), Corner3D(v ^ 2), sphere->GetRadius()), t))
			tmin = min(t, tmin);
		if (seg.IntersectCapsule3D(BoundingCapsule(Corner3D(v), Corner3D(v ^ 4), sphere->GetRadius()), t))
			tmin = min(t, tmin);

		if (tmin == FLT_MAX) return false;
		t = tmin;
		return true;
	}

	//if only one bit set in m, then p is in a face region
	if ((m & (m - 1)) == 0)
	{
		// Do nothing. Time t from intersection with
		// expanded box is correct intersection time
		return true;
	}

	//p is in an edge region. intersect against capsule at the edge
	return seg.IntersectCapsule3D(BoundingCapsule(Corner3D(u ^ 7), Corner3D(v), sphere->GetRadius()), t);
}

bool AABB::Intersect3DSphere(BoundingSphere* sphere, XMFLOAT3& closestPnt)
{
	// Find point p on AABB closest to sphere center
	closestPnt = ClosestPtPointAABB3D(sphere->GetCenter3D());

	// Sphere and AABB intersect if the (squared) distance from sphere
	// center to point p is less than the (squared) sphere radius
	XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&closestPnt), XMLoadFloat3(&sphere->GetCenter3D()));

	//sqDist < sqRadius
	return *XMVector3Dot(v, v).m128_f32 <= sphere->GetRadius() * sphere->GetRadius();
}

bool AABB::IntersectLineSeg2D(LineSeg* lineSeg)
{
	XMFLOAT2 c = GetCenter2D();
	XMFLOAT2 e = GetExtent2D();
	XMFLOAT2 m;	XMStoreFloat2(&m, XMVectorScale(XMVectorAdd(XMLoadFloat2(&lineSeg->GetBottom2D()), XMLoadFloat2(&lineSeg->GetTop2D())), 0.5f));

	XMFLOAT2 d;	XMStoreFloat2(&d, XMVectorSubtract(XMLoadFloat2(&lineSeg->GetTop2D()), XMLoadFloat2(&m)));

	XMStoreFloat2(&m, XMVectorSubtract(XMLoadFloat2(&m), XMLoadFloat2(&GetCenter2D())));

	//try the world coordinate axes as seperating axes
	float adx = abs(d.x);		if (abs(m.x) > e.x + adx) return SetIsCollided(false);
	float ady = abs(d.y);		if (abs(m.y) > e.y + ady) return SetIsCollided(false);

	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis
	adx += FLT_EPSILON;		ady += FLT_EPSILON;

	// Try cross products of segment direction vector with coordinate axes
	if (abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return SetIsCollided(false);
	if (abs(m.y * d.x - m.x * d.y) > e.y * adx + e.x * ady) return SetIsCollided(false);

	// No separating axis found; segment must be overlapping AABB
	return SetIsCollided(true);
}

bool AABB::IntersectLineSeg3D(LineSeg* lineSeg)
{
	XMFLOAT3 c = GetCenter3D();
	XMFLOAT3 e = GetExtent3D();
	XMFLOAT3 m;	XMStoreFloat3(&m, XMVectorScale(XMVectorAdd(XMLoadFloat3(&lineSeg->GetBottom3D()), XMLoadFloat3(&lineSeg->GetTop3D())), 0.5f));

	XMFLOAT3 d;	XMStoreFloat3(&d, XMVectorSubtract(XMLoadFloat3(&lineSeg->GetTop3D()), XMLoadFloat3(&m)));

	XMStoreFloat3(&m, XMVectorSubtract(XMLoadFloat3(&m), XMLoadFloat3(&GetCenter3D())));

	//try the world coordinate axes as seperating axes
	float adx = abs(d.x);		if (abs(m.x) > e.x + adx) return false;
	float ady = abs(d.y);		if (abs(m.y) > e.y + ady) return false;
	float adz = abs(d.z);		if (abs(m.z) > e.z + adz) return false;

	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis
	adx += FLT_EPSILON;		ady += FLT_EPSILON;		adz += FLT_EPSILON;

	// Try cross products of segment direction vector with coordinate axes
	if (abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) return false;
	if (abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) return false;
	if (abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return false;

	// No separating axis found; segment must be overlapping AABB
	return true;
}

bool AABB::IntersectRayAABB2D(XMFLOAT2 p, XMFLOAT2 d, float& tmin, XMFLOAT2& q)
{
	tmin = 0.f;					
	float tmax = FLT_MAX;		//set to max distance ray can travel(for segment)

	//add to array's to avoid code repition and provide more clarity
	float D[2] = { d.x,d.y };
	float P[2] = { p.x,p.y };
	float bMin[2] = { GetMin2D().x , GetMin2D().y };
	float bMax[2] = { GetMax2D().x , GetMax2D().y };

	//for both slabs
	for (int i = 0; i < 2; i++)
	{
		//if the ray is parallel to the slab. no hit unless origin is not within the slab
		if (abs(D[i]) < FLT_EPSILON) if (P[i] < bMin[i] || P[i] > bMax[i]) return SetIsCollided(false);
		//compute the intersection t value of ray with near and far plane of the slab
		float ood = 1.f / D[i];
		float t1 = (bMin[i] - P[i]) * ood;		
		float t2 = (bMax[i] - P[i]) * ood;

		//make t1 be intersection t value of the ray with the near and far plane of the slab
		if (t1 > t2) swap(t1, t2);
		//compute the intersection of slab intersection intervals
		if (t1 > tmin) tmin = t1;		
		if (t2 > tmax) tmax = t2;
		
		if (tmin > tmax) return SetIsCollided(false);		//exit with no collision as soon as slab intersection becomes empty
	}

	//ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
	//q = p + d * tmin

	XMVECTOR dt = XMLoadFloat2(&d) * tmin;

	XMStoreFloat2(&q, XMVectorAdd(XMLoadFloat2(&p), dt));

	return SetIsCollided(true);
}

bool AABB::IntersectRayAABB3D(XMFLOAT3 p, XMFLOAT3 d, float& tmin, XMFLOAT3& q)
{
	tmin = 0.f;					//set to -FLT_MAX to get the first hit on line
	float tmax = FLT_MAX;		//set to max distance ray can travel(for segment)

	//add to array's to avoid code repition
	float D[3] = { d.x, d.y, d.z };
	float P[3] = { p.x, p.y, p.z};
	float bMin[3] = { GetMin3D().x , GetMin3D().y, GetMin3D().z };
	float bMax[3] = { GetMax3D().x , GetMax3D().y, GetMax3D().z };

	//for all three slabs
	for (int i = 0; i < 2; i++)
	{
		//if the ray is parallel to the slab. no hit unless origin is not within the slab
		if (abs(D[i]) < FLT_EPSILON) if (P[i] < bMin[i] || P[i] > bMax[i]) return false;
		//compute the intersection t value of ray with near and far plane of the slab
		float ood = 1.f / D[i];
		float t1 = (bMin[i] - P[i]) * ood;		float t2 = (bMax[i] - P[i]) * ood;

		//make t1 be intersection t value of the ray with the near and far plane of the slab
		if (t1 > t2) swap(t1, t2);
		//compute the intersection of slab intersection intervals
		if (t1 > tmin) tmin = t1;		if (t2 > tmax) tmax = t2;

		if (tmin > tmax) return false;		//exit with no collision as soon as slab intersection becomes empty
	}

	//ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
	//q = p + d * tmin
	XMStoreFloat3(&q, XMVectorAdd(XMLoadFloat3(&p), XMVectorScale(XMLoadFloat3(&d), tmin)));

	return true;
}

XMFLOAT2 AABB::Corner2D(int n)
{
	float pX, pY;
	pX = (n & 1) ? m_f3Max.x : m_f3Min.x;
	pY = (n & 1) ? m_f3Max.y : m_f3Min.y;

	return XMFLOAT2(pX, pY);
}

XMFLOAT3 AABB::Corner3D(int n)
{
	float pX, pY, pZ;
	pX = (n & 1) ? m_f3Max.x : m_f3Min.x;
	pY = (n & 1) ? m_f3Max.y : m_f3Min.y;
	pZ = (n & 1) ? m_f3Max.z : m_f3Min.z;

	return XMFLOAT3(pX, pY, pZ);
}

bool AABB::IntersectMovingAABB2D(AABB* other, XMFLOAT2 velA, XMFLOAT2 VelB, float& tfirst, float& tlast)
{
	//early exit if a and b intially overlap
	if (Intersect2DAABB(other))
	{
		tfirst = tlast = 0.f;
		return SetIsCollided(other->SetIsCollided(true));
	}

	//use relative veloctity, to effectively treat a as stationary
	XMFLOAT2 vel;
	XMStoreFloat2(&vel, XMVectorSubtract(XMLoadFloat2(&VelB), XMLoadFloat2(&velA)));
	tfirst = 0.f;		tlast = 1.f;

	//using array for readability and to avoid code repetition
	float v[2] = { vel.x, vel.y };
	float MinA[2] = { GetMin2D().x, GetMin2D().y };		float MaxA[2] = { GetMax2D().x, GetMax2D().y };
	float MinB[2] = { other->GetMin2D().x, other->GetMin2D().y };		float MaxB[2] = { other->GetMax2D().x, other->GetMax2D().y };

	//for each axis, determine times of first and last contact, if any
	for (int i = 0; i < 2; i++)
	{
		if (v[i] < 0.f)
		{
			if (MaxB[i] < MinA[i]) return SetIsCollided(other->SetIsCollided(false));	//nonintersecting and moving away from one another
			if (MaxA[i] < MinB[i]) tfirst = max((MaxA[i] - MinB[i]) / v[i], tfirst);
			if (MaxB[i] > MinA[i]) tlast = min((MinA[i] - MaxB[i]) / v[i], tlast);
		}
		
		if (v[i] > 0.f)
		{
			if (MinB[i] > MaxA[i]) return SetIsCollided(other->SetIsCollided(false));	//nonintersecting and moving away from one another
			if (MaxB[i] < MinA[i]) tfirst = max((MinA[i] - MaxB[i]) / v[i], tfirst);
			if (MaxA[i] > MinB[i]) tlast = min((MaxA[i] - MinB[i]) / v[i], tlast);
		}
	}

	//no overlap possible if time of first contact occurs after time of the last contact
	if (tfirst > tlast)  return SetIsCollided(other->SetIsCollided(false));

	return SetIsCollided(other->SetIsCollided(true)); 
}

bool AABB::IntersectMovingAABB3D(AABB* aabb, XMFLOAT3 velA, XMFLOAT3 VelB, float& tfirst, float& tlast)
{
	//early exit if a and b intially overlap
	if (Intersect3DAABB(aabb))
	{
		tfirst = tlast = 0.f;
		return true;
	}

	//use relative veloctity, to effectively treat a as stationary
	XMFLOAT3 vel;
	XMStoreFloat3(&vel, XMVectorSubtract(XMLoadFloat3(&VelB), XMLoadFloat3(&velA)));
	tfirst = 0.f;		tlast = 1.f;

	//using array for readability and to avoid code repetition
	float v[3] = { vel.x, vel.y, vel.y };
	float MinA[3] = { GetMin3D().x, GetMin3D().y, GetMin3D().z };		float MaxA[3] = { GetMax3D().x, GetMax3D().y, GetMax3D().z };
	float MinB[3] = { aabb->GetMin3D().x, aabb->GetMin3D().y, aabb->GetMin3D().z };		float MaxB[3] = { aabb->GetMax3D().x, aabb->GetMax3D().y, aabb->GetMax3D().z };

	//for each axis, determine times of first and last contact, if any
	for (int i = 0; i < 3; i++)
	{
		if (v[i] < 0.f)
		{
			if (MaxB[i] < MinA[i]) return false;	//nonintersecting and moving away from one another
			if (MaxA[i] < MinB[i]) tfirst = max((MaxA[i] - MinB[i]) / v[i], tfirst);
			if (MaxB[i] > MinA[i]) tlast = min((MinA[i] - MaxB[i]) / v[i], tlast);
		}

		if (v[i] > 0.f)
		{
			if (MinB[i] > MaxA[i]) return false;	//nonintersecting and moving away from one another
			if (MaxB[i] < MinA[i]) tfirst = max((MinA[i] - MaxB[i]) / v[i], tfirst);
			if (MaxA[i] > MinB[i]) tlast = min((MaxA[i] - MinB[i]) / v[i], tlast);
		}
	}

	//no overlap possible if time of first contact occurs after time of the last contact
	if (tfirst > tlast) return false;

	return true;
}


float AABB::SqDistPointAABB2D(XMFLOAT2 point)
{
	float squareDist = 0.f;

	// For each axis count any excess distance outside box extents
	if (point.x < GetMin2D().x) squareDist += (GetMin2D().x - point.x) * (GetMin2D().x - point.x);
	if (point.x > GetMax2D().x) squareDist += (point.x - GetMax2D().x) * (point.x - GetMax2D().x);

	if (point.y < GetMin2D().y) squareDist += (GetMin2D().y - point.y) * (GetMin2D().y - point.y);
	if (point.y > GetMax2D().y) squareDist += (point.y - GetMax2D().y) * (point.y - GetMax2D().y);

	return squareDist;
}

float AABB::SqDistPointAABB3D(XMFLOAT3 point)
{
	float squareDist = 0.f;

	// For each axis count any excess distance outside box extents
	if (point.x < GetMin3D().x) squareDist += (GetMin3D().x - point.x) * (GetMin3D().x - point.x);
	if (point.x > GetMax3D().x) squareDist += (point.x - GetMax3D().x) * (point.x - GetMax3D().x);

	if (point.y < GetMin3D().y) squareDist += (GetMin3D().y - point.y) * (GetMin3D().y - point.y);
	if (point.y > GetMax3D().y) squareDist += (point.y - GetMax3D().y) * (point.y - GetMax3D().y);

	if (point.z < GetMin3D().z) squareDist += (GetMin3D().z - point.z) * (GetMin3D().z - point.z);
	if (point.z > GetMax3D().z) squareDist += (point.z - GetMax3D().z) * (point.z - GetMax3D().z);

	return squareDist == 0;
}

XMFLOAT2 AABB::ClosestPtPointAABB2D(XMFLOAT2 pnt)
{
	//for each coordinate axis, if the point coordinate value is 
	//outside the box, clamp it to the box, else it remains as original value
	XMFLOAT2 q = pnt;

	if (q.x < GetMin2D().x) q.x = GetMin2D().x;	
	if (q.x > GetMax2D().x) q.x = GetMax2D().x;	
	if (q.y < GetMin2D().y) q.y = GetMin2D().y;	
	if (q.y > GetMax2D().y) q.y = GetMax2D().y;	

	return q;
}

XMFLOAT3 AABB::ClosestPtPointAABB3D(XMFLOAT3 pnt)
{
	//for each coordinate axis, if the point coordinate value is 
	//outside the box, clamp it to the box, else it remains as original value
	XMFLOAT3 q = pnt;

	//if x is less than box min
	if (q.x < GetMin3D().x) q.x = GetMin3D().x;
	if (q.x > GetMax3D().x) q.x = GetMax3D().x;
	if (q.y < GetMin3D().y) q.y = GetMin3D().y;
	if (q.y > GetMax3D().y) q.y = GetMax3D().y;
	if (q.z < GetMin3D().z) q.z = GetMin3D().z;
	if (q.z > GetMax3D().z) q.z = GetMax3D().z;

	return q;
}

float AABB::Max3(float a, float b, float c)
{
	return	(b < a) ? (c < a) ? a : c : (c < b) ? b : c;
}

float AABB::Min3(float a, float b, float c) 
{ 
	return	(b > a) ? (c > a) ? a : c : (c > b) ? b : c; 
}

bool AABB::TestAxis2D(XMFLOAT2 u, XMFLOAT2 f, XMFLOAT2 locAxis[2], XMFLOAT2 v[3])
{
	XMVECTOR uf = XMVector2Cross(XMLoadFloat2(&u), XMLoadFloat2(&f));
	float p0, p1, p2, r;
	p0 = *XMVector2Dot(XMLoadFloat2(&v[0]), uf).m128_f32;
	p1 = *XMVector2Dot(XMLoadFloat2(&v[1]), uf).m128_f32;
	p2 = *XMVector2Dot(XMLoadFloat2(&v[2]), uf).m128_f32;

	r = m_f3HalfWidths.x * abs(*XMVector2Dot(XMLoadFloat2(&locAxis[0]), uf).m128_f32) +
		m_f3HalfWidths.y * abs(*XMVector2Dot(XMLoadFloat2(&locAxis[1]), uf).m128_f32);

	return (max(Max3(p0, p1, p2), Min3(p0, p1, p2)) > r);
}

bool AABB::TestAxis3D(XMFLOAT3 u, XMFLOAT3 f, XMFLOAT3 locAxis[3], XMFLOAT3 v[3])
{
	XMVECTOR uf = XMVector3Cross(XMLoadFloat3(&u), XMLoadFloat3(&f));
	float p0, p1, p2, r;
	p0 = *XMVector3Dot(XMLoadFloat3(&v[0]), uf).m128_f32;
	p1 = *XMVector3Dot(XMLoadFloat3(&v[1]), uf).m128_f32;
	p2 = *XMVector3Dot(XMLoadFloat3(&v[2]), uf).m128_f32;

	r = m_f3HalfWidths.x * abs(*XMVector3Dot(XMLoadFloat3(&locAxis[0]), uf).m128_f32) +
		m_f3HalfWidths.y * abs(*XMVector3Dot(XMLoadFloat3(&locAxis[1]), uf).m128_f32) +
		m_f3HalfWidths.y * abs(*XMVector3Dot(XMLoadFloat3(&locAxis[1]), uf).m128_f32);

	return (max(Max3(p0, p1, p2), Min3(p0, p1, p2)) > r);
}

bool AABB::TestAxis3D(XMFLOAT3 v[3], XMFLOAT3 axis)
{
	float p0, p1, p2, r;
	p0 = *XMVector3Dot(XMLoadFloat3(&v[0]), XMLoadFloat3(&axis)).m128_f32;
	p1 = *XMVector3Dot(XMLoadFloat3(&v[1]), XMLoadFloat3(&axis)).m128_f32;
	p2 = *XMVector3Dot(XMLoadFloat3(&v[2]), XMLoadFloat3(&axis)).m128_f32;
	
	r = m_f3HalfWidths.x * axis.x + m_f3HalfWidths.y * axis.y + m_f3HalfWidths.z * axis.z;

	return max(Max3(p0, p1, p2), Min3(p0, p1, p2)) > r;
}



