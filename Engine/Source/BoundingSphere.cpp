#include "../Headers/BoundingSphere.h"

BoundingSphere::BoundingSphere(BVType type, bool is3Dim)
	:BoundingVolume(type,is3Dim)
{
	m_fRadius = 0.f;
}

BoundingSphere::BoundingSphere(bool is3Dim, XMFLOAT2 center, float radius)
	: BoundingVolume(BVType::Sphere2D, is3Dim)
{
	m_bIs3D = is3Dim;
	m_f3Center = XMFLOAT3(center.x, center.y, 1.f);
	m_fRadius = radius;
}

BoundingSphere::BoundingSphere(bool is3Dim, XMFLOAT3 center, float radius)
	:BoundingVolume(BVType::Sphere3D, is3Dim)
{
	m_bIs3D = is3Dim;
	m_f3Center = center;
	m_fRadius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere& other)
	:BoundingVolume(other.m_bvType, other.m_bIs3D)
{
	m_f3Center = m_f3Center;
	m_f3Size = other.m_f3Size;
	m_f3PrevSize = other.m_f3PrevSize;
	m_f3HalfWidths = other.m_f3HalfWidths;
	m_f3Padding = other.m_f3Padding;
	m_f3Percentile = other.m_f3Percentile;

	m_fRadius = other.m_fRadius;
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::Update(XMFLOAT2 Pos, XMFLOAT2 Size)
{
	m_fRadius = (Size.x * 0.5f) * m_f3Percentile.x;
	m_f3Size = XMFLOAT3(m_fRadius, m_fRadius, m_fRadius);
	
	m_f3Center.x = (Pos.x + m_f3Padding.x) + m_fRadius;
	m_f3Center.y = (Pos.y + m_f3Padding.y) + m_fRadius;
}

void BoundingSphere::Update(XMFLOAT3 Pos, XMFLOAT3 Size)
{
	
	m_fRadius = (Size.x * 0.5f) * m_f3Percentile.x;
	m_f3Size = XMFLOAT3(m_fRadius, m_fRadius, m_fRadius);

	m_f3Center.x = (Pos.x + m_f3Padding.x) + m_fRadius;
	m_f3Center.y = (Pos.y + m_f3Padding.y) + m_fRadius;
	m_f3Center.z = (Pos.z + m_f3Padding.z) + m_fRadius;
}

bool BoundingSphere::Contains2DPoint(XMFLOAT2 point)
{
	// Calculate squared distance between centers
	XMVECTOR d = XMVectorSubtract(XMLoadFloat2(&point), XMLoadFloat2(&GetCenter2D()));

	float distSquared = *XMVector2Dot(d, d).m128_f32;

	return SetIsCollided(distSquared < GetRadius()* GetRadius());
}

bool BoundingSphere::Contains3DPoint(XMFLOAT3 point)
{
	// Calculate squared distance between centers
	XMVECTOR d = XMVectorSubtract(XMLoadFloat3(&point), XMLoadFloat3(&GetCenter3D()));

	float distSquared = *XMVector3Dot(d, d).m128_f32;

	return SetIsCollided(distSquared < GetRadius() * GetRadius());
}

bool BoundingSphere::IntersectTriangle2D(Triangle tri)
{
	//find the closest point P on triangle ABC closest to the sphere center
	XMFLOAT2 p = ClosestPtPointTriangle(GetCenter2D(), tri);

	//find the squared distance from sphere center to the closest point
	XMVECTOR v = XMVectorSubtract(XMLoadFloat2(&p), XMLoadFloat2(&GetCenter2D()));

	//if the squared distance is less than the radius squared there is an interection
	return SetIsCollided(*XMVector2Dot(v, v).m128_f32 < GetRadius() * GetRadius());
}

bool BoundingSphere::IntersectTriangle3D(Triangle tri)
{
	//find the closest point P on triangle ABC closest to the sphere center
	XMFLOAT3 p = ClosestPtPointTriangle(GetCenter3D(), tri);

	//find the squared distance from sphere center to the closest point
	XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&p), XMLoadFloat3(&GetCenter3D()));

	//if the squared distance is less than the radius squared there is an interection
	return SetIsCollided(*XMVector3Dot(v, v).m128_f32 < GetRadius() * GetRadius());
}

bool BoundingSphere::IntersectSphere2D(BoundingSphere* sphere2)
{
	// Calculate squared distance between centers
	XMVECTOR d = XMVectorSubtract(XMLoadFloat2(&GetCenter2D()), XMLoadFloat2(&sphere2->GetCenter2D()));
	float distSquared = *XMVector2Dot(d, d).m128_f32;

	float radiusSum = GetRadius() + sphere2->GetRadius();

	//intersection if distSquared is less than the sum of the radii
	return SetIsCollided(sphere2->SetIsCollided(distSquared < radiusSum * radiusSum));
}

bool BoundingSphere::IntersectSphere3D(BoundingSphere* sphere2)
{
	// Calculate squared distance between centers
	XMVECTOR d = XMVectorSubtract(XMLoadFloat3(&GetCenter3D()), XMLoadFloat3(&sphere2->GetCenter3D()));
	float distSquared = *XMVector3Dot(d, d).m128_f32;

	float radiusSum = GetRadius() + sphere2->GetRadius();

	//intersection if distSquared is less than the sum of the radii
	return SetIsCollided(sphere2->SetIsCollided(distSquared < radiusSum * radiusSum));
}

bool BoundingSphere::IntersectMovingSphere2D(BoundingSphere* sphere2, XMFLOAT2 velA, XMFLOAT2 velB, float& t)
{
	//expand sphere2 by the radius of sphere1
	BoundingSphere s2 = BoundingSphere(false, sphere2->GetCenter2D(), GetRadius() + sphere2->GetRadius());
	XMVECTOR v = XMVectorSubtract(XMLoadFloat2(&velA), XMLoadFloat2(&velB));
	float vLen = *XMVector2Length(v).m128_f32;
	XMFLOAT2 q;

	XMFLOAT2 vf2; XMStoreFloat2(&vf2, v / vLen);

	if (s2.IntersectRay2D(GetCenter2D(), vf2, t, q))
	{
		return sphere2->SetIsCollided(SetIsCollided(t <= vLen));
	}

	return sphere2->SetIsCollided(SetIsCollided(false));
}

bool BoundingSphere::IntersectMovingSphere3D(BoundingSphere* sphere2, XMFLOAT3 velA, XMFLOAT3 velB, float& t)
{
	//expand sphere2 by the radius of sphere1
	BoundingSphere s2 = BoundingSphere(false, sphere2->GetCenter3D(), GetRadius() + sphere2->GetRadius());
	XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&velA), XMLoadFloat3(&velB));
	float vLen = *XMVector3Length(v).m128_f32;
	XMFLOAT3 q;

	XMFLOAT3 vf3; XMStoreFloat3(&vf3, v / vLen);

	if (s2.IntersectRay3D(GetCenter3D(), vf3, t, q))
	{
		return sphere2->SetIsCollided(SetIsCollided(t <= vLen));
	}

	return sphere2->SetIsCollided(SetIsCollided(false));
}

//for a normalised plane (|plane.normal| = 1, determine whether plane p intersects sphere s
bool BoundingSphere::IntersectPlane2D(Plane plane)
{
	// for a point gives the signed distance of the point to the plane
	float dist = *XMVector2Dot(XMLoadFloat2(&GetCenter2D()), XMLoadFloat2(&plane.Get2DNorm())).m128_f32 - plane.GetDistance();

	// If sphere center within +/-radius from plane, plane intersects sphere
	return fabs(dist) <= GetRadius();
}

//for a normalised plane (|plane.normal| = 1, determine whether plane p intersects sphere s
bool BoundingSphere::IntersectPlane3D(Plane plane)
{
	// for a point gives the signed distance of the point to the plane
	float dist = *XMVector3Dot(XMLoadFloat3(&GetCenter3D()), XMLoadFloat3(&plane.Get3DNorm())).m128_f32 - plane.GetDistance();

	// If sphere center within +/-radius from plane, plane intersects sphere
	return fabs(dist) <= GetRadius();
}

bool BoundingSphere::IntersectMovingSphere2Plane2D(XMFLOAT2 v, Plane p, float& t, XMFLOAT2& q)
{
	//computer the distance of sphere center to the plane
	float dist = *XMVector2Dot(XMLoadFloat2(&p.Get2DNorm()), XMLoadFloat2(&GetCenter2D())).m128_f32 - p.GetDistance();
	if (fabs(dist) <= GetRadius())
	{
		//the sphere is already overlapping the plane
		t = 0.f;	q = GetCenter2D();
		return true;
	}
	else
	{
		float denom = *XMVector2Dot(XMLoadFloat2(&p.Get2DNorm()), XMLoadFloat2(&v)).m128_f32;
		if (denom * dist >= 0.f) return false;		//sphere moving parallel to or away from the plane
		else
		{
			//sphere is moving towards the plane
			//use +r in computation if sphere in from of plane, else - r
			float r = dist > 0.f ? GetRadius() : -GetRadius();
			t = (r - dist) / denom;

			//q = sphere.center + t * v - r * plane.normal
			XMStoreFloat2(&q, XMLoadFloat2(&GetCenter2D()) + t * XMLoadFloat2(&v) - r * XMLoadFloat2(&p.Get2DNorm()));

			return true;
		}
	}
}

bool BoundingSphere::IntersectMovingSphere2Plane3D(XMFLOAT3 v, Plane p, float& t, XMFLOAT3& q)
{
	//computer the distance of sphere center to the plane
	float dist = *XMVector3Dot(XMLoadFloat3(&p.Get3DNorm()), XMLoadFloat3(&GetCenter3D())).m128_f32 - p.GetDistance();
	if (fabs(dist) <= GetRadius())
	{
		//the sphere is already overlapping the plane
		t = 0.f;	q = GetCenter3D();
		return true;
	}
	else
	{
		float denom = *XMVector3Dot(XMLoadFloat3(&p.Get3DNorm()), XMLoadFloat3(&v)).m128_f32;
		if (denom * dist >= 0.f) return false;		//sphere moving parallel to or away from the plane
		else
		{
			//sphere is moving towards the plane
			//use +r in computation if sphere in from of plane, else - r
			float r = dist > 0.f ? GetRadius() : -GetRadius();
			t = (r - dist) / denom;

			//q = sphere.center + t * v - r * plane.normal
			XMStoreFloat3(&q, XMLoadFloat3(&GetCenter3D()) + t * XMLoadFloat3(&v) - r * XMLoadFloat3(&p.Get3DNorm()));
			return true;
		}
	}
}

bool BoundingSphere::IntersectLineSeg2D(LineSeg lineSeg, float& t, XMFLOAT2& q)
{
	//p = a
	XMFLOAT2 p = lineSeg.GetBottom2D();
	//d = b - a / |b-a|
	XMVECTOR ba = XMVectorSubtract(XMLoadFloat2(&lineSeg.GetTop2D()), XMLoadFloat2(&p));
	float baLen = *XMVector3Length(ba).m128_f32;
	XMFLOAT2 d; XMStoreFloat2(&d, ba / baLen);

	XMVECTOR m = XMVectorSubtract(XMLoadFloat2(&p), XMLoadFloat2(&GetCenter2D()));
	float b = *XMVector2Dot(m, XMLoadFloat2(&d)).m128_f32;
	float c = *XMVector2Dot(m, m).m128_f32 - GetRadius() * GetRadius();

	//early exit if the origin is outside the s(c > 0) and r pointing away from s (b > 0)
	if (c > 0.f && b > 0.f) return SetIsCollided(false);

	float discr = b * b - c;
	if (discr < 0.f) return SetIsCollided(false); //if discrement is negative ray misses sphere

	//ray intersects sphere, compute the smallest t value of intersection
	t = -b - sqrtf(discr);

	if (t < 0.f) t = 0.f;	// if t is negative, ray started inside sphere clamp to to zero
	if (t > baLen) return SetIsCollided(false); //t ≤ |B − A| so that the detected intersection does not lie beyond the end of the segment

	//q = p + t * d
	XMStoreFloat2(&q, XMVectorAdd(XMLoadFloat2(&p), XMVectorScale(XMLoadFloat2(&d), t)));

	return SetIsCollided(true);
}

bool BoundingSphere::IntersectLineSeg3D(LineSeg lineSeg, float& t, XMFLOAT3& q)
{
	//p = a
	XMFLOAT3 p = lineSeg.GetBottom3D();
	//d = b - a / |b-a|
	XMVECTOR ba = XMVectorSubtract(XMLoadFloat3(&lineSeg.GetTop3D()), XMLoadFloat3(&p));
	float baLen = *XMVector3Length(ba).m128_f32;
	XMFLOAT3 d; XMStoreFloat3(&d, ba / baLen);

	XMVECTOR m = XMVectorSubtract(XMLoadFloat3(&p), XMLoadFloat3(&GetCenter3D()));
	float b = *XMVector3Dot(m, XMLoadFloat3(&d)).m128_f32;
	float c = *XMVector3Dot(m, m).m128_f32 - GetRadius() * GetRadius();

	//early exit is if the origin is outside the s(c > 0) and r pointing away from s (b > 0)
	if (c > 0.f && b > 0.f) return SetIsCollided(false);

	float discr = b * b - c;
	if (discr < 0.f) return SetIsCollided(false); //if discrement is negative ray misses sphere

	//ray intersects sphere, compute the smallest t value of intersection
	t = -b - sqrtf(discr);

	if (t < 0.f) t = 0.f;		// if t is negative, ray started inside sphere clamp to to zero
	if (t <= baLen)	SetIsCollided(false);	//t <= |B-A| so that the intersection doesnt go beyond the end of the segment

	//q = p + t * d
	XMStoreFloat3(&q, XMVectorAdd(XMLoadFloat3(&p), XMVectorScale(XMLoadFloat3(&d), t)));

	return SetIsCollided(true);
}



bool BoundingSphere::IntersectRay2D(XMFLOAT2 p, XMFLOAT2 d, float& t, XMFLOAT2& q)
{
	XMVECTOR m = XMVectorSubtract(XMLoadFloat2(&p), XMLoadFloat2(&GetCenter2D()));
	float b = *XMVector2Dot(m, XMLoadFloat2(&d)).m128_f32;
	float c = *XMVector2Dot(m, m).m128_f32 - GetRadius() * GetRadius();

	//early exit if the origin is outside the s(c > 0) and r pointing away from s (b > 0)
	if (c > 0.f && b > 0.f) return SetIsCollided(false);

	float discr = b * b - c;
	if (discr < 0.f) return SetIsCollided(false); //if discrement is negative ray misses sphere

	//ray intersects sphere, compute the smallest t value of intersection
	t = -b - sqrtf(discr);

	if (t < 0.f) t = 0.f;	// if t is negative, ray started inside sphere clamp to to zero

	//q = p + t * d
	XMStoreFloat2(&q, XMVectorAdd(XMLoadFloat2(&p), XMVectorScale(XMLoadFloat2(&d), t)));

	return SetIsCollided(true);
}

bool BoundingSphere::IntersectRay3D(XMFLOAT3 p, XMFLOAT3 d, float& t, XMFLOAT3& q)
{
	XMVECTOR m = XMVectorSubtract(XMLoadFloat3(&p), XMLoadFloat3(&GetCenter3D()));
	float b = *XMVector3Dot(m, XMLoadFloat3(&d)).m128_f32;
	float c = *XMVector3Dot(m, m).m128_f32 - GetRadius() * GetRadius();

	//early exit if the origin is outside the s(c > 0) and r pointing away from s (b > 0)
	if (c > 0.f && b > 0.f) return false;

	float discr = b * b - c;
	if (discr < 0.f) return false; //if discrement is negative ray misses sphere

	//ray intersects sphere, compute the smallest t value of intersection
	t = -b - sqrtf(discr);

	if (t < 0.f) t = 0.f;	// if t is negative, ray started inside sphere clamp to to zero

	//q = p + t * d
	XMStoreFloat3(&q, XMVectorAdd(XMLoadFloat3(&p), XMVectorScale(XMLoadFloat3(&d), t)));

	return true;
}

XMFLOAT2 BoundingSphere::ClosestPtPointTriangle(XMFLOAT2 pnt, Triangle tri)
{
	XMFLOAT2 tmp;
	XMFLOAT2 pnts[3] = { tri.Get2DPntByIdx(0),tri.Get2DPntByIdx(1), tri.Get2DPntByIdx(2),};
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat2(&pnts[1]), XMLoadFloat2(&pnts[0]));
	XMVECTOR ac = XMVectorSubtract(XMLoadFloat2(&pnts[2]), XMLoadFloat2(&pnts[0]));
	XMVECTOR ap = XMVectorSubtract(XMLoadFloat2(&pnt), XMLoadFloat2(&pnts[0]));

	float d1 = *XMVector2Dot(ab, ap).m128_f32;		float d2 = *XMVector2Dot(ac, ap).m128_f32;
	//check if p is in the vertex region outside A
	if (d1 <= 0.f && d2 <= 0.f)	return pnts[0];		//barycentric coordinates (1,0,0)

	XMVECTOR bp = XMVectorSubtract(XMLoadFloat2(&pnt), XMLoadFloat2(&pnts[1]));
	float d3 = *XMVector2Dot(ab, bp).m128_f32;		float d4 = *XMVector2Dot(ac, bp).m128_f32;
	//check if p is in the vertex region outside B
	if (d3 >= 0.f && d4 <= d3)	return pnts[1];		//barycentric coordinates (0,1,0)

	float vc = d1 * d4 - d3 * d2;
	//check if p is in the edge region of AB, if so return projection of p onto AB
	if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f)
	{
		float v = (d1 / (d1 - d3));
		XMStoreFloat2(&tmp, XMLoadFloat2(&pnts[0]) + v * ab);
		return tmp;									//barycentric coordinates (1-v,v,0)
	}

	XMVECTOR cp = XMVectorSubtract(XMLoadFloat2(&pnt), XMLoadFloat2(&pnts[2]));
	float d5 = *XMVector2Dot(ab, cp).m128_f32;		float d6 = *XMVector2Dot(ac, cp).m128_f32;
	//check if p is in the vertex region outside C
	if (d6 >= 0.f && d5 <= d3)	return pnts[2];		//barycentric coordinates (0,1,0)

	float vb = d5 * d2 - d1 * d6;
	// Check if P in edge region of AC, if so return projection of P onto AC
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		XMStoreFloat2(&tmp, XMLoadFloat2(&pnts[0]) + w * ac); // barycentric coordinates (1-w,0,w)
		return tmp;
	}

	float va = d3 * d6 - d5 * d4;
	//check if p is in the edge region of BC, if so return projection of p onto BC
	if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) <= 0.f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		XMStoreFloat2(&tmp, XMLoadFloat2(&pnts[1]) + w * XMVectorSubtract(XMLoadFloat2(&pnts[2]), XMLoadFloat2(&pnts[1])));
		return tmp;									//barycentric coordinates (0,1-w,w)
	}

	//p likes inside the face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.f / (va + vb + vc);
	float v = vb * denom;		float w = vc * denom;

	// = u*a + v*b + w*c, u = va * denom = 1.0f-v-w
	XMStoreFloat2(&tmp, XMLoadFloat2(&pnts[0]) + ab * v + ac * w);
	return tmp;
}

XMFLOAT3 BoundingSphere::ClosestPtPointTriangle(XMFLOAT3 pnt, Triangle tri)
{
	XMFLOAT3 tmp;

	XMVECTOR ab = XMVectorSubtract(XMLoadFloat3(&tri.Get3DPntByIdx(1)), XMLoadFloat3(&tri.Get3DPntByIdx(0)));
	XMVECTOR ac = XMVectorSubtract(XMLoadFloat3(&tri.Get3DPntByIdx(2)), XMLoadFloat3(&tri.Get3DPntByIdx(0)));
	XMVECTOR ap = XMVectorSubtract(XMLoadFloat3(&pnt), XMLoadFloat3(&tri.Get3DPntByIdx(0)));

	float d1 = *XMVector3Dot(ab, ap).m128_f32;		float d2 = *XMVector3Dot(ac, ap).m128_f32;
	//check if p is in the vertex region outside A
	if (d1 <= 0.f && d2 <= 0.f)	return tri.Get3DPntByIdx(0);		//barycentric coordinates (1,0,0)

	XMVECTOR bp = XMVectorSubtract(XMLoadFloat3(&pnt), XMLoadFloat3(&tri.Get3DPntByIdx(1)));
	float d3 = *XMVector3Dot(ab, bp).m128_f32;		float d4 = *XMVector3Dot(ac, bp).m128_f32;
	//check if p is in the vertex region outside B
	if (d3 >= 0.f && d4 <= d3)	return tri.Get3DPntByIdx(1);		//barycentric coordinates (0,1,0)

	float vc = d1 * d4 - d3 * d2;
	//check if p is in the edge region of AB, if so return projection of p onto AB
	if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f)
	{
		float v = (d1 / (d1 - d3));
		XMStoreFloat3(&tmp, XMLoadFloat3(&tri.Get3DPntByIdx(0)) + v * ab);
		return tmp;									//barycentric coordinates (1-v,v,0)
	}

	XMVECTOR cp = XMVectorSubtract(XMLoadFloat3(&pnt), XMLoadFloat3(&tri.Get3DPntByIdx(2)));
	float d5 = *XMVector3Dot(ab, cp).m128_f32;		float d6 = *XMVector3Dot(ac, cp).m128_f32;

	//check if p is in the vertex region outside C
	if (d6 >= 0.f && d5 <= d3)	return tri.Get3DPntByIdx(2);		//barycentric coordinates (0,1,0)

	float vb = d5 * d2 - d1 * d6;
	// Check if P in edge region of AC, if so return projection of P onto AC
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		XMStoreFloat3(&tmp, XMLoadFloat3(&tri.Get3DPntByIdx(0)) + w * ac); // barycentric coordinates (1-w,0,w)
		return tmp;
	}

	float va = d3 * d6 - d5 * d4;
	//check if p is in the edge region of BC, if so return projection of p onto BC
	if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) <= 0.f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		XMStoreFloat3(&tmp, XMLoadFloat3(&tri.Get3DPntByIdx(1)) + w * XMVectorSubtract(XMLoadFloat3(&tri.Get3DPntByIdx(2)), XMLoadFloat3(&tri.Get3DPntByIdx(1))));
		return tmp;									//barycentric coordinates (0,1-w,w)
	}

	//p lies inside the face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.f / (va + vb + vc);
	float v = vb * denom;		float w = vc * denom;

	// = u*a + v*b + w*c, u = va * denom = 1.0f-v-w
	XMStoreFloat3(&tmp, XMLoadFloat3(&tri.Get3DPntByIdx(0)) + ab * v + ac * w);
	return tmp;
}

