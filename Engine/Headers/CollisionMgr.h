////////////////////////////////////////////////////////////////////////////////
// Filename: CollisionMgr.h
////////////////////////////////////////////////////////////////////////////////
#ifndef CollisionMgr_H
#define CollisionMgr_H

/////////////////
// MY INCLUDES //
/////////////////
#include <vector>
#include <algorithm>
#include "../Headers/D3DMgr.h"
#include "../Headers/DebugDraw.h"
using namespace DirectX;
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Sprite.h"
#include "../Headers/AABB.h"
#include "../Headers/BoundingSphere.h"
#include "../Headers/BoundingCapsule.h"
#include "../Headers/Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: CollisionMgr
////////////////////////////////////////////////////////////////////////////////
class CollisionMgr
{
public:
	CollisionMgr(Graphics* graphicsMgr);
	~CollisionMgr();
	void Release();

	AABB* Add2DAABB();
	AABB* Add3DAABB();

	OBB* Add2DOBB();
	OBB* Add3DOBB();

	BoundingSphere* Add2DBoundingSphere();
	BoundingSphere* Add3DBoundingSphere();

	BoundingCapsule* Add2DBoundingCapsule();
	BoundingCapsule* Add3DBoundingCapsule();

	void ProcessCollisions();

	void DrawDebug(Graphics* graphicsMgr);
	DebugDraw* m_pDebugDraw;
private:

	void ClosestPtLine2D(XMFLOAT2 bot, XMFLOAT2 top, XMFLOAT2 pnt, float& t, XMFLOAT2& c);
	void ClosestPtLine3D(XMFLOAT3 bot, XMFLOAT3 top, XMFLOAT3 pnt, float& t, XMFLOAT3& c);

	void ClosestPtRay3D(XMFLOAT3 ori, XMFLOAT3 norm, XMFLOAT3 pnt, float& t, XMFLOAT3& c);
	void ClosestPtRay2D(XMFLOAT2 ori, XMFLOAT2 norm, XMFLOAT2 pnt, float& t, XMFLOAT2& c);

private:
	vector<Sprite*> m_vpCollidableSprites;

	vector<BoundingVolume*> m_pBoundingVolumes2D;
	vector<BoundingVolume*> m_pBoundingVolumes3D;

	vector<AABB*> m_pVecAABB2D;
	vector<AABB*> m_pVecAABB3D;

	vector<OBB*> m_pVecOBB2D;
	vector<OBB*> m_pVecOBB3D;

	vector<BoundingSphere*> m_pVecBoundingSphere2D;
	vector<BoundingSphere*> m_pVecBoundingSphere3D;

	vector<BoundingCapsule*> m_pVecBoundingCapsule2D;
	vector<BoundingCapsule*> m_pVecBoundingCapsule3D;
};

#endif // !CollisionMgr_H
