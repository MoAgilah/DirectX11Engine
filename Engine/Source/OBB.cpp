#include "../Headers/OBB.h"
#include "..\Headers\OBB.h"


OBB::OBB(BVType bvType, bool is3Dim)
	:BoundingVolume(bvType, is3Dim)
{
	m_ModelMAt = XMMatrixIdentity();

	m_f3Min = XMFLOAT3(0.f, 0.f, 0.f);
	m_f3Max = XMFLOAT3(0.f, 0.f, 0.f);

	if (is3Dim) 
	{
		m_vf3CornerPoints.resize(8);
		m_vf3LocalAxis.resize(3);
	}
	else 
	{
		m_vf3CornerPoints.resize(4);
		m_vf3LocalAxis.resize(2);
	};
}

OBB::OBB(XMFLOAT2 min, XMFLOAT2 max)
	:BoundingVolume(BVType::OBB2D, false)
{
	m_ModelMAt = XMMatrixIdentity();

	m_vf3CornerPoints.resize(4);

	m_f3Min = m_vf3CornerPoints[0] = XMFLOAT3(min.x, min.y, 1.f);
	m_f3Max = m_vf3CornerPoints[2] = XMFLOAT3(max.x, min.y, 1.f);

	m_f3Center = XMFLOAT3((m_f3Min.x + m_f3Max.x) * 0.5f, (m_f3Min.y + m_f3Max.y) * 0.5f, 1.f);

	m_vf3CornerPoints[1] = XMFLOAT3(m_f3Min.x, m_f3Max.y, 1.f);
	m_vf3CornerPoints[3] = XMFLOAT3(m_f3Max.x, m_f3Min.y, 1.f);
}

OBB::OBB(XMFLOAT3 min, XMFLOAT3 max)
	:BoundingVolume(BVType::OBB3D, true)
{
	m_ModelMAt = XMMatrixIdentity();

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

OBB::OBB(const OBB& other)
	:BoundingVolume(other.m_bvType, other.m_bIs3D)
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
	m_vf3LocalAxis = other.m_vf3LocalAxis;
}

OBB::~OBB()
{
}

void OBB::Update(XMFLOAT2 Pos, XMFLOAT2 Size, XMFLOAT2 rot)
{
	SetRotation2D(rot);
	Update(Pos, Size);
}

void OBB::Update(XMFLOAT3 Pos, XMFLOAT3 Size, XMFLOAT3 rot)
{
	SetRotation3D(rot);
	Update(Pos, Size);
}

void OBB::SetRotation2D(XMFLOAT2 rot)
{

	XMStoreFloat3(&m_vf3LocalAxis[0], XMLoadFloat3(&XMFLOAT3(rot.x, 0, 0)));
	XMStoreFloat3(&m_vf3LocalAxis[1], XMLoadFloat3(&XMFLOAT3(0, rot.y, 0)));
}

void OBB::SetRotation3D(XMFLOAT3 rot)
{
	XMStoreFloat3(&m_vf3LocalAxis[0],XMLoadFloat3(&XMFLOAT3(rot.x, 0, 0)));
	XMStoreFloat3(&m_vf3LocalAxis[1],XMLoadFloat3(&XMFLOAT3(0, rot.y, 0)));
	XMStoreFloat3(&m_vf3LocalAxis[2],XMLoadFloat3(&XMFLOAT3(0, 0, rot.z)));
}

int test = 0;


void OBB::Update(XMFLOAT2 Pos, XMFLOAT2 Size)
{
	
	m_f3Size = XMFLOAT3(Size.x * m_f3Percentile.x, Size.y * m_f3Percentile.y, 1.f);
	m_f3HalfWidths = XMFLOAT3(m_f3Size.x * 0.5f, m_f3Size.y * 0.5f, 1.f);
	m_f3PrevSize = XMFLOAT3(Size.x, Size.y, 1.f);

	//Chewby----------------------------------------------------------------------------
	XMFLOAT2 scaleOri(0.f,0.f), scale(1.f,1.f);
	XMMATRIX model, trans, rot;
	
	XMFLOAT2 rotori = XMFLOAT2(Pos.x + m_f3HalfWidths.x, Pos.y + m_f3HalfWidths.y);

	trans = XMMatrixTranslationFromVector(XMLoadFloat2(&Pos));
	//model = XMMatrixTransformation2D(XMLoadFloat2(&scaleOri), 0, XMLoadFloat2(&scale), XMLoadFloat2(&pos), m_vf3LocalAxis[0].x, XMLoadFloat2(&Pos));
	
	m_vf3CornerPoints[0] = XMFLOAT3(model.r[3].m128_f32[0], model.r[3].m128_f32[1], 1.f);
	
	//----------------------------------------------------------------------------------



	m_f3Min = m_vf3CornerPoints[0];

	m_f3Center = XMFLOAT3(m_f3Min.x + m_f3HalfWidths.x, m_f3Min.y + m_f3HalfWidths.y, 1.f);
	m_f3Max = m_vf3CornerPoints[2] = XMFLOAT3(m_f3Min.x + m_f3Size.x, m_f3Min.y + m_f3Size.y, 1.f);
	m_vf3CornerPoints[1] = XMFLOAT3(m_f3Min.x, m_f3Max.y, 1.f);
	m_vf3CornerPoints[3] = XMFLOAT3(m_f3Max.x, m_f3Min.y, 1.f);
}

void OBB::Update(XMFLOAT3 Pos, XMFLOAT3 Size)
{
	
	m_f3Size = XMFLOAT3(Size.x * m_f3Percentile.x, Size.y * m_f3Percentile.y, Size.z * m_f3Percentile.z);
	m_f3HalfWidths = XMFLOAT3(m_f3Size.x * 0.5f, m_f3Size.y * 0.5f, m_f3Size.z * 0.5f);
	m_f3PrevSize = Size;

	XMMATRIX trans = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&m_vf3CornerPoints[0]));
	XMMATRIX rot = DirectX::XMMatrixRotationX(m_vf3LocalAxis[0].x) * DirectX::XMMatrixRotationY(m_vf3LocalAxis[1].y) * DirectX::XMMatrixRotationZ(m_vf3LocalAxis[2].z);
	XMFLOAT4X4 model;
	XMStoreFloat4x4(&model, trans * rot);

	//min
	XMStoreFloat3(&m_vf3CornerPoints[0], XMLoadFloat3(&XMFLOAT3(model._31, model._32, model._33)));

	m_f3Min = m_vf3CornerPoints[0];
	m_f3Max = m_vf3CornerPoints[1] = XMFLOAT3(m_f3Min.x + m_f3Size.x, m_f3Min.y + m_f3Size.y, Pos.z + m_f3Size.z);
	m_vf3CornerPoints[2] = XMFLOAT3(m_f3Min.x, m_f3Max.y, m_f3Min.z);
	m_vf3CornerPoints[3] = XMFLOAT3(m_f3Min.x, m_f3Min.y, m_f3Max.z);
	m_vf3CornerPoints[4] = XMFLOAT3(m_f3Min.x, m_f3Max.y, m_f3Max.z);
	m_vf3CornerPoints[5] = XMFLOAT3(m_f3Max.x, m_f3Min.y, m_f3Min.z);
	m_vf3CornerPoints[6] = XMFLOAT3(m_f3Max.x, m_f3Max.y, m_f3Min.z);
	m_vf3CornerPoints[7] = XMFLOAT3(m_f3Max.x, m_f3Min.y, m_f3Max.z);

	m_f3Center = XMFLOAT3(m_f3Min.x + m_f3HalfWidths.x, m_f3Min.y + m_f3HalfWidths.y, m_f3Min.z + m_f3HalfWidths.z);
}

bool OBB::IntersectOBB2D(OBB* other)
{
	float ra, rb;
	XMFLOAT3X3 R, AbsR;

	//compute the rotation matrix expression b in a's coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			R.m[i][j] = *XMVector3Dot(XMLoadFloat3(&m_vf3LocalAxis[i]), XMLoadFloat3(&other->GetLocalAxis()[j])).m128_f32;

	//compute the translation vector t
	XMVECTOR v = XMVectorSubtract(XMLoadFloat2(&other->GetCenter2D()), XMLoadFloat2(&GetCenter2D()));

	XMVECTOR T = XMVectorSet(*XMVector2Dot(v, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32,
		*XMVector2Dot(v, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32, 1.f, 1.f);

	float exts[2] = { m_f3HalfWidths.x, m_f3HalfWidths.y };
	float othExts[2] = { other->GetExtent2D().x, other->GetExtent2D().y };

	//compute the common subexpressions. add in an epsilon term to counteract arithmentic
	//errors when the two edges are parallel and their cross product is (near) null
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			AbsR.m[i][j] = abs(R.m[i][j]) + FLT_EPSILON;

	// Test axes L = A0, L = A1
	for (int i = 0; i < 2; i++)
	{
		ra = exts[i];
		rb = othExts[0] * AbsR.m[i][0] + othExts[1] * AbsR.m[i][1];
		if (abs(T.m128_f32[i]) > ra + rb) return false;
	}

	// Test axes L = B0, L = B1
	for (int i = 0; i < 2; i++)
	{
		ra = exts[0] * AbsR.m[0][i] + exts[1] * AbsR.m[1][i];
		rb = othExts[i];
		if (abs(T.m128_f32[0] * R.m[0][i] + T.m128_f32[1] * R.m[1][i]) > ra + rb) return false;
	}

	// Since no separating axis is found, the OBBs must be intersecting
	return true;
}

bool OBB::IntersectOBB3D(OBB* other)
{
	float ra, rb;
	XMFLOAT3X3 R, AbsR;

	//compute the rotation matrix expression b in a's coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			R.m[i][j] = *XMVector3Dot(XMLoadFloat3(&m_vf3LocalAxis[i]), XMLoadFloat3(&other->GetLocalAxis()[j])).m128_f32;

	//compute the translation vector t
	XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&other->GetCenter3D()), XMLoadFloat3(&GetCenter3D()));

	//bring translation into a's coordinate frame
	XMVECTOR T = XMVectorSet(*XMVector3Dot(v, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32,
		*XMVector3Dot(v, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32, *XMVector3Dot(v, XMLoadFloat3(&m_vf3LocalAxis[2])).m128_f32, 1.f);
	
	float exts[3] = { m_f3HalfWidths.x, m_f3HalfWidths.y, m_f3HalfWidths.z };
	float othExts[3] = { other->GetExtent3D().x, other->GetExtent3D().y, other->GetExtent3D().z };

	//compute the common subexpressions. add in an epsilon term to counteract arithmentic
	//errors when the two edges are parallel and their cross product is (near) null
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			AbsR.m[i][j] = abs(R.m[i][j]) + FLT_EPSILON;

	// Test axes L = A0, L = A1, L = A2
	for (int i = 0; i < 3; i++)
	{
		ra = exts[i];
		rb = othExts[0] * AbsR.m[i][0] + othExts[1] * AbsR.m[i][1] + othExts[2] * AbsR.m[i][2];
		if (abs(T.m128_f32[i]) > ra + rb) return false;
	}

	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++)
	{
		ra = exts[0] * AbsR.m[0][i] + exts[1] * AbsR.m[1][i] + othExts[2] * AbsR.m[2][i];
		rb = othExts[i];
		if (abs(T.m128_f32[0] * R.m[0][i] + T.m128_f32[1] * R.m[1][i] + T.m128_f32[2] * R.m[2][i]) > ra + rb) return false;
	}

	// Test axis L = A0 x B0
	ra = exts[1] * AbsR.m[2][0] + exts[2] * AbsR.m[1][0];
	rb = othExts[1] * AbsR.m[0][2] + othExts[2] * AbsR.m[0][1];
	if (abs(T.m128_f32[2] * R.m[1][0] - T.m128_f32[1] * R.m[2][0]) > ra + rb) return 0;

	// Test axis L = A0 x B1
	ra = exts[1] * AbsR.m[2][1] + exts[2] * AbsR.m[1][1];
	rb = othExts[0] * AbsR.m[0][2] + othExts[2] * AbsR.m[0][0];
	if (abs(T.m128_f32[2] * R.m[1][1] - T.m128_f32[1] * R.m[2][1]) > ra + rb) return 0;

	// Test axis L = A0 x B2
	ra = exts[1] * AbsR.m[2][2] + exts[2] * AbsR.m[1][2];
	rb = othExts[0] * AbsR.m[0][1] + othExts[1] * AbsR.m[0][0];
	if (abs(T.m128_f32[2] * R.m[1][2] - T.m128_f32[1] * R.m[2][2]) > ra + rb) return 0;

	// Test axis L = A1 x B0
	ra = exts[0] * AbsR.m[2][0] + exts[2] * AbsR.m[0][0];
	rb = othExts[1] * AbsR.m[1][2] + othExts[2] * AbsR.m[1][1];
	if (abs(T.m128_f32[0] * R.m[2][0] - T.m128_f32[2] * R.m[0][0]) > ra + rb) return 0;

	// Test axis L = A1 x B1
	ra = exts[0] * AbsR.m[2][1] + exts[2] * AbsR.m[0][1];
	rb = othExts[0] * AbsR.m[1][2] + othExts[2] * AbsR.m[1][0];
	if (abs(T.m128_f32[0] * R.m[2][1] - T.m128_f32[2] * R.m[0][1]) > ra + rb) return 0;

	// Test axis L = A1 x B2
	ra = exts[0] * AbsR.m[2][2] + exts[2] * AbsR.m[0][2];
	rb = othExts[0] * AbsR.m[1][1] + othExts[1] * AbsR.m[1][0];
	if (abs(T.m128_f32[0] * R.m[2][2] - T.m128_f32[2] * R.m[0][2]) > ra + rb) return 0;

	// Test axis L = A2 x B0
	ra = exts[0] * AbsR.m[1][0] + exts[1] * AbsR.m[0][0];
	rb = othExts[1] * AbsR.m[2][2] + othExts[2] * AbsR.m[2][1];
	if (abs(T.m128_f32[1] * R.m[0][0] - T.m128_f32[0] * R.m[1][0]) > ra + rb) return 0;

	// Test axis L = A2 x B1
	ra = exts[0] * AbsR.m[1][1] + exts[1] * AbsR.m[0][1];
	rb = othExts[0] * AbsR.m[2][2] + othExts[2] * AbsR.m[2][0];
	if (abs(T.m128_f32[1] * R.m[0][1] - T.m128_f32[0] * R.m[1][1]) > ra + rb) return 0;

	// Test axis L = A2 x B2
	ra = exts[0] * AbsR.m[1][2] + exts[1] * AbsR.m[0][2];
	rb = othExts[0] * AbsR.m[2][1] + othExts[1] * AbsR.m[2][0];
	if (abs(T.m128_f32[1] * R.m[0][2] - T.m128_f32[0] * R.m[1][2]) > ra + rb) return 0;

	// Since no separating axis is found, the OBBs must be intersecting
	return true;
}

bool OBB::Intersect2DSphere(BoundingSphere* sphere)
{
	XMFLOAT2 c;
	//find the point p on OBB closest to the sphere center
	ClosestPtPointOBB2D(sphere->GetCenter2D(), c);

	XMVECTOR v = XMVectorSubtract(XMLoadFloat2(&c), XMLoadFloat2(&sphere->GetCenter2D()));

	return *XMVector2Dot(v, v).m128_f32 <= sphere->GetRadius()* sphere->GetRadius();
}

bool OBB::Intersect2DSphere(BoundingSphere* sphere, XMFLOAT2& c)
{
	//find the point p on OBB closest to the sphere center
	ClosestPtPointOBB2D(sphere->GetCenter2D(), c);

	XMVECTOR v = XMVectorSubtract(XMLoadFloat2(&c), XMLoadFloat2(&sphere->GetCenter2D()));

	return *XMVector2Dot(v, v).m128_f32 <= sphere->GetRadius() * sphere->GetRadius();
}

bool OBB::Intersect3DSphere(BoundingSphere* sphere)
{
	XMFLOAT3 c;
	//find the point p on OBB closest to the sphere center
	ClosestPtPointOBB3D(sphere->GetCenter3D(), c);

	XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&c), XMLoadFloat3(&sphere->GetCenter3D()));

	return *XMVector3Dot(v, v).m128_f32 <= sphere->GetRadius() * sphere->GetRadius();
}

bool OBB::Intersect3DSphere(BoundingSphere* sphere, XMFLOAT3& c)
{
	//find the point p on OBB closest to the sphere center
	ClosestPtPointOBB3D(sphere->GetCenter3D(), c);

	XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&c), XMLoadFloat3(&sphere->GetCenter3D()));

	return *XMVector3Dot(v, v).m128_f32 <= sphere->GetRadius() * sphere->GetRadius();
}

bool OBB::IntersectMovingSphere2D(BoundingSphere s, XMFLOAT2 d, float& t)
{
	XMVECTOR scv = XMLoadFloat2(&s.GetCenter2D());
	XMVECTOR sdv = XMLoadFloat2(&d);

	//sphere center in obb's local space
	float x = *XMVector2Dot(scv, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32;
	float y = *XMVector2Dot(scv, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32;
	XMFLOAT2 sc = XMFLOAT2(x, y);

	//sphere direction in obb's local space
	x = *XMVector2Dot(sdv, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32;
	y = *XMVector2Dot(sdv, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32;
	XMFLOAT2 sd = XMFLOAT2(x, y);

	//now the test reduces to that of a moving sphere to moving aabb
	// Compute the AABB resulting from expanding obb by sphere radius r
	AABB e = AABB(GetMin2D(), GetMax2D());
	e.Expand2D(s.GetRadius());

	// Intersect ray against expanded AABB e. Exit with no intersection if ray
	// misses e, else get intersection point p and time t as result
	XMFLOAT2 p;
	if (!e.IntersectRayAABB2D(sc, sd, t, p) || t > 1.0f)
		return false;

	// Compute which min and max faces of b the intersection point p lies
	// outside of. Note, u and v cannot have the same bits set and
	// they must have at least one bit set among them
	int u = 0, v = 0;

	if (p.x < m_f3Min.x) u |= 1;
	if (p.x > m_f3Max.x) v |= 1;
	if (p.y < m_f3Min.y) u |= 2;
	if (p.y > m_f3Max.y) v |= 2;

	// ‘Or’ all set bits together into a bit mask (note: here u + v == u | v)
	int m = u + v;

	// Define line segment [c, c+d] specified by the sphere movement
	XMStoreFloat2(&sd, XMVectorAdd(XMLoadFloat2(&sc), XMLoadFloat2(&sd)));
	LineSeg seg = LineSeg(sc, sd);

	// If both bits set (m == 3) then p is in a vertex region
	if (m == 3)
	{
		// Must now intersect segment [c, c+d] against the capsules of the three
		// edges meeting at the vertex and return the best time, if one or more hit
		float tmin = FLT_MAX;
		if (seg.IntersectCapsule2D(BoundingCapsule(Corner2D(v), Corner2D(v ^ 1), s.GetRadius()), t)) tmin = min(t, tmin);
		if (seg.IntersectCapsule2D(BoundingCapsule(Corner2D(v), Corner2D(v ^ 2), s.GetRadius()), t)) tmin = min(t, tmin);
		if (tmin = FLT_MAX) return false;	//no intersection
		t = tmin;
		return true;
	}

	if ((m & (m - 1)) == 0)
	{
		// Do nothing. Time t from intersection with
		// expanded box is correct intersection time
		return true;
	}

	

	// p is in an edge region. Intersect against the capsule at the edge
	return seg.IntersectCapsule2D(BoundingCapsule(Corner2D(u ^ 3), Corner2D(v ^ 1), s.GetRadius()), t);
}

bool OBB::IntersectMovingSphere3D(BoundingSphere s, XMFLOAT3 d, float& t)
{
	XMVECTOR scv = XMLoadFloat3(&s.GetCenter3D());
	XMVECTOR sdv = XMLoadFloat3(&d);

	//sphere center in obb's local space
	float x = *XMVector3Dot(scv, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32;
	float y = *XMVector3Dot(scv, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32;
	float z = *XMVector3Dot(scv, XMLoadFloat3(&m_vf3LocalAxis[2])).m128_f32;

	XMFLOAT3 sc = XMFLOAT3(x, y, z);

	//sphere direction in obb's local space
	x = *XMVector3Dot(sdv, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32;
	y = *XMVector3Dot(sdv, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32;
	z = *XMVector3Dot(sdv, XMLoadFloat3(&m_vf3LocalAxis[2])).m128_f32;
	
	XMFLOAT3 sd = XMFLOAT3(x, y, z);

	//now the test reduces to that of a moving sphere to moving aabb
	// Compute the AABB resulting from expanding obb by sphere radius r
	AABB e = AABB(m_f3Min, m_f3Max);
	e.Expand3D(s.GetRadius());

	// Intersect ray against expanded AABB e. Exit with no intersection if ray
	// misses e, else get intersection point p and time t as result
	XMFLOAT3 p;
	if (!e.IntersectRayAABB3D(sc, sd, t, p) || t > 1.0f)
		return false;

	// Compute which min and max faces of b the intersection point p lies
	// outside of. Note, u and v cannot have the same bits set and
	// they must have at least one bit set among them
	int u = 0, v = 0;
	
	if (p.x < m_f3Min.x) u |= 1;
	if (p.x > m_f3Max.x) v |= 1;
	if (p.y < m_f3Min.y) u |= 2;
	if (p.y > m_f3Max.y) v |= 2;
	if (p.z < m_f3Min.z) u |= 4;
	if (p.z > m_f3Max.z) v |= 4;

	// ‘Or’ all set bits together into a bit mask (note: here u + v == u | v)
	int m = u + v;

	// Define line segment [c, c+d] specified by the sphere movement
	XMStoreFloat3(&sd, XMVectorAdd(XMLoadFloat3(&sc), XMLoadFloat3(&sd)));
	LineSeg seg = LineSeg(sc, sd);
	
	// If all 3 bits set (m == 7) then p is in a vertex region
	if (m == 7)
	{
		// Must now intersect segment [c, c+d] against the capsules of the three
		// edges meeting at the vertex and return the best time, if one or more hit
		float tmin = FLT_MAX;
		if (seg.IntersectCapsule2D(BoundingCapsule(Corner3D(v), Corner3D(v ^ 1), s.GetRadius()), t)) tmin = min(t, tmin);
		if (seg.IntersectCapsule2D(BoundingCapsule(Corner3D(v), Corner3D(v ^ 2), s.GetRadius()), t)) tmin = min(t, tmin);
		if (seg.IntersectCapsule2D(BoundingCapsule(Corner3D(v), Corner3D(v ^ 4), s.GetRadius()), t)) tmin = min(t, tmin);
		if (tmin = FLT_MAX) return false;	//no intersection
		t = tmin;
		return true;
	}

	if ((m & (m - 1)) == 0)
	{
		// Do nothing. Time t from intersection with
		// expanded box is correct intersection time
		return true;
	}
	
	// p is in an edge region. Intersect against the capsule at the edge
	return seg.IntersectCapsule2D(BoundingCapsule(Corner3D(u ^ 7), Corner3D(v), s.GetRadius()), t);
}

bool OBB::IntersectMovingOBB2D(OBB* obb, XMFLOAT2 velA, XMFLOAT2 VelB, float& tfirst, float& tSecond)
{
	float eA[2] = { m_f3HalfWidths.x, m_f3HalfWidths.y };
	float eB[3] = { obb->m_f3HalfWidths.x, obb->m_f3HalfWidths.y };

	//tranlastion in parent frame
	XMVECTOR v = XMVectorSubtract(XMLoadFloat2(&VelB), XMLoadFloat2(&velA));

	//translation in A's local frame
	XMVECTOR T = XMVectorSet(*XMVector2Dot(v, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32,
		*XMVector2Dot(v, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32,
		*XMVector2Dot(v, XMLoadFloat3(&m_vf3LocalAxis[2])).m128_f32,
		1.f);

	//Obb B's basis with respect to A's local frame
	XMFLOAT3X3 R;
	float ra, rb, t;
	int i, j;

	//calculate rotation matrix
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j++)
		{
			R.m[i][j] = *XMVector3Dot(XMLoadFloat3(&m_vf3LocalAxis[i]), XMLoadFloat3(&obb->m_vf3LocalAxis[j])).m128_f32;

			//Algorithm: using the seperating axis test for all 4 potential seperating axis. If a seperating axis could
			//not be found, the two oob's overlap

			//A's basis vectors
			for (i = 0; i < 2; i++)
			{
				ra = eA[i];
				rb = eB[0] * abs(R.m[i][0]) + eB[1] * abs(R.m[i][1]);

				t = abs(T.m128_f32[i]);
				if (t > ra + rb) return false;
			}

			//B's basis vector
			for (j = 0; j < 2; j++)
			{
				ra = eA[0] * abs(R.m[0][j]) + eA[1] * abs(R.m[1][j]);
				rb = eB[j];

				t = abs(T.m128_f32[0] * R.m[0][j] + T.m128_f32[1] * R.m[1][j]);
				if (t > ra + rb) return false;
			}
		}
	}

	//no seperating axis found they must be colliding
	return true;
}

bool OBB::IntersectMovingOBB3D(OBB* obb, XMFLOAT3 velA, XMFLOAT3 VelB, float& tfirst, float& tSecond)
{
	float eA[3] = { m_f3HalfWidths.x, m_f3HalfWidths.y, m_f3HalfWidths.z };
	float eB[3] = { obb->m_f3HalfWidths.x, obb->m_f3HalfWidths.y, obb->m_f3HalfWidths.z };

	//tranlastion in parent frame
	XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&VelB), XMLoadFloat3(&velA));
	
	//translation in A's local frame
	XMVECTOR T = XMVectorSet(*XMVector3Dot(v, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32,
		*XMVector3Dot(v, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32,
		*XMVector3Dot(v, XMLoadFloat3(&m_vf3LocalAxis[2])).m128_f32,
		1.f);


	//Obb B's basis with respect to A's local frame
	XMFLOAT3X3 R;
	float ra, rb, t;
	int i, j;

	//calculate rotation matrix
	for ( i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			R.m[i][j] = *XMVector3Dot(XMLoadFloat3(&m_vf3LocalAxis[i]), XMLoadFloat3(&obb->m_vf3LocalAxis[j])).m128_f32;
		
			//Algorithm: using the seperating axis test for all 15 potential seperating axis. If a seperating axis could
			//not be found, the two oob's overlap

			//A's basis vectors
			for (i = 0; i < 3; i++)
			{
				ra = eA[i];
				rb = eB[0] * abs(R.m[i][0]) + eB[1] * abs(R.m[i][1]) + eB[2] * abs(R.m[i][2]);

				t = abs(T.m128_f32[i]);
				if (t > ra + rb) return false;
			}

			//B's basis vector
			for ( j = 0; j < 3; j++)
			{
				ra = eA[0] * abs(R.m[0][j]) + eA[1] * abs(R.m[1][j]) + eA[2] * abs(R.m[2][j]);
				rb = eB[j];

				t = abs(T.m128_f32[0] * R.m[0][j] + T.m128_f32[1] * R.m[1][j] + T.m128_f32[2] * R.m[2][j]);
				if (t > ra + rb) return false;
			}
		}
	}

	//9 cross products
	//L = A0 x B0
	ra = eA[1] * abs(R.m[2][0]) + eA[2] * abs(R.m[1][0]);
	rb = eB[1] * abs(R.m[0][2]) + eB[2] * abs(R.m[0][1]);
	t =	abs(T.m128_f32[2] * R.m[1][0] - T.m128_f32[1] * R.m[2][0]);

	if (t > ra + rb) return false;

	//L = A0 x B1
	ra =eA[1] * abs(R.m[2][1]) + eA[2] * abs(R.m[1][1]); 
	rb = eB[0] * abs(R.m[0][2]) + eB[2] * abs(R.m[0][0]); 
	t = abs(T.m128_f32[2] * R.m[1][1] - T.m128_f32[1] * R.m[2][1]);

	if (t > ra + rb) return false;

	//L = A0 x B2
	ra = eA[1] * abs(R.m[2][2]) + eA[2] * abs(R.m[1][2]);
	rb = eB[0] * abs(R.m[0][1]) + eB[1] * abs(R.m[0][0]);
	t = abs(T.m128_f32[2] * R.m[1][2] - T.m128_f32[1] * R.m[2][2]);

	if (t > ra + rb)return false;

	//L = A1 x B0
	ra = eA[0] * abs(R.m[2][0]) + eA[2] * abs(R.m[0][0]);
	rb = eB[1] * abs(R.m[1][2]) + eB[2] * abs(R.m[1][1]);
	t = abs(T.m128_f32[0] * R.m[2][0] - T.m128_f32[2] * R.m[0][0]);

	if (t > ra + rb) return false;

	//L = A1 x B1
	ra = eA[0] * abs(R.m[2][1]) + eA[2] * abs(R.m[0][1]);
	rb = eB[0] * abs(R.m[1][2]) + eB[2] * abs(R.m[1][0]);
	t = abs(T.m128_f32[0] * R.m[2][1] - T.m128_f32[2] * R.m[0][1]);

	if (t > ra + rb) return false;

	//L = A1 x B2
	ra = eA[0] * abs(R.m[2][2]) + eA[2] * abs(R.m[0][2]);
	rb = eB[0] * abs(R.m[1][1]) + eB[1] * abs(R.m[1][0]);
	t = abs(T.m128_f32[0] * R.m[2][2] - T.m128_f32[2] * R.m[0][2]);

	if (t > ra + rb) return false;

	//L = A2 x B0
	ra = eA[0] * abs(R.m[1][0]) + eA[1] * abs(R.m[0][0]);
	rb = eB[1] * abs(R.m[2][2]) + eB[2] * abs(R.m[2][1]);
	t = abs(T.m128_f32[1] * R.m[0][0] - T.m128_f32[0] * R.m[1][0]);

	if (t > ra + rb) return false;

	//L = A2 x B1
	ra = eA[0] * abs(R.m[1][1]) + eA[1] * abs(R.m[0][1]);
	rb = eB[0] * abs(R.m[2][2]) + eB[2] * abs(R.m[2][0]);
	t = abs(T.m128_f32[1] * R.m[0][1] - T.m128_f32[0] * R.m[1][1]);

	if (t > ra + rb) return false;

	//L = A2 x B2
	ra = eA[0] * abs(R.m[1][2]) + eA[1] * abs(R.m[0][2]);
	rb = eB[0] * abs(R.m[2][1]) + eB[1] * abs(R.m[2][0]);
	t = abs(T.m128_f32[1] * R.m[0][2] - T.m128_f32[0] * R.m[1][2]);

	if (t > ra + rb) return false;

	//no seperating axis found the two boxes overlap
	return true;
}

bool OBB::IntersectOBB2D(LineSeg* lineSeg)
{
	XMVECTOR bcv = XMVectorSubtract(XMLoadFloat2(&lineSeg->GetBottom2D()), XMLoadFloat2(&GetCenter2D()));
	XMVECTOR tcv = XMVectorSubtract(XMLoadFloat2(&lineSeg->GetTop2D()), XMLoadFloat2(&GetCenter2D()));

	//bottom in Box's local frame
	XMVECTOR B = XMVectorSet(*XMVector2Dot(bcv, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32,
		*XMVector2Dot(bcv, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32,
		1.f,
		1.f);

	//top in Box's local frame
	XMVECTOR T = XMVectorSet(*XMVector3Dot(tcv, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32,
		*XMVector3Dot(tcv, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32,
		1.f,
		1.f);

	XMFLOAT2 c = GetCenter2D();
	XMFLOAT2 e = GetExtent2D();
	XMFLOAT2 m;	XMStoreFloat2(&m, XMVectorScale(XMVectorAdd(B, T), 0.5f));

	XMFLOAT2 d;	XMStoreFloat2(&d, XMVectorSubtract(T, XMLoadFloat2(&m)));

	XMStoreFloat2(&m, XMVectorSubtract(XMLoadFloat2(&m), XMLoadFloat2(&c)));

	//try the world coordinate axes as seperating axes
	float adx = abs(*XMVector2Dot(XMLoadFloat2(&d), XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32);
	if (abs(m.x) > e.x + adx) return false;

	float ady = abs(*XMVector2Dot(XMLoadFloat2(&d), XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32);
	if (abs(m.y) > e.y + ady) return false;

	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis
	adx += FLT_EPSILON;		ady += FLT_EPSILON;	

	// Try cross products of segment direction vector with coordinate axes
	if (abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return false;

	// No separating axis found; segment must be overlapping AABB
	return true;
}

bool OBB::IntersectOBB3D(LineSeg* lineSeg)
{
	XMVECTOR bcv = XMVectorSubtract(XMLoadFloat3(&lineSeg->GetBottom3D()),XMLoadFloat3(&m_f3Center));
	XMVECTOR tcv = XMVectorSubtract(XMLoadFloat3(&lineSeg->GetTop3D()), XMLoadFloat3(&m_f3Center));

	//bottom in Box's local frame
	XMVECTOR B = XMVectorSet(*XMVector3Dot(bcv, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32,
		*XMVector3Dot(bcv, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32,
		*XMVector3Dot(bcv, XMLoadFloat3(&m_vf3LocalAxis[2])).m128_f32,
		1.f);

	//top in Box's local frame
	XMVECTOR T = XMVectorSet(*XMVector3Dot(tcv, XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32,
		*XMVector3Dot(tcv, XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32,
		*XMVector3Dot(tcv, XMLoadFloat3(&m_vf3LocalAxis[2])).m128_f32,
		1.f);
	
	XMFLOAT3 c = m_f3Center;
	XMFLOAT3 e = m_f3HalfWidths;
	XMFLOAT3 m;	XMStoreFloat3(&m, XMVectorScale(XMVectorAdd(B, T), 0.5f));

	XMFLOAT3 d;	XMStoreFloat3(&d, XMVectorSubtract(T, XMLoadFloat3(&m)));

	XMStoreFloat3(&m, XMVectorSubtract(XMLoadFloat3(&m), XMLoadFloat3(&m_f3Center)));

	//try the world coordinate axes as seperating axes
	float adx = abs(*XMVector3Dot(XMLoadFloat3(&d), XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32);		
	if (abs(m.x) > e.x + adx) return false;

	float ady = abs(*XMVector3Dot(XMLoadFloat3(&d), XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32);		
	if (abs(m.y) > e.y + ady) return false;

	float adz = abs(*XMVector3Dot(XMLoadFloat3(&d), XMLoadFloat3(&m_vf3LocalAxis[2])).m128_f32);		
	if (abs(m.z) > e.z + adz) return false;

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

bool OBB::IntersectRayOBBIntersection2D(XMFLOAT2 origin, XMFLOAT2 direction, float& tmin)
{
	tmin = 0.f;
	float tmax = FLT_MAX;
	
	XMFLOAT3X3 model;
	XMStoreFloat3x3(&model, m_ModelMAt);
	
	XMVECTOR delta = XMVectorSubtract(XMLoadFloat2(&XMFLOAT2(model._31, model._32)), XMLoadFloat2(&origin));
	
	//xaxis
	XMVECTOR xAxis = XMLoadFloat2(&XMFLOAT2(model._11, model._12));
	float e = *XMVector2Dot(xAxis, delta).m128_f32;
	float f = *XMVector2Dot(XMLoadFloat2(&direction), xAxis).m128_f32;
	
	// Beware, don't do the division if f is near 0
	float t1 = (e + m_f3Min.x) / f;		//intersection with the left plane
	float t2 = (e + m_f3Max.x) / f;		//intersection with the right plane
	
	if (t1 > t2) swap(t1, t2); // swap t1 and t2
	
	if (t2 < tmax) tmax = t2;
	if (t1 > tmin)	tmin = t1;
	
	if (tmax < tmin)	return false;
	
	//yaxis
	XMVECTOR yAxis = XMLoadFloat2(&XMFLOAT2(model._21, model._22));
	e = *XMVector2Dot(yAxis, delta).m128_f32;
	f = *XMVector2Dot(XMLoadFloat2(&direction), yAxis).m128_f32;
	
	// Beware, don't do the division if f is near 0
	t1 = (e + m_f3Min.y) / abs(f);		//intersection with the left plane
	t2 = (e + m_f3Max.y) / abs(f);		//intersection with the right plane
	
	if (t1 > t2) swap(t1, t2); // swap t1 and t2
	
	if (t2 < tmax) tmax = t2;
	if (t1 > tmin)	tmin = t1;
	
	if (tmax < tmin)	return false;
	
	return true;
	
}

bool OBB::IntersectRayOBBIntersection3D(XMFLOAT3 origin, XMFLOAT3 direction, float& tmin)
{
	tmin = 0.f;
	float tmax = FLT_MAX;

	XMMATRIX trans= DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&m_vf3CornerPoints[0]));
	XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&XMFLOAT3(m_vf3LocalAxis[0].x, m_vf3LocalAxis[1].y, m_vf3LocalAxis[2].z)));
	XMFLOAT4X4 model;
	XMStoreFloat4x4(&model, m_ModelMAt);

	
	XMVECTOR delta = XMVectorSubtract(XMLoadFloat3(&XMFLOAT3(model._41, model._42, model._43)), XMLoadFloat3(&origin));
	
	//xaxis
	XMVECTOR xAxis = XMLoadFloat3(&XMFLOAT3(model._11, model._12, model._13));
	float e = *XMVector3Dot(xAxis, delta).m128_f32;
	float f = *XMVector3Dot(XMLoadFloat3(&direction), xAxis).m128_f32;

	// Beware, don't do the division if f is near 0
	float t1 = (e + m_f3Min.x) / f;		//intersection with the left plane
	float t2 = (e + m_f3Max.x) / f;		//intersection with the right plane

	if (t1 > t2) swap(t1, t2); // swap t1 and t2
	
	if (t2 < tmax) tmax = t2;
	if (t1 > tmin)	tmin = t1;

	if (tmax < tmin)	return false;

	//yaxis
	XMVECTOR yAxis = XMLoadFloat3(&XMFLOAT3(model._21, model._22, model._23));
	e = *XMVector3Dot(yAxis, delta).m128_f32;
	f = *XMVector3Dot(XMLoadFloat3(&direction), yAxis).m128_f32;

	// Beware, don't do the division if f is near 0
	t1 = (e + m_f3Min.y) / abs(f);		//intersection with the left plane
	t2 = (e + m_f3Max.y) / abs(f);		//intersection with the right plane

	if (t1 > t2) swap(t1, t2); // swap t1 and t2

	if (t2 < tmax) tmax = t2;
	if (t1 > tmin)	tmin = t1;

	if (tmax < tmin)	return false;

	//zaxis
	XMVECTOR zAxis = XMLoadFloat3(&XMFLOAT3(model._31, model._32, model._33));
	e = *XMVector3Dot(zAxis, delta).m128_f32;
	f = *XMVector3Dot(XMLoadFloat3(&direction), zAxis).m128_f32;

	// Beware, don't do the division if f is near 0
	t1 = (e + m_f3Min.z) / abs(f);		//intersection with the left plane
	t2 = (e + m_f3Max.z) / abs(f);		//intersection with the right plane

	if (t1 > t2) swap(t1, t2); // swap t1 and t2

	if (t2 < tmax) tmax = t2;
	if (t1 > tmin)	tmin = t1;

	if (tmax < tmin)	return false;

	return true;
}

XMFLOAT2 OBB::Corner2D(int n)
{
	float pX, pY;
	pX = (n & 1) ? m_f3Max.x : m_f3Min.x;
	pY = (n & 1) ? m_f3Max.y : m_f3Min.y;

	return XMFLOAT2(pX, pY);
}

XMFLOAT3 OBB::Corner3D(int n)
{
	float pX, pY, pZ;
	pX = (n & 1) ? m_f3Max.x : m_f3Min.x;
	pY = (n & 1) ? m_f3Max.y : m_f3Min.y;
	pZ = (n & 1) ? m_f3Max.z : m_f3Min.z;

	return XMFLOAT3(pX, pY, pZ);
}

float OBB::SqDistPointOBB2D(XMFLOAT2 p)
{
	XMFLOAT2 c;
	ClosestPtPointOBB2D(p, c);
	
	return *XMVector2Dot(XMVectorSubtract(XMLoadFloat2(&c), XMLoadFloat2(&p)),
		XMVectorSubtract(XMLoadFloat2(&c), XMLoadFloat2(&p))).m128_f32;
}

float OBB::SqDistPointOBB3D(XMFLOAT3 p)
{
	XMFLOAT3 c;
	ClosestPtPointOBB3D(p, c);

	return *XMVector3Dot(XMVectorSubtract(XMLoadFloat3(&c), XMLoadFloat3(&p)),
		XMVectorSubtract(XMLoadFloat3(&c), XMLoadFloat3(&p))).m128_f32;
}

void OBB::ClosestPtPointOBB2D(XMFLOAT2 p, XMFLOAT2& q)
{
	XMVECTOR d = XMVectorSubtract(XMLoadFloat2(&p), XMLoadFloat2(&GetCenter2D()));
	//start results at the center of box; make steps from there

	XMVECTOR qv = XMLoadFloat2(&GetCenter2D());

	float e[2] = { m_f3HalfWidths.x, m_f3HalfWidths.y };

	for (int i = 0; i < 2; i++)
	{
		// ...project d onto that axis to get the distance
		// along the axis of d from the box center
		float dist = *XMVector2Dot(d, XMLoadFloat3(&m_vf3LocalAxis[i])).m128_f32;
		if (dist > e[i]) dist = e[i];
		if (dist < -e[i]) dist = -e[i];

		//step that distance along the acis to get the world coordinate
		qv += XMVectorMultiply(XMLoadFloat(&dist), XMLoadFloat3(&m_vf3LocalAxis[i]));
	}

	XMStoreFloat2(&q, qv);
}

void OBB::ClosestPtPointOBB3D(XMFLOAT3 p, XMFLOAT3& q)
{
	XMVECTOR d = XMVectorSubtract(XMLoadFloat3(&p), XMLoadFloat3(&m_f3Center));
	//start results at the center of box; make steps from there

	XMVECTOR qv = XMLoadFloat3(&m_f3Center);

	float e[3] = { m_f3HalfWidths.x, m_f3HalfWidths.y, m_f3HalfWidths.z };

	for (int i = 0; i < 3; i++)
	{
		// ...project d onto that axis to get the distance
		// along the axis of d from the box center
		float dist = *XMVector3Dot(d, XMLoadFloat3(&m_vf3LocalAxis[i])).m128_f32;
		if (dist > e[i]) dist = e[i];
		if (dist < -e[i]) dist = -e[i];

		//step that distance along the acis to get the world coordinate
		qv +=	XMVectorMultiply(XMLoadFloat(&dist), XMLoadFloat3(&m_vf3LocalAxis[i]));
	}

	XMStoreFloat3(&q, qv);
}

bool OBB::IntersectPlane2D(Plane p)
{
	
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	float r = m_f3HalfWidths.x * abs(*XMVector2Dot(XMLoadFloat2(&p.Get2DNorm()), XMLoadFloat2(&XMFLOAT2(m_vf3LocalAxis[0].x, m_vf3LocalAxis[0].y))).m128_f32) +
		m_f3HalfWidths.y * abs(*XMVector3Dot(XMLoadFloat3(&p.Get3DNorm()), XMLoadFloat2(&XMFLOAT2(m_vf3LocalAxis[0].x, m_vf3LocalAxis[0].y))).m128_f32);

	// Compute distance of box center from plane
	float s = *XMVector2Dot(XMLoadFloat2(&p.Get2DNorm()), XMLoadFloat2(&XMFLOAT2(m_f3Center.x, m_f3Center.y))).m128_f32 - p.GetDistance();

	// Intersection occurs when distance s falls within [-r,+r] interval
	return abs(s) <= r;
}

bool OBB::IntersectPlane3D(Plane p)
{
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	float r = m_f3HalfWidths.x * abs(*XMVector3Dot(XMLoadFloat3(&p.Get3DNorm()), XMLoadFloat3(&m_vf3LocalAxis[0])).m128_f32) +
		m_f3HalfWidths.y * abs(*XMVector3Dot(XMLoadFloat3(&p.Get3DNorm()), XMLoadFloat3(&m_vf3LocalAxis[1])).m128_f32) +
		m_f3HalfWidths.z * abs(*XMVector3Dot(XMLoadFloat3(&p.Get3DNorm()), XMLoadFloat3(&m_vf3LocalAxis[2])).m128_f32);

	// Compute distance of box center from plane
	float s = *XMVector3Dot(XMLoadFloat3(&p.Get3DNorm()), XMLoadFloat3(&m_f3Center)).m128_f32 - p.GetDistance();

	// Intersection occurs when distance s falls within [-r,+r] interval
	return abs(s) <= r;
}
