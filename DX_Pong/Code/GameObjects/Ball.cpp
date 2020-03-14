#include "pch.h"
#include "Ball.h"
#include "../Game.h"

Ball::Ball(const std::string& name, DirectX::XMFLOAT3 position)
	: GeometryObject(name, position)
	, m_size(0)
	, m_speedX(DEFAULT_SPEED_X)
	, m_speedY(DEFAULT_SPEED_Y)
	, m_bb(0, 0, 0, 0)
{

}

Ball::Ball(const std::string& name)
	: Ball(name, DirectX::XMFLOAT3(0, 0, 0))
{

}

int Ball::GetSize() {
	return m_size;
}

float Ball::GetSpeedX() {
	return m_speedX;
}

float Ball::GetSpeedY() {
	return m_speedY;
}

void Ball::SetSize(int value) {
	m_size = value;
}

void Ball::SetSpeedX(float value) {
	m_speedX = value;
}

void Ball::SetSpeedY(float value) {
	m_speedY = value;
}

void Ball::InverseSpeedX() {
	m_speedX *= -1;
}

void Ball::InverseSpeedY() {
	m_speedY *= -1;
}

void Ball::Update(float dt) {
	UpdateBB();
}

void Ball::UpdateBB() {
	float sizeDiv2 = (float)m_size / 2;

	m_bb.Left	= GetPosition().x + sizeDiv2;
	m_bb.Right	= GetPosition().x - sizeDiv2;
	m_bb.Top	= GetPosition().y + sizeDiv2;
	m_bb.Bottom = GetPosition().y - sizeDiv2;
}

void Ball::Reset() {
	XMStoreFloat4x4(&m_transform, XMMatrixIdentity());

	m_speedX = DEFAULT_SPEED_X;
	m_speedY = DEFAULT_SPEED_Y;
}
