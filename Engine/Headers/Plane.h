////////////////////////////////////////////////////////////////////////////////
// Filename: Plane.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Plane_H
#define Plane_H

/////////////////
// MY INCLUDES //
/////////////////
#include <DirectXMath.h>
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: Plane
////////////////////////////////////////////////////////////////////////////////
class Plane
{
public:
	Plane(XMFLOAT2 norm, float dist);
	Plane(XMFLOAT3 norm, float dist);
	~Plane();

	float GetDistance() { return distance; };

	XMFLOAT2 Get2DNorm() { return XMFLOAT2(normal.x, normal.y); };
	XMFLOAT3 Get3DNorm() { return normal; };
	XMFLOAT2 ClosestPtPointPlane2D(XMFLOAT2 q);
	XMFLOAT3 ClosestPtPointPlane3D(XMFLOAT3 q);
private:

	XMFLOAT3 normal;
	float distance;
};






#endif