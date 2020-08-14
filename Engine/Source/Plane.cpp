#include "../Headers/Plane.h"

Plane::Plane(XMFLOAT3 norm, float dist)
{
	XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&norm)));;
	distance = dist;
}

Plane::Plane(XMFLOAT2 norm, float dist)
{
	XMStoreFloat2(&norm, XMVector2Normalize(XMLoadFloat2(&norm)));
	normal = XMFLOAT3(norm.x, norm.y, 1.f);
	distance = dist;
}

Plane::~Plane()
{
}

XMFLOAT2 Plane::ClosestPtPointPlane2D(XMFLOAT2 q)
{
	XMFLOAT2 c;
	//t = ((n.q)-d)/(n.n)
	float t = (*XMVector2Dot(XMLoadFloat2(&Get2DNorm()), XMLoadFloat2(&q)).m128_f32 - distance) / *XMVector2Dot(XMLoadFloat2(&Get2DNorm()), XMLoadFloat2(&Get2DNorm())).m128_f32;

	//c = q - t * p.n
	XMStoreFloat2(&c, XMVectorSubtract(XMLoadFloat2(&q), XMVectorScale(XMLoadFloat2(&Get2DNorm()), t)));

	return c;
}

XMFLOAT3 Plane::ClosestPtPointPlane3D(XMFLOAT3 q)
{
	XMFLOAT3 c;
	//t = ((n.q)-d)/(n.n)
	float t = (*XMVector3Dot(XMLoadFloat3(&normal), XMLoadFloat3(&q)).m128_f32 - distance) / *XMVector3Dot(XMLoadFloat3(&normal), XMLoadFloat3(&normal)).m128_f32;

	//c = q - t * p.n
	XMStoreFloat3(&c, XMVectorSubtract(XMLoadFloat3(&q), XMVectorScale(XMLoadFloat3(&normal), t)));

	return c;
}
