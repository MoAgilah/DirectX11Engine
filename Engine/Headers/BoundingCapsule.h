////////////////////////////////////////////////////////////////////////////////
// Filename: BoundingCapsule.h
////////////////////////////////////////////////////////////////////////////////
#ifndef BoundingCapsule_H
#define BoundingCapsule_H

/////////////////
// MY INCLUDES //
/////////////////
#include <DirectXMath.h>
#include <algorithm>
using namespace std;
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/BoundingVolume.h"
#include "../Headers/AABB.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: BoundingCapsule
////////////////////////////////////////////////////////////////////////////////
class BoundingCapsule : public BoundingVolume
{
public:
	BoundingCapsule(BVType type, bool is3Dim);
	BoundingCapsule(XMFLOAT2 bot, XMFLOAT2 top, float radius);
	BoundingCapsule(XMFLOAT3 bot, XMFLOAT3 top, float radius);
	~BoundingCapsule();

	void Update(XMFLOAT2 center, XMFLOAT2 size);
	void Update(XMFLOAT3 center, XMFLOAT3 size);

	float GetRadius() { return m_fRadius; }
	LineSeg GetLineSeg() { return m_LineSeg; }

	bool Contains2DPoint(XMFLOAT2 point);
	bool Contains3DPoint(XMFLOAT3 point);

	bool Intersect2DAABB(AABB* AABB2D);
	bool Intersect3DAABB(AABB* AABB3D);

	bool IntersectSphere2D(BoundingSphere* sphere);
	bool IntersectSphere3D(BoundingSphere* sphere);

	bool IntersectCapsule2D(BoundingCapsule* capsule2);
	bool IntersectCapsule3D(BoundingCapsule* capsule2);

	bool IntersectLineSegment2D(LineSeg lineSeg, float& t);
	bool IntersectLineSegment3D(LineSeg lineSeg, float& t);

	bool IntersectRay2D(XMFLOAT2 o, XMFLOAT2 dist, float& t);
	bool IntersectRay3D(XMFLOAT3 o, XMFLOAT3 dist, float& t);
private:
	LineSeg m_LineSeg;
	float m_fRadius;
};

#endif // !BoundingSphere_H
