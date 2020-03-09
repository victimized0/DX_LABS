#include "pch.h"
#include "Ball.h"
#include "../Pong.h"

Ball::Ball(const std::string& name, DirectX::XMFLOAT3 position)
	: GeometryObject(name, position)
	, m_size(0)
	, m_speedX(0)
	, m_speedY(0)
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

void Ball::Initialise() {
	m_speedX = DEFAULT_SPEED_X;
	m_speedY = DEFAULT_SPEED_Y;
}

void Ball::Update(float dt) {
	float sizeDiv2		= (float)m_size / 2;
	float ballLeft		= m_pos.x - sizeDiv2;
	float ballRight		= m_pos.x + sizeDiv2;
	float ballBottom	= m_pos.y - sizeDiv2;
	float ballTop		= m_pos.y + sizeDiv2;

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
	for (int i = 0; i < m_vertices.size(); ++i) {
		m_vertices[i].Position = XMFLOAT3(m_vertices[i].Position.x - m_pos.x,
										  m_vertices[i].Position.y - m_pos.y,
										  m_vertices[i].Position.z - m_pos.z);
	}

	m_pos.x = 0;
	m_pos.y = 0;
	m_pos.z = 0;

	m_speedX = DEFAULT_SPEED_X;
	m_speedY = DEFAULT_SPEED_Y;
}
