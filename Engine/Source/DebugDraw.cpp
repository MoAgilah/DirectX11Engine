#include "../Headers/DebugDraw.h"

DebugDraw::DebugDraw(Graphics* graphicsMgr)
{
	m_vpPrimitiveBatch = new PrimitiveBatch<VertexTypeCol>(graphicsMgr->GetD3DMgr()->GetDeviceContext());
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::Release()
{
}

void DebugDraw::Begin()
{
	m_vpPrimitiveBatch->Begin();
}

void DebugDraw::End()
{
	m_vpPrimitiveBatch->End();
}

void DebugDraw::DrawTriangle(XMFLOAT2 pnt1, XMFLOAT2 pnt2, XMFLOAT2 pnt3, Graphics* graphicsMgr)
{
	Triangle tri = Triangle(pnt1, pnt2, pnt3);
	DrawTriangle(&tri, graphicsMgr);
}

void DebugDraw::DrawTriangle(Triangle* tri, Graphics* graphicsMgr)
{
	XMFLOAT4 col = XMFLOAT4(0, 0, 0, 1);
	float halfScrX((float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1)), halfScrY((float)(graphicsMgr->GetScreenDimensions().y * .5f));
	vector<VertexTypeCol> verts;
	

	

	verts.push_back(VertexTypeCol(XMFLOAT2(halfScrX + tri->Get2DPntByIdx(0).x, halfScrY - tri->Get2DPntByIdx(0).y), col));

	verts.push_back(VertexTypeCol(XMFLOAT2(halfScrX + tri->Get2DPntByIdx(1).x, halfScrY - tri->Get2DPntByIdx(1).y), col));

	verts.push_back(VertexTypeCol(XMFLOAT2(halfScrX + tri->Get2DPntByIdx(2).x, halfScrY - tri->Get2DPntByIdx(2).y), col));

	verts.push_back(verts[0]);

	m_vpPrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP, &verts[0], verts.size());
	graphicsMgr->GetShaderMgr()->RenderColourShader(graphicsMgr->GetD3DMgr()->GetDeviceContext(), (int)verts.size(), *graphicsMgr->GetD3DMgr()->GetWorldMatrix(), *graphicsMgr->GetCamera()->GetViewMatrix(),
		*graphicsMgr->GetD3DMgr()->GetOrthoMatrix());
}

void DebugDraw::DrawOBB(OBB* aabb2d, Graphics* graphicsMgr)
{
	XMFLOAT4 col;

	FRECT r;
	// Calculate the screen coordinates of the left side of the bitmap.
	r.left = (float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1) + (float)aabb2d->GetMin2D().x;

	// Calculate the screen coordinates of the right side of the bitmap.
	r.right = r.left + aabb2d->GetSize2D().x;

	// Calculate the screen coordinates of the top of the bitmap.
	r.top = (float)(graphicsMgr->GetScreenDimensions().y * .5f) - (float)aabb2d->GetMin2D().y;

	// Calculate the screen coordinates of the bottom of the bitmap.
	r.bottom = r.top - aabb2d->GetSize2D().y;

	aabb2d->GetIsCollided() ? col = XMFLOAT4(1, 0, 0, 1) : col = XMFLOAT4(0, 0, 0, 1);

	//white					   //red
	//aabb2d->GetIsCollided() ? col = XMFLOAT4(1, 0, 0, 1) : col = XMFLOAT4(0, 0, 0, 1);

	vector<VertexTypeCol> verts;
	verts.push_back(VertexTypeCol(XMFLOAT2(r.left, r.top), col));
	verts.push_back(VertexTypeCol(XMFLOAT2(r.right, r.top), col));
	verts.push_back(VertexTypeCol(XMFLOAT2(r.right, r.bottom), col));
	verts.push_back(VertexTypeCol(XMFLOAT2(r.left, r.bottom), col));
	verts.push_back(VertexTypeCol(XMFLOAT2(r.left, r.top), col));

	m_vpPrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, &verts[0], verts.size());

	graphicsMgr->GetShaderMgr()->RenderColourShader(graphicsMgr->GetD3DMgr()->GetDeviceContext(), (int)verts.size(),
		*graphicsMgr->GetD3DMgr()->GetWorldMatrix(), *graphicsMgr->GetCamera()->GetViewMatrix(),
		*graphicsMgr->GetD3DMgr()->GetOrthoMatrix());
}

void DebugDraw::DrawAABB(AABB* aabb2d, Graphics* graphicsMgr)
{
	XMFLOAT4 col;
	
	FRECT r;
	// Calculate the screen coordinates of the left side of the bitmap.
	r.left = (float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1) + (float)aabb2d->GetMin2D().x;

	// Calculate the screen coordinates of the right side of the bitmap.
	r.right = r.left + aabb2d->GetSize2D().x;

	// Calculate the screen coordinates of the top of the bitmap.
	r.top = (float)(graphicsMgr->GetScreenDimensions().y * .5f) - (float)aabb2d->GetMin2D().y;

	// Calculate the screen coordinates of the bottom of the bitmap.
	r.bottom = r.top - aabb2d->GetSize2D().y;

	aabb2d->GetIsCollided() ? col = XMFLOAT4(1, 0, 0, 1) : col = XMFLOAT4(0, 0, 0, 1);

	//white					   //red
	//aabb2d->GetIsCollided() ? col = XMFLOAT4(1, 0, 0, 1) : col = XMFLOAT4(0, 0, 0, 1);

	vector<VertexTypeCol> verts;
	verts.push_back(VertexTypeCol(XMFLOAT2(r.left, r.top), col));
	verts.push_back(VertexTypeCol(XMFLOAT2(r.right, r.top), col));
	verts.push_back(VertexTypeCol(XMFLOAT2(r.right, r.bottom), col));
	verts.push_back(VertexTypeCol(XMFLOAT2(r.left, r.bottom), col));
	verts.push_back(VertexTypeCol(XMFLOAT2(r.left, r.top), col));

	m_vpPrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP, &verts[0], verts.size());

	graphicsMgr->GetShaderMgr()->RenderColourShader(graphicsMgr->GetD3DMgr()->GetDeviceContext(), (int)verts.size(),
		*graphicsMgr->GetD3DMgr()->GetWorldMatrix(), *graphicsMgr->GetCamera()->GetViewMatrix(),
			*graphicsMgr->GetD3DMgr()->GetOrthoMatrix());
}

void DebugDraw::DrawCircle(BoundingSphere* sphere, Graphics* graphicsMgr)
{
	XMFLOAT4 col;

	// Calculate the screen coordinates of the x component of the spheres center
	float xCent = (float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1) + (float)sphere->GetCenter2D().x;

	// Calculate the screen coordinates of the top of the bitmap.
	float yCent = (float)(graphicsMgr->GetScreenDimensions().y * .5f) - (float)sphere->GetCenter2D().y;

	XMFLOAT3 center = XMFLOAT3(xCent, yCent, 1);

	float radius = sphere->GetRadius();

	vector<VertexTypeCol> verts;
	//white					   //red
	sphere->GetIsCollided() ? col = XMFLOAT4(1, 0, 0, 1) : col = XMFLOAT4(0, 0, 0, 1);

	int x = (int)radius, y = 0, err = 0, i = 0;

	while (x >= y)
	{
		//top half
		//(-x, y) octant 7
		verts.push_back(VertexTypeCol(XMFLOAT2(-x + center.x, y + center.y), col));

		//(-y, x)octant 8
		verts.push_back(VertexTypeCol(XMFLOAT2(-y + center.x, x + center.y), col));

		//(y, x) octant 1
		verts.push_back(VertexTypeCol(XMFLOAT2(y + center.x, x + center.y), col));

		//(x,y) octant 2
		verts.push_back(VertexTypeCol(XMFLOAT2(x + center.x, y + center.y), col));

		//bottom half
		//(x,-y) octant 3
		verts.push_back(VertexTypeCol(XMFLOAT2(x + center.x, -y + center.y), col));

		//(y, -x) octant 4
		verts.push_back(VertexTypeCol(XMFLOAT2(y + center.x, -x + center.y), col));

		//(-y,-x) octant 5
		verts.push_back(VertexTypeCol(XMFLOAT2(-y + center.x, -x + center.y), col));

		//(-x, -y) octant 6
		verts.push_back(VertexTypeCol(XMFLOAT2(-x + center.x, -y + center.y), col));

		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}

		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}

	}

	m_vpPrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &verts[0], verts.size());

	graphicsMgr->GetShaderMgr()->RenderColourShader(graphicsMgr->GetD3DMgr()->GetDeviceContext(), (int)verts.size(),
		*graphicsMgr->GetD3DMgr()->GetWorldMatrix(), *graphicsMgr->GetCamera()->GetViewMatrix(),
				*graphicsMgr->GetD3DMgr()->GetOrthoMatrix());
}

void DebugDraw::DrawCapsule(BoundingCapsule* capsule, Graphics* graphicsMgr)
{
	XMFLOAT4 col;
	float centX, centY, centX2, centY2;
	
	// Calculate the screen coordinates of the left side of the bitmap.
	centX = (float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1) + (float)capsule->GetLineSeg().GetTop2D().x;

	// Calculate the screen coordinates of the top of the bitmap.
	centY = (float)(graphicsMgr->GetScreenDimensions().y * .5f) - (float)capsule->GetLineSeg().GetTop2D().y;

	XMFLOAT3 center = XMFLOAT3(centX, centY, 1);

	// Calculate the screen coordinates of the left side of the bitmap.
	centX2 = (float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1) + (float)capsule->GetLineSeg().GetBottom2D().x;

	// Calculate the screen coordinates of the top of the bitmap.
	centY2 = (float)(graphicsMgr->GetScreenDimensions().y * .5f) - (float)capsule->GetLineSeg().GetBottom2D().y;
	
	XMFLOAT3 center2 = XMFLOAT3(centX2, centY2, 1);

	float radius = capsule->GetRadius();
	vector< VertexTypeCol> verts;

	//white					   //red
	capsule->GetIsCollided() ? col = XMFLOAT4(1, 0, 0, 1) : col = XMFLOAT4(0, 0, 0, 1);
	int x = (int)radius, y = 0, err = 0, i = 0;

	while (x >= y)
	{
		//top half
		//(-x, y) octant 7
		verts.push_back(VertexTypeCol(XMFLOAT2(-x + center.x, y + center.y), col));

		//(-y, x)octant 8
		verts.push_back(VertexTypeCol(XMFLOAT2(-y + center.x, x + center.y), col));

		//(y, x) octant 1
		verts.push_back(VertexTypeCol(XMFLOAT2(y + center.x, x + center.y), col));

		//(x,y) octant 2
		verts.push_back(VertexTypeCol(XMFLOAT2(x + center.x, y + center.y), col));

		//bottom half
		//(x,-y) octant 3
		verts.push_back(VertexTypeCol(XMFLOAT2(x + center2.x, -y + center2.y), col));

		//(y, -x) octant 4
		verts.push_back(VertexTypeCol(XMFLOAT2(y + center2.x, -x + center2.y), col));

		//(-y,-x) octant 5
		verts.push_back(VertexTypeCol(XMFLOAT2(-y + center2.x, -x + center2.y), col));

		//(-x, -y) octant 6
		verts.push_back(VertexTypeCol(XMFLOAT2(-x + center2.x, -y + center2.y), col));

		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}

		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}
	}

	m_vpPrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &verts[0], verts.size());
	graphicsMgr->GetShaderMgr()->RenderColourShader(graphicsMgr->GetD3DMgr()->GetDeviceContext(), (int)verts.size(),
		*graphicsMgr->GetD3DMgr()->GetWorldMatrix(), *graphicsMgr->GetCamera()->GetViewMatrix(),
			*graphicsMgr->GetD3DMgr()->GetOrthoMatrix());

	vector<VertexTypeCol> lines;
	lines.push_back(VertexTypeCol(XMFLOAT2(center.x - radius, center.y), col));
	lines.push_back(VertexTypeCol(XMFLOAT2(center2.x - radius, center2.y), col));
	lines.push_back(VertexTypeCol(XMFLOAT2(center.x + radius, center.y), col));
	lines.push_back(VertexTypeCol(XMFLOAT2(center2.x + radius, center2.y), col));

	m_vpPrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, &lines[0], lines.size());
	graphicsMgr->GetShaderMgr()->RenderColourShader(graphicsMgr->GetD3DMgr()->GetDeviceContext(), (int)lines.size()
		,
		*graphicsMgr->GetD3DMgr()->GetWorldMatrix(),*graphicsMgr->GetCamera()->GetViewMatrix(),
				*graphicsMgr->GetD3DMgr()->GetOrthoMatrix());
}

void DebugDraw::DrawLine(XMFLOAT2 pnt1, XMFLOAT2 pnt2, Graphics* graphicsMgr)
{
	float centX, centY, centX2, centY2;

	//// Calculate the screen coordinates of the left side of the bitmap.
	centX = (float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1) + (float)pnt1.x;

	//// Calculate the screen coordinates of the top of the bitmap.
	centY = (float)(graphicsMgr->GetScreenDimensions().y * .5f) - (float)pnt1.y;

	//// Calculate the screen coordinates of the left side of the bitmap.
	centX2 = (float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1) + (float)pnt2.x;

	//// Calculate the screen coordinates of the top of the bitmap.
	centY2 = (float)(graphicsMgr->GetScreenDimensions().y * .5f) - (float)pnt2.y;

	//extend - for line and ray
	//a variable to give a length gaurenteed to extend beyond boundaries
	XMFLOAT2 len = XMFLOAT2(graphicsMgr->GetScreenDimensions().x , graphicsMgr->GetScreenDimensions().y);

	//end point extend
	float lenAB = sqrtf(powf(centX - centX2, 2.0) + powf(centY - centY2, 2.0));
	float exX = centX2 + (centX2 - centX) / lenAB * len.x;
	float exY = centY2 + (centY2 - centY) / lenAB * len.y;

	vector<VertexTypeCol> verts;
	verts.push_back(VertexTypeCol(XMFLOAT2(exX, exY), XMFLOAT4(0, 0, 0, 1)));

	//extend for 
	//start point extstar
	exX = centX - (centX2 - centX) / lenAB * len.x;
	exY = centY - (centY2 - centY) / lenAB * len.y;

	verts.push_back(VertexTypeCol(XMFLOAT2(exX, exY), XMFLOAT4(0, 0, 0, 1)));
	
	m_vpPrimitiveBatch->DrawLine(verts.front(), verts.back());
	graphicsMgr->GetShaderMgr()->RenderColourShader(graphicsMgr->GetD3DMgr()->GetDeviceContext(), (int)verts.size(), *graphicsMgr->GetD3DMgr()->GetWorldMatrix(),
		*graphicsMgr->GetCamera()->GetViewMatrix(), *graphicsMgr->GetD3DMgr()->GetOrthoMatrix());
}

void DebugDraw::DrawLineSeg(XMFLOAT2 bot, XMFLOAT2 top, Graphics* graphicsMgr)
{
	DrawLineSeg(&LineSeg(bot, top),graphicsMgr);
}

void DebugDraw::DrawLineSeg(LineSeg* lineSeg, Graphics* graphicsMgr)
{
	XMFLOAT4 col = XMFLOAT4(0, 0, 0, 1);
	col = XMFLOAT4(0, 0, 0, 1);
	float centX, centY, centX2, centY2;
	float halfScrX((float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1)), halfScrY((float)(graphicsMgr->GetScreenDimensions().y * .5f));
	
	//// Calculate the screen coordinates of the left side of the bitmap.
	centX = halfScrX + (float)lineSeg->GetBottom2D().x;

	//// Calculate the screen coordinates of the top of the bitmap.
	centY = halfScrY - (float)lineSeg->GetBottom2D().y;

	//// Calculate the screen coordinates of the left side of the bitmap.
	centX2 = halfScrX + (float)lineSeg->GetTop2D().x;

	//// Calculate the screen coordinates of the top of the bitmap.
	centY2 = halfScrY - (float)lineSeg->GetTop2D().y;

	vector<VertexTypeCol> verts;
	verts.push_back(VertexTypeCol(XMFLOAT2(centX, centY), col));
	verts.push_back(VertexTypeCol(XMFLOAT2(centX2, centY2), col));

	

	m_vpPrimitiveBatch->DrawLine(verts.front(), verts.back());
	graphicsMgr->GetShaderMgr()->RenderColourShader(graphicsMgr->GetD3DMgr()->GetDeviceContext(), (int)verts.size(),
		*graphicsMgr->GetD3DMgr()->GetWorldMatrix(), *graphicsMgr->GetCamera()->GetViewMatrix(),
			*graphicsMgr->GetD3DMgr()->GetOrthoMatrix());
}

void DebugDraw::DrawRay(XMFLOAT2 pnt1, XMFLOAT2 dir, Graphics* graphicsMgr)
{
	float centX, centY, centX2, centY2;

	//// Calculate the screen coordinates of the left side of the bitmap.
	centX = (float)((graphicsMgr->GetScreenDimensions().x * .5f) * -1) + (float)pnt1.x;

	//// Calculate the screen coordinates of the top of the bitmap.
	centY = (float)(graphicsMgr->GetScreenDimensions().y * .5f) - (float)pnt1.y;

	float t = 800;
	
		centX2 = centX + t * dir.x;

		
		centY2 = centY + t * dir.y;
	
	VertexTypeCol verts[2];
	verts[0] = VertexTypeCol(XMFLOAT2(centX, centY), XMFLOAT4(0, 0, 0, 1));
	verts[1] = VertexTypeCol(XMFLOAT2(centX2, centY2), XMFLOAT4(0, 0, 0, 1));


	m_vpPrimitiveBatch->DrawLine(verts[0], verts[1]);
	graphicsMgr->GetShaderMgr()->RenderColourShader(graphicsMgr->GetD3DMgr()->GetDeviceContext(), 2, *graphicsMgr->GetD3DMgr()->GetWorldMatrix(),
		*graphicsMgr->GetCamera()->GetViewMatrix(), *graphicsMgr->GetD3DMgr()->GetOrthoMatrix());
}

void DebugDraw::DrawRay(XMFLOAT2 ori, float Dir, Graphics* graphicsMgr)
{
}
