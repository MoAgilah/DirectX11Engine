#ifndef DebugDraw_H
#define DebugDraw_H

#include <vector>
using namespace std; 

#include <DirectXMath.h>
using namespace DirectX;

#include "../DirectXTemplate/PrimitiveBatch.h"

#include "../Headers/Graphics.h"

#include "../Headers/OBB.h"
#include "../Headers/AABB.h"
#include "../Headers/BoundingCapsule.h"
#include "../Headers/BoundingSphere.h"
#include "../Headers/Utils.h"

class DebugDraw
{
public:
	DebugDraw(Graphics* graphicsMgr);
	~DebugDraw();
	void Release();

	void Begin();
	void End();

	void DrawLineSeg(XMFLOAT2 bot, XMFLOAT2 top, Graphics* graphicsMgr);
	void DrawLineSeg(LineSeg* lineSeg, Graphics* graphicsMgr);

	void DrawTriangle(XMFLOAT2 pnt1, XMFLOAT2 pnt2, XMFLOAT2 pnt3, Graphics* graphicsMgr);
	void DrawTriangle(Triangle* tri, Graphics* graphicsMgr);

	void DrawAABB(AABB* aabb2d, Graphics* graphicsMgr);
	void DrawOBB(OBB* aabb2d, Graphics* graphicsMgr);


	void DrawCircle(BoundingSphere* sphere, Graphics* graphicsMgr);
	void DrawCapsule(BoundingCapsule* capsule, Graphics* graphicsMgr);
	void DrawLine(XMFLOAT2 pnt1, XMFLOAT2 pnt2, Graphics* graphicsMgr);

	

	void DrawRay(XMFLOAT2 pnt1, XMFLOAT2 pnt2, Graphics* graphicsMgr);
	/*
	void DrawRay(XMFLOAT2 pnt1, XMFLOAT2 norm, Graphics* graphicsMgr);
		pnt1 = a
		pnt1 + norm = b
		ray in the direction of 
	*/
	void DrawRay(XMFLOAT2 ori, float Dir, Graphics* graphicsMgr);
private:
	PrimitiveBatch<VertexTypeCol>* m_vpPrimitiveBatch;


};

#endif
