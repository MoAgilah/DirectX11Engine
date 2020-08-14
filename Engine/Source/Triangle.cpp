#include "../Headers/Triangle.h"

Triangle::Triangle()
{
	pnts[0] = XMFLOAT3(0.f, 0.f, 0.f);
	pnts[1] = XMFLOAT3(0.f, 0.f, 0.f);
	pnts[2] = XMFLOAT3(0.f, 0.f, 0.f);
}

Triangle::~Triangle()
{

}

Triangle::Triangle(XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2)
{
	pnts[0] = XMFLOAT3(t0.x, t0.y, 1.f);
	pnts[1] = XMFLOAT3(t1.x, t1.y, 1.f);
	pnts[2] = XMFLOAT3(t2.x, t2.y, 1.f);
}

Triangle::Triangle(XMFLOAT3 t0, XMFLOAT3 t1, XMFLOAT3 t2)
{
	pnts[0] = t0;
	pnts[1] = t1;
	pnts[2] = t2;
}

XMFLOAT2 Triangle::Get2DPntByIdx(int i)
{
	if (i >= 3) return XMFLOAT2(-1.f, -1.f);
	return XMFLOAT2(pnts[i].x,pnts[i].y);
}

XMFLOAT3 Triangle::Get3DPntByIdx(int i)
{
	if (i >= 3) return XMFLOAT3(-1.f, -1.f, -1.f);
	return XMFLOAT3(pnts[i]);
}

bool Triangle::SameSide2D(XMFLOAT2 p1, XMFLOAT2 p2, XMFLOAT2 a, XMFLOAT2 b)
{
	XMVECTOR ba = XMVectorSubtract(XMLoadFloat2(&b), XMLoadFloat2(&a));
	XMVECTOR cp1 = XMVector2Cross(ba, XMVectorSubtract(XMLoadFloat2(&p1), XMLoadFloat2(&a)));
	XMVECTOR cp2 = XMVector2Cross(ba, XMVectorSubtract(XMLoadFloat2(&p2), XMLoadFloat2(&a)));

	return *XMVector2Dot(cp1, cp2).m128_f32 >= 0;
}

bool Triangle::SameSide3D(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 a, XMFLOAT3 b)
{
	XMVECTOR ba = XMVectorSubtract(XMLoadFloat3(&b), XMLoadFloat3(&a));
	XMVECTOR cp1 = XMVector3Cross(ba, XMVectorSubtract(XMLoadFloat3(&p1), XMLoadFloat3(&a)));
	XMVECTOR cp2 = XMVector3Cross(ba, XMVectorSubtract(XMLoadFloat3(&p2), XMLoadFloat3(&a)));

	return *XMVector3Dot(cp1, cp2).m128_f32 >= 0;
}

bool Triangle::Contains(XMFLOAT2 p)
{
	XMFLOAT2 a = XMFLOAT2(pnts[0].x, pnts[0].y);
	XMFLOAT2 b = XMFLOAT2(pnts[1].x, pnts[1].y);
	XMFLOAT2 c = XMFLOAT2(pnts[2].x, pnts[2].y);

	return (SameSide2D(p, a, b, c) && SameSide2D(p, b, a, c) && SameSide2D(p, c, a, b));
}

bool Triangle::Contains(XMFLOAT3 p)
{
	XMFLOAT3 a = pnts[0];
	XMFLOAT3 b = pnts[1];
	XMFLOAT3 c = pnts[2];
	return (SameSide3D(p, a, b, c) && SameSide3D(p, b, a, c) && SameSide3D(p, c, a, b));
}

XMFLOAT2 Triangle::ClosestPtPointTriangle2D(XMFLOAT2 pnt, Triangle tri)
{
	XMFLOAT2 tmp;

	XMVECTOR ab = XMVectorSubtract(XMLoadFloat2(&tri.Get2DPntByIdx(1)), XMLoadFloat2(&tri.Get2DPntByIdx(0)));
	XMVECTOR ac = XMVectorSubtract(XMLoadFloat2(&tri.Get2DPntByIdx(2)), XMLoadFloat2(&tri.Get2DPntByIdx(0)));
	XMVECTOR ap = XMVectorSubtract(XMLoadFloat2(&pnt), XMLoadFloat2(&tri.Get2DPntByIdx(0)));

	float d1 = *XMVector2Dot(ab, ap).m128_f32;
	float d2 = *XMVector2Dot(ac, ap).m128_f32;

	//check if p is in the vertex region outside A
	if (d1 <= 0.f && d2 <= 0.f)	return tri.Get2DPntByIdx(0);		//barycentric coordinates (1,0,0)

	XMVECTOR bp = XMVectorSubtract(XMLoadFloat2(&pnt), XMLoadFloat2(&tri.Get2DPntByIdx(1)));
	float d3 = *XMVector2Dot(ab, bp).m128_f32;
	float d4 = *XMVector2Dot(ac, bp).m128_f32;

	//check if p is in the vertex region outside B
	if (d3 >= 0.f && d4 <= d3)	return tri.Get2DPntByIdx(1);		//barycentric coordinates (0,1,0)

	float vc = d1 * d4 - d3 * d2;

	//check if p is in the edge region of AB, if so return projection of p onto AB
	if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f)
	{
		float v = (d1 / (d1 - d3));
		XMStoreFloat2(&tmp, XMLoadFloat2(&tri.Get2DPntByIdx(0)) + v * ab);
		return tmp;									//barycentric coordinates (1-v,v,0)
	}

	XMVECTOR cp = XMVectorSubtract(XMLoadFloat2(&pnt), XMLoadFloat2(&tri.Get2DPntByIdx(2)));
	float d5 = *XMVector2Dot(ab, cp).m128_f32;
	float d6 = *XMVector2Dot(ac, cp).m128_f32;

	//check if p is in the vertex region outside C
	if (d6 >= 0.f && d5 <= d3)	return tri.Get2DPntByIdx(2);		//barycentric coordinates (0,1,0)

	float vb = d5 * d2 - d1 * d6;

	// Check if P in edge region of AC, if so return projection of P onto AC
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		XMStoreFloat2(&tmp, XMLoadFloat2(&tri.Get2DPntByIdx(0)) + w * ac); // barycentric coordinates (1-w,0,w)
		return tmp;
	}

	float va = d3 * d6 - d5 * d4;

	//check if p is in the edge region of BC, if so return projection of p onto BC
	if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) <= 0.f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		XMStoreFloat2(&tmp, XMLoadFloat2(&tri.Get2DPntByIdx(1)) + w * XMVectorSubtract(XMLoadFloat2(&tri.Get2DPntByIdx(2)), XMLoadFloat2(&tri.Get2DPntByIdx(1))));
		return tmp;									//barycentric coordinates (0,1-w,w)
	}

	//p likes inside the face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	// = u*a + v*b + w*c, u = va * denom = 1.0f-v-w
	XMStoreFloat2(&tmp, XMLoadFloat2(&tri.Get2DPntByIdx(0)) + ab * v + ac * w);
	return tmp;
}

XMFLOAT3 Triangle::ClosestPtPointTriangle3D(XMFLOAT3 pnt, Triangle tri)
{
	XMFLOAT3 tmp;

	XMVECTOR ab = XMVectorSubtract(XMLoadFloat3(&tri.Get3DPntByIdx(1)), XMLoadFloat3(&tri.Get3DPntByIdx(0)));
	XMVECTOR ac = XMVectorSubtract(XMLoadFloat3(&tri.Get3DPntByIdx(2)), XMLoadFloat3(&tri.Get3DPntByIdx(0)));
	XMVECTOR ap = XMVectorSubtract(XMLoadFloat3(&pnt), XMLoadFloat3(&tri.Get3DPntByIdx(0)));

	float d1 = *XMVector3Dot(ab, ap).m128_f32;
	float d2 = *XMVector3Dot(ac, ap).m128_f32;

	//check if p is in the vertex region outside A
	if (d1 <= 0.f && d2 <= 0.f)	return tri.Get3DPntByIdx(0);		//barycentric coordinates (1,0,0)

	XMVECTOR bp = XMVectorSubtract(XMLoadFloat3(&pnt), XMLoadFloat3(&tri.Get3DPntByIdx(1)));
	float d3 = *XMVector3Dot(ab, bp).m128_f32;
	float d4 = *XMVector3Dot(ac, bp).m128_f32;

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
	float d5 = *XMVector3Dot(ab, cp).m128_f32;
	float d6 = *XMVector3Dot(ac, cp).m128_f32;

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

	//p likes inside the face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	// = u*a + v*b + w*c, u = va * denom = 1.0f-v-w
	XMStoreFloat3(&tmp, XMLoadFloat3(&tri.Get3DPntByIdx(0)) + ab * v + ac * w);
	return tmp;
}

XMFLOAT3 Triangle::ClosestPtPointTetrahedron(XMFLOAT3 pnt, XMFLOAT3 tet[4])
{
	//start assuming point inside all halfspaces, so closest to itself
	XMFLOAT3 c = pnt, q;
	float bestSqDist = FLT_MAX, sqDist;

	vector<Triangle> tri;
	tri.push_back(Triangle(tet[0], tet[1], tet[2]));	//abc
	tri.push_back(Triangle(tet[0], tet[2], tet[3]));	//acd
	tri.push_back(Triangle(tet[0], tet[3], tet[1]));	//adb
	tri.push_back(Triangle(tet[1], tet[3], tet[2]));	//bdc

	//incase winding order isnt known in advance
	int o[4] = { 3, 1, 2, 0 };

	for (int i = 0; i < 4; i++)
	{

		if (PointOutsideOfPlane(pnt, tri[i].Get3DPntByIdx(0), tri[i].Get3DPntByIdx(1), tri[i].Get3DPntByIdx(2), tet[o[i]]))
		{
			q = ClosestPtPointTriangle3D(pnt, tri[i]);
			sqDist = *XMVector3Dot(XMVectorSubtract(XMLoadFloat3(&q), XMLoadFloat3(&pnt)),
				XMVectorSubtract(XMLoadFloat3(&q), XMLoadFloat3(&pnt))).m128_f32;

			//update the bestSqDist
			if (sqDist < bestSqDist) bestSqDist = sqDist, c = q;
		}
	}

	return c;
}

int Triangle::PointOutsideOfPlane(XMFLOAT3 p, XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, XMFLOAT3 d)
{
	XMVECTOR abac = XMVector3Cross(XMVectorSubtract(XMLoadFloat3(&b), XMLoadFloat3(&a)),
		XMVectorSubtract(XMLoadFloat3(&c), XMLoadFloat3(&a)));

	float signP = *XMVector3Dot(XMVectorSubtract(XMLoadFloat3(&p), XMLoadFloat3(&a)), abac).m128_f32;	//[AP AB AC]

	float signD = *XMVector3Dot(XMVectorSubtract(XMLoadFloat3(&d), XMLoadFloat3(&a)), abac).m128_f32;	//[AD AB AC]

	//point on opposite sides if expression signs are opposite
	return signP * signD < 0.f;
}