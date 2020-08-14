#include "../Headers/CollisionMgr.h"

CollisionMgr::CollisionMgr(Graphics* graphicsMgr)
{
	m_pDebugDraw = new DebugDraw(graphicsMgr);
	if (!m_pDebugDraw)
	{
		//msg
	}
}

CollisionMgr::~CollisionMgr()
{
}

void CollisionMgr::Release()
{
	SAFE_RELEASE(m_pDebugDraw);
	SAFE_DELETEV(m_pVecAABB2D);
	SAFE_DELETEV(m_pVecAABB3D);
	SAFE_DELETEV(m_pVecBoundingSphere2D);
	SAFE_DELETEV(m_pVecBoundingSphere3D);
	SAFE_DELETEV(m_pVecBoundingCapsule2D);
	SAFE_DELETEV(m_pVecBoundingCapsule3D);
}

AABB* CollisionMgr::Add2DAABB()
{
	m_pVecAABB2D.push_back(new AABB(BVType::AABB2D, true));
	return m_pVecAABB2D.back();
}

AABB* CollisionMgr::Add3DAABB()
{
	m_pVecAABB3D.push_back(new AABB(BVType::AABB3D, true));
	return m_pVecAABB3D.back();
}

OBB* CollisionMgr::Add2DOBB()
{
	m_pVecOBB2D.push_back(new OBB(BVType::OBB2D, true));
	return m_pVecOBB2D.back();
}

OBB* CollisionMgr::Add3DOBB()
{
	m_pVecOBB3D.push_back(new OBB(BVType::OBB3D, true));
	return m_pVecOBB3D.back();
}

BoundingSphere* CollisionMgr::Add2DBoundingSphere()
{
	m_pVecBoundingSphere2D.push_back(new BoundingSphere(BVType::Sphere2D, false));
	return m_pVecBoundingSphere2D.back();
}

BoundingSphere* CollisionMgr::Add3DBoundingSphere()
{
	m_pVecBoundingSphere3D.push_back(new BoundingSphere(BVType::Sphere3D, true));
	return m_pVecBoundingSphere3D.back();
}

BoundingCapsule* CollisionMgr::Add2DBoundingCapsule()
{
	m_pVecBoundingCapsule2D.push_back(new BoundingCapsule(BVType::Capsule2D, false));
	return m_pVecBoundingCapsule2D.back();
}

BoundingCapsule* CollisionMgr::Add3DBoundingCapsule()
{
	m_pVecBoundingCapsule3D.push_back(new BoundingCapsule(BVType::Capsule3D, true));
	return m_pVecBoundingCapsule3D.back();
}
										 
void CollisionMgr::ProcessCollisions()
{
	
}

void CollisionMgr::DrawDebug(Graphics* graphicsMgr)
{
	m_pDebugDraw->Begin();

	for (int i = 0; i < m_pVecAABB2D.size(); i++)
	{
		m_pDebugDraw->DrawAABB(m_pVecAABB2D[i], graphicsMgr);
	}

	for (int i = 0; i < m_pVecOBB2D.size(); i++)
	{
		m_pDebugDraw->DrawOBB(m_pVecOBB2D[i], graphicsMgr);
	}

	for (int i = 0; i < m_pVecBoundingSphere2D.size(); i++)
	{
		m_pDebugDraw->DrawCircle(m_pVecBoundingSphere2D[i], graphicsMgr);
	}

	for (int i = 0; i < m_pVecBoundingCapsule2D.size(); i++)
	{
		m_pDebugDraw->DrawCapsule(m_pVecBoundingCapsule2D[i], graphicsMgr);
	}

	m_pDebugDraw->End();
}


//Given two points on a line a & b and the point c, computes the closest point d on the line
//Also returns t for the position of d, d(t) = a + t * (b-a)
//Similar functionality as the segment variation, with no need to clamp t
void CollisionMgr::ClosestPtLine2D(XMFLOAT2 bot, XMFLOAT2 top, XMFLOAT2 pnt, float& t, XMFLOAT2& c)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat2(&top), XMLoadFloat2(&bot));

	//project pnt onto ab (bot top), but deferring divide by dot(ab,ab)
	t = *XMVector2Dot(XMVectorSubtract(XMLoadFloat2(&pnt), XMLoadFloat2(&bot)), ab).m128_f32;

	//(c)losest point = a +  (ab * t)
	XMStoreFloat2(&c, XMVectorAdd(XMLoadFloat2(&bot), XMVectorScale(ab, t)));
}

//given the origin of the ray and it's direction and the point pnt, find the closest point d on the ray
//Also returns t for the position of d, d(t) = a + t * (b-a)
//Similar functionality as the segment variation, with no need to clamp t
void CollisionMgr::ClosestPtRay2D(XMFLOAT2 ori, XMFLOAT2 norm, XMFLOAT2 pnt, float& t, XMFLOAT2& c)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat2(&ori), XMVectorAdd(XMLoadFloat2(&ori), XMLoadFloat2(&norm)));

	//project pnt onto ab (bot top), but deferring divide by dot(ab,ab)
	t = *XMVector2Dot(XMVectorSubtract(XMLoadFloat2(&pnt), XMLoadFloat2(&ori)), ab).m128_f32;

	//if t is negative clamp
	if (t < 0.f)
	{
		t = 0.f;	c = ori;
	}
	else
	{
		//(c)losest point = a +  (norm * t)
		//this is to have the ray in the direction of the normal more explicitly
		XMStoreFloat2(&c, XMVectorAdd(XMLoadFloat2(&ori), XMVectorScale(XMLoadFloat2(&norm), t)));
	}
}

void CollisionMgr::ClosestPtRay3D(XMFLOAT3 ori, XMFLOAT3 norm, XMFLOAT3 pnt, float& t, XMFLOAT3& c)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat3(&ori), XMVectorAdd(XMLoadFloat3(&ori), XMLoadFloat3(&norm)));

	//project pnt onto ab (bot top), but deferring divide by dot(ab,ab)
	t = *XMVector3Dot(XMVectorSubtract(XMLoadFloat3(&pnt), XMLoadFloat3(&ori)), ab).m128_f32;

	//if t is negative clamp
	if (t < 0.f)
	{
		t = 0.f;	c = ori;
	}
	else
	{
		//(c)losest point = a +  (norm * t)
		//this is to have the ray in the direction of the normal more explicitly
		XMStoreFloat3(&c, XMVectorAdd(XMLoadFloat3(&ori), XMVectorScale(XMLoadFloat3(&norm), t)));
	}
}

void CollisionMgr::ClosestPtLine3D(XMFLOAT3 bot, XMFLOAT3 top, XMFLOAT3 pnt, float& t, XMFLOAT3& c)
{
	XMVECTOR ab = XMVectorSubtract(XMLoadFloat3(&top), XMLoadFloat3(&bot));

	//project pnt onto ab (bot top), but deferring divide by dot(ab,ab)
	t = *XMVector2Dot(XMVectorSubtract(XMLoadFloat3(&pnt), XMLoadFloat3(&bot)), ab).m128_f32;

	//(c)losest point = a +  (ab * t)
	XMStoreFloat3(&c, XMVectorAdd(XMLoadFloat3(&bot), XMVectorScale(ab, t)));
}



