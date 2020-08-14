#ifndef Triangle_H
#define Triangle_H



//////////////////
// MY INCLUDES  //
//////////////////
#include <vector>
using namespace std;
#include <directxmath.h>
using namespace DirectX;

class Triangle
{
public:
	Triangle();
	~Triangle();
	Triangle(XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2);
	Triangle(XMFLOAT3 t0, XMFLOAT3 t1, XMFLOAT3 t2);

	XMFLOAT2 Get2DPntByIdx(int i);
	XMFLOAT3 Get3DPntByIdx(int i);

	bool Contains(XMFLOAT2 p);
	bool Contains(XMFLOAT3 p);

	XMFLOAT2 ClosestPtPointTriangle2D(XMFLOAT2 pnt, Triangle tri);
	XMFLOAT3 ClosestPtPointTriangle3D(XMFLOAT3 pnt, Triangle tri);

	XMFLOAT3 ClosestPtPointTetrahedron(XMFLOAT3 pnt, XMFLOAT3 tet[4]);
	
private:
	int PointOutsideOfPlane(XMFLOAT3 p, XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, XMFLOAT3 d);
	bool SameSide2D(XMFLOAT2 p1, XMFLOAT2 p2, XMFLOAT2 a, XMFLOAT2 b);
	bool SameSide3D(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 a, XMFLOAT3 b);

	XMFLOAT3 pnts[3];
};

#endif // !Triangle_H
