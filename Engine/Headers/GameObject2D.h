////////////////////////////////////////////////////////////////////////////////
// Filename: GameObject2D.h
////////////////////////////////////////////////////////////////////////////////
#ifndef GameObject2D_H
#define GameObject2D_H

/////////////////
// MY INCLUDES //
/////////////////
#include <string>
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/D3DMgr.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GameObject2D
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class GameObject2D
{
public:
	GameObject2D();
	GameObject2D(const GameObject2D& other);
	~GameObject2D();

	string GetName();
	void SetPosition(const XMFLOAT2& pos);
	void SetPosition(float x, float y);
	XMFLOAT2 GetPosition();
	void SetRotation(float angle);

	virtual void Update(const float& dt) = 0;
protected:
	string m_sName;
	XMFLOAT2 m_f2Position;
	float m_fAngle;
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

#endif // !GameObject2D_H
