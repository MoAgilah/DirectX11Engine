////////////////////////////////////////////////////////////////////////////////
// Filename: GameObject3D.h
////////////////////////////////////////////////////////////////////////////////
#ifndef GameObject3D_H
#define GameObject3D_H

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
// Class name: GameObject3D
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class GameObject3D
{
public:
	GameObject3D();
	GameObject3D(const GameObject3D& other);
	~GameObject3D();

	string GetName();
	void SetPosition( XMFLOAT4& pos);
	void SetPosition( float x,  float y,  float z);
	XMFLOAT4 GetPosition();
	void SetRotation( XMFLOAT4& pos);
	void SetRotation( float x,  float y,  float z);
	XMFLOAT4 GetRotation();
	void SetWorldMatrix( XMMATRIX& world);
	XMMATRIX GetWorldMatrix();

	virtual void UpdateMatrices() = 0;
	virtual void Update(const float& dt) = 0;
protected:
	string m_sName;
	XMFLOAT4 m_f4Position;
	XMFLOAT4 m_f4Rotation;
	XMMATRIX m_mWorldMatrix;
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

#endif // !GameObject3D_H
