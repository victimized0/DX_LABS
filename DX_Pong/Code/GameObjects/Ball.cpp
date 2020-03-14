#include "pch.h"
#include "Ball.h"
#include "../Game.h"

Ball::Ball(const std::string& name, DirectX::XMFLOAT3 position)
	: GeometryObject(name, position)
	, m_size(0)
	, m_speedX(DEFAULT_SPEED_X)
	, m_speedY(DEFAULT_SPEED_Y)
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

void Ball::Update(float dt) {
	float sizeDiv2		= (float)m_size / 2;
	float ballLeft		= GetPosition().x - sizeDiv2;
	float ballRight		= GetPosition().x + sizeDiv2;
	float ballBottom	= GetPosition().y - sizeDiv2;
	float ballTop		= GetPosition().y + sizeDiv2;

	bool collidesBorder = fabs(ballTop - GRID_TOP_BORDER) < 0.1
					   || fabs(ballBottom - GRID_BOTTOM_BORDER) < 0.1;

	if (collidesBorder) {
		m_speedY *= -1;
	}

	float dx = m_speedX * 0.02f;
	float dy = m_speedY * 0.02f;
	Translate(dx, dy, 0.0f);

	bool crossedBorderRight = fabs(ballRight - GRID_RIGHT_BORDER) < sizeDiv2 + dt;
	bool crossedBorderLeft  = fabs(ballLeft  - GRID_LEFT_BORDER)  < sizeDiv2 + dt;

	if (crossedBorderRight || crossedBorderLeft) {
		Reset();
	}
}

void Ball::Reset() {
	XMStoreFloat4x4(&m_transform, XMMatrixIdentity());

	m_speedX = DEFAULT_SPEED_X;
	m_speedY = DEFAULT_SPEED_Y;
}
