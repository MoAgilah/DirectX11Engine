#include "../Headers/Bullet.h"

Bullet::Bullet(Mesh* Bullet)
	:Static3DObj(Bullet)
{
	m_bActive = false;

	m_mWorldMatrix = XMMatrixIdentity();

	//the intention was to use gravity
	m_vGravity = XMVectorSet(0, -0.981f, 0, 0);
}

Bullet::~Bullet()
{
}

void Bullet::Activate(const XMMATRIX& gunMat, const XMVECTOR& Direction)
{
	m_bActive = true;

	m_vForwardVector = Direction;

	//do scaling once and parent to gun
	m_mWorldMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * gunMat;

	//reintialise timer
	m_dTime = 0.0f;
}

bool Bullet::GetActive() const
{
	return m_bActive;
}

void Bullet::Deactivate()
{
	m_bActive = false;
}

bool Bullet::Update(const double& dt)
{
	//update lifetime
	m_dTime += dt;

	UpdateMatrices();

	//if lifetime over deactivate
	return m_dTime >= 2;
}

void Bullet::Draw(Graphics* graphics)
{
	Render(graphics);
}

void Bullet::UpdateMatrices(void)
{
	//update world with direction	
	m_mWorldMatrix *= XMMatrixTranslationFromVector(m_vForwardVector);

	SetWorldMatrix(m_mWorldMatrix);
}