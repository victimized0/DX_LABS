#include "pch.h"
#include "Ball.h"
#include "../Game.h"

Ball::Ball(const std::string& name, DirectX::XMFLOAT3 position)
	: GeometryObject(name, position)
	, m_speedX(DEFAULT_SPEED_X)
	, m_speedY(DEFAULT_SPEED_Y)
{

}

Ball::Ball(const std::string& name)
	: Ball(name, DirectX::XMFLOAT3(0, 0, 0))
{

}

float Ball::GetSpeedX() {
	return m_speedX;
}

float Ball::GetSpeedY() {
	return m_speedY;
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

void Ball::Reset() {
	XMStoreFloat4x4(&m_transform, XMMatrixIdentity());

	m_speedX = DEFAULT_SPEED_X;
	m_speedY = DEFAULT_SPEED_Y;
}
