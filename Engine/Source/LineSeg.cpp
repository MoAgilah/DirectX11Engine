#include "../Headers/LineSeg.h"
#include "../Headers/BoundingSphere.h"
#include "../Headers/AABB.h"
#include "../Headers/OBB.h"
#include "../Headers/BoundingCapsule.h"

LineSeg::LineSeg()
{
	m_f3Center = XMFLOAT3(0.f, 0.f, 0.f);
	m_f3Bottom = XMFLOAT3(0.f, 0.f, 0.f);
	m_f3Top = XMFLOAT3(0.f, 0.f, 0.f);
}

LineSeg::LineSeg(XMFLOAT2 bot, XMFLOAT2 top)
{
	m_f3Bottom = XMFLOAT3(bot.x, bot.y, 1.f);
	m_f3Top = XMFLOAT3(top.x, top.y, 1.f);
	m_f3Center = XMFLOAT3((bot.x - top.x) * 0.5f, (bot.y - top.y) * 0.5f, 1.f);
}

LineSeg::LineSeg(XMFLOAT3 bot, XMFLOAT3 top)
{
	m_f3Bottom = bot;
	m_f3Top = top;
	m_f3Center = XMFLOAT3((bot.x - top.x) * 0.5f, (bot.y - top.y) * 0.5f, (bot.z - top.z) * 0.5f);
}

LineSeg::~LineSeg()
{
}

bool LineSeg::IntersectLineSeg2D(LineSeg lineSeg, float& t, XMFLOAT2 pnt)
{
	//signs of ares corresponds to which side of ab point c and d are
	float a1 = Signed2DTriArea(GetTop2D(), GetBottom2D(), lineSeg.GetBottom2D());	//compute winding order abd (+ or -)
	float a2 = Signed2DTriArea(GetTop2D(), GetBottom2D(), lineSeg.GetTop2D());	// To intersect, must have sign opposite of a1

	//if c and d are on different sides of ab, i.e. areas haves different signs
	if (a1 * a2 < 0.f)
	{
		//compute signs for a and b with respect to segment cd
		float a3 = Signed2DTriArea(lineSeg.GetTop2D(), lineSeg.GetBottom2D(), GetTop2D());	//compute winding order abd (+ or -)
		//area is constant i.e. a1 - a2 = a3 - a4, then a4 = a3 + a2 - a1
		float a4 = a3 + a2 - a1;

		//if c and d are on different sides of ab, i.e. areas haves different signs
		if (a3 * a4 < 1.f)
		{
			//then segments intersect, find intersection point along 
			//L(t) = a + t * (b - a)
			// Given height h1 of a over cd and height h2 of b over cd
			// t = h1 / (h1 - h2) = (b*h1/2) / (b*h1/2 - b*h2/2) = a3 / (a3 - a4),
			// where b (the base of the triangles cda and cdb, i.e., the length
			// of cd) cancels out.
			t = a3 / (a3 - a4);

			//p = a + t * (b - a);
			XMStoreFloat2(&pnt, XMVectorAdd(XMLoadFloat2(&GetTop2D()), XMVectorScale(XMVectorSubtract(XMLoadFloat2(&GetBottom2D()), XMLoadFloat2(&GetTop2D())), t)));

			return true;
		}
	}

	return false;
}

bool LineSeg::IntersectOBB2D(OBB* obb)
{
	return false;
}

bool LineSeg::IntersectOBB3D(OBB* obb)
{
	return false;
}

bool LineSeg::IntersectBoundingSphere2D(BoundingSphere* sphere)
{
	return false;
}

bool LineSeg::IntersectBoundingSphere3D(BoundingSphere* sphere)
{
	return false;
}

bool LineSeg::IntersectAABB2D(AABB* aabb)
{
	XMFLOAT2 c = aabb->GetCenter2D();
	XMFLOAT2 e = aabb->GetExtent2D();
	XMFLOAT2 m;	XMStoreFloat2(&m, XMVectorScale(XMVectorAdd(XMLoadFloat2(&GetBottom2D()), XMLoadFloat2(&GetTop2D())), 0.5f));

	XMFLOAT2 d;	XMStoreFloat2(&d, XMVectorSubtract(XMLoadFloat2(&GetTop2D()), XMLoadFloat2(&m)));

	XMStoreFloat2(&m, XMVectorSubtract(XMLoadFloat2(&m), XMLoadFloat2(&aabb->GetCenter2D())));

	//try the world coordinate axes as seperating axes
	float adx = abs(d.x);		if (abs(m.x) > e.x + adx) return aabb->SetIsCollided(false);
	float ady = abs(d.y);		if (abs(m.y) > e.y + ady) return aabb->SetIsCollided(false);

	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis
	adx += FLT_EPSILON;		ady += FLT_EPSILON;

	// Try cross products of segment direction vector with coordinate axes
	if (abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return aabb->SetIsCollided(false);
	if (abs(m.y * d.x - m.x * d.y) > e.y * adx + e.x * ady) return aabb->SetIsCollided(false);

	// No separating axis found; segment must be overlapping AABB
	return aabb->SetIsCollided(true);
}

bool LineSeg::IntersectAABB3D(AABB* aabb)
{
	XMFLOAT3 c = aabb->GetCenter3D();
	XMFLOAT3 e = aabb->GetExtent3D();
	XMFLOAT3 m;	XMStoreFloat3(&m, XMVectorScale(XMVectorAdd(XMLoadFloat3(&GetBottom3D()), XMLoadFloat3(&GetTop3D())), 0.5f));

	XMFLOAT3 d;	XMStoreFloat3(&d, XMVectorSubtract(XMLoadFloat3(&GetTop3D()), XMLoadFloat3(&m)));

	XMStoreFloat3(&m, XMVectorSubtract(XMLoadFloat3(&m), XMLoadFloat3(&aabb->GetCenter3D())));

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

//doesnt explicitly handle division by zero
bool LineSeg::IntersectPlane2D(Plane plane, float& t, XMFLOAT2 q)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat2(&GetTop2D()), XMLoadFloat2(&GetBottom2D()));
	t = (plane.GetDistance() - *XMVector2Dot(XMLoadFloat2(&plane.Get2DNorm()), XMLoadFloat2(&GetBottom2D())).m128_f32) / *XMVector2Dot(XMLoadFloat2(&plane.Get2DNorm()), ab).m128_f32;

	//if t is within 0 & 1 then there is an intersection compute intersection point
	if (t >= 0.f && t <= 1.f)
	{
		XMStoreFloat2(&q, XMVectorAdd(XMLoadFloat2(&GetBottom2D()), t * ab));
		return true;
	}

	return false;
}

//doesnt explicitly handle division by zero
bool LineSeg::IntersectPlane3D(Plane plane, float& t, XMFLOAT3 q)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat3(&GetTop3D()), XMLoadFloat3(&GetBottom3D()));
	t = (plane.GetDistance() - *XMVector3Dot(XMLoadFloat3(&plane.Get3DNorm()), XMLoadFloat3(&GetBottom3D())).m128_f32) / *XMVector3Dot(XMLoadFloat3(&plane.Get3DNorm()), ab).m128_f32;

	//if t is within 0 & 1 then there is an intersection compute intersection point
	if (t >= 0.f && t <= 1.f)
	{
		XMStoreFloat3(&q, XMVectorAdd(XMLoadFloat3(&GetBottom3D()), t * ab));
		return true;
	}

	return false;
}

// Intersect segment ab against plane of triangle def. If intersecting,
// return t value and position q of intersection
bool LineSeg::IntersectPlane2D(XMFLOAT2 a, XMFLOAT2 b, XMFLOAT2 c, float& t, XMFLOAT2 q)
{
	XMFLOAT2 norm;
	XMStoreFloat2(&norm,
	XMVector2Cross(XMVectorSubtract(XMLoadFloat2(&b), XMLoadFloat2(&a)),
		XMVectorSubtract(XMLoadFloat2(&c), XMLoadFloat2(&a))));
	
	Plane plane = Plane(norm, *XMVector2Dot(XMLoadFloat2(&norm), XMLoadFloat2(&a)).m128_f32);

	return IntersectPlane2D(plane, t, q);
}

// Intersect segment ab against plane of triangle def. If intersecting,
// return t value and position q of intersection
bool LineSeg::IntersectPlane3D(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, float& t, XMFLOAT3 q)
{
	XMFLOAT3 norm;
	XMStoreFloat3(&norm,
		XMVector3Cross(XMVectorSubtract(XMLoadFloat3(&b), XMLoadFloat3(&a)),
			XMVectorSubtract(XMLoadFloat3(&c), XMLoadFloat3(&a))));

	Plane plane = Plane(norm, *XMVector3Dot(XMLoadFloat3(&norm), XMLoadFloat3(&a)).m128_f32);

	return IntersectPlane3D(plane, t, q);
}

bool LineSeg::IntersectCapsule2D(BoundingCapsule capsule, float& t)
{
	float s;
	XMFLOAT2 c1, c2;

	float distSquared = ClosestPtSegmentSegment2D(capsule.GetLineSeg(), s, t, c1, c2);

	return distSquared <= capsule.GetRadius() * capsule.GetRadius();
}

bool LineSeg::IntersectCapsule3D(BoundingCapsule capsule, float& t)
{
	float s;
	XMFLOAT3 c1, c2;

	float distSquared = ClosestPtSegmentSegment3D(capsule.GetLineSeg(), s, t, c1, c2);

	return distSquared <= capsule.GetRadius() * capsule.GetRadius();
}

float LineSeg::Clamp(float n, float min, float max)
{
	if (n < min) return min;
	if (n > max) return max;
	return n;
}

float LineSeg::Signed2DTriArea(XMFLOAT2 pnt1, XMFLOAT2 pnt2, XMFLOAT2 pnt3)
{
	return (pnt1.x - pnt3.x) * (pnt2.y - pnt3.y) - (pnt1.y - pnt3.y) * (pnt2.x - pnt3.x);
}

void LineSeg::ClosestPtPointSegment2D(XMFLOAT2 pnt, float& t, XMFLOAT2& c)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat2(&GetTop2D()), XMLoadFloat2(&GetBottom2D()));

	//project pnt onto ab (bot top), but deferring divide by dot(ab,ab)
	t = *XMVector2Dot(XMVectorSubtract(XMLoadFloat2(&pnt), XMLoadFloat2(&GetBottom2D())), ab).m128_f32;

	//if pnt projects outside the [a,b] interva; on the a side clamp to a
	if (t <= 0.f)
	{
		t = 0.f;	c = GetBottom2D();
	}
	else {
		float denom = *XMVector2Dot(ab, ab).m128_f32; //always non negative since dnom = ||ab||^2

		//if pnt projects outside the [a,b] interval; clamp to b
		if (t >= denom)
		{
			t = 1.f;	c = GetTop2D();
		}
		else {
			//pnt projects inside the [a,b] interval; deferred division required 
			t = t / denom;

			//(c)losest point = a +  (ab * t)
			XMStoreFloat2(&c, XMVectorAdd(XMLoadFloat2(&GetBottom2D()), XMVectorScale(ab, t)));
		}
	}
}

void LineSeg::ClosestPtPointSegment3D(XMFLOAT3 pnt, float& t, XMFLOAT3& c)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat3(&GetTop3D()), XMLoadFloat3(&GetBottom3D()));

	//project pnt onto ab, but deferring divide by dot ab,ab
	t = *XMVector3Dot(XMVectorSubtract(XMLoadFloat3(&pnt), XMLoadFloat3(&GetBottom3D())), ab).m128_f32;
	if (t <= 0.f)
	{
		//pnt projects outside the [a,b] interval, pn the a side; clamp to a 
		t = 0.f;
		c = GetBottom3D();
	}
	else
	{
		float denom = *XMVector3Dot(ab, ab).m128_f32;
		if (t >= denom)
		{
			//pnt projects outside the [a,b] interval; clamp to b
			t = 1.f;
			c = GetTop3D();
		}
		else
		{
			//c projects inside the [a,b] interval; must do deferred divide now
			t = t / denom;

			XMStoreFloat3(&c, XMVectorAdd(XMLoadFloat3(&GetBottom3D()), XMVectorScale(ab, t)));
		}
	}
}

float LineSeg::SquareDistPointSegment2D(XMFLOAT2 point)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat2(&GetTop2D()), XMLoadFloat2(&GetBottom2D()));
	XMVECTOR ac = XMVectorSubtract(XMLoadFloat2(&point), XMLoadFloat2(&GetBottom2D()));
	XMVECTOR bc = XMVectorSubtract(XMLoadFloat2(&point), XMLoadFloat2(&GetTop2D()));

	float e = *XMVector2Dot(ac, ab).m128_f32;

	//handle cases where point projects outside ab
		/*When AC · AB ≤ 0, A is closest to C and
			the squared distance is given by AC · AC*/
	if (e <= 0.f) return *XMVector2Dot(ac, ac).m128_f32;

	float f = *XMVector2Dot(ab, ab).m128_f32;

	/*When AC · AB ≥ AB · AB, B is closest to C
			and the squared distance is BC · BC*/
	if (e >= f) return *XMVector2Dot(bc, bc).m128_f32;

	//handles cases where c project onto ab
		/*the squared distance is given by CD · CD
			0 < AC · AB < AB · AB, the sqDist is given by CD · CD
				AC.AC - (AC.AB)^2 / AB.AB*/

	return *XMVector2Dot(ac, ac).m128_f32 - (e * e) / f;
}

float LineSeg::SquareDistPointSegment3D(XMFLOAT3 point)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat3(&GetTop3D()), XMLoadFloat3(&GetBottom3D()));
	XMVECTOR ac = XMVectorSubtract(XMLoadFloat3(&point), XMLoadFloat3(&GetBottom3D()));
	XMVECTOR bc = XMVectorSubtract(XMLoadFloat3(&point), XMLoadFloat3(&GetTop3D()));

	float e = *XMVector3Dot(ac, ab).m128_f32;

	//handle cases where point projects outside ab
		/*When AC · AB ≤ 0, A is closest to C and
			the squared distance is given by AC · AC*/
	if (e <= 0.f) return *XMVector3Dot(ac, ac).m128_f32;

	float f = *XMVector3Dot(ab, ab).m128_f32;

	/*When AC · AB ≥ AB · AB, B is closest to C
		and the squared distance is BC · BC*/
	if (e >= f) return *XMVector3Dot(bc, bc).m128_f32;

	//handles cases where c project onto ab
		/*the squared distance is given by CD · CD
			0 < AC · AB < AB · AB, the sqDist is given by CD · CD
				AC.AC - (AC.AB)^2 / AB.AB*/

	return *XMVector3Dot(ac, ac).m128_f32 - (e * e) / f;
}

float LineSeg::ClosestPtSegmentSegment2D(LineSeg lineSeg, float& s, float& t, XMFLOAT2& c1, XMFLOAT2& c2)
{
	XMVECTOR d1 = XMVectorSubtract(XMLoadFloat2(&lineSeg.GetBottom2D()), XMLoadFloat2(&GetBottom2D())); // Direction vector of segment S1
	XMVECTOR d2 = XMVectorSubtract(XMLoadFloat2(&lineSeg.GetTop2D()), XMLoadFloat2(&GetTop2D())); // Direction vector of segment S2
	XMVECTOR r = XMVectorSubtract(XMLoadFloat2(&GetBottom2D()), XMLoadFloat2(&GetTop2D()));

	float a = *XMVector2Dot(d1, d1).m128_f32; //Dot(d1, d1); // Squared length of segment S1, always nonnegative
	float e = *XMVector2Dot(d2, d2).m128_f32; //Dot(d2, d2); // Squared length of segment S2, always nonnegative
	float f = *XMVector2Dot(d2, r).m128_f32;  //Dot(d2, r);

	// Check if either or both segments degenerate into points
	if (a <= FLT_EPSILON && e <= FLT_EPSILON) {
		// Both segments degenerate into points
		s = t = 0.0f;
		c1 = GetBottom2D();
		c2 = GetTop2D();

		XMVECTOR c = XMVectorSubtract(XMLoadFloat2(&c1), XMLoadFloat2(&c2));

		return *XMVector2Dot(c, c).m128_f32; //Dot(c1 - c2, c1 - c2);
	}
	if (a <= FLT_EPSILON) {
		// First segment degenerates into a point
		s = 0.0f;
		t = f / e; // s = 0 => t = (b*s + f) / e = f / e
		t = Clamp(t, 0.0f, 1.0f);
	}
	else {
		float c = *XMVector2Dot(d1, r).m128_f32;; //Dot(d1, r);
		if (e <= FLT_EPSILON) {
			// Second segment degenerates into a point
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
		}
		else {
			// The general nondegenerate case starts here
			float b = *XMVector2Dot(d1, d2).m128_f32;; //Dot(d1, d2);
			float denom = a * e - b * b; // Always nonnegative

			// If segments not parallel, compute closest point on L1 to L2, and
			// clamp to segment S1. Else pick arbitrary s (here 0)
			if (denom != 0.0f) {
				s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else s = 0.0f;

			// Compute point on L2 closest to S1(s) using
			// t = Dot((P1+D1*s)-P2,D2) / Dot(D2,D2) = (b*s + f) / e
			//the division by e can be deferred until t is known to be in the range[0, 1]
			float tnom = (b * s + f);

			// If t in [0,1] done. Else clamp t, recompute s for the new value
			// of t using s = Dot((P2+D2*t)-P1,D1) / Dot(D1,D1)= (t*b - c) / a
			// and clamp s to [0, 1]
			if (tnom < 0.0f) {
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (tnom > e) {
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
			else
			{
				t = tnom / e;
			}
		}
	}

	XMStoreFloat2(&c1, XMVectorAdd(XMLoadFloat2(&GetBottom2D()), d1 * s));
	XMStoreFloat2(&c2, XMVectorAdd(XMLoadFloat2(&GetTop2D()), d2 * t));

	XMVECTOR c = XMVectorSubtract(XMLoadFloat2(&c1), XMLoadFloat2(&c2));

	return *XMVector2Dot(c, c).m128_f32; //Dot(c1 - c2, c1 - c2);
}

float LineSeg::ClosestPtSegmentSegment3D(LineSeg lineSeg, float& s, float& t, XMFLOAT3& c1, XMFLOAT3& c2)
{
	XMVECTOR d1 = XMVectorSubtract(XMLoadFloat3(&lineSeg.GetBottom3D()), XMLoadFloat3(&GetBottom3D())); // Direction vector of segment S1
	XMVECTOR d2 = XMVectorSubtract(XMLoadFloat3(&lineSeg.GetTop3D()), XMLoadFloat3(&GetTop3D())); // Direction vector of segment S2
	XMVECTOR r = XMVectorSubtract(XMLoadFloat3(&GetBottom3D()), XMLoadFloat3(&GetTop3D()));

	float a = *XMVector3Dot(d1, d1).m128_f32; //Dot(d1, d1); // Squared length of segment S1, always nonnegative
	float e = *XMVector3Dot(d2, d2).m128_f32; //Dot(d2, d2); // Squared length of segment S2, always nonnegative
	float f = *XMVector3Dot(d2, r).m128_f32;  //Dot(d2, r);

	// Check if either or both segments degenerate into points
	if (a <= FLT_EPSILON && e <= FLT_EPSILON) {
		// Both segments degenerate into points
		s = t = 0.0f;
		c1 = GetBottom3D();
		c2 = GetTop3D();

		XMVECTOR c = XMVectorSubtract(XMLoadFloat3(&c1), XMLoadFloat3(&c2));

		return *XMVector3Dot(c, c).m128_f32; //Dot(c1 - c2, c1 - c2);
	}
	if (a <= FLT_EPSILON) {
		// First segment degenerates into a point
		s = 0.0f;
		t = f / e; // s = 0 => t = (b*s + f) / e = f / e
		t = Clamp(t, 0.0f, 1.0f);
	}
	else {
		float c = *XMVector3Dot(d1, r).m128_f32;; //Dot(d1, r);
		if (e <= FLT_EPSILON) {
			// Second segment degenerates into a point
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
		}
		else {
			// The general nondegenerate case starts here
			float b = *XMVector3Dot(d1, d2).m128_f32;; //Dot(d1, d2);
			float denom = a * e - b * b; // Always nonnegative

			// If segments not parallel, compute closest point on L1 to L2, and
			// clamp to segment S1. Else pick arbitrary s (here 0)
			if (denom != 0.0f) {
				s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else s = 0.0f;

			// Compute point on L2 closest to S1(s) using
			// t = Dot((P1+D1*s)-P2,D2) / Dot(D2,D2) = (b*s + f) / e
			//the division by e can be deferred until t is known to be in the range[0, 1]
			float tnom = (b * s + f);

			// If t in [0,1] done. Else clamp t, recompute s for the new value
			// of t using s = Dot((P2+D2*t)-P1,D1) / Dot(D1,D1)= (t*b - c) / a
			// and clamp s to [0, 1]
			if (tnom < 0.0f) {
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (tnom > e) {
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
			else
			{
				t = tnom / e;
			}
		}
	}

	XMStoreFloat3(&c1, XMVectorAdd(XMLoadFloat3(&GetBottom3D()), d1 * s));
	XMStoreFloat3(&c2, XMVectorAdd(XMLoadFloat3(&GetTop3D()), d2 * t));

	XMVECTOR c = XMVectorSubtract(XMLoadFloat3(&c1), XMLoadFloat3(&c2));

	return *XMVector3Dot(c, c).m128_f32; //Dot(c1 - c2, c1 - c2);
}

