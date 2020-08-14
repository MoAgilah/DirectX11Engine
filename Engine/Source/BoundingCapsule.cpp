#include "../Headers/BoundingCapsule.h"

BoundingCapsule::BoundingCapsule(BVType type, bool is3Dim)
	:BoundingVolume(type,is3Dim)
{
	m_fRadius = 0.f;
}

BoundingCapsule::BoundingCapsule(XMFLOAT2 bot, XMFLOAT2 top, float radius)
	: BoundingVolume(BVType::Capsule2D, false)
{
	m_fRadius = radius;
	m_LineSeg.SetBottom2D(bot);
	m_LineSeg.SetTop2D(top);
	m_LineSeg.SetCenter2D(XMFLOAT2((top.x - bot.x) * 0.5f, (top.y - bot.y) * 0.5f));
}

BoundingCapsule::BoundingCapsule(XMFLOAT3 bot, XMFLOAT3 top, float radius)
	: BoundingVolume(BVType::Capsule3D, false)
{
	m_fRadius = radius;
	m_LineSeg.SetBottom3D(bot);
	m_LineSeg.SetTop3D(top);
	m_LineSeg.SetCenter3D(XMFLOAT3((top.x - bot.x) * 0.5f, (top.y - bot.y) * 0.5f, (top.z - bot.z) * 0.5f));
}

BoundingCapsule::~BoundingCapsule()
{
}

void BoundingCapsule::Update(XMFLOAT2 center, XMFLOAT2 size)
{
	m_fRadius = (size.x * .5f) * m_f3Percentile.x;
	m_f3Size = XMFLOAT3(m_fRadius, m_fRadius, m_fRadius);

	m_LineSeg.SetCenter2D(XMFLOAT2((center.x + m_f3Padding.x) + m_fRadius, (center.y + m_f3Padding.y) + m_fRadius));
	m_LineSeg.SetBottom2D(XMFLOAT2(m_LineSeg.GetCenter2D().x, m_LineSeg.GetCenter2D().y + m_fRadius));
	m_LineSeg.SetTop2D(XMFLOAT2(m_LineSeg.GetCenter2D().x, m_LineSeg.GetCenter2D().y - m_fRadius));
}

void BoundingCapsule::Update(XMFLOAT3 center, XMFLOAT3 size)
{
	m_fRadius = (size.x * .5f) * m_f3Percentile.x;
	m_f3Size = XMFLOAT3(m_fRadius, m_fRadius, m_fRadius);

	m_LineSeg.SetCenter2D(XMFLOAT2((center.x + m_f3Padding.x) + m_fRadius, (center.y + m_f3Padding.y) + m_fRadius));
	m_LineSeg.SetBottom2D(XMFLOAT2(m_LineSeg.GetCenter2D().x, m_LineSeg.GetCenter2D().y + m_fRadius));
	m_LineSeg.SetTop2D(XMFLOAT2(m_LineSeg.GetCenter2D().x, m_LineSeg.GetCenter2D().y - m_fRadius));
}

bool BoundingCapsule::Contains2DPoint(XMFLOAT2 point)
{
	float distSquared = m_LineSeg.SquareDistPointSegment2D(point);

	return SetIsCollided(distSquared < GetRadius() * GetRadius());
}

bool BoundingCapsule::Contains3DPoint(XMFLOAT3 point)
{
	float distSquared = m_LineSeg.SquareDistPointSegment3D(point);

	return SetIsCollided(distSquared < GetRadius() * GetRadius());
}

bool BoundingCapsule::IntersectLineSegment2D(LineSeg lineSeg, float& t)
{
	float s;
	XMFLOAT2 c1, c2;

	float distSquared = lineSeg.ClosestPtSegmentSegment2D(GetLineSeg(), s, t, c1, c2);

	return SetIsCollided(distSquared <= GetRadius() * GetRadius());
}

bool BoundingCapsule::IntersectLineSegment3D(LineSeg lineSeg, float& t)
{
	float s;
	XMFLOAT3 c1, c2;

	float distSquared = GetLineSeg().ClosestPtSegmentSegment3D(lineSeg, s, t, c1, c2);

	return SetIsCollided(distSquared <= GetRadius() * GetRadius());
}



bool BoundingCapsule::Intersect2DAABB(AABB* AABB2D)
{
	vector<XMFLOAT3> vec = AABB2D->GetCornerPoints();
	float sqDist[4];
	sqDist[0] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[0].x, vec[0].y));
	sqDist[1] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[1].x, vec[1].y));
	sqDist[2] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[2].x, vec[2].y));
	sqDist[3] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[3].x, vec[3].y));

	int  i = 0;
	float t = 0, temp = sqDist[0];
	XMFLOAT2 c;

	for (int j = 1; j < 4; j++) {
		if (temp > sqDist[j]) 
		{
			temp = sqDist[j]; 
			i = j;
		}
	}

	GetLineSeg().ClosestPtPointSegment2D(XMFLOAT2(vec[i].x, vec[i].y), t, c);

	//get closest point on aabb through clamping
	float x = max(AABB2D->GetMin2D().x, min(c.x, AABB2D->GetMax2D().x));
	float y = max(AABB2D->GetMin2D().y, min(c.y, AABB2D->GetMax2D().y));

	//get squared dist
	float csqDist = (x - c.x) * (x - c.x) + (y - c.y) * (y - c.y);

	//if squared dist is < radius * radius
	return AABB2D->SetIsCollided(SetIsCollided(csqDist < GetRadius()* GetRadius()));
}

bool BoundingCapsule::Intersect3DAABB(AABB* AABB3D)
{
	vector<XMFLOAT3> vec = AABB3D->GetCornerPoints();
	float sqDist[8];
	sqDist[0] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[0].x, vec[0].y));
	sqDist[1] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[1].x, vec[1].y));
	sqDist[2] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[2].x, vec[2].y));
	sqDist[3] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[3].x, vec[3].y));
	sqDist[4] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[4].x, vec[4].y));
	sqDist[5] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[5].x, vec[5].y));
	sqDist[6] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[6].x, vec[6].y));
	sqDist[7] = GetLineSeg().SquareDistPointSegment2D(XMFLOAT2(vec[7].x, vec[7].y));

	int  i = 0;
	float t = 0, temp = sqDist[0];
	XMFLOAT3 c;

	for (int j = 1; j < 4; j++) {
		if (temp > sqDist[j])
		{
			temp = sqDist[j];
			i = j;
		}
	}

	GetLineSeg().ClosestPtPointSegment3D(vec[i], t, c);

	//get closest point on aabb through clamping
	float x = max(AABB3D->GetMin3D().x, min(c.x, AABB3D->GetMax3D().x));
	float y = max(AABB3D->GetMin3D().y, min(c.y, AABB3D->GetMax3D().y));
	float z = max(AABB3D->GetMin3D().z, min(c.z, AABB3D->GetMax3D().z));

	//get squared dist
	float csqDist = (x - c.x) * (x - c.x) + (y - c.y) * (y - c.y) + (z - c.z) * (z - c.z);

	//if squared dist is < radius * radius
	return AABB3D->SetIsCollided(SetIsCollided(csqDist < GetRadius() * GetRadius()));
}

bool BoundingCapsule::IntersectSphere2D(BoundingSphere* sphere)
{
	float distSquared = GetLineSeg().SquareDistPointSegment2D(sphere->GetCenter2D());

	float radiusSum = sphere->GetRadius() + GetRadius();

	return sphere->SetIsCollided(SetIsCollided(distSquared <= radiusSum * radiusSum));
}

bool BoundingCapsule::IntersectSphere3D(BoundingSphere* sphere)
{
	float distSquared = GetLineSeg().SquareDistPointSegment3D(sphere->GetCenter3D());

	float radiusSum = sphere->GetRadius() + GetRadius();

	return sphere->SetIsCollided(SetIsCollided(distSquared <= radiusSum * radiusSum));
}

bool BoundingCapsule::IntersectCapsule2D(BoundingCapsule* capsule2)
{
	float s, t;
	XMFLOAT2 c1, c2;

	float distSquared = GetLineSeg().ClosestPtSegmentSegment2D( capsule2->GetLineSeg(), s, t, c1, c2);

	float radiusSum = GetRadius() + capsule2->GetRadius();

	return SetIsCollided(capsule2->SetIsCollided(distSquared <= radiusSum * radiusSum));
}

bool BoundingCapsule::IntersectCapsule3D(BoundingCapsule* capsule2)
{
	float s, t;
	XMFLOAT3 c1, c2;

	float distSquared = GetLineSeg().ClosestPtSegmentSegment3D(capsule2->GetLineSeg(), s, t, c1, c2);

	float radiusSum = GetRadius() + capsule2->GetRadius();

	return SetIsCollided(capsule2->SetIsCollided(distSquared <= radiusSum * radiusSum));
}

bool BoundingCapsule::IntersectRay2D(XMFLOAT2 origin, XMFLOAT2 distance, float& t)
{
	XMVECTOR d = XMLoadFloat2(&distance);
	XMVECTOR m = XMVectorSubtract(XMLoadFloat2(&origin), XMLoadFloat2(&m_LineSeg.GetBottom2D()));
	XMVECTOR n = XMVectorSubtract(XMVectorAdd(XMLoadFloat2(&origin), XMLoadFloat2(&distance)), XMLoadFloat2(&origin));
	float nd = *XMVector2Dot(n, d).m128_f32;
	float dd = *XMVector2Dot(d, d).m128_f32;

	//var for bottom spherical end cap
	float md = *XMVector2Dot(m, d).m128_f32;
	float sc = *XMVector2Dot(m, m).m128_f32 - m_fRadius * m_fRadius;

	//var the top spherical end cap
	XMVECTOR m2 = XMVectorSubtract(XMVectorAdd(XMLoadFloat2(&origin), XMLoadFloat2(&distance)), XMLoadFloat2(&m_LineSeg.GetTop2D()));
	float md2 = *XMVector2Dot(m2, d).m128_f32;
	float sc2 = *XMVector2Dot(m2, m2).m128_f32 - m_fRadius * m_fRadius;

	//var for cylinder
	float nn = *XMVector2Dot(n, n).m128_f32;
	float mn = *XMVector2Dot(m, n).m128_f32;
	float a = dd * nn - nd * nd;
	float k = *XMVector2Dot(m, m).m128_f32 - m_fRadius * m_fRadius;
	float c = dd * k - md * md;

	if (abs(a) < FLT_EPSILON)
	{
		//segment runs parallel to the cylinder axis
		if (c > 0.f) return SetIsCollided(false); // 'a' and thus the segmnet lie outside cylinder

		//check for collision with p sphere
		if (sc > 0.0f && md > 0.0f) return 0;
		else
		{
			float discr = md * md - sc;
			//if negative discriminant corresponds to ray misisng sphere
			if (discr < 0.f) return SetIsCollided(false);
			else
			{
				t = -md - sqrtf(discr);
				if (t < 0.f) t = 0.f;
				return  SetIsCollided(true);
			}
		}

		//check for collision with q sphere
		if (sc2 > 0.0f && md2 > 0.0f) return  SetIsCollided(false);
		else
		{
			float discr2 = md2 * md2 - sc2;
			//if negative discriminant corresponds to ray misisng sphere
			if (discr2 < 0.f) return  SetIsCollided(false);
			else
			{
				t = -md2 - sqrtf(discr2);
				if (t < 0.f) t = 0.f;
				return  SetIsCollided(true);
			}
		}
	}

	float b = dd * mn - nd * md;
	float discr = b * b - a * c;
	if (discr < 0.f) return  SetIsCollided(false);	//no real roots; no intersection

	t = (-b - sqrtf(discr) / a);
	if (t < 0.f || t > 1.f)	return  SetIsCollided(false);	//intersection lies outside segment

	if (md + t * nd < 0.f)
	{
		//intersection outside cylinder on 'p' side
		if (nd <= 0.f) return  SetIsCollided(false);		//segment pointing away from endcap

		t = -md / nd;

		//keep intersection if dot(S(t) - p, S(t) - p) <= r ^2
		return k + 2 * t * (mn * t * nn) <= 0.f;
	}
	else if (md + t * nd > dd) {
		//intersection outside cylinder on 'q' side
		if (nd >= 0.f) return  SetIsCollided(false);	//segment pointing away from endcap
		t = (dd - md) / nd;

		//keep intersection if dot(S(t) - q, S(t) - q) <= r ^2
		return SetIsCollided(k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.f);
	}

	//segment intersects cylinder betweeen the endcaps; t is correct
	return  SetIsCollided(true);
}

bool BoundingCapsule::IntersectRay3D(XMFLOAT3 origin, XMFLOAT3 dist, float& t)
{
	//D = (B-A) / |B-A|
	XMVECTOR d = XMVectorSubtract(XMLoadFloat3(&m_LineSeg.GetTop3D()), XMLoadFloat3(&m_LineSeg.GetBottom3D()));
	d /= *XMVector3Length(d).m128_f32;
	XMVECTOR m = XMVectorSubtract(XMLoadFloat3(&origin), XMLoadFloat3(&m_LineSeg.GetBottom3D()));
	XMVECTOR n = XMVectorSubtract(XMVectorAdd(XMLoadFloat3(&origin), XMLoadFloat3(&dist)), XMLoadFloat3(&origin));
	float nd = *XMVector3Dot(n, d).m128_f32;
	float dd = *XMVector3Dot(d, d).m128_f32;

	//var for bottom spherical end cap
	float md = *XMVector3Dot(m, d).m128_f32;
	float sc = *XMVector3Dot(m, m).m128_f32 - m_fRadius * m_fRadius;

	//var the top spherical end cap
	XMVECTOR m2 = XMVectorSubtract(XMVectorAdd(XMLoadFloat3(&origin), XMLoadFloat3(&dist)), XMLoadFloat3(&m_LineSeg.GetTop3D()));
	float md2 = *XMVector3Dot(m2, d).m128_f32;
	float sc2 = *XMVector3Dot(m2, m2).m128_f32 - m_fRadius * m_fRadius;

	//var for cylinder
	float nn = *XMVector3Dot(n, n).m128_f32;
	float mn = *XMVector3Dot(m, n).m128_f32;
	float a = dd * nn - nd * nd;
	float k = *XMVector3Dot(m, m).m128_f32 - m_fRadius * m_fRadius;
	float c = dd * k - md * md;

	if (abs(a) < FLT_EPSILON)
	{
		//segment runs parallel to the cylinder axis
		if (c > 0.f) return false; // 'a' and thus the segmnet lie outside cylinder

		//check for collision with p sphere
		if (sc > 0.0f && md > 0.0f) return 0;
		else
		{
			float discr = md * md - sc;
			//if negative discriminant corresponds to ray misisng sphere
			if (discr < 0.f) return false;
			else
			{
				t = -md - sqrtf(discr);
				if (t < 0.f) t = 0.f;
				return true;
			}
		}

		//check for collision with q sphere
		if (sc2 > 0.0f && md2 > 0.0f) return false;
		else
		{
			float discr2 = md2 * md2 - sc2;
			//if negative discriminant corresponds to ray misisng sphere
			if (discr2 < 0.f) return false;
			else
			{
				t = -md2 - sqrtf(discr2);
				if (t < 0.f) t = 0.f;
				return true;
			}
		}
	}

	float b = dd * mn - nd * md;
	float discr = b * b - a * c;
	if (discr < 0.f) return false;		//no real roots; no intersection

	t = (-b - sqrtf(discr) / a);
	if (t < 0.f || t > 1.f)	return false;	//intersection lies outside segment

	if (md + t * nd < 0.f)
	{
		//intersection outside cylinder on 'p' side
		if (nd <= 0.f) return false;		//segment pointing away from endcap

		t = -md / nd;

		//keep intersection if dot(S(t) - p, S(t) - p) <= r ^2
		return k + 2 * t * (mn * t * nn) <= 0.f;
	}
	else if (md + t * nd > dd) {
		//intersection outside cylinder on 'q' side
		if (nd >= 0.f) return false;	//segment pointing away from endcap
		t = (dd - md) / nd;

		//keep intersection if dot(S(t) - q, S(t) - q) <= r ^2
		return k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.f;
	}

	//segment intersects cylinder betweeen the endcaps; t is correct
	return true;
}
